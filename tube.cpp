#include "tube.h"
#include "xmlutils.h"
#include <QFile>
#include <QDebug>
#include <math.h>

using namespace std;

/// /// /// /// /// /// /// /// /// /// /// /// /// ///
/// PORTAL CLASS
/// /// /// /// /// /// /// /// /// /// /// /// /// ///

Tube::Portal::Portal()
{
    widthPortal =  1.2;
    widthTrumpet = 0;
    eyesCount = 1;
    
    materialPortal = materials()[0];
    materialBody = materials()[0];
    type = portalTypes()[1];
    typeCut = portalCuts()[0];
    
    thickness = 0.1;
    
    size.diameter = 1.0;
    size.height = 1.0;
    size.width = 1.0;
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

bool Tube::Portal::operator==(Tube::Portal &p)
{
    return p.eyesCount == eyesCount &&
            p.materialBody == materialBody &&
            p.materialPortal == materialPortal &&
            p.size.diameter == size.diameter &&
            p.size.width == size.width &&
            p.size.height == size.height &&
            p.thickness == thickness &&
            p.type == type &&
            p.typeCut == typeCut &&
            p.widthPortal == widthPortal &&
            p.widthTrumpet == widthTrumpet;
}

bool Tube::Portal::operator!=(Tube::Portal &p)
{
    return !(p==*this);
}

/// /// /// /// /// /// /// /// /// /// /// /// /// ///
/// TUBE CLASS
/// /// /// /// /// /// /// /// /// /// /// /// /// /// 

QStringList Tube::portalTypes()
{
    QStringList types;
    types << "без оголовка" << "раструбный" << "портальный";
    return types;
}

QStringList Tube::materials()
{
    QStringList types;
    types << "железобетон" << "металл" << "дерево";
    return types;
}

QStringList Tube::portalCuts()
{
    QStringList types;
    types << "круглое" << "прямоугольное";
    return types;   
}

QStringList Tube::obstacles()
{
    QStringList types;
    types << "" 
          << "периодический водоток" 
          << "постоянный водоток"
          << "суходол";
    return types;      
}

QStringList Tube::waterCourses()
{
    QStringList types;
    types   << "" 
            << "суходол" 
            << "пруд"
            << "ручей"
            << "болото";
    return types;       
}

QStringList Tube::schedules()
{
    QStringList types;
    types << "" 
          << "безнапорный" 
          << "напорный";
    return types;     
}

QStringList Tube::conditions()
{
    QStringList types;
    types << "отличное" 
          << "хорошее" 
          << "удовл."
             << "плохое"
                << "аварийное";
    return types;     
}

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
    place = 262;
    
    condition = conditions()[2];
    obstacle = obstacles()[1];
    schedule = schedules()[1];
    
    skew = 0;
}

QString Tube::SQLInsertScript()
{ 
    QString holeSize = QString::number(in.size.diameter==0?in.size.width:in.size.diameter,'g', 1).replace(',','.');
    QString comment = QString("'%1 %2'").arg(obstacle, waterCourse).trimmed();
    QString cnd = condition=="отличное" ? "2322" :
                  condition=="хорошее"  ? "2321" :
                  condition=="удовл."   ? "2320" :
                  condition=="плохое"   ? "2319" :
                  condition=="аварийное"? "2323" : "-1";
    
    std::vector<std::tuple<QString, QString>> insert_values;
    insert_values.push_back(make_tuple("DataSource", "@NumDataSource"));
    insert_values.push_back(make_tuple("NumRoad", "@NumRoad"));
    insert_values.push_back(make_tuple("NumTypeTubeCuttingShape", in.typeCut=="круглое"?"159":"160"));
    insert_values.push_back(make_tuple("NumTypeTubeWorkMode", schedule=="напорный"?"185":"187"));
    insert_values.push_back(make_tuple("Displacement", QString::number(position)));
    insert_values.push_back(make_tuple("Extent", QString::number(fullLength)));
    insert_values.push_back(make_tuple("NumPlace", QString::number(place)));
    insert_values.push_back(make_tuple("NumMaterial", in.materialBody=="железобетон"?"2293":"2292"));
    insert_values.push_back(make_tuple("HoleSize", holeSize));
    insert_values.push_back(make_tuple("TubeScheme", QString("'%1x%2'").arg(holeSize).arg(in.eyesCount)));
    insert_values.push_back(make_tuple("NumCondition", cnd));
    insert_values.push_back(make_tuple("NumDataSource", "@NumDataSource"));
    insert_values.push_back(make_tuple("PointsCount", "0"));
    insert_values.push_back(make_tuple("NumReason", "2385124"));
    insert_values.push_back(make_tuple("Comment", comment));
    
    QString scriptHead = "INSERT ListTubes(";
    QString scriptBottom = "VALUES (";
    
    bool first = true;
    for(auto& item : insert_values) {
        if (!first){
            scriptHead += ", ";
            scriptBottom += ", ";
        } else {
            first = false;
        }
        scriptHead += get<0>(item);
        scriptBottom += get<1>(item);
    }
    scriptHead += ")\n";
    scriptBottom += ")\n";
    
    return scriptHead + scriptBottom;
}

void Tube::writeToXml(QXmlStreamWriter *xml)
{
    xml->writeStartElement("tube");
    xml->writeAttribute("number", QString::number(number));
    
    XmlUtils::writeBool(xml, "ready", ready);
    XmlUtils::writeInt(xml, "position", position);
    XmlUtils::writeInt(xml, "place", place);
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
    
    xml->writeStartElement("deffects");
        XmlUtils::writeStringList(xml, deffects);
    xml->writeEndElement();
    
    xml->writeTextElement("customDeffects", customDeffects);
    
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
    tube.place = XmlUtils::readFirstReal(&tubeNode, "place");
    
    tube.position = XmlUtils::readFirstInt(&tubeNode, "position");
    
    tube.in = Portal::readFromXml(tubeNode.firstChildElement("in"));
    tube.out = Portal::readFromXml(tubeNode.firstChildElement("out"));
    
    XmlUtils::getStringList(tubeNode.firstChildElement("deffects"), tube.deffects);
    tube.customDeffects = XmlUtils::readFirstString(&tubeNode, "customDeffects");
    
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
