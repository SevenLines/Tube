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
    lastImageDir = dirWithImages;
    
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
    
    /// формируем карты с информацией по длине и положению труб
    /// берутся из файлов length.txt / position.txt в папке с изображениями
    auto tubesLengthInfo = getLengthInfo(dirWithImages);
    auto tubePositionInfo = getPositionInfo(dirWithImages);
    
    for(int i=0;i<tubes.count();++i) {
        /// set length if any
        if (tubesLengthInfo.contains(tubes[i].number)) {
            tubes[i].length = tubesLengthInfo[tubes[i].number];
        }
        
        /// set position if any
        if (tubePositionInfo.contains(tubes[i].number)) {
//            qDebug() << 
            tubes[i].position = tubePositionInfo[tubes[i].number];
        }
        
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

        // create tubes xml dir
        QDir xmlDir(QString("%1/%2")
                     .arg(dirWithImages)
                     .arg(xmlFolderName));
        
        // create xml dir if not exists
        if (!xmlDir.exists()) {
            xmlDir.mkpath(xmlDir.path());
        }
        
        tubes[i].xmlPath = xmlDir.filePath(QString("%1.xml")
                                           .arg(tubes[i].number, 3, 10, QLatin1Char('0')));
        
        // set ready state
        tubes[i].ready = QFile(tubes[i].xmlPath).exists();
        
        if (verbose) qDebug() << tubes[i].xmlPath;
        if (verbose) qDebug() << "-=-=-=-";
    }
    
    emit afterLoadFromDir();
    
}

QMap<int, int> TubesData::getPositionInfo(QString dirWithImages)
{
    QMap<int, int> position;
    QFile file(dirWithImages + "/" + "position.txt");
    QRegExp regExp("(\\d+)\\s+(\\d+)");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream ts(&file);
        while(!ts.atEnd()) {
           QString line = ts.readLine();
           if (regExp.indexIn(line) != -1) {
               int tubeNumber = regExp.cap(1).toInt();
               int tubePosition = regExp.cap(2).toInt();
               position[tubeNumber] = tubePosition;
           }
        }
        
    }
    
    return position;
}

QMap<int, qreal> TubesData::getLengthInfo(QString dirWithImages)
{
    QMap<int, qreal> length;
    QFile file(dirWithImages + "/" + "length.txt");
    QRegExp regExp("(\\d+)\\s+(\\d+)");
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream ts(&file);
        while(!ts.atEnd()) {
           QString line = ts.readLine();
           line = line.trimmed();
           if (regExp.indexIn(line) != -1) {
               int tubeNumber = regExp.cap(1).toInt();
               bool result = false;
               qreal tubeLength = regExp.cap(2).toDouble(&result);
               if (result) {
                length[tubeNumber] = tubeLength;
               }
               
           }
        }
        
    }
    
    return length;    
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

void TubesData::generateSaveScript(GenerateScriptOptions &options)
{
    QString script;
    
//    auto tubesLengthInfo = getLengthInfo(dirWithImages);
    auto tubePositionInfo = getPositionInfo(lastImageDir);
    
    script = "DECLARE @NumDataSource int, @NumRoad int, @lastTube int";
    script += "\nSELECT @NumDataSource = " + QString::number(options.NumDataSource) +
                     ", @NumRoad = " + QString::number(options.NumRoad);
    foreach(TubeEx tube, tubes) {
        if (QFile(tube.xmlPath).exists()) {
            // load tube info
            Tube t = Tube::readFromFile(tube.xmlPath);
             
            script += "\n--труба №" + QString::number(tube.number);
            
            // load position info from external file if present
            if (t.position == 0 && tube.position >=0) {
                t.position = tube.position;
                // update tube xml file with new position
                t.writeToFile(tube.xmlPath);
            }           
            
            if (t.fullLength <= 0) {
                if (tube.length > 0) {
                    t.fullLength = tube.length;
                } else {
                    t.fullLength = 0;
                }
                t.writeToFile(tube.xmlPath);
            }
            
            // skip tubes with zero (undefined) position
            if (t.position <= 0 && options.excludeZeroPosition)  {
                script += "\n-- >> пропускаю так как отсутствует информация о дислокации";
                script += "\n-- *-*-*-*-*-*-*-*-* -- \n";
                continue;         
            }
            
            script += "\n" + t.SQLInsertScript();
            script += "\n" + tube.bindImagesScript(options.NumRoad, options.NumDataSource);
            script += "\n-- *-*-*-*-*-*-*-*-* -- \n";
            
        }
    }
    
    QFile file(options.filePath);
    if (file.open(QFile::WriteOnly)) {
        QTextStream ts(&file);
        ts << script;
        
        file.close();
    }
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

QString tubeValuesString(QString tubeName, QString inOut, QString path, int position, int NumRoad, int NumDataSource)
{
    QString d = ", ";
    return "VALUES ("
            "'" + tubeName + "'" + d // doc_name
            + "@lastTube" + d // doc_object
            + "'" + path + "'" + d // doc_path
            + QString::number(position) + d // doc_StartPos
            + QString::number(position) + d // doc_EndPos
            + "3" + d // doc_Type
            + "42"+ d // doc_celement
            + "914" + d // doc_organ
            + "914" + d // doc_isp
            + "-17" + d // doc_operator
            + "'" + inOut +"'" + d // doc_annot
            + "@NumDataRoad" + d // doc_road
            + "0" + d
            + "@NumDataSource" + ")";    
}

QString TubesData::TubeEx::bindImagesScript(int NumRoad, int NumDataSource)
{   
    QString d = ", ";
    
    QString scriptHead = "INSERT RDocuments("
            "doc_name,"
            "doc_object,"
            "doc_path,"
            "doc_StartPos,"
            "doc_EndPos,"
            "doc_Type,"
            "doc_celement,"
            "doc_organ,"
            "doc_isp,"
            "doc_operator,"
            "doc_annot,"
            "doc_road,"
            "doc_serv,"
            "NumDataSource)";
    
    QString script = "SET @lastTube = @@identity";
    QString tubeName;
    foreach(QString path, imagesListIn) {
        tubeName = QFileInfo(path).baseName();
        QString scriptBottom = tubeValuesString(tubeName, "вход", path, position, NumRoad, NumDataSource);
        Q_ASSERT(scriptBottom.count(',') == scriptHead.count(','));
        script += "\n" + scriptHead + "\n" + scriptBottom;
    }
    
    foreach(QString path, imagesListOut) {
        tubeName = QFileInfo(path).baseName();
        QString scriptBottom = tubeValuesString(tubeName, "выход", path, position, NumRoad, NumDataSource);
        Q_ASSERT(scriptBottom.count(',') == scriptHead.count(','));
        script += "\n" + scriptHead + "\n" + scriptBottom;
    }
    
    
    return script;
}
