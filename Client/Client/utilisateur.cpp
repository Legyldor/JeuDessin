#include "utilisateur.h"

Utilisateur::Utilisateur(QString pseudo, int score, QString couleur){
    this->setCouleur(couleur);
    this->setPseudo(pseudo);
    this->setScore(score);
}

Utilisateur::~Utilisateur(){

}

void Utilisateur::setCouleur(QString couleur){
    this->couleur = couleur;
}

QString Utilisateur::getCouleur(){
    return this->couleur;
}

void Utilisateur::setPseudo(QString pseudo){
    this->pseudo = pseudo;
}

QString Utilisateur::getPseudo(){
    return this->pseudo;
}

void Utilisateur::setScore(int score){
    this->score = score;
}

int Utilisateur::getScore(){
    return this->score;
}
