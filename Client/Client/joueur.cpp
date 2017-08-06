#include "joueur.h"

Joueur::Joueur()
{
    this->pseudo = "unnamed";
    this->joue = false;
    this->dessine = false;
    this->score = 0;
}

Joueur::~Joueur()
{

}
void Joueur::setScore(long score){
    this->score = score;
}

long Joueur::getScore(){
    return this->score;
}

void Joueur::setDessine(bool dessine){
    this->dessine = dessine;
}

void Joueur::setPseudo(QString pseudo){
    this->pseudo = pseudo;
}

void Joueur::setId(QString id){
    this->id = id;
}

QString Joueur::getPseudo(){
    return this->pseudo;
}

bool Joueur::getDessine(){
    return this->dessine;
}

QString Joueur::getId(){
    return this->id;
}

bool Joueur::getJoue(){
    return this->joue;
}

void Joueur::setJoue(bool joue){
    this->joue = joue;
}
