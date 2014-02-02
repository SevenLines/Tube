#ifndef QLABELIMAGE_H
#define QLABELIMAGE_H

#include <QLabel>

class QLabelImage : public QLabel
{
    Q_OBJECT
private:
    QMovie *loadingMovie;
    QPixmap originPixmap;
    void showPixmapProp();
    void showImageAtPos(QPoint pos);
    
public:
    explicit QLabelImage(QWidget *parent = 0);
    void setOriginPixmap(QPixmap originPixmap);
    
signals:
    
public slots:
    void showLoadingPixmap();
    void setEmpty();

    
protected:
    void mouseMoveEvent(QMouseEvent *e);
    
    QPoint mapToPixmap(QPoint labelPoint);
    
    void resizeEvent(QResizeEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
};

#endif // QLABELIMAGE_H
