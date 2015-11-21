#include "qlabelimage.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMovie>
#include <QMutexLocker>


void QLabelImage::showPixmapProp()
{
    if (mPixmap.isNull())
        return;
    setPixmap(mPixmap.scaled(width(),
                                height(),
                                  Qt::KeepAspectRatio));
}


QLabelImage::QLabelImage(QWidget *parent) :
    QLabel(parent)
{
    loadingMovie = new QMovie(":images/assets/loading.gif");
    loadingMovie->setParent(this);
   
    mFitImage = true;
}

QPixmap QLabelImage::pixmap()
{
    return mPixmap;
}


void QLabelImage::showLoadingPixmap()
{ 
    setMovie(loadingMovie);
    loadingMovie->start();
}

void QLabelImage::setEmpty()
{
    setPixmap(QPixmap());
    setText("<span style='color:black;font-size:16px;font-weight:bold'>картинок нет T_T</span>");
}

void QLabelImage::loadPixmap(QString path)
{
    
    QLoadImageThread *imageLoader = new QLoadImageThread();
    setEmpty();
    showLoadingPixmap();

    connect(imageLoader, SIGNAL(completed(QPixmap)),
            SLOT(setPrivatePixmap(QPixmap)));
    connect(imageLoader, SIGNAL(finished()),
            imageLoader, SLOT(deleteLater()));

    imageLoader->setPath(path);
    imageLoader->start();
}

void QLabelImage::loadPixmap(QPixmap pixmap)
{
    setPrivatePixmap(pixmap);    
}

void QLabelImage::showPixmap(QPixmap pixmap)
{
    loadingMovie->stop();
    if (mFitImage) {
        showPixmapProp();
    } else {
        setPixmap(pixmap);
    }
}

void QLabelImage::showPixmap()
{
    showPixmap(mPixmap);
}


void QLabelImage::resizeEvent(QResizeEvent *)
{
    if (mFitImage) {
        showPixmapProp();
    }
}

void QLabelImage::setFitImage(bool fit)
{
    mFitImage = fit;
    showPixmap();
}

void QLabelImage::setPrivatePixmap(QPixmap pixmap)
{
    QMutexLocker lock();
    
    mPixmap = pixmap;
    showPixmap(mPixmap);
}



