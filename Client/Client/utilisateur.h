#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include <QObject>

class Utilisateur : public QObject
{
    Q_OBJECT
public:
    Utilisateur(QString pseudo, int score, QString couleur);
    ~Utilisateur();

    void setPseudo(QString pseudo);
    QString getPseudo();
    void setScore(int score);
    int getScore();
    void setCouleur(QString couleur);
    QString getCouleur();

private:
    QString pseudo;
    int score;
    QString couleur;

signals:

public slots:

};

#endif // UTILISATEUR_H
