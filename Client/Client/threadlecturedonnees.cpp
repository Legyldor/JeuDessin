#include "threadlecturedonnees.h"

ThreadLectureDonnees::ThreadLectureDonnees(QTcpSocket *socketClient)
{

    socket = socketClient;
    tailleMessage = 0;

}

ThreadLectureDonnees::~ThreadLectureDonnees()
{

}


void ThreadLectureDonnees::run()
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
        tailleMessage = 0;
        emit donneLue(messageRecu);

    }while(socket->size() > 0);
}
