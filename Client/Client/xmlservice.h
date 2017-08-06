#ifndef XMLSERVICE_H
#define XMLSERVICE_H

#include <QObject>
#include <QtNetwork>
#include <QtWidgets>
#include <QDomDocument>

class xmlService : public QObject
{
    Q_OBJECT
public:
    explicit xmlService(QObject *parent = 0);
    QString EcrireTchatMessage(QString message, QString nomJoueur);
    QString EcrireCoordonnerMessage(QString message);

    QString EcrirJoueurVeutJouer(QString message);


    QString TraitementMessageRecuTchatServeur(QString messageRecu);
    QString TraitementMessageRecuTchat(QString messageRecu);
    QString TaitementDessinOuiOuNon(QString motRecu);

    QString TraitementTempsRecus(QString motRecu);
    QString TraitementMotRecus(QString motRecu);
    void TraitementScore(QString motRecu);
    void TraitementJoueur(QString joueurRecu);


    QXmlStreamWriter xmlEcriture;



private:



signals:

    void envoieJoueurDonnees(QString etat, QString id, QString nom, QString points);
    void majScoreJoueur(QString id, QString score);


public slots:

};

#endif // XMLSERVICE_H
