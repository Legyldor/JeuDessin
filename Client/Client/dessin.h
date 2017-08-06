#ifndef DESSIN_H
#define DESSIN_H

#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QImage>
#include "threaddessin.h"

class Dessin : public QWidget
{
    Q_OBJECT
public:
    explicit Dessin(QWidget *parent = 0);
    void setCouleurStylo(const QColor &nouvelleCouleur);
    void setLargeurStylo(int nouvelleLargeur);

    bool estModifier() const { return modifier; }
    QColor couleurStylo() const { return maCouleurStylo; }
    int largeurStylo() const { return maLargeurStylo; }

    ThreadDessin toto;


    QPoint dernierPoint;

    bool tempon;
    QColor oldCouleur;
    int oldLargeur;



signals :
    void coordonner_Point(int x , int y , int eventType, int cliqueType, int tailleStylo, int couleurStylo);

public slots:
    void setCoordonner(int x ,int y, int eventType, int cliqueType, int tailleStylo, int couleurStylo);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void dessinerLigneA(const QPoint &pointDeFin);
    void dessinerLigneB(const QPoint &pointDeFin);
    void dessinerLigneEXT(const QPoint &pointDeFin, const QColor &color, const int &taille);
    void redimensionnerImage(QImage *image, const QSize &nouvelleTaille);

    bool modifier;
    bool gribouillage;
    int maLargeurStylo;
    QColor maCouleurStylo;
    QImage image;



};

#endif // DESSIN_H
