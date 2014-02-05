#include "portalform.h"
#include "ui_portalform.h"

#include "qwidgethelper.h"

PortalForm::PortalForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortalForm)
{
    ui->setupUi(this);
    
    // fill combos
    QWidgetHelper::fillComboBox(ui->cmbTypeCut, Tube::portalCuts());
    QWidgetHelper::fillComboBox(ui->cmbMaterialBody, Tube::materials());
    QWidgetHelper::fillComboBox(ui->cmbMaterialPortal, Tube::materials());
    QWidgetHelper::fillComboBox(ui->cmbType, Tube::portalTypes());

    // connect helper signals
    connect(ui->cmbTypeCut, SIGNAL(editTextChanged(QString)),
            SLOT(typeCutChanged(QString)));
    connect(ui->spnDiameter, SIGNAL(valueChanged(double)),
            SLOT(diameterChanged(double)));
    connect(ui->spnWidth, SIGNAL(valueChanged(double)),
            SLOT(diameterChanged(double)));
    connect(ui->spnEyesCount, SIGNAL(valueChanged(int)),
            SLOT(eyesCountChanged(int)));
    
    // turn on helper
    useHelper();
}

void PortalForm::setPortal(Tube::Portal &portal)
{
    // turn off helper
    bool helperState = mUseHelper;
    useHelper(false);
    
    QWidgetHelper::setEditText(ui->cmbMaterialPortal, portal.materialPortal);
    QWidgetHelper::setEditText(ui->cmbMaterialBody, portal.materialBody);
    QWidgetHelper::setEditText(ui->cmbType, portal.type);
    
    ui->cmbTypeCut->setCurrentText("");
    QWidgetHelper::setEditText(ui->cmbTypeCut, portal.typeCut);
    
    ui->spnDiameter->setValue(portal.size.diameter);
    
    ui->spnHeight->setValue(portal.size.height);

    ui->spnWidth->setValue(portal.size.width);
    
    ui->spnEyesCount->setValue(portal.eyesCount);
    ui->spnThickness->setValue(portal.thickness);
    ui->spnWidthPortal->setValue(portal.widthPortal);
    ui->spnWidthTrumpet->setValue(portal.widthTrumpet);
    
    // turn helper
    useHelper(helperState);
}

Tube::Portal PortalForm::portal()
{
    Tube::Portal p;
    p.materialPortal = ui->cmbMaterialPortal->currentText();
    p.materialBody = ui->cmbMaterialBody->currentText();
    p.type = ui->cmbType->currentText();
    p.typeCut = ui->cmbTypeCut->currentText();
    
    p.size.diameter = ui->spnDiameter->isEnabled()?ui->spnDiameter->value():0;
    p.size.height = ui->spnHeight->isEnabled()?ui->spnHeight->value():0;
    p.size.width = ui->spnWidth->isEnabled()?ui->spnWidth->value():0;
    
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

void PortalForm::typeCutChanged(QString value)
{
    if (value == Tube::portalCuts()[0] /*круглое */) {
        ui->spnDiameter->setEnabled(true);
        ui->spnWidth->setEnabled(false);
        ui->spnHeight->setEnabled(false);
    }
    
    if (value == Tube::portalCuts()[1] /*прямоугольное */) {
        ui->spnDiameter->setEnabled(false);
        ui->spnWidth->setEnabled(true);
        ui->spnHeight->setEnabled(true);
    }
}

void PortalForm::diameterChanged(double value)
{
    if (!mUseHelper) 
        return;
    
    int eyesCount = ui->spnEyesCount->value();
    ui->spnWidthPortal->setValue( eyesCount * (value + 0.1f) );
}

void PortalForm::eyesCountChanged(int value)
{
    if (ui->spnDiameter->isEnabled()) {
        diameterChanged(ui->spnDiameter->value());
    }
    if (ui->spnWidth->isEnabled()) {
        diameterChanged(ui->spnWidth->value());
    }
}

void PortalForm::useHelper(bool use)
{
    mUseHelper = use;
}
