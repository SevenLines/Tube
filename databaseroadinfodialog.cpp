#include "databaseroadinfodialog.h"
#include "ui_databaseroadinfodialog.h"

DatabaseRoadInfoDialog::DatabaseRoadInfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseRoadInfoDialog)
{
    ui->setupUi(this);
}

DatabaseRoadInfoDialog::~DatabaseRoadInfoDialog()
{
    delete ui;
}

int DatabaseRoadInfoDialog::NumDataSource()
{
    return ui->edtNumDataSource->text().toInt();
}

int DatabaseRoadInfoDialog::NumRoad()
{
    return ui->edtNumRoad->text().toInt();
}

void DatabaseRoadInfoDialog::saveIni(QSettings *ini)
{
    ini->beginGroup("DatabaseRoadInfoDialog");
        ini->setValue("NumDataSource", NumDataSource());
        ini->setValue("NumRoad", NumRoad());
    ini->endGroup();
}

void DatabaseRoadInfoDialog::loadIni(QSettings *ini)
{
    ini->beginGroup("DatabaseRoadInfoDialog");
        ui->edtNumDataSource->setText(ini->value("NumDataSource", "0").toString());
        ui->edtNumRoad->setText(ini->value("NumRoad", "0").toString());
    ini->endGroup();
}

void DatabaseRoadInfoDialog::on_DatabaseRoadInfoDialog_accepted()
{
    
}
