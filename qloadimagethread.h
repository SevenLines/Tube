#ifndef QLOADIMAGETHREAD_H
#define QLOADIMAGETHREAD_H

#include <QThread>
#include <QPixmap>

class QLoadImageThread : public QThread
{
    Q_OBJECT
    QPixmap pixmap;
    QString imagePath;
    
    bool fDeleteOnComplete;
    
public:
    explicit QLoadImageThread(QObject *parent = 0);
    QPixmap getPixmap();
    
signals:
    void completed(QPixmap pixmap);
    
public slots:
    void setPath(QString imagePath);
    void setDeleteOnCompleted(bool fDelete = true);
    
protected:
    void run();
};

#endif // QLOADIMAGETHREAD_H
