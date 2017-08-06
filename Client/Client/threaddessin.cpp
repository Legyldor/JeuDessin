#include "threaddessin.h"

ThreadDessin::ThreadDessin()
{

}

ThreadDessin::ThreadDessin(int coorX, int coorY, int eventTypeClient, int cliqueTypeClient, int tailleStyloClient, int couleurStyloClient, QPoint dernierPointClient, bool gribouillageClient, QImage imageClient)
{

    x = coorX;
    y = coorY;
    eventType = eventTypeClient;
    cliqueType = cliqueTypeClient;
    tailleStylo = tailleStyloClient;
    couleurStylo = couleurStyloClient;
    dernierPoint = dernierPointClient;
    gribouillage = gribouillageClient;
    image = imageClient;

}

ThreadDessin::~ThreadDessin()
{

}

void ThreadDessin::run()
{
    QPoint pointRecup;
    pointRecup.setX(x);
    pointRecup.setY(y);
    setLargeurStylo(tailleStylo);
    setCouleurStylo(couleurStylo);



    if(eventType == 1)
    {
        if (cliqueType == 1) // Clique type 1
        {
            dernierPoint = pointRecup;
            gribouillage = true;
        }else if(cliqueType == 2)// Clique type 2
        {
            dernierPoint = pointRecup;
            gribouillage = true;
        }
    }else if (eventType == 2)
    {
        if (cliqueType == 1 && gribouillage) // Clique type 1
        {
            dessinerLigneA(pointRecup);

        }else if(cliqueType == 2 && gribouillage)// Clique type 2
        {
            dessinerLigneB(pointRecup);
        }
    }else if (eventType == 3)
    {
        if (cliqueType == 1 && gribouillage) // Clique type 1
        {
            dessinerLigneA(pointRecup);
            gribouillage = false;
        }else if(cliqueType == 2 && gribouillage)// Clique type 2
        {
            dessinerLigneB(pointRecup);
            gribouillage = false;
        }
    }

}


void ThreadDessin::dessinerLigneA(const QPoint &pointDeFin)
{
    QPainter peindre(&image);

    peindre.setPen(QPen(maCouleurStylo, maLargeurStylo, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    peindre.drawLine(dernierPoint, pointDeFin);

    int rad = (maLargeurStylo / 2) + 2;
    update(QRect(dernierPoint, pointDeFin).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    dernierPoint = pointDeFin;

}

void ThreadDessin::dessinerLigneB(const QPoint &pointDeFin)
{
    QPainter peindre(&image);
    peindre.setPen(QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap,Qt::RoundJoin));
    peindre.drawLine(dernierPoint, pointDeFin);

    int rad = (maLargeurStylo / 2) + 2;
    update(QRect(dernierPoint, pointDeFin).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    dernierPoint = pointDeFin;
}

void ThreadDessin::setCouleurStylo(const QColor &nouvelleCouleur)
{
    maCouleurStylo = nouvelleCouleur;
}

void ThreadDessin::setLargeurStylo(int nouvelleLargeur)
{
    maLargeurStylo = nouvelleLargeur;
}


