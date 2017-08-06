#ifndef PALETTE_H
#define PALETTE_H

#include <QObject>

class palette : public QObject
{
    Q_OBJECT
public:
    explicit palette(QObject *parent = 0);
    ~palette();

signals:

public slots:
};

#endif // PALETTE_H
