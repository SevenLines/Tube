#include "qloadimagethread.h"
#include <QPixmap>
#include <QDebug>

QLoadImageThread::QLoadImageThread(QObject *parent) :
    QThread(parent)
{
    setDeleteOnCompleted(true);
}

QPixmap QLoadImageThread::pixmap()
{
    return mPixmap;   
}

void QLoadImageThread::setPath(QString imagePath)
{
    this->imagePath = imagePath;
}

void QLoadImageThread::setDeleteOnCompleted(bool fDelete)
{
    fDeleteOnComplete = fDelete;
}


void QLoadImageThread::run()
{
    mPixmap = QPixmap();
    mPixmap.load(this->imagePath);
    emit completed(mPixmap);
}
