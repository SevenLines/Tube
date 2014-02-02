#include "tubeform.h"
#include "ui_tubeform.h"

#include <QDebug>
#include <QMouseEvent>
#include <QMutexLocker>
#include <QFileInfo>

#include "qloadimagethread.h"

TubeForm::TubeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TubeForm)
{
    ui->setupUi(this);
    
    connect(ui->btnSave, SIGNAL(clicked()),
            SLOT(save()));
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
    
    return tube;
}

void TubeForm::synchronizeWithTube(Tube t)
{
    ui->lblTubeNumber->setText(QString("%1")
                               .arg( tube.number, 3, 10, QLatin1Char('0')));
    
    ui->cmbCondition->setEditText(t.condition);
    ui->cmbObstacle->setEditText(t.obstacle);
    ui->cmbSchedule->setEditText(t.schedule);
    ui->cmbWatercourse->setEditText(t.waterCourse);
    
    ui->spnLength->setValue(t.fullLength);
    ui->spnMoundHeight->setValue(t.moundHeight);
    ui->spnSkew->setValue(t.skew);
    ui->spnPositionKm->setValue(t.position / 1000);
    ui->spnPositionM->setValue(t.position % 1000);
    
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
    QLoadImageThread *thread = new QLoadImageThread(this);
    thread->setPath(path);
    
    ui->lblImageIn->showLoadingPixmap();
    
    connect(thread, SIGNAL(finished()),
            thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(completed(QPixmap)),
            SLOT(setInImage(QPixmap)));
    
    thread->start();
}

void TubeForm::setInImage(QPixmap pixmap)
{
    QMutexLocker mutex();  
    setImage(pixmap, ui->lblImageIn);
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
    QLoadImageThread *thread = new QLoadImageThread(this);
    thread->setPath(path);

    
    ui->lblImageOut->showLoadingPixmap();
    connect(thread, SIGNAL(finished()),
            thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(completed(QPixmap)),
            SLOT(setOutImage(QPixmap)));

    thread->start();
}

void TubeForm::setOutImage(QPixmap pixmap)
{
    QMutexLocker mutex();    
    setImage(pixmap, ui->lblImageOut);
}

void TubeForm::setImage(QPixmap &pixmap, QLabelImage *label)
{
    if (!pixmap.isNull()) {
        label->setOriginPixmap(pixmap);
    }
}

void TubeForm::save()
{
    getTube().writeToFile(tube.xmlPath);
}


