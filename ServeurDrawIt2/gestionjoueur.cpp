#include "gestionjoueur.h"

//Constructeur
GestionJoueur::GestionJoueur(){
    this->lectureXML = new LectureXMLFichier();
    this->listeMots = this->lectureXML->lectureXML();
    this->minuteur = new Minuteur();
    this->setJeuEnCours(false);
}

//Destructeur
GestionJoueur::~GestionJoueur()
{

}

//Setter/Getter du booléen permettant de savoir si une partie est en cours sur le serveur
void GestionJoueur::setJeuEnCours(bool jeuEnCours){
    this->jeuEnCours = jeuEnCours;
}

bool GestionJoueur::getJeuEnCours(){
    return this->jeuEnCours;
}

//Change l'état du joueur sur le serveur en pret pour jouer
void GestionJoueur::joueurPret(QString ip){
    for(int i =0; i<this->listeJoueur.length(); i++) {
        if(this->listeJoueur[i]->getId() == ip){
            this->listeJoueur[i]->setJoue(true);
            this->messageTchat(this->listeJoueur[i]->getPseudo() + " est prêt à jouer.");
            this->commencerPartie();
        }
    }
}

//Recupère un joueur en fonction de leur id (Adresse IP)
Joueur *  GestionJoueur::getJoueurParId(QString id){
    for(int i =0; i<this->listeJoueur.length(); i++) {
        if(this->listeJoueur[i]->getId() == id){
            return this->listeJoueur[i];
        }
    }
    return NULL;
}

//Supprime un joueur de la liste
void GestionJoueur::enleverJoueur(QString ip){
    for(int i =0; i<this->listeJoueur.length(); i++) {
        if(this->listeJoueur[i]->getId() == ip){
            this->messageJoueur(1,this->listeJoueur[i]);
            this->messageTchat(this->listeJoueur[i]->getPseudo() + " s'est déconnecter.");
            this->listeJoueur.removeAt(i);
        }
    }
}

//Ajoute un joueur
void GestionJoueur::ajouterJoueur(Joueur *joueur){
    this->listeJoueur.append(joueur);
}

void GestionJoueur::commencerPartie(){
    bool jouer = true;
    //Vérifie que tout les joueurs sont prets
    if(this->listeJoueur.count() > 1){
        for(int i =0; i<this->listeJoueur.length(); i++) {
            if(this->listeJoueur[i]->getJoue() == false){
                jouer = false;
            }
        }
    }else{
        jouer = false;
        this->messageTchat("Vous devez au moins être deux pour jouer, attendez qu'un autre joueur vous rejoigne et soit prêt.");
    }
    //Si tout le monde est prêt lance le minuteur et remet a zero les points
    //Envoie des messages pour avertir le client qu'il dessine
    if(jouer){
        this->listeJoueur[0]->setDessine(true);
        this->messageTchat("C'est au tour de " + this->listeJoueur[0]->getPseudo() + " de dessiner.");
        connect(minuteur, SIGNAL(timeout()), this, SLOT(minuteurTic()));
        this->minuteur->lancerMinuteur();
        this->nbPoint = 3;
        this->nbTic = 0;
        this->messageDessine();
        this->messageTime();
        this->choisirMotAleatoire();
        this->setJeuEnCours(true);
        this->messageTchat("La partie commence.");
    }
}

void GestionJoueur::choisirMotAleatoire(){
    srand (time(NULL));
    int random = rand() % this->listeMots->count();
    this->motChoisis = this->listeMots->at(random);
    this->messageMot();
    this->listeMots->removeAt(random);
}

void GestionJoueur::messageDessine(){
    for(int i =0; i<this->listeJoueur.length(); i++) {
        QString message;
        bool toto = this->listeJoueur[i]->getDessine();
        QString valide;
        if(toto)
            valide = "true";
        else
            valide = "false";
        message += "<dessin>";
        message += valide;
        message += "</dessin>";
        emit envoieParJoueur(message, this->listeJoueur[i]->getId());
    }
}

void GestionJoueur::messageMot(){
    for(int i =0; i<this->listeJoueur.length(); i++) {
        if(this->listeJoueur[i]->getDessine() == true){
            QString message;
            message += "<mot>";
            message += this->motChoisis;
            message += "</mot>";
            emit envoieParJoueur(message, this->listeJoueur[i]->getId());
        }
    }
}


void GestionJoueur::changerTours(){
       int index = 0;
       for(int i =0; i<this->listeJoueur.length(); i++) {
           if(this->listeJoueur[i]->getDessine() == true){
                index = i;
                this->listeJoueur[i]->setDessine(false);
           }
       }
       if(index < this->listeJoueur.length()-1){
            this->listeJoueur[index+1]->setDessine(true);
           this->messageTchat("C'est au tour de " + this->listeJoueur[index+1]->getPseudo() + " de dessiner.");
       } else {
           this->listeJoueur[0]->setDessine(true);
           this->messageTchat("C'est au tour de " + this->listeJoueur[0]->getPseudo() + " de dessiner.");
       }
       this->choisirMotAleatoire();
       this->messageDessine();
}

