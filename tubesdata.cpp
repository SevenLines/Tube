#include "tubesdata.h"

#include <QFile>
#include <QDir>
#include <QFileInfoList>
#include <QRegExp>
#include <QDebug>
#include <QUrl>

TubesData::TubesData()
    : TubesData(QString())
{
}

TubesData::TubesData(QString dirWithImages)
{
    xmlFolderName = "xml";
    imageNameTemplate = "(\\d+)_(in|out)_(\\d+)";
    
    if (!dirWithImages.isNull()) {
        loadFromDir(dirWithImages);
    }
}

void TubesData::loadFromDir(QString dirWithImages)
{
    QDir dir(dirWithImages);
    
    bool verbose = false;
    
    if (!dir.exists()) 
        return;
    
    emit aboutToLoadFromDir();
    
    QFileInfoList files = dir.entryInfoList(QDir::Files, QDir::Name);
    
    QRegExp regExp(imageNameTemplate);
    
    tubes.clear();

    
    // first get all available tubes
    foreach(QFileInfo file, files) {
        QString name = file.baseName();
        int i = regExp.indexIn(name);
        if (i != -1) {
            int number = regExp.cap(1).toInt();
            addTube(number, regExp.cap(2), file.absoluteFilePath());
        }
    }    
   
    /// упорядачываем все массивы, блин не смог написать слово >_>
    qSort(tubes.begin(), tubes.end(),
          [](TubeEx t1, TubeEx t2){ return t1.number < t2.number;});
    
    for(int i=0;i<tubes.count();++i) {
        qSort(tubes[i].imagesListIn);
        qSort(tubes[i].imagesListOut);
        
        if (verbose) qDebug() << "number:" << tubes[i].number;
        
        if (verbose) qDebug() << "in:";     
        foreach(QString file, tubes[i].imagesListIn) {
            if (verbose) qDebug() << file;
        }
        
        if (verbose) qDebug() << "out:";
        foreach(QString file, tubes[i].imagesListOut) {
            if (verbose) qDebug() << file;
        }
        
        int number = tubes[i].number;
        
        // create tubes xml dir
        QDir xmlDir(QString("%1/%2")
                     .arg(dirWithImages)
                     .arg(xmlFolderName));
        
        if (!xmlDir.exists()) {
            xmlDir.mkpath(xmlDir.path());
        }
        
        // create xml file handler
        QFileInfo xmlFileTube(xmlDir.filePath(QString("%1.xml")
                                          .arg(number, 3, 10, QLatin1Char('0'))));
        
        tubes[i].xmlPath = xmlFileTube.filePath();
        if (verbose) qDebug() << tubes[i].xmlPath;
        
        /*if (!xmlFileTube.exists()) {
            if (verbose) qDebug() << "create new";
            if (xmlFileTube.open(QIODevice::WriteOnly)) {
                xmlFileTube.close();
            } else {
                if (verbose) qDebug() << xmlFileTube.errorString();
            }
        }
        tubes[i].tube.number = number;*/
        
        if (verbose) qDebug() << "-=-=-=-";
    }
    
    emit afterLoadFromDir();
    
}

int TubesData::count() const
{
    return tubes.count();
}

const TubesData::TubeEx &TubesData::operator[](int index) const
{
    return tubes[index];
}

TubesData::TubeEx &TubesData::operator[](int index)
{
    return tubes[index];
}

const TubesData::TubeEx *TubesData::operator()(int tubeNumber) const
{
    return getTubeWithNumber(tubeNumber);
}

TubesData::TubeEx * TubesData::operator()(int tubeNumber)
{
    return getTubeWithNumber(tubeNumber);
}

const TubesData::TubeEx * TubesData::getTubeWithNumber(int num) const
{
    return getTubeWithNumber(num);
}

TubesData::TubeEx * TubesData::getTubeWithNumber(int num)
{
    for(int i=0;i<tubes.count();++i) {
        if (tubes[i].number == num)    {
            return &tubes[i];
        }
    }
    return nullptr; 
}

void TubesData::addTube(int num, QString inOut, QString path)
{
    TubeEx newTube;
    
    TubeEx *tube = getTubeWithNumber(num);
    
    if (!tube){
        newTube.number = num;
        tube = &newTube;
    }
    
    if (inOut == "in") {
        tube->imagesListIn.append(path);
    }
    if (inOut == "out") {
        tube->imagesListOut.append(path);
    }
    
    if (tube == &newTube) {
        tubes.append(newTube);
    }
}
