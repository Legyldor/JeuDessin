#ifndef GESTIONJOUEUR_H
#define GESTIONJOUEUR_H

#include <QObject>
#include "joueur.h"
#include "minuteur.h"
#include "lecturexmlfichier.h"
#include <stdlib.h>
#include <time.h>

class GestionJoueur : public QObject
{
    Q_OBJECT
public:
    GestionJoueur();
    ~GestionJoueur();
        QList<Joueur> getListeJoueur();
        Joueur *getJoueurParId(QString id);
        void ajouterJoueur(Joueur *joueur);
        void commencerPartie();
        int getNbPoint();
        Minuteur *minuteur;
        LectureXMLFichier *lectureXML;


        void instanceTest();
        void changerTours();
        void messageDessine();
        void messageTime();
        void messageMot();
        void messageJoueurById(bool etat, Joueur *joueur, QString ip);
        void messageScore(Joueur *joueur);
        void messageJoueur(bool etat, Joueur *joueur);
        void messageTchat(QString tchatMessage);
        void choisirMotAleatoire();

        void setJeuEnCours(bool);
        bool getJeuEnCours();
    signals:

    private:
        Q_DISABLE_COPY(GestionJoueur)
        QList<Joueur*> listeJoueur;
        QList<QString> *listeMots;
        int nbTic;
        int nbPoint;
        bool jeuEnCours;
        QString motChoisis;
signals:
    void envoieParJoueur(QString msg, QString addresse);
    void envoieTous(QString msg);
public slots:
    void minuteurTic();
    void joueurPret(QString ip);
    void enleverJoueur(QString ip);
    void joueurTestTchatMot(QString ip, QString message);
    void joueurNouveau(QString ip, QString message);
};

#endif // GESTIONJOUEUR_H
