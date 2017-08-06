#include "serveur.h"

//Constructeur
Serveur::Serveur()
{
    serveur = new QTcpServer(this);
    tailleMessage = 0;
    indice =0;
    this->initialiseJeu();
    this->ecouteDesClients();
}

// Initialise tout les signaux nécessaires pour la gestion du jeu et des joueurs.
void Serveur::initialiseJeu(){
    gestionjoueur = new GestionJoueur();
    connect(this, SIGNAL(joueurConnexion(QString, QString)), this->gestionjoueur, SLOT(joueurNouveau(QString,QString)));
    connect(this, SIGNAL(joueurPretServeur(QString)), this->gestionjoueur, SLOT(joueurPret(QString)));
    connect(this, SIGNAL(joueurTchat(QString, QString)), this->gestionjoueur, SLOT(joueurTestTchatMot(QString, QString)));
    connect(this, SIGNAL(supprimerJoueur(QString)), this->gestionjoueur, SLOT(enleverJoueur(QString)));
    connect(this->gestionjoueur, SIGNAL(envoieParJoueur(QString, QString)), this, SLOT(envoyerAUnClient(QString, QString)));
    connect(this->gestionjoueur, SIGNAL(envoieTous(QString)), this, SLOT(envoyerATous(QString)));

}

//Destructeur
Serveur::~Serveur()
{
    serveur->close();
}

//Ecoute des clients
void Serveur::ecouteDesClients()
{
    while(serveur->listen(QHostAddress::Any, 4000))
    {
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }
}

void Serveur::nouvelleConnexion()
{
    QString ip = NULL;
    if(serveur->hasPendingConnections())
    {
        nouveauClient = serveur->nextPendingConnection();

        bool adresseConnecte = false;

        //Vérifie si un utilisateur avec la même adresse ip n'est pas connecté (Une instance par machine)
        for(int i = 0; i<this->clients.length();i++){
            if(this->clients[i]->peerAddress() == nouveauClient->peerAddress()){
                adresseConnecte = true;
            }
        }
        //Si il n'est pas déjà connecté un nouveau joueur est crée.
        //Un signal est connecté pour lire des données si jamais le client en envoie.
        if((!adresseConnecte) && (this->gestionjoueur->getJeuEnCours() == false)){
            clients << nouveauClient;
            ip = nouveauClient->peerAddress().toString();
            Joueur * joueur = new Joueur();
            joueur->setId(nouveauClient->peerAddress().toString());
            joueur->setJoue(0);
            joueur->setDessine(0);
            this->gestionjoueur->ajouterJoueur(joueur);

            connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
            connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
            this->envoyerATous("<tchatServeur>Un joueur vient de se connecter.</tchatServeur>");
        } else {
            nouveauClient->disconnectFromHost();
        }
    }
}

void Serveur::run()
{

}

//Traitement de la lecture et création d'un thread qui lira les données.
void Serveur::donneesRecues()
{
    QTcpSocket * socket = new QTcpSocket();
    socket = qobject_cast<QTcpSocket *>(sender());
    adresseIPSender = socket->peerAddress().toString();

    ThreadLecture * lectureThread = new ThreadLecture(socket);
    lectureThread->connect(lectureThread,SIGNAL(envoieMessage(QString)), this,SLOT(envoyerATous(QString)));
    socket->moveToThread(lectureThread);
    lectureThread->start();

}

//Slot lors de la deconnexion d'un client
void Serveur::deconnexionClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if (socket == 0)
        return;
    clients.removeOne(socket);
    //Réinitialise les informations de la partie pour pouvoir en relancer une
    if(this->clients.count() == 0){
        delete this->gestionjoueur;
        this->initialiseJeu();
    }
    emit supprimerJoueur(socket->peerAddress().toString()); 
    socket->deleteLater();
}

//Slot d'envoie a tout les utilisateurs connecter
void Serveur::envoyerATous(QString message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    //Test de la trame pour identifier ce qu'il faut faire dans chaque cas
    if(message.contains(QRegExp("</PosDessin>")))
    {
        for (int i = 0; i < clients.size(); i++)
        {

            if(clients[i]->peerAddress().toString() != adresseIPSender)
            {
                clients[i]->write(paquet);
            }
        }
    }
    if(message.contains(QRegExp("</pseudo>")))
    {
        emit joueurConnexion(adresseIPSender, message);
    }
    if(message.contains(QRegExp("</jouer>")))
    {      
        emit joueurPretServeur(adresseIPSender);
    }
    if(!message.contains(QRegExp("</PosDessin>")))
    {
        for (int i = 0; i < clients.size(); i++)
        {
            clients[i]->write(paquet);
        }
    }
    if(message.contains(QRegExp("</tchat>")))
    {
        if(this->gestionjoueur->getJeuEnCours() == true){
            emit joueurTchat(adresseIPSender, message);
        }
    }
}

//Slot d'envoie a un utilisateur en fonction de son adresse IP
void Serveur::envoyerAUnClient(QString message, QString adresseIP)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    for (int i = 0; i < clients.size(); i++)
    {
        if(clients[i]->peerAddress().toString() == adresseIP)
        {
            clients[i]->write(paquet);
        }
    }
}
