#include "qloadimagethread.h"
#include <QPixmap>

QLoadImageThread::QLoadImageThread(QObject *parent) :
    QThread(parent)
{
    setDeleteOnCompleted(true);
}

QPixmap QLoadImageThread::getPixmap()
{
    return pixmap;
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
    pixmap.load(this->imagePath);
    emit completed(pixmap);
}
