#ifndef TUBESDATA_H
#define TUBESDATA_H

#include <QList>
#include <QObject>
#include <QMap>

#include "tube.h"

class TubesData : public QObject
{
Q_OBJECT
 
    
public:
    struct TubeEx {
        int number; // номер трубы
        int length; // длина трубы в метрах
        int position;
        bool ready;
        
        QString xmlPath;
        /// изображения входного отверстия
        QList<QString> imagesListIn;
        /// изображения выходного отверстия
        QList<QString> imagesListOut;
        
        TubeEx() : length(-1), number(-1), position(-1), ready(false) {}
    };
    
public:
    TubesData();
    TubesData(QString dirWithImages);
    
    void loadFromDir(QString dirWithImages);
    QMap<int, int> getPositionInfo(QString dirWithImages);
    QMap<int, qreal> getLengthInfo(QString dirWithImages);
    
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
