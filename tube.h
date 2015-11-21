#ifndef TUBE_H
#define TUBE_H

#include <QString>
#include <QStringList>
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
    
    static QStringList portalTypes();
    static QStringList materials();
    static QStringList portalCuts();
    static QStringList obstacles();
    static QStringList waterCourses();
    static QStringList schedules();
    static QStringList conditions();
    
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
        
        bool operator==(Portal &p);
        bool operator!=(Portal &p);
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
    
    QStringList deffects;
    QString customDeffects;
    
    qreal fullLength; // полная длина
    qreal moundHeight; // высота насыпи
    
    qint16 skew; // средний уклон по лотку
    
    int place; // положение трубы относительно дороги, слева(257)/справа(258)/пересекает(262)

    /// входной / выходной портал
    Portal in, out; 
    
    QString SQLInsertScript();
    
public:
    void writeToXml(QXmlStreamWriter *xml);
    void writeToFile(QString path);
    static Tube readFromXml(QDomNode tubeNode);
    static Tube readFromFile(QString path);
};

#endif // TUBE_H
