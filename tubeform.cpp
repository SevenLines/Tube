#include "tubeform.h"
#include "ui_tubeform.h"

#include <QDebug>
#include <QMouseEvent>
#include <QMutexLocker>
#include <QFileInfo>
#include "qwidgethelper.h"

#include "qloadimagethread.h"

TubeForm::TubeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TubeForm)
{
    ui->setupUi(this);
    
    connect(ui->btnSave, SIGNAL(clicked()),
            SLOT(save()));
    

    QWidgetHelper::fillComboBox(ui->cmbCondition, Tube::conditions());
    QWidgetHelper::fillComboBox(ui->cmbObstacle, Tube::obstacles());
    QWidgetHelper::fillComboBox(ui->cmbSchedule, Tube::schedules());
    QWidgetHelper::fillComboBox(ui->cmbWatercourse, Tube::waterCourses());
}

TubeForm::~TubeForm()
{
    delete ui;
}

void TubeForm::setTube(TubesData::TubeEx &tube)
{
    this->tube = tube;
    
    // turn off current images
    ui->lblImageIn->setEmpty();
    ui->lblImageOut->setEmpty();
    
    // load tube card
    Tube t = Tube::readFromFile(tube.xmlPath);
    if (t.fullLength == 0 && tube.length > 0) {
        t.fullLength = tube.length;
    }
    
    if (t.position <= 0 && tube.position > 0) {
//        qDebug() << tube.position;
        t.position = tube.position;
    }
    // synchronize form with loaded tube
    synchronizeWithTube(t);
    
    // clear lblImageIn context menu action
    qDeleteAll(ui->lblImageIn->actions());
    ui->lblImageIn->actions().clear();
    
    QAction *firstAction = 0;
    // fiil lblImageIn contex menu actions
    foreach(QString file, tube.imagesListIn) {
        QAction *action = new QAction(QFileInfo(file).fileName(), this);
        action->setData(file);
        action->setCheckable(true);
        action->setChecked(false);
        connect(action, SIGNAL(triggered()),SLOT(setInImage()));
        ui->lblImageIn->addAction(action);

        if (!firstAction)
            firstAction = action;
    }
    // execute first added action if any, which is should to load image
    if (firstAction) firstAction->trigger();
    
    // clear lblImageOut context menu action
    qDeleteAll(ui->lblImageOut->actions());
    ui->lblImageOut->actions().clear();
    
    // fiil lblImageOut contex menu actions
    firstAction = 0;
    foreach(QString file, tube.imagesListOut) {
        QAction *action = new QAction(QFileInfo(file).fileName(), this);
        action->setData(file);
        action->setCheckable(true);
        action->setChecked(false);
        connect(action, SIGNAL(triggered()),SLOT(setOutImage()));
        ui->lblImageOut->addAction(action);
        if (!firstAction)
            firstAction = action;
    }
    // execute first added action if any, which is should to load image
    if (firstAction) firstAction->trigger();
}

Tube TubeForm::getTube()
{
    // create tube object according to current interface state
    Tube tube;
    
    tube.number = ui->lblTubeNumber->text().toInt();
    
    tube.condition = ui->cmbCondition->currentText();
    tube.obstacle = ui->cmbObstacle->currentText();
    tube.schedule = ui->cmbSchedule->currentText();
    tube.waterCourse = ui->cmbWatercourse->currentText();
    
    tube.fullLength = ui->spnLength->value();
    tube.moundHeight = ui->spnMoundHeight->value();
    tube.skew = ui->spnSkew->value();
    
    tube.position = ui->spnPositionKm->value()*1000 
            + ui->spnPositionM->value();
    
    tube.in = ui->wdgPortalIn->portal();
    if (ui->grpPortalOut->isChecked()) {
        tube.out = ui->wdgPortalOut->portal();
    } else {
        tube.out = tube.in;
    }
    
    tube.deffects = deffectList();
    tube.customDeffects = ui->txtCustomDeffects->toPlainText();
    
    return tube;
}

QStringList TubeForm::deffectList()
{
    QStringList lst;
    foreach(auto w, ui->scrollAreaWidgetContents->children()) {
        QCheckBox *chk = dynamic_cast<QCheckBox*>(w);
        if (chk) {
            if (chk->isChecked()) {
               lst.append(chk->text()); 
            }
        }
    }
    return lst;
}

void TubeForm::setDeffects(QStringList &deffectsList)
{
    foreach(auto w, ui->scrollAreaWidgetContents->children()) {
        QCheckBox *chk = dynamic_cast<QCheckBox*>(w);
        if (chk) {
            chk->setChecked(false);
            foreach (QString str, deffectsList) {
                if (str == chk->text()) {
                    chk->setChecked(true);
                }
            }
        }
    }
}

void TubeForm::synchronizeWithTube(Tube t)
{
    ui->lblTubeNumber->setText(QString("%1")
                               .arg( tube.number, 3, 10, QLatin1Char('0')));
    
    QWidgetHelper::setEditText(ui->cmbCondition, t.condition);
    QWidgetHelper::setEditText(ui->cmbObstacle, t.obstacle);
    QWidgetHelper::setEditText(ui->cmbSchedule, t.schedule);
    QWidgetHelper::setEditText(ui->cmbWatercourse, t.waterCourse);
    
    ui->spnLength->setValue(t.fullLength);
    ui->spnMoundHeight->setValue(t.moundHeight);
    ui->spnSkew->setValue(t.skew);
    ui->spnPositionKm->setValue(t.position / 1000);
    ui->spnPositionM->setValue(t.position % 1000);
    
    ui->txtCustomDeffects->setPlainText(t.customDeffects);
    setDeffects(t.deffects);
    
    ui->grpPortalOut->setChecked(t.in != t.out);
    
    ui->wdgPortalIn->setPortal(t.in);
    ui->wdgPortalOut->setPortal(t.out);
}

void TubeForm::setInImage()
{
    // check sender, should be QAction object
    QAction *action = dynamic_cast<QAction*>(sender());
    if (!action)
        return;
    
    // uncheck all actions
    foreach(QAction *a, ui->lblImageIn->actions()) {
        a->setChecked(false);
        a->setEnabled(true);
    }
    
    // check sender action
    action->setChecked(true);
    action->setEnabled(false);
    
    // load image
    setInImage(action->data().toString());
}

void TubeForm::setInImage(QString path)
{
    ui->lblImageIn->loadPixmap(path);
}


void TubeForm::setOutImage()
{
    // check for sender, should be QAction object
    QAction *action = dynamic_cast<QAction*>(sender());
    if (!action)
        return;

    
    // uncheck all actions    
    foreach(QAction *a, ui->lblImageOut->actions()) {
        a->setChecked(false);
        a->setEnabled(true);
    }
    // check sender action
    action->setChecked(true);
    action->setEnabled(false);

    
    // load image
    setOutImage(action->data().toString());   
}

void TubeForm::setOutImage(QString path)
{
    ui->lblImageOut->loadPixmap(path);
}


void TubeForm::save()
{
    ui->lblInfo->showProcess(); // fake animation
    getTube().writeToFile(tube.xmlPath);
    emit saved(tube);
}

void TubeForm::useHelper(bool use)
{
    ui->wdgPortalIn->useHelper(use);
    ui->wdgPortalOut->useHelper(use);
}


