#include <QtWidgets>
#include "mainwindow.h"
#include "dessin.h"
#include "ui_mainwindow.h"
#include "clienttcp.h"
#include <QColorDialog>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->BouttonJouer->setEnabled(false);
    monDessin = new Dessin();
    compteurSeconde = 90;

    compteurTampon = 0;
    ui->GbConnexion->setHidden(false);
    monDessin->setEnabled(false);
    ui->BouttonEnvoyer->setShortcut(Qt::Key_Enter);
    ui->BouttonEnvoyer->setShortcut(Qt::Key_Return);
    ui->log->verticalScrollBar()->setSliderDown(true);
    connect(ui->ButtonConnexion,SIGNAL(clicked()), this,SLOT(Connexion()));
    connect(ui->BouttonEnvoyer, SIGNAL(clicked()), this, SLOT(EnvoyerMessage()));

    ui->logEnvoyer->setEnabled(false);
    ui->BouttonEnvoyer->setEnabled(false);
    ui->log->setReadOnly(true);
    ui->dessin->setReadOnly(true);
    ui->dessin->setEnabled(false);
    ui->verticalLayout->setEnabled(true);

    connect(ui->boutonPalette, SIGNAL(clicked()), this, SLOT(OuvrirPalette()));
    connect(ui->sliderLargeur, SIGNAL(valueChanged(int)), this, SLOT(ValeurSlider(int)));
    ui->labelCouleur->setPalette(monDessin->couleurStylo());
    ui->labelCouleur->setAutoFillBackground(true);

    ui->lcdNumber->display("1:30");
    ui->lcdNumber->setPalette(QColor(84,175,255,255));

    ui->ButtonConnexion->setIconSize(QSize(100,40));
    ui->ButtonConnexion->setIcon(QIcon(":/connexion.png"));

    ui->pseudo->setFocus();
    doublons = false;
    dizaine =false;

    this->gestionBarreOutils();
    ui->logListeJoueur->setReadOnly(true);
    ui->log->verticalScrollBar()->setStyleSheet("QScrollBar::up-arrow:vertical{color: blue;}");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gestionBarreOutils()
{
    QMenu *menuFichier = menuBar()->addMenu("?");


    QAction *actionAbout = menuFichier->addAction("À propos");
    QAction *actionDoc = menuFichier->addAction("Fonctionnement logiciel");
    QAction *actionRegle = menuFichier->addAction("Règles du jeu");

    connect(actionAbout, SIGNAL(triggered()), this,SLOT(fenetreAbout()));
    connect(actionDoc, SIGNAL(triggered()), this, SLOT(ouvrirDocumentation()));
    connect(actionRegle, SIGNAL(triggered()), this,SLOT(ouvrirRegleDuJeu()));

}

void MainWindow::Connexion()
{
   if(ui->pseudo->text() != "")
   {
       connect(&client, SIGNAL(vers_IHM_Texte(QString)), this, SLOT(EcrirLog(QString)),Qt::UniqueConnection);
       connect(&client,SIGNAL(vers_IHM_Dessine_Ou_Pas(QString)),this,SLOT(dessineOuPas(QString)),Qt::UniqueConnection);
       connect(&client, SIGNAL(vers_IHM_Texte_Chat(QString)), this, SLOT(EcrirLogChat(QString)),Qt::UniqueConnection);
       connect(&client, SIGNAL(vers_IHM_Texte_Erreur(QString)), this, SLOT(EcrirLogErreur(QString)),Qt::UniqueConnection);
       connect(&client, SIGNAL(vers_IHM_Texte_Ok(QString)), this, SLOT(EcrirLogOk(QString)),Qt::UniqueConnection);
       connect(&client,SIGNAL(vers_IHM_temps(QString)), this, SLOT(compteur(QString)),Qt::UniqueConnection);
       connect(&client,SIGNAL(vers_IHM_Mot(QString)), this,SLOT(motATrouver(QString)),Qt::UniqueConnection);
       connect(&client,SIGNAL(envoieScoreIHM(QString,QString)), this,SLOT(miseAJourScore(QString, QString)));
       connect(&client, SIGNAL(vers_IHM_Connexion_Etablie()), this, SLOT(ConnexionEtablie()),Qt::UniqueConnection);
       connect(monDessin, SIGNAL(coordonner_Point(int, int, int, int, int, int)), &client, SLOT(informationsPointDessin(int, int, int, int, int, int)),Qt::UniqueConnection);
       connect(&client, SIGNAL(vers_IHM_Texte_Mot(QString)), this, SLOT(EcrirMot(QString)));
       connect(&client, SIGNAL(dessineCoordonner(int,int,int,int,int,int)), monDessin, SLOT(setCoordonner(int,int,int,int,int,int)),Qt::UniqueConnection);
       connect(ui->BouttonJouer, SIGNAL(clicked()), this, SLOT(veutJouer()));
       connect(&client,SIGNAL(envoieJoueurComplet(QString,QString,QString,QString)),this,SLOT(donnesJoueur(QString,QString,QString,QString)),Qt::UniqueConnection);
       QString ip = ui->adresseIp->text();
       qint16 port = 4000;
       client.setNomDuJoueur(ui->pseudo->text());

       client.recoit_IP(ip, port);
   }
   else{
        EcrirLogErreur("ERREUR : Pseudo incorrecte.");
   }
}

