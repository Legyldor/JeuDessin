#ifndef THREADLECTUREDONNEES_H
#define THREADLECTUREDONNEES_H

#include <QThread>
#include <QTcpSocket>
#include <QObject>


class ThreadLectureDonnees : public QThread
{
    Q_OBJECT
public:
    explicit ThreadLectureDonnees(QTcpSocket* socketClient);
    ~ThreadLectureDonnees();

    virtual void run();

signals:
    void donneLue(QString);

private :
    QTcpSocket * socket;
    quint16 tailleMessage;
};

#endif // THREADLECTUREDONNEES_H
