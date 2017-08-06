#ifndef LECTUREXMLFICHIER_H
#define LECTUREXMLFICHIER_H

#include <QObject>
#include <QFile>
#include <QDomDocument>
#include <QDebug>

class LectureXMLFichier : public QObject
{
    Q_OBJECT
public:
    explicit LectureXMLFichier(QObject *parent = 0);
    ~LectureXMLFichier();
    QList<QString> *lectureXML();

signals:

public slots:
};

#endif // LECTUREXMLFICHIER_H
