#include "qlabelimagezoom.h"
#include <QMouseEvent>


void QLabelImageZoom::showImageAtPos(QPoint pos)
{
    if (pixmap().isNull())
        return;
    
    pos.rx() -= width() /2;
    pos.ry() -= height() /2;
            
    pos.rx() = qMax(0, qMin(pos.x(), pixmap().width() - width()));
    pos.ry() = qMax(0, qMin(pos.y(), pixmap().height() - height()));
    setPixmap(pixmap().copy(pos.x(),
                                pos.y(),
                                width(),
                                height())); 

}


QLabelImageZoom::QLabelImageZoom(QWidget *parent) :
    QLabelImage(parent)
{
    setFitImage(true);
}

void QLabelImageZoom::mouseMoveEvent(QMouseEvent *e)
{  
    if (e->buttons().testFlag(Qt::LeftButton)) {
        showImageAtPos(mapToPixmap(e->pos()));
    }
}


QPoint QLabelImageZoom::mapToPixmap(QPoint labelPoint)
{
    qreal k;
    int xOffset = 0, yOffset = 0;
    
    if (pixmap().isNull())
        return QPoint();
    
    int pxWidth = pixmap().width();
    int pxHeight = pixmap().height();
    
    qreal kOrigin = pxWidth /pxHeight;
    qreal kLabel = width() / height();
    
    if (kLabel > kOrigin) {
        k = (qreal)pxHeight / height();
        xOffset = width() - pxWidth / k;
    } else {
        k = (qreal)pxWidth / width();
        yOffset = height() - pxHeight / k;
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


void QLabelImageZoom::mouseReleaseEvent(QMouseEvent *)
{
    showPixmapProp();
}


void QLabelImageZoom::mousePressEvent(QMouseEvent *e)
{
    if (e->buttons().testFlag(Qt::LeftButton)) {
        showImageAtPos(mapToPixmap(e->pos()));
    }
}

void QLabelImageZoom::showPixmap(QPixmap pixmap)
{
    showPixmapProp();
}
