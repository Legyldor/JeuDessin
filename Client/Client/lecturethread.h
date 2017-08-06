#ifndef LECTURETHREAD_H
#define LECTURETHREAD_H

#include <QThread>

class LectureThread : public QThread
{
public:
    LectureThread();
    ~LectureThread();
};

#endif // LECTURETHREAD_H
