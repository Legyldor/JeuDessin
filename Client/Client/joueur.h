#ifndef JOUEUR_H
#define JOUEUR_H

#include <QObject>

class Joueur : public QObject
{
    Q_OBJECT

public:

     Joueur();
    ~Joueur();

    void setDessine(bool dessine);
    void setPseudo(QString pseudo);
    void setId(QString id);
    void setJoue(bool joue);
    void setScore(long score);

    bool getDessine();
    QString getPseudo();
    QString getId();
    bool getJoue();
    long getScore();


private:
        Q_DISABLE_COPY(Joueur)
        bool dessine;
        QString pseudo;
        QString id;
        bool joue;
        long score;

signals:

public slots:
};

#endif // JOUEUR_H
