#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clienttcp.h"
#include <QTimer>
#include <windows.h>
#include "xmlService.h"
#include "joueur.h"


namespace Ui {
class MainWindow;
}

class Dessin;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Dessin *monDessin;
    void gestionBarreOutils();
    void ajouterJoueurScore(Joueur *joueur);
    bool plusPetitQue(const QString &v1, const QString &v2);

  public slots :
    void Connexion();
    void EcrirLog(QString);
    void EcrirLogChat(QString);
    void EcrirLogErreur(QString);
    void EcrirLogOk(QString);
    void EcrirMot(QString);
    void EnvoyerMessage();
    void ConnexionEtablie();
    void OuvrirPalette();
    void ValeurSlider(int);
    void veutJouer();
    void viderLaFenetre();
    void dessineOuPas(QString);
    void compteur(QString);
    void motATrouver(QString);
    void miseAJourScore(QString, QString);
    void fenetreAbout();
    void ouvrirDocumentation();
    void ouvrirRegleDuJeu();
    void donnesJoueur(QString etat,QString id, QString nom, QString point);
    void rafraichirListe();
    void supprimerJoueur(QString id);

private:
    Ui::MainWindow *ui;
    ClientTcp client;
    QTimer * compteARebour;
    int compteurSeconde, compteurTampon;
    bool centaine, dizaine;
    QToolButton *btnBarreOutils;
    QList < Joueur *> listeJoueur;
    QList <QString> scoreJoueurs;
    bool doublons;
    QString buffer;
    bool nouvellePartie;

};

#endif // MAINWINDOW_H
