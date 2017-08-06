#ifndef THREADENVOIDONNEES_H
#define THREADENVOIDONNEES_H

#include <QThread>
#include <QTcpSocket>
#include "xmlservice.h"

class ThreadEnvoiDonnees : public QThread
{
public:
    ThreadEnvoiDonnees();
    explicit ThreadEnvoiDonnees(QString nomDuJoueur,QString messageClient, int typeMessageClient, QTcpSocket* socketClient);
    ~ThreadEnvoiDonnees();

    void setInformation(QString nomDuJoueur,QString messageClient, int typeMessageClient, QTcpSocket* socketClient);


    void run();
    private :

        QString nomjoueur;
        QString message;
        int typeMessage;
        QTcpSocket* socket;
        xmlService *serviceXml;
};

#endif // THREADENVOIDONNEES_H
