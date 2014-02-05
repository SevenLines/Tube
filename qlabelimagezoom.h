#ifndef QLABELIMAGEZOOM_H
#define QLABELIMAGEZOOM_H

#include "qlabelimage.h"

class QLabelImageZoom : public QLabelImage
{
    Q_OBJECT
    
protected:   

    void showImageAtPos(QPoint pos);
public:
    explicit QLabelImageZoom(QWidget *parent = 0);
    
signals:
    
public slots:
    
protected:
    void mouseMoveEvent(QMouseEvent *e);
    
    QPoint mapToPixmap(QPoint labelPoint);

    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    
    // QLabelImage interface
public slots:
    void showPixmap(QPixmap pixmap);
};

#endif // QLABELIMAGEZOOM_H
