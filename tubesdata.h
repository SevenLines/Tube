#ifndef TUBESDATA_H
#define TUBESDATA_H

#include <QList>
#include <QObject>
#include <QMap>

#include "tube.h"

struct GenerateScriptOptions
{
    QString filePath;
    int NumDataSource;
    bool excludeZeroPosition;
    
    GenerateScriptOptions(QString filePath, int NumDataSource) {
        this->filePath = filePath;
        this->NumDataSource = NumDataSource;
    }
};

class TubesData : public QObject
{
Q_OBJECT
 
    
public:
    struct TubeEx {
        int number; // номер трубы
        int length; // длина трубы в метрах
        int position;
        int place;
        bool ready;
        
        QString xmlPath;
        /// изображения входного отверстия
        QList<QString> imagesListIn;
        /// изображения выходного отверстия
        QList<QString> imagesListOut;
        
        TubeEx() : length(-1), number(-1), position(-1), ready(false), place(-1) {}
        
        QString bindImagesScript();
    };
    
public:
    TubesData();
    TubesData(QString dirWithImages);
    
    void loadFromDir(QString dirWithImages);
    QMap<int, int> getPositionInfo(QString dirWithImages); // пикетаж по дороге
    QMap<int, int> getPlaceInfo(QString dirWithImages); // расположение относительно дороги
    QMap<int, qreal> getLengthInfo(QString dirWithImages);
    
    QList<TubeEx> tubes;
    
    int count() const;
    
    /// возвращает трубу из списка tubes под номером
    const TubeEx & operator[](int index) const;
    TubeEx & operator[](int index);
    
    /// возвращает трубу
    const TubeEx * operator()(int tubeNumber) const;
    
    TubeEx * operator()(int tubeNumber);
 
    
    void generateSaveScript(GenerateScriptOptions &options);
    void updateFromExternalFiles();
private:
    QString xmlFolderName;
    QString imageNameTemplate;
    QString imageNameTemplateDirs;
    QString lastImageDir;
    
    const TubeEx *getTubeWithNumber(int num) const;
    TubeEx *getTubeWithNumber(int num);
    
    void addTube(int num, QString inOut, QString path);
    
signals:
    void aboutToLoadFromDir();
    void afterLoadFromDir();
};

#endif // TUBESDATA_H
