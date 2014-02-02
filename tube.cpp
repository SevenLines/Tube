#include "tube.h"
#include "xmlutils.h"
#include <QFile>
#include <QDebug>


/// /// /// /// /// /// /// /// /// /// /// /// /// ///
/// PORTAL CLASS
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

Tube::Portal::Portal()
{
    widthPortal = 0;
    widthTrumpet = 0;
    eyesCount = 1;
    
    materialPortal = "железобетонный";
    materialBody = "железобетонный";
    type = "раструбный";
    typeCut = "круглый";
    
    thickness = 0.1;
    
    size.diameter = 0;
    size.height = 0;
    size.width = 0;
}

void Tube::Portal::writeToXml(QXmlStreamWriter *xml)
{
    XmlUtils::writeReal(xml, "widthPortal", widthPortal);
    XmlUtils::writeReal(xml, "widthTrumpet", widthTrumpet);
    XmlUtils::writeInt(xml, "eyesCount", eyesCount);
    XmlUtils::writeReal(xml, "thickness", thickness, 2);

    xml->writeTextElement("materialPortal", materialPortal);
    xml->writeTextElement("materialBody", materialBody);
    xml->writeTextElement("typeCut", typeCut);
    xml->writeTextElement("type", type);
    
    xml->writeStartElement("size");
        XmlUtils::writeReal(xml, "diameter", size.diameter);
        XmlUtils::writeReal(xml, "height", size.height);
        XmlUtils::writeReal(xml, "width", size.width);
    xml->writeEndElement();
}

Tube::Portal Tube::Portal::readFromXml(QDomNode portalNode)
{
    Portal portal;
    
    if (portalNode.isNull()) 
        return portal;
    
    portal.eyesCount = XmlUtils::readFirstInt(&portalNode, "eyesCount");
    portal.widthPortal = XmlUtils::readFirstReal(&portalNode, "widthPortal");
    portal.widthTrumpet = XmlUtils::readFirstReal(&portalNode, "widthTrumpet");
    portal.thickness = XmlUtils::readFirstReal(&portalNode, "thickness");
    
    portal.materialBody = XmlUtils::readFirstString(&portalNode, "materialBody");
    portal.materialPortal = XmlUtils::readFirstString(&portalNode, "materialPortal");
    portal.type = XmlUtils::readFirstString(&portalNode, "type");
    portal.typeCut = XmlUtils::readFirstString(&portalNode, "typeCut");

    QDomElement sizeNode = portalNode.firstChildElement("size");
    if (sizeNode.isNull()) 
        return portal;
    
    portal.size.diameter = XmlUtils::readFirstReal(&sizeNode, "diameter");
    portal.size.height = XmlUtils::readFirstReal(&sizeNode, "height");
    portal.size.width = XmlUtils::readFirstReal(&sizeNode, "width");
    
    return portal;
}

/// /// /// /// /// /// /// /// /// /// /// /// /// ///
/// TUBE CLASS
/// /// /// /// /// /// /// /// /// /// /// /// /// /// 

Tube::Tube()
    : ready(false)
{
    number = -1;
    obstacle = "";
    waterCourse = "";
    schedule = "";
    condition = "";
    fullLength = 0;
    moundHeight = 0;
    position = 0;
    
    condition = "удовлетворительное";
    obstacle = "периодический водоток";
    schedule = "безнапорный";
    
    skew = 0;
}

void Tube::writeToXml(QXmlStreamWriter *xml)
{
    xml->writeStartElement("tube");
    xml->writeAttribute("number", QString::number(number));
    
    XmlUtils::writeBool(xml, "ready", ready);
    XmlUtils::writeInt(xml, "position", position);
    xml->writeTextElement("condition", condition);
    xml->writeTextElement("obstacle", obstacle);
    xml->writeTextElement("waterCourse", waterCourse);
    xml->writeTextElement("schedule", schedule);
    XmlUtils::writeReal(xml, "fullLength", fullLength);
    XmlUtils::writeReal(xml, "moundHeight", moundHeight);
    XmlUtils::writeReal(xml, "skew", skew);
    
    xml->writeStartElement("in");
        in.writeToXml(xml);
    xml->writeEndElement();
    
    xml->writeStartElement("out");
        out.writeToXml(xml);
    xml->writeEndElement();   
    
    xml->writeEndElement();
}

void Tube::writeToFile(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QXmlStreamWriter wr(&file);
        
        wr.setAutoFormatting(true);
        
        wr.writeStartDocument();

        writeToXml(&wr);
;
        wr.writeEndDocument();
        
        file.close();
    } else {
        qWarning() << file.errorString();
    }
}

Tube Tube::readFromXml(QDomNode tubeNode)
{
    Tube tube;
    
    if (tubeNode.isNull()) 
        return tube;
    
    tube.condition = XmlUtils::readFirstString(&tubeNode, "condition");
    tube.obstacle = XmlUtils::readFirstString(&tubeNode, "obstacle");
    tube.waterCourse = XmlUtils::readFirstString(&tubeNode, "waterCourse");
    tube.schedule = XmlUtils::readFirstString(&tubeNode, "schedule");
    
    tube.fullLength = XmlUtils::readFirstReal(&tubeNode, "fullLength");
    tube.moundHeight = XmlUtils::readFirstReal(&tubeNode, "moundHeight");
    tube.skew = XmlUtils::readFirstReal(&tubeNode, "skew");
    
    tube.position = XmlUtils::readFirstInt(&tubeNode, "position");
    
    tube.in = Portal::readFromXml(tubeNode.firstChildElement("in"));
    tube.out = Portal::readFromXml(tubeNode.firstChildElement("out"));
    
    return tube;
}

Tube Tube::readFromFile(QString path)
{
    QDomDocument doc;
    if (!XmlUtils::loadDocument(path, doc)) {
        return Tube();
    }
    
    return readFromXml(doc.documentElement());
}
