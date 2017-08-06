#ifndef THREADLECTURE_H
#define THREADLECTURE_H

#include <QThread>
#include <QTcpSocket>
#include <QObject>

class ThreadLecture : public QThread
{
    Q_OBJECT

public:
    explicit ThreadLecture(QTcpSocket *socket);

    ~ThreadLecture();


signals :
     void envoieMessage(QString);

protected :
    virtual void run();

private:
    QTcpSocket * socketClient;
    quint16 tailleMessage;
    bool stop;
};

#endif // THREADLECTURE_H
