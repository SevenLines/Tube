#include "qlabelimage.h"
#include <QMouseEvent>
#include <QDebug>
#include <QMovie>

void QLabelImage::showPixmapProp()
{
    if (originPixmap.isNull())
        return;
    setPixmap(originPixmap.scaled(width(),
                                height(),
                                  Qt::KeepAspectRatio));
}

void QLabelImage::showImageAtPos(QPoint pos)
{
    if (originPixmap.isNull())
        return;
    
    pos.rx() -= width() /2;
    pos.ry() -= height() /2;
            
    pos.rx() = qMax(0, qMin(pos.x(), originPixmap.width() - width()));
    pos.ry() = qMax(0, qMin(pos.y(), originPixmap.height() - height()));
    setPixmap(originPixmap.copy(pos.x(),
                                pos.y(),
                                width(),
                                height())); 
}

QLabelImage::QLabelImage(QWidget *parent) :
    QLabel(parent)
{
    loadingMovie = new QMovie(":images/assets/loading.gif");
    loadingMovie->setParent(this);
   
    
//    qDebug() << loadingMovie->frameCount();
}

void QLabelImage::setOriginPixmap(QPixmap originPixmap)
{
    this->originPixmap = originPixmap;
    loadingMovie->stop();
    showPixmapProp();
//    setScaledContents(true);
}

void QLabelImage::showLoadingPixmap()
{ 
    setMovie(loadingMovie);
//    setScaledContents(false);
    loadingMovie->start();
}

void QLabelImage::setEmpty()
{
    originPixmap = QPixmap();
    setText("<span style='color:black;font-size:16px;font-weight:bold'>картинок нет T_T</span>");
}


void QLabelImage::mouseMoveEvent(QMouseEvent *e)
{  
    if (e->buttons().testFlag(Qt::LeftButton)) {
        showImageAtPos(mapToPixmap(e->pos()));
    }
}


QPoint QLabelImage::mapToPixmap(QPoint labelPoint)
{
    qreal k;
    int xOffset = 0, yOffset = 0;
    
    if (originPixmap.isNull())
        return QPoint();
    
    qreal kOrigin = originPixmap.width() / originPixmap.height();
    qreal kLabel = width() / height();
    
    if (kLabel > kOrigin) {
        k = (qreal)originPixmap.height() / height();
        xOffset = width() - originPixmap.width() / k;
    } else {
        k = (qreal)originPixmap.width() / width();
        yOffset = height() - originPixmap.height() / k;
    }
    
    if (alignment().testFlag(Qt::AlignLeft))
        xOffset = 0;
    if (alignment().testFlag(Qt::AlignHCenter))
        xOffset = xOffset / 2;
    
    if (alignment().testFlag(Qt::AlignTop))
        yOffset = 0;
    if (alignment().testFlag(Qt::AlignVCenter))
        yOffset = yOffset / 2;
    
    return (labelPoint - QPoint(xOffset, yOffset)) * k;
}


void QLabelImage::resizeEvent(QResizeEvent *)
{
    showPixmapProp();
}


void QLabelImage::mouseReleaseEvent(QMouseEvent *)
{
    showPixmapProp();
}


void QLabelImage::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons().testFlag(Qt::LeftButton)) {
        showImageAtPos(mapToPixmap(e->pos()));
    }
}