bool MainWindow::plusPetitQue(const QString &v1, const QString &v2)
{
   return v1.toInt() < v2.toInt();
}

void MainWindow::motATrouver(QString mot)
{
    ui->dessin->setText(mot);
}

void MainWindow::compteur(QString temps)
{
    compteurSeconde -= temps.toInt();
    if( temps.toInt() == 0)
    {
        ui->lcdNumber->display("1:30");
        compteurSeconde = 90;
        dizaine= false;
    }
    QString affichage;
    if(compteurSeconde == 69)
    {
        dizaine = true;
    }

    if( (compteurSeconde > 60) && dizaine == false)
        affichage = "1:"+QString::number(compteurSeconde-60);
    else if(dizaine && (compteurSeconde <= 69))
        affichage = "1:0"+QString::number(compteurSeconde-60);
    else if(compteurSeconde == 60)
        affichage = "1:0"+QString::number(compteurSeconde-60);

    if(compteurSeconde < 60)
    {
        if(compteurSeconde < 10)
            affichage = "0:0"+QString::number(compteurSeconde);
        else
            affichage = "0:"+QString::number(compteurSeconde);
    }

    ui->lcdNumber->display(affichage);
    compteurSeconde = 90;
}

void MainWindow::dessineOuPas(QString message)
{
    this->viderLaFenetre();
    if(message == "true")
    {
        monDessin->setEnabled(true);
    }
    else if (message == "false")
    {
        monDessin->setEnabled(false);
        ui->dessin->setText("");
    }

}

void MainWindow::EnvoyerMessage()
{
    if(ui->logEnvoyer->text() != "")
    {
        QString message =  ui->logEnvoyer->text();
        QString nomJoueur = ui->pseudo->text();
        client.envoie_message(message, 1,nomJoueur );
        ui->logEnvoyer->clear();
        ui->logEnvoyer->setFocus();

    }
    else
    {
       this->EcrirLogErreur("Ne pas envoyer de message vide. Merci !");
       ui->log->verticalScrollBar()->setSliderPosition(ui->log->verticalScrollBar()->maximum());
    }

}

void MainWindow::fenetreAbout()
{
    const QString text = "Cette application a été réalisée dans le cadre du cours de Projet Graphique au Cégep de Matane. \nLes développeurs de cette application, au nombre de quatre, sont : Joffrey Davaïc, Julien Le Corre, Léo Dedit et Alexandre Nottet.";
    const QString titre = "À propos de Draw-It !";
    QMessageBox fenetre;

    fenetre.setWindowTitle(titre);
    fenetre.setText(text);
    fenetre.setWindowIcon(QIcon(":/iconeAppli.png"));
    fenetre.exec();
}

void MainWindow::ouvrirDocumentation()
{
    QString emplacementFichier =  QCoreApplication::applicationDirPath();
    emplacementFichier+= "/documents/Documentation.pdf";

    if(!QDesktopServices::openUrl(QUrl(emplacementFichier)))
    {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier . . ."));
    }
}

void MainWindow::ouvrirRegleDuJeu()
{
    QString emplacementFichier =  QCoreApplication::applicationDirPath();
    emplacementFichier+= "/documents/RegleDuJeu.pdf";

    if(!QDesktopServices::openUrl(QUrl(emplacementFichier)))
    {
        QMessageBox::critical(this, tr("Erreur"), tr("Impossible d'ouvrir le fichier . . ."));
    }

}

void MainWindow::veutJouer()
{
    ui->BouttonJouer->setEnabled(false);
    client.envoie_message("true",3,ui->pseudo->text());
}

void MainWindow::EcrirLogErreur(QString message)
{
    if(ui->log->toPlainText() != "")
        ui->log->setHtml(ui->log->toHtml()+ "</br>" + "<font color =\"red\">" + message +"<\font>" );
    else
        ui->log->setText("<font color=\"red\">" + message + "<\font>");
    ui->log->verticalScrollBar()->setSliderPosition(ui->log->verticalScrollBar()->maximum());
}
void MainWindow::EcrirLogOk(QString message)
{
    if(ui->log->toPlainText() != "")
        ui->log->setHtml(ui->log->toHtml()+ "</br>" + "<font color =\"Green\">" + message +"<\font>" );
    else
        ui->log->setText("<font color=\"Green\">" + message + "<\font>");
    ui->log->verticalScrollBar()->setSliderPosition(ui->log->verticalScrollBar()->maximum());
}

