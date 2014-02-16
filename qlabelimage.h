#ifndef QLABELIMAGE_H
#define QLABELIMAGE_H

#include <QLabel>
#include "qloadimagethread.h"

class QLabelImage : public QLabel
{
    Q_OBJECT
protected:
    QMovie *loadingMovie;
//    QLoadImageThread imageLoader;
    bool mFitImage;
    QPixmap mPixmap;
    
    void showPixmapProp();
    void resizeEvent(QResizeEvent *);
    void setFitImage(bool fit = true);
    
    
public:
    explicit QLabelImage(QWidget *parent = 0);
    QPixmap pixmap();
    
    
signals:
    
private slots:
    void setPrivatePixmap(QPixmap pixmap);
    
public slots:
    void showLoadingPixmap();
    void setEmpty();
    void loadPixmap(QString path);
    void loadPixmap(QPixmap pixmap);
    
    /// override this method if u want
    /// make some specific action to show image
    virtual void showPixmap(QPixmap pixmap);
    void showPixmap();
};

#endif // QLABELIMAGE_H
