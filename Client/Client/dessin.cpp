#include <QtWidgets>
#include <QPrinter>
#include <QPrintDialog>
#include <QBitmap>

#include "Dessin.h"

Dessin::Dessin(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    modifier = false;
    gribouillage = false;
    maLargeurStylo = 5;
    maCouleurStylo = QColor(84,175,255,255);
    tempon = true;
    QCursor curseur(Qt::CrossCursor);

    this->setCursor(curseur);
}

void Dessin::setCoordonner(int x, int y, int eventType, int cliqueType, int tailleStylo, int couleurStylo)
{

    QPoint pointRecup;
    pointRecup.setX(x);
    pointRecup.setY(y);

    //setLargeurStylo(tailleStylo);
    //setCouleurStylo(couleurStylo);


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
            dessinerLigneEXT(pointRecup, couleurStylo, tailleStylo);

        }else if(cliqueType == 2 && gribouillage)// Clique type 2
        {
            dessinerLigneB(pointRecup);
        }
    }else if (eventType == 3)
    {
        if (cliqueType == 1 && gribouillage) // Clique type 1
        {
            dessinerLigneEXT(pointRecup, couleurStylo, tailleStylo);
            gribouillage = false;

        }else if(cliqueType == 2 && gribouillage)// Clique type 2
        {
            dessinerLigneB(pointRecup);
            gribouillage = false;
        }
    }



}

void Dessin::setCouleurStylo(const QColor &nouvelleCouleur)
{
    maCouleurStylo = nouvelleCouleur;
}

void Dessin::setLargeurStylo(int nouvelleLargeur)
{
    maLargeurStylo = nouvelleLargeur;
}

void Dessin::mousePressEvent(QMouseEvent *event) // Event 1
{

    if (event->button() == Qt::LeftButton) // Clique type 1
    {
        dernierPoint = event->pos();
        emit coordonner_Point(event->pos().x(), event->pos().y(), 1, 1, maLargeurStylo, maCouleurStylo.rgba());
        gribouillage = true;
    }else if(event->buttons() & Qt::RightButton)// Clique type 2
    {
        dernierPoint = event->pos();
        emit coordonner_Point(event->pos().x(), event->pos().y(), 1, 2, maLargeurStylo, maCouleurStylo.rgba());
        gribouillage = true;
    }

}

void Dessin::mouseMoveEvent(QMouseEvent *event) // Event 2
{
    if ((event->buttons() & Qt::LeftButton) && gribouillage) // Clique type 1
    {
        dessinerLigneA(event->pos());
        emit coordonner_Point(event->pos().x(), event->pos().y(), 2, 1, maLargeurStylo, maCouleurStylo.rgba());
    }else if((event->buttons() & Qt::RightButton) && gribouillage)// Clique type 2
    {
        dessinerLigneB(event->pos());
        emit coordonner_Point(event->pos().x(), event->pos().y(), 2, 2, maLargeurStylo, maCouleurStylo.rgba());
    }
}

void Dessin::mouseReleaseEvent(QMouseEvent *event) // Event 3
{
    if (event->button() == Qt::LeftButton && gribouillage) // Clique type 1
    {
        dessinerLigneA(event->pos());
        emit coordonner_Point(event->pos().x(), event->pos().y(), 3, 1, maLargeurStylo, maCouleurStylo.rgba());
        gribouillage = false;
    }else if((event->buttons() & Qt::RightButton) && gribouillage)// Clique type 2
    {
        dessinerLigneB(event->pos());
        emit coordonner_Point(event->pos().x(), event->pos().y(), 3, 2, maLargeurStylo, maCouleurStylo.rgba());
        gribouillage = false;
    }
}

void Dessin::paintEvent(QPaintEvent *event)
{
    QPainter peindre(this);
    QRect rectangle = event->rect();
    peindre.drawImage(rectangle, image, rectangle);
}

void Dessin::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int nouvelleLargeur = qMax(width() + 128, image.width());
        int nouvelleHauteur = qMax(height() + 128, image.height());
        redimensionnerImage(&image, QSize(nouvelleLargeur, nouvelleHauteur));
        update();
    }
    QWidget::resizeEvent(event);
}

void Dessin::dessinerLigneA(const QPoint &pointDeFin)
{
    QPainter peindre(&image);
    peindre.setPen(QPen(maCouleurStylo, maLargeurStylo, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    peindre.drawLine(dernierPoint, pointDeFin);
    modifier = true;


    int rad = (maLargeurStylo / 2) + 2;
    update(QRect(dernierPoint, pointDeFin).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    dernierPoint = pointDeFin;

}

void Dessin::dessinerLigneEXT(const QPoint &pointDeFin, const QColor &color, const int &taille)
{
    QPainter peindre(&image);
    peindre.setPen(QPen(color, taille, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    peindre.drawLine(dernierPoint, pointDeFin);
    modifier = true;


    int rad = (taille / 2) + 2;
    update(QRect(dernierPoint, pointDeFin).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    dernierPoint = pointDeFin;

}

void Dessin::dessinerLigneB(const QPoint &pointDeFin)
{
    QPainter peindre(&image);
    peindre.setPen(QPen(Qt::white, 20, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    peindre.drawLine(dernierPoint, pointDeFin);
    modifier = true;

    int rad = (maLargeurStylo / 2) + 2;
    update(QRect(dernierPoint, pointDeFin).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));
    dernierPoint = pointDeFin;
}

void Dessin::redimensionnerImage(QImage *image, const QSize &nouvelleTaille)
{
    if (image->size() == nouvelleTaille)
        return;
    QImage nouvelleImage(nouvelleTaille, QImage::Format_RGB32);
    nouvelleImage.fill(qRgb(255, 255, 255));
    QPainter peindre(&nouvelleImage);
    peindre.drawImage(QPoint(0, 0), *image);
    *image = nouvelleImage;
}



