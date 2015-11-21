#include "dialogreport.h"
#include "ui_dialogreport.h"
#include <QCompleter>
#include <QStringListModel>
#include <QTextCursor>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>

DialogReport::DialogReport(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::DialogReport)
{
    ui->setupUi(this);

    loadTemplates();

    ui->cmbTemplate->addItem("{number}\\t{in.diameter}\\n");
    ui->cmbTemplate->addItem(
        "{in.typeCut}\\t{fullLength}\\t{in.materialPortal}\\t"
        "цементобетон\\td{in.diameter}мx{in.eyesCount}\\t{in.diameter}\n");

    QStringList wordsList;

    wordsList.append("number");
    wordsList.append("position");
    wordsList.append("obstacle");
    wordsList.append("waterCourse");
    wordsList.append("schedule");
    wordsList.append("fullLength");
    wordsList.append("moundHeight}");
    wordsList.append("skew");

    wordsList.append("in_widthPortal");
    wordsList.append("in_widthTrumpet");
    wordsList.append("in_thickness");
    wordsList.append("in_materialPortal");
    wordsList.append("in_materialBody");
    wordsList.append("in_typeCut");
    wordsList.append("in_type");
    wordsList.append("in_diameter");
    wordsList.append("in_height");
    wordsList.append("in_width");

    wordsList.append("out_widthPortal");
    wordsList.append("out_widthTrumpet");
    wordsList.append("out_thickness");
    wordsList.append("out_materialPortal");
    wordsList.append("out_materialBody");
    wordsList.append("out_tRypeCut");
    wordsList.append("out_type");
    wordsList.append("out_diameter");
    wordsList.append("out_height");
    wordsList.append("out_width");

    QCompleter* completer = new QCompleter(this);
    completer->setModel(new QStringListModel(wordsList, this));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    ui->textEdit->setCompleter(completer);
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
    QString text = ui->textEdit->toPlainText();
    text.replace("\\t", "\t");
    text.replace("\\n", "\n");
    text.replace("\\r", "\r");
    return text;
}

QString DialogReport::report()
{
    QString text = "";
    QString frmt = format();
    foreach(TubesData::TubeEx tube, *tubes)
    {
        Tube t = Tube::readFromFile(tube.xmlPath);
        QString out = frmt;

        out.replace("{number}", QString::number(t.number == -1 ? tube.number : t.number));
        out.replace("{position}", QString::number(t.position));
        out.replace("{obstacle}", QString(t.obstacle));
        out.replace("{waterCourse}", QString(t.waterCourse));
        out.replace("{schedule}", QString(t.schedule));
        out.replace("{fullLength}", QString::number(t.fullLength));
        out.replace("{moundHeight}", QString::number(t.moundHeight));
        out.replace("{skew}", QString::number(t.skew));

        out.replace("{in_widthPortal}", QString::number(t.in.widthPortal));
        out.replace("{in_eyesCount}", QString::number(t.in.eyesCount));
        out.replace("{in_widthTrumpet}", QString::number(t.in.widthTrumpet));
        out.replace("{in_thickness}", QString::number(t.in.thickness));
        out.replace("{in_materialPortal}", QString(t.in.materialPortal));
        out.replace("{in_materialBody}", QString(t.in.materialBody));
        out.replace("{in_typeCut}", QString(t.in.typeCut));
        out.replace("{in_type}", QString(t.in.type));
        out.replace("{in_diameter}", QString::number(t.in.size.diameter));
        out.replace("{in_height}", QString::number(t.in.size.height));
        out.replace("{in_width}", QString::number(t.in.size.width));

        out.replace("{out_widthPortal}", QString::number(t.out.widthPortal));
        out.replace("{out_eyesCount}", QString::number(t.out.eyesCount));
        out.replace("{out_widthTrumpet}", QString::number(t.out.widthTrumpet));
        out.replace("{out_thickness}", QString::number(t.out.thickness));
        out.replace("{out_materialPortal}", QString(t.out.materialPortal));
        out.replace("{out_materialBody}", QString(t.out.materialBody));
        out.replace("{out_typeCut}", QString(t.out.typeCut));
        out.replace("{out_type}", QString(t.out.type));
        out.replace("{out_diameter}", QString::number(t.out.size.diameter));
        out.replace("{out_height}", QString::number(t.out.size.height));
        out.replace("{out_width}", QString::number(t.out.size.width));

        out.replace(QRegularExpression("{.*?}"), "");

        text.append(out);
    }
    ui->txtReport->setText(text);
}

void DialogReport::showAutoComplete()
{
}

void DialogReport::loadTemplates()
{
    QFile file("reports.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return;
    }

    QJsonDocument json = QJsonDocument::fromJson(file.readAll());
}

void DialogReport::setTubeList(QList<TubesData::TubeEx>* tubes)
{
    this->tubes = tubes;
}
