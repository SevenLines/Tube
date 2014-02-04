#ifndef XMLUTILS_H
#define XMLUTILS_H

#include <QStringList>
#include <QXmlStreamWriter>
#include <QtXml/QDomDocument>

class XmlUtils
{
public:
    static void writeInt(QXmlStreamWriter *xml, QString name, int value);
    static void writeReal(QXmlStreamWriter *xml, QString name, qreal value, int precision = 1);
    static void writeBool(QXmlStreamWriter *xml, QString name, bool value);
    static bool loadDocument(QString path, QDomDocument &xmlDoc);
    
    static int readFirstInt(QDomNode *node, QString name);
    static qreal readFirstReal(QDomNode *node, QString name);
    static QString readFirstString(QDomNode *node, QString name);
    
    static void getStringList(QDomNode listRootNode, QStringList &list);
    static void writeStringList(QXmlStreamWriter *xml, QStringList &list);
};

#endif // XMLUTILS_H
