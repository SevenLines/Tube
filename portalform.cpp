#include "portalform.h"
#include "ui_portalform.h"

PortalForm::PortalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortalForm)
{
    ui->setupUi(this);
}

void PortalForm::setPortal(Tube::Portal &portal)
{
    ui->cmbMaterialPortal->setEditText(portal.materialPortal);
    ui->cmbMaterialBody->setEditText(portal.materialBody);
    ui->cmbType->setEditText(portal.type);
    ui->cmbTypeCut->setEditText(portal.typeCut);
    
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
