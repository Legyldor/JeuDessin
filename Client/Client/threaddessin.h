#ifndef THREADDESSIN_H
#define THREADDESSIN_H
#include <QThread>
#include <QObject>
#include <QWidget>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QPaintDevice>

class ThreadDessin : public QThread, public QWidget
{
public:
    ThreadDessin();
    explicit ThreadDessin(int coorX, int coorY, int eventTypeClient, int cliqueTypeClient, int tailleStyloClient, int couleurStyloClient, QPoint dernierPointClient, bool gribouillageClient, QImage imageClient);
    ~ThreadDessin();

    void run();

private :

    int x;
    int y;
    int eventType;
    int cliqueType;
    int tailleStylo;
    int couleurStylo;
    QPoint dernierPoint;
    bool gribouillage;
    QImage image;

    int maLargeurStylo;
    QColor maCouleurStylo;

    void dessinerLigneA(const QPoint &pointDeFin);
    void dessinerLigneB(const QPoint &pointDeFin);

    void setCouleurStylo(const QColor &nouvelleCouleur);
    void setLargeurStylo(int nouvelleLargeur);


};

#endif // THREADDESSIN_H
