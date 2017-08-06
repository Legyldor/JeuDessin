#include "threadenvoidonnees.h"

ThreadEnvoiDonnees::ThreadEnvoiDonnees()
{

}

ThreadEnvoiDonnees::ThreadEnvoiDonnees(QString nomDujoueur,QString messageClient,int typeMessageClient,QTcpSocket* socketClient)
{
  nomjoueur = nomDujoueur;
  message = messageClient;
  typeMessage = typeMessageClient;
  socket = socketClient;

}

void ThreadEnvoiDonnees::setInformation(QString nomDuJoueur, QString messageClient, int typeMessageClient, QTcpSocket *socketClient)
{
    nomjoueur = nomDuJoueur;
    message = messageClient;
    typeMessage = typeMessageClient;
    socket = socketClient;

}


ThreadEnvoiDonnees::~ThreadEnvoiDonnees()
{

}

void ThreadEnvoiDonnees::run()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    QString xmlMessage;


    if(typeMessage == 1)
        xmlMessage = serviceXml->EcrireTchatMessage(message, nomjoueur);

    else if(typeMessage == 3)
        xmlMessage = serviceXml->EcrirJoueurVeutJouer(message);
    else
        xmlMessage = serviceXml->EcrireCoordonnerMessage(message);

    out << (quint16) 0;
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

    //socket->flush();
}

