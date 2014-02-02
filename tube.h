#ifndef TUBE_H
#define TUBE_H

#include <QString>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>

class Tube
{
public:
    /// тип портала
    enum PortalType { 
        without=0, // без оголовка
        rastrub=1, // раструбный
        portal=2 // портальный
    };
    
    
    /// материал
    enum Material {
        concrete=0, // железобетон
        metal=1, // металлический
        wood=2, // деревянный
    };
    
    /// тип разрез
    enum PortalCut {
        round=0,
        square=1
    };
    
    /// портал
    struct Portal { 
        Portal();
        
        QString /*PortalType*/ type; // тип
        qreal widthPortal; // ширина портала
        qreal widthTrumpet; // ширина раструба
        QString /*Material*/ materialPortal; // материал портала
        QString /*Material*/ materialBody; // материал тела трубы
        QString /*PortalCut*/ typeCut; // разрез портала
        qint8 eyesCount; // кол-во очков
        
        struct Size { // размеры порталов
            qreal diameter; // диаметр
            qreal height; // высота
            qreal width; // ширина
        } size;
        
        qreal thickness; // тощина стенки
        
        void writeToXml(QXmlStreamWriter *xml);
        static Portal readFromXml(QDomNode portalNode);
    };
    
public:
    
    Tube();
    
    /// флаг обозначающий обследована ли труба
    bool ready; 
    
    /// порядковый номер трубы
    int number;
    
    int position;
    
    QString obstacle; // препятствие
    QString waterCourse; // тип водотока
    QString schedule; // режим водотока
    QString condition; // состояние
    
    qreal fullLength; // полная длина
    qreal moundHeight; // высота насыпи
    
    qint16 skew; // средний уклон по лотку

    /// входной / выходной портал
    Portal in, out; 
    
public:
    void writeToXml(QXmlStreamWriter *xml);
    void writeToFile(QString path);
    static Tube readFromXml(QDomNode tubeNode);
    static Tube readFromFile(QString path);
};

#endif // TUBE_H
