#ifndef TUBESDATA_H
#define TUBESDATA_H

#include <QList>
#include <QObject>

#include "tube.h"

class TubesData : public QObject
{
Q_OBJECT
 
    
public:
    struct TubeEx {
//        Tube tube;
        int number;
        QString xmlPath;
        /// изображения входного отверстия
        QList<QString> imagesListIn;
        /// изображения выходного отверстия
        QList<QString> imagesListOut;
    };
    
public:
    TubesData();
    TubesData(QString dirWithImages);
    
    void loadFromDir(QString dirWithImages);
    
    QList<TubeEx> tubes;
    
    int count() const;
    
    /// возвращает трубу из списка tubes под номером
    const TubeEx & operator[](int index) const;
    TubeEx & operator[](int index);
    
    /// возвращает трубу
    const TubeEx * operator()(int tubeNumber) const;
    TubeEx * operator()(int tubeNumber);
    
private:
    QString xmlFolderName;
    QString imageNameTemplate;
    
    const TubeEx *getTubeWithNumber(int num) const;
    TubeEx *getTubeWithNumber(int num);
    
    void addTube(int num, QString inOut, QString path);
    
signals:
    void aboutToLoadFromDir();
    void afterLoadFromDir();
};

#endif // TUBESDATA_H