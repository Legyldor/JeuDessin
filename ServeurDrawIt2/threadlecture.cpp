#include "threadlecture.h"

//Constructeur
ThreadLecture::ThreadLecture(QTcpSocket * socket)
{
    socketClient = socket;
    stop = true;
    tailleMessage = 0;
}

//Destructeur
ThreadLecture::~ThreadLecture()
{

}

void ThreadLecture::run()
{

    //Lecture de la trame puis elle est renvoyé
    //dans le signal pour être renvoyé aux autres utilisateurs
    //ou être traité différemment
        while(socketClient->bytesAvailable())
        {
            if (socketClient == 0)
            {
                return;
            }
            QDataStream in(socketClient);

            if (tailleMessage == 0)
            {
               if (socketClient->bytesAvailable() < (int)sizeof(quint16))
                    return;
               in >> tailleMessage;
            }

            if (socketClient->bytesAvailable() < tailleMessage)
               return;

            QString message;
           in >> message;
            tailleMessage = 0;
            emit envoieMessage(message);
        }
}

