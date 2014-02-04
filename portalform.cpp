#include "portalform.h"
#include "ui_portalform.h"

#include "qwidgethelper.h"

PortalForm::PortalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortalForm)
{
    ui->setupUi(this);
    
    // fill combos
    foreach(QString t, Tube::portalCuts()) {
        ui->cmbTypeCut->addItem(t);
    }
    
    foreach(QString t, Tube::materials()) {
        ui->cmbMaterialBody->addItem(t);
        ui->cmbMaterialPortal->addItem(t);
    }
    
    foreach(QString t, Tube::portalTypes()) {
        ui->cmbType->addItem(t);
    }
    
}

void PortalForm::setPortal(Tube::Portal &portal)
{
    QWidgetHelper::setEditText(ui->cmbMaterialPortal, portal.materialPortal);
    QWidgetHelper::setEditText(ui->cmbMaterialBody, portal.materialBody);
    QWidgetHelper::setEditText(ui->cmbType, portal.type);
    QWidgetHelper::setEditText(ui->cmbTypeCut, portal.typeCut);
    
    ui->spnDiameter->setValue(portal.size.diameter);
    ui->spnHeight->setValue(portal.size.height);
    ui->spnWidth->setValue(portal.size.width);
    
    ui->spnEyesCount->setValue(portal.eyesCount);
    ui->spnThickness->setValue(portal.thickness);
    ui->spnWidthPortal->setValue(portal.widthPortal);
    ui->spnWidthTrumpet->setValue(portal.widthTrumpet);
}

Tube::Portal PortalForm::portal()
{
    Tube::Portal p;
    p.materialPortal = ui->cmbMaterialPortal->currentText();
    p.materialBody = ui->cmbMaterialBody->currentText();
    p.type = ui->cmbType->currentText();
    p.typeCut = ui->cmbTypeCut->currentText();
    
    p.size.diameter = ui->spnDiameter->value();
    p.size.height = ui->spnHeight->value();
    p.size.width = ui->spnWidth->value();
    
    p.eyesCount = ui->spnEyesCount->value();
    p.thickness = ui->spnThickness->value();
    p.widthPortal = ui->spnWidthPortal->value();
    p.widthTrumpet = ui->spnWidthTrumpet->value();
    
    return p;
}

PortalForm::~PortalForm()
{
    delete ui;
}
