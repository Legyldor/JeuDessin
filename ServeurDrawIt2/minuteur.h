#ifndef MINUTEUR_H
#define MINUTEUR_H

#include <QTimer>
#include <QDebug>


class Minuteur : public QTimer
{
    Q_OBJECT
public:
    explicit Minuteur(QTimer *parent = 0);
    void lancerMinuteur();

    long timePassed;
private:


signals:

public slots:

private slots:

};

#endif // MINUTEUR_H
