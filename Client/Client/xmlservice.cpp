#include "xmlservice.h"

xmlService::xmlService(QObject *parent) :
    QObject(parent)
{
    xmlEcriture.setAutoFormatting(true);

}

QString xmlService::EcrirJoueurVeutJouer(QString message)
{
    QString xmlMessage;
    xmlMessage = "<jouer>"+message+ "</jouer>";
    return xmlMessage;

}

QString xmlService::EcrireTchatMessage(QString message, QString nomJoueur)
{
    QString xmlMessage;
    xmlMessage = "<tchat pseudo="+nomJoueur+">"+message+ "</tchat>";
    return xmlMessage;
}

QString xmlService::EcrireCoordonnerMessage(QString message)
{
    QString xmlMessage;
    xmlMessage = "<PosDessin>"+message+ "</PosDessin>";
    return xmlMessage;
}


QString xmlService::TraitementMessageRecuTchat(QString messageRecu)
{
    int indice1 =  messageRecu.indexOf("=");
    int indice2 = messageRecu.indexOf(">", indice1);
    QString nomDuJoueurAdversse = messageRecu.mid(indice1+1,(indice2-indice1)-1);
    messageRecu.remove("<tchat pseudo=");
    messageRecu.remove("</tchat>");
    messageRecu.remove(">");
    messageRecu.remove(nomDuJoueurAdversse);
    messageRecu = nomDuJoueurAdversse + " : " + messageRecu;

    return messageRecu;
}

QString xmlService::TraitementMessageRecuTchatServeur(QString messageRecu)
{
    messageRecu.remove("<tchatServeur>");
    messageRecu.remove("</tchatServeur>");
    messageRecu = messageRecu;

    return messageRecu;
}

QString xmlService::TaitementDessinOuiOuNon(QString motRecus)
{
    motRecus.remove("<dessin>");
    motRecus.remove("</dessin>");
    return motRecus;

}

QString xmlService::TraitementTempsRecus(QString motRecus)
{
    motRecus.remove("<temps>");
    motRecus.remove("</temps>");
    return motRecus;
}

QString xmlService::TraitementMotRecus(QString motRecu)
{
    motRecu.remove("<mot>");
    motRecu.remove("</mot>");
    return motRecu;
}

void xmlService::TraitementScore(QString scoreRecu)
{

    int index = scoreRecu.indexOf("</score>");
    QString score = scoreRecu.left(index);
    score.remove("<score id=");
    index = score.indexOf(">");
    QString id = score.left(index);
    score.remove(id);
    score.remove(">");

    QString scoreFinal = score.right(index);


    emit majScoreJoueur(id,scoreFinal);
}
void xmlService::TraitementJoueur(QString joueurRecu)
{
    int index = joueurRecu.indexOf("<id>");
    QString etat = joueurRecu.left(index);
    joueurRecu.remove(etat);
    etat.remove("<joueur etat=");
    etat.remove(">");

    index = joueurRecu.indexOf("<name>");
    QString id = joueurRecu.left(index);
    joueurRecu.remove(id);
    id.remove("<id>");
    id.remove("</id>");

    index = joueurRecu.indexOf("<points>");
    QString nom = joueurRecu.left(index);
    joueurRecu.remove(nom);
    nom.remove("<name>");
    nom.remove("</name>");

    index = joueurRecu.indexOf("</joueur>");
    QString points = joueurRecu.left(index);
    joueurRecu.remove(points);
    points.remove("<points>");
    points.remove("</points>");

    emit envoieJoueurDonnees(etat,id,nom,points);
}