//Chaque seconde ce slot est appelé pour mettre a jour le nombres de points
//à gagner et vérifie si le temps est écoulé
void GestionJoueur::minuteurTic(){
        if(this->minuteur->timePassed == 30 || this->minuteur->timePassed == 60 || this->minuteur->timePassed == 90){
            if(this->nbTic == 2){
                this->changerTours();
                this->nbTic = 0;
                this->nbPoint = 3;
            } else {
                this->nbTic ++;
                this->nbPoint --;
            }
        }
        if(this->minuteur->timePassed >= 90){
            this->minuteur->timePassed = 0;
        }
        this->messageTime();
        this->minuteur->timePassed ++;
}

//Methode de test pour savoir si le mot à deviné est trouvé
void GestionJoueur::joueurTestTchatMot(QString ip, QString message){
    Joueur *joueur = this->getJoueurParId(ip);
    if(joueur->getDessine() == false){
        int indice1 =  message.indexOf("=");
        int indice2 = message.indexOf(">", indice1);
        QString nomDuJoueurAdversse = message.mid(indice1+1,(indice2-indice1)-1);
        message.remove("<tchat pseudo=");
        message.remove("</tchat>");
        message.remove(">");
        message.remove(nomDuJoueurAdversse);
        message.remove("\n");
        if(message == this->motChoisis){
            int points = this->nbPoint;
            this->nbTic = 0;
            this->nbPoint = 3;
            this->minuteur->timePassed = 0;

            joueur->setScore(joueur->getScore() + points);
            this->messageScore(joueur);


            for(int i = 0; i<this->listeJoueur.length(); i++){
                if(this->listeJoueur[i]->getDessine() == true){
                    this->listeJoueur[i]->setScore(this->listeJoueur[i]->getScore() + 1);
                    this->messageScore(this->listeJoueur[i]);
                }
            }
            this->changerTours();
        }
    }
}

//Envoi la liste des joueurs aux utilisateurs ainsi que les nouveaux joueurs se connectant
void GestionJoueur::joueurNouveau(QString ip, QString message){
    if(this->getJoueurParId(ip)->getDessine() == false){
        message.remove("<pseudo>");
        message.remove("</pseudo>");
        message.remove("\n");
        for(int i = 0; i<this->listeJoueur.length(); i++){
            if(this->listeJoueur[i]->getId() == ip){
                this->listeJoueur[i]->setPseudo(message);
            }
        }
    }
    if(this->listeJoueur.length() > 0){
        for(int i =0; i <this->listeJoueur.length(); i++){
            this->messageJoueurById(0,this->listeJoueur[i], ip);
        }
    }
    for(int i =0; i <this->listeJoueur.length(); i++){
        if(this->listeJoueur[i]->getId() == ip){
            this->messageJoueur(0,this->listeJoueur[i]);
        }
    }
}

//Méthodes de création de message avec balise xml
void GestionJoueur::messageJoueur(bool etat, Joueur *joueur){
    QString message;
    message += "<joueur etat=";
    message += QString::number(etat);
    message += ">";
    message += "<id>";
    message += joueur->getId();
    message += "</id>";
    message += "<name>";
    message += joueur->getPseudo();
    message += "</name>";
    message += "<points>";
    message += QString::number(joueur->getScore());
    message += "</points>";
    message += "</joueur>";
    emit envoieTous(message);

}

void GestionJoueur::messageJoueurById(bool etat, Joueur *joueur, QString ip){
    QString message;
    message += "<joueur etat=";
    message += QString::number(etat);
    message += ">";
    message += "<id>";
    message += joueur->getId();
    message += "</id>";
    message += "<name>";
    message += joueur->getPseudo();
    message += "</name>";
    message += "<points>";
    message += QString::number(joueur->getScore());
    message += "</points>";
    message += "</joueur>";
    emit envoieParJoueur(message, ip);

}

void GestionJoueur::messageScore(Joueur *joueur){
    QString message;
    message += "<score id=";
    message += joueur->getId();
    message += ">";
    message += QString::number(joueur->getScore());
    message += "</score>";
    emit envoieTous(message);

}

void GestionJoueur::messageTime(){
    QString message;
    message += "<temps>";
    message += QString::number(this->minuteur->timePassed);
    message += "</temps>";
    emit envoieTous(message);
}

void GestionJoueur::messageTchat(QString tchatMessage){
    QString message;
    message += "<tchatServeur>";
    message += tchatMessage;
    message += "</tchatServeur>";
    emit envoieTous(message);
}

int GestionJoueur::getNbPoint(){
    return this->nbPoint;
}