void MainWindow::EcrirLog(QString message)
{
        ui->log->append(message);
        ui->log->verticalScrollBar()->setSliderPosition(ui->log->verticalScrollBar()->maximum());
}
void MainWindow::EcrirLogChat(QString message)
{
    if(ui->log->toPlainText() != "")
        ui->log->setHtml(ui->log->toHtml()+ "</br>" + "<font color =\"Blue\">" + message +"<\font>" );
    else
        ui->log->setText("<font color=\"blue\">" + message + "<\font>");
    ui->log->verticalScrollBar()->setSliderPosition(ui->log->verticalScrollBar()->maximum());
}
void MainWindow::ConnexionEtablie()
{
    ui->adresseIp->setEnabled(false);
    ui->pseudo->setEnabled(false);
    ui->ButtonConnexion->setEnabled(false);
    ui->BouttonJouer->setEnabled(true);
    ui->BouttonEnvoyer->setEnabled(true);
    ui->logEnvoyer->setEnabled(true);
    ui->dessin->setEnabled(true);
    ui->verticalLayout->setEnabled(true);
    ui->verticalLayout->addWidget(monDessin);
    ui->GbConnexion->setHidden(true);
    OuvrirPalette();
}

void MainWindow::EcrirMot(QString mot)
{
    ui->dessin->setText(mot);

}
void MainWindow::OuvrirPalette()
{
    QColor couleur = QColorDialog::getColor(monDessin->couleurStylo(), this, "Palette de couleurs");

    if(couleur.isValid()){
        ui->labelCouleur->setPalette(couleur);
        ui->labelCouleur->setAutoFillBackground(true);
        monDessin->setCouleurStylo(couleur);
    }
}

void MainWindow::ValeurSlider(int valeur)
{
    QString valeurText = QString::number(valeur);
    ui->labelLargeur->setText("Largeur du stylo : "+ valeurText );
    monDessin->setLargeurStylo(valeur);
}

void MainWindow::viderLaFenetre()
{
    int ancienneLargeur = monDessin->largeurStylo();
    QColor ancienneCouleur = monDessin->couleurStylo();
    delete monDessin;
    monDessin = new Dessin();
    ui->verticalLayout->addWidget(monDessin);
    connect(monDessin, SIGNAL(coordonner_Point(int, int, int, int, int, int)), &client, SLOT(informationsPointDessin(int, int, int, int, int, int)));
    connect(&client, SIGNAL(dessineCoordonner(int,int,int,int,int,int)), monDessin, SLOT(setCoordonner(int,int,int,int,int,int)));
    monDessin->setCouleurStylo(ancienneCouleur);
    monDessin->setLargeurStylo(ancienneLargeur);
}

void MainWindow::donnesJoueur(QString etat,QString id, QString nom, QString point)
{
    if(etat.toInt() == 0){
        Joueur *joueur = new Joueur();
        joueur->setId(id);
        joueur->setPseudo(nom);
        joueur->setScore(point.toInt());
        this->ajouterJoueurScore(joueur);
    } else if(etat.toInt() == 1){
        this->supprimerJoueur(id);
    }
}

void MainWindow::rafraichirListe(){
    ui->logListeJoueur->setText("");
    for(int i=0; i <listeJoueur.length(); i++)
    {
        QString message = listeJoueur[i]->getPseudo() + " : " + QString::number(listeJoueur[i]->getScore());
        ui->logListeJoueur->setText(ui->logListeJoueur->toPlainText()+message+"\n");
    }
}

void MainWindow::miseAJourScore(QString id, QString score)
{
    for(int i=0; i <listeJoueur.count(); i++)
    {
        if(listeJoueur[i]->getId() == id)
        {
            listeJoueur[i]->setScore(score.toLong());
        }
    }
    this->rafraichirListe();
}

void MainWindow::ajouterJoueurScore(Joueur* joueur)
{
    bool existant = false;
    for(int i =0; i<this->listeJoueur.length();i++){
        if(this->listeJoueur[i]->getId() == joueur->getId()){
            existant = true;
        }
    }
    if(!existant){
        this->listeJoueur.append(joueur);
        this->rafraichirListe();
    }
}

void MainWindow::supprimerJoueur(QString id){

    for(int i =0; i<this->listeJoueur.length();i++){
        if(this->listeJoueur[i]->getId() == id){
            this->listeJoueur.removeAt(i);
            this->rafraichirListe();
        }
    }
}
