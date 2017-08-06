#include "clienttcp.h"


ClientTcp::ClientTcp()
{
    serviceXml = new xmlService();
    socket = new QTcpSocket(this);

    envoieMessage = new ThreadEnvoiDonnees();

    connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));

    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(envoiePres()),envoieMessage, SLOT(start()));
    connect(serviceXml,SIGNAL(envoieJoueurDonnees(QString,QString,QString,QString)), this, SLOT(envoieJoueur(QString,QString,QString,QString)));
    connect(serviceXml,SIGNAL(majScoreJoueur(QString,QString)), this,SLOT(nouveauScore(QString,QString)));

    tailleMessage = 0;
    bufferEnvoie = 0;

}

ClientTcp::~ClientTcp()
{
    socket->close();
}

void ClientTcp::setNomDuJoueur(QString nom)
{
     nomDuJoueur = nom;
}

void ClientTcp::informationsPointDessin(int x, int y, int eventType, int cliqueType, int tailleStylo, int couleurStylo)
{
    QString coordonnerPointDessin = QString::number(x) + "," + QString::number(y) + "," + QString::number(eventType) + "," + QString::number(cliqueType) + "," + QString::number(tailleStylo) + "," + QString::number(couleurStylo);
    this->envoie_message(coordonnerPointDessin, 2, nomDuJoueur);
}

void ClientTcp::recoit_IP(QString IP2, qint16 Port2)
{

    emit vers_IHM_Texte_Ok("Tentative de connexion en cours...");
    ip=IP2;
    port= Port2;
    socket->abort(); //abandonne la dernièer connexion si il y avait une dernière connexion
    socket->connectToHost(ip, 4000); //connexion
}

void ClientTcp::envoie_message(QString message, int typeMessage, QString nomJoueur)
{

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    QString xmlMessage;


    if(typeMessage == 1)
        xmlMessage = serviceXml->EcrireTchatMessage(message, nomJoueur);

    else if(typeMessage == 3)
        xmlMessage = serviceXml->EcrirJoueurVeutJouer(message);
    else if(typeMessage == 2)
        xmlMessage = serviceXml->EcrireCoordonnerMessage(message);
    else if(typeMessage == 4)
        xmlMessage = message;


    out << (quint16) 0;
    xmlMessage += "\n";
    out << xmlMessage;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    if(socket->open(QIODevice::ReadWrite))
    {
       if(socket->isReadable()) // lisible
       {
           if(socket->isWritable()) //Ecrivable
           {
               if(socket->isValid()) // non corrumpu
               {
                   socket->write(paquet); //Envoi du paquet au serveur
               }
           }
       }
    }
}
void ClientTcp::donneesRecues()
{
    do{
       QDataStream in(socket);

        if (tailleMessage == 0)
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16))
                 return;
            in >> tailleMessage;
        }


        if (socket->bytesAvailable() < tailleMessage)
            return;

        //le message est complet, on le recupère !
        QString messageRecu;
        in >> messageRecu;


        if(messageRecu.contains(QRegExp("</tchat>")))
        {
            QString messageTraiter = serviceXml->TraitementMessageRecuTchat(messageRecu);
            emit vers_IHM_Texte_Chat(messageTraiter);
        }

        if(messageRecu.contains(QRegExp("</tchatServeur>")))
        {
            QString messageTraiter = serviceXml->TraitementMessageRecuTchatServeur(messageRecu);
            emit vers_IHM_Texte_Ok(messageTraiter);
        }

        if(messageRecu.contains(QRegExp("</PosDessin>")))
        {
            messageRecu.remove("<PosDessin>");
            messageRecu.remove("</PosDessin>");

            QStringList list = QString(messageRecu).split(",");
            QString x = list[0];
            QString y = list[1];
            QString eventType = list[2];
            QString cliqueType = list[3];
            QString tailleStylo = list[4];
            QString couleurStylo = list[5];
            emit dessineCoordonner(x.toInt(), y.toInt(), eventType.toInt(), cliqueType.toInt(), tailleStylo.toInt(), couleurStylo.toInt());
        }
        if(messageRecu.contains(QRegExp("</dessin>")))
        {
            QString messageTraiter = serviceXml->TaitementDessinOuiOuNon(messageRecu);
            emit vers_IHM_Dessine_Ou_Pas(messageTraiter);
        }
        if(messageRecu.contains(QRegExp("</temps>")))
        {
            QString messageTaiter = serviceXml->TraitementTempsRecus(messageRecu);
            emit vers_IHM_temps(messageTaiter);
        }
        if(messageRecu.contains(QRegExp("</mot>")))
        {
            QString messageTaiter = serviceXml->TraitementMotRecus(messageRecu);
            emit vers_IHM_Mot(messageTaiter);
        }
        if(messageRecu.contains(QRegExp("</score>")))
        {
            serviceXml->TraitementScore(messageRecu);
        }
        if(messageRecu.contains(QRegExp("<joueur etat")))
        {
            serviceXml->TraitementJoueur(messageRecu);
        }
        tailleMessage = 0;

    }while(socket->bytesAvailable()> 0);
}

void ClientTcp::envoieJoueur(QString etat, QString id, QString nom, QString point)
{
    emit envoieJoueurComplet(etat,id,nom,point);
}

void ClientTcp::nouveauScore(QString id, QString score)
{
    emit envoieScoreIHM(id,score);
}

void ClientTcp::traitementDonnesLue(QString messageRecu)
{
    if(messageRecu.contains(QRegExp("</tchat>")))
    {
        QString messageTraiter = serviceXml->TraitementMessageRecuTchat(messageRecu);
        emit vers_IHM_Texte_Chat(messageTraiter);
    }

    if(messageRecu.contains(QRegExp("</PosDessin>")))
    {
        messageRecu.remove("<PosDessin>");
        messageRecu.remove("</PosDessin>");

        QStringList list = QString(messageRecu).split(",");
        QString x = list[0];
        QString y = list[1];
        QString eventType = list[2];
        QString cliqueType = list[3];
        QString tailleStylo = list[4];
        QString couleurStylo = list[5];
        emit dessineCoordonner(x.toInt(), y.toInt(), eventType.toInt(), cliqueType.toInt(), tailleStylo.toInt(), couleurStylo.toInt());
    }
    if(messageRecu.contains(QRegExp("</dessin>")))
    {
        QString messageTraiter = serviceXml->TraitementMotRecus(messageRecu);
        emit vers_IHM_Texte_Mot(messageTraiter);
    }

}

void ClientTcp::connecte()
{
    emit vers_IHM_Texte_Ok("Connexion réussie !");
    emit vers_IHM_Connexion_Etablie();
    this->envoie_message("<pseudo>"+nomDuJoueur+"</pseudo>",4,nomDuJoueur);

}

void ClientTcp::deconnecte()
{
    emit vers_IHM_Texte_Erreur("Déconnecté du serveur");
}

void ClientTcp::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur)
    {
        case QAbstractSocket::HostNotFoundError:
            emit vers_IHM_Texte_Erreur("ERREUR : le serveur n'a pas pu être trouvé. Vérifiez l'IP.");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            emit vers_IHM_Texte_Erreur("ERREUR : le serveur a refusé la connexion. Vérifiez si le programme serveur a bien été lancé. Vérifiez aussi l'IP.");
            break;
        case QAbstractSocket::RemoteHostClosedError:
            emit vers_IHM_Texte_Erreur("ERREUR : le serveur a coupé la connexion.");
            break;
        default:
            emit vers_IHM_Texte_Erreur("ERREUR : " + socket->errorString());
    }
}
