#include "dialogreport.h"
#include "ui_dialogreport.h"
#include <QCompleter>
#include <QStringListModel>
#include <QTextCursor>

DialogReport::DialogReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogReport)
{
    ui->setupUi(this);
    ui->cmbTemplate->addItem(
    "{in.typeCut}\\t{fullLength}\\t{in.materialPortal}\\t"
    "цементобетон\\td{in.diameter}мx{in.eyesCount}\\t{in.diameter}\n");
    
    QStringList wordsList;
    
    wordsList.append("{number}");
    wordsList.append("{position}");
    wordsList.append("{obstacle}");
    wordsList.append("{waterCourse}");
    wordsList.append("{schedule}");
    wordsList.append("{fullLength}");
    wordsList.append("{moundHeight}");
    wordsList.append("{skew}");
    
    wordsList.append("{in.widthPortal}");
    wordsList.append("{in.widthTrumpet}");
    wordsList.append("{in.thickness}");
    wordsList.append("{in.materialPortal}");
    wordsList.append("{in.materialBody}");
    wordsList.append("{in.typeCut}");
    wordsList.append("{in.type}");
    wordsList.append("{in.diameter}");
    wordsList.append("{in.height}");
    wordsList.append("{in.width}");
    
    wordsList.append("{out.widthPortal}");
    wordsList.append("{out.widthTrumpet}");
    wordsList.append("{out.thickness}");
    wordsList.append("{out.materialPortal}");
    wordsList.append("{out.materialBody}");
    wordsList.append("{out.typeCut}");
    wordsList.append("{out.type}");
    wordsList.append("{out.diameter}");
    wordsList.append("{out.height}");
    wordsList.append("{out.width}");
    
    QCompleter *completer = new QCompleter(wordsList, this);
//    ui->edtFormat->setCompleter(completer);
    
    connect(ui->btnReport, SIGNAL(clicked()),
            SLOT(report()));
}

DialogReport::~DialogReport()
{
    delete ui;
}



QString DialogReport::format()
{
    QString text =  ui->cmbTemplate->currentText();
    text.replace("\\t", "\t");
    text.replace("\\n", "\n");
    text.replace("\\r", "\r");
    return text;
}

QString DialogReport::report()
{
    QString text = "";
    QString frmt = format();
    foreach(TubesData::TubeEx tube, *tubes) {
        Tube t = Tube::readFromFile(tube.xmlPath);
        QString out = frmt;
        
        out.replace("{number}", QString::number(t.number==-1?tube.number:t.number));
        out.replace("{position}", QString(t.position));
        out.replace("{obstacle}", QString(t.obstacle));
        out.replace("{waterCourse}", QString(t.waterCourse));
        out.replace("{schedule}", QString(t.schedule));
        out.replace("{fullLength}", QString::number(t.fullLength));
        out.replace("{moundHeight}", QString::number(t.moundHeight));
        out.replace("{skew}", QString::number(t.skew));
        
        out.replace("{in.widthPortal}", QString::number(t.in.widthPortal));
        out.replace("{in.eyesCount}", QString::number(t.in.eyesCount));
        out.replace("{in.widthTrumpet}", QString::number(t.in.widthTrumpet));
        out.replace("{in.thickness}", QString::number(t.in.thickness));
        out.replace("{in.materialPortal}", QString(t.in.materialPortal));
        out.replace("{in.materialBody}", QString(t.in.materialBody));
        out.replace("{in.typeCut}", QString(t.in.typeCut));
        out.replace("{in.type}", QString(t.in.type));
        out.replace("{in.diameter}", QString::number(t.in.size.diameter));
        out.replace("{in.height}", QString::number(t.in.size.height));
        out.replace("{in.width}", QString::number(t.in.size.width));
        
        out.replace("{out.widthPortal}", QString::number(t.out.widthPortal));
        out.replace("{out.eyesCount}", QString::number(t.out.eyesCount));
        out.replace("{out.widthTrumpet}", QString::number(t.out.widthTrumpet));
        out.replace("{out.thickness}", QString::number(t.out.thickness));
        out.replace("{out.materialPortal}", QString(t.out.materialPortal));
        out.replace("{out.materialBody}", QString(t.out.materialBody));
        out.replace("{out.typeCut}", QString(t.out.typeCut));
        out.replace("{out.type}", QString(t.out.type));
        out.replace("{out.diameter}", QString::number(t.out.size.diameter));
        out.replace("{out.height}", QString::number(t.out.size.height));
        out.replace("{out.width}", QString::number(t.out.size.width));
        
        out.replace(QRegularExpression("{.*?}"), "");
        
        text.append(out);
    }
    ui->txtReport->setText(text);
}

void DialogReport::showAutoComplete()
{
}

void DialogReport::setTubeList(QList<TubesData::TubeEx> *tubes)
{
    this->tubes = tubes;
}
