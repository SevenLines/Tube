#ifndef TUBESDATALOADER_H
#define TUBESDATALOADER_H

#include <QThread>
#include <tubesdata.h>

class TubesDataLoader : public QThread
{
    Q_OBJECT
    
    QString mDirWithImages;
    
public:
    explicit TubesDataLoader(QObject *parent = 0);
    TubesData tubesData;
    QString dirWithImages();
    
signals:
    
public slots:
    void setDirWithImages(QString dirWithImages);
    void loadTubesInfo(); // the same as start
    
    // QThread interface
protected:
    void run();
};

#endif // TUBESDATALOADER_H
