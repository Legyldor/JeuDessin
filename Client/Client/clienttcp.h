#ifndef CLIENTTCP_H
#define CLIENTTCP_H

#include <QString>
#include <QTcpSocket>
#include "dessin.h"
#include "xmlservice.h"
#include "threadenvoidonnees.h"
#include "threadlecturedonnees.h"
#include "joueur.h"

class ClientTcp : public QObject
{
Q_OBJECT
public :
    ClientTcp();
    ~ClientTcp();

    void recoit_IP(QString,qint16);
    void envoie_message(QString message, int typeMessage, QString nomJoueur);
    void setNomDuJoueur(QString nom);


private slots:
    void donneesRecues();
    void connecte();
    void deconnecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void informationsPointDessin(int, int, int, int, int, int);
    void traitementDonnesLue(QString messageRecu);
    void envoieJoueur(QString,QString,QString,QString);
    void nouveauScore(QString, QString);


 signals :
    void vers_IHM_Texte(QString);
    void vers_IHM_Texte_Erreur(QString);
    void vers_IHM_Texte_Ok(QString);
    void vers_IHM_Texte_Chat(QString);
    void vers_IHM_Texte_Mot(QString);
    void vers_IHM_Dessine_Ou_Pas(QString);
    void vers_IHM_temps(QString);
    void vers_IHM_Mot(QString);
    void vers_IHM_Score(QString);
    void vers_IHM_Connexion_Etablie();
    void dessineCoordonner(int x, int y, int eventType, int cliqueType, int tailleStylo, int couleurStylo);
    void envoiePres();
    void envoieJoueurComplet(QString,QString,QString,QString);
    void envoieScoreIHM(QString,QString);



private:
    QTcpSocket *socket; // Représente le serveur
    quint16 tailleMessage;
    QString ip;
    qint16 port;
    xmlService *serviceXml;
    QString nomDuJoueur;
    int bufferEnvoie;
    ThreadEnvoiDonnees * envoieMessage;




};

#endif // CLIENTTCP_H
