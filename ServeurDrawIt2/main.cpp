#include "serveur.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Serveur *serveur;
    serveur = new Serveur();

    return a.exec();
}
