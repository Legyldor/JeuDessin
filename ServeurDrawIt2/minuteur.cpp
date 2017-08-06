#include "minuteur.h"


Minuteur::Minuteur(QTimer *parent) :
    QTimer(parent)
{
    timePassed = 0;
    this->setInterval(1000);
}

void Minuteur::lancerMinuteur(){
    qDebug()<<"lancerMinuteur()";
    this->start();
}
