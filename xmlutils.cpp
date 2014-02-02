#include "xmlutils.h"
#include <QFile>
#include <QDebug>


void XmlUtils::writeInt(QXmlStreamWriter *xml, QString name, int value)
{
    xml->writeTextElement(name, QString::number(value));
}

void XmlUtils::writeReal(QXmlStreamWriter *xml, QString name, qreal value, int precision)
{
    xml->writeTextElement(name, QString::number(value, 'f', precision));
}

void XmlUtils::writeBool(QXmlStreamWriter *xml, QString name, bool value)
{
    writeInt(xml, name, value);
}

bool XmlUtils::loadDocument(QString path, QDomDocument &xmlDoc)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << file.errorString();
        return false;
    }
    
    QString errorMsg;
    if(!xmlDoc.setContent(&file, false, &errorMsg)) {
        qWarning() << errorMsg;
        return false;
    }
    
    return true;
}

int XmlUtils::readFirstInt(QDomNode *node, QString name)
{
    QDomElement elem = node->firstChildElement(name);
    if (elem.isNull()) return 0;
    return elem.text().toInt(); 
}

qreal XmlUtils::readFirstReal(QDomNode *node, QString name)
{
    QDomElement elem = node->firstChildElement(name);
    if (elem.isNull()) return 0;
    return elem.text().toFloat();  
}

QString XmlUtils::readFirstString(QDomNode *node, QString name)
{
    QDomElement elem = node->firstChildElement(name);
    if (elem.isNull()) return QString();
    return elem.text();
}
