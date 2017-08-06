#ifndef SERVEUR_H
#define SERVEUR_H

#include <QtGui>
#include <QtNetwork>
#include <QWidget>
#include <QThread>
#include "gestionjoueur.h"
#include "threadlecture.h"
#include "joueur.h"

class Serveur : public QThread
{
    Q_OBJECT

    public:
        Serveur();
        ~Serveur();

        void ecouteDesClients();
        virtual void run();
        void initialiseJeu();


    private slots:
        void nouvelleConnexion();
        void donneesRecues();
        void deconnexionClient();
        void envoyerATous(QString);
        void envoyerAUnClient(QString message, QString adresseIP);

    signals:
        void joueurPretServeur(QString);
        void joueurConnexion(QString, QString);
        void joueurTchat(QString, QString);
        void supprimerJoueur(QString);

    private:

        QList<QTcpSocket *> clients;
        QTcpServer *serveur;
        GestionJoueur *gestionjoueur;
        quint16 tailleMessage;
        QTcpSocket *nouveauClient;
        QTcpSocket *socketTest;
        QString adresseIPSender;
        ThreadLecture * threadLectureSocket;
        int indice;

};
#endif // SERVEUR_H
