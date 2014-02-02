#include "tubeform.h"
#include "ui_tubeform.h"

#include <QDebug>
#include <QMouseEvent>
#include <QMutexLocker>

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
    // load tube card
    Tube t = Tube::readFromFile(tube.xmlPath);
    
    synchronizeWithTube(t);
    
    // show images
    if (tube.imagesListIn.count())
        setInImage(tube.imagesListIn[0]);
    if (tube.imagesListOut.count()) 
        setOutImage(tube.imagesListOut[0]);
}

Tube TubeForm::getTube()
{
    Tube tube;
    tube.condition = ui->cmbCondition->currentText();
    tube.obstacle = ui->cmbObstacle->currentText();
    tube.schedule = ui->cmbSchedule->currentText();
    tube.waterCourse = ui->cmbWatercourse->currentText();
    
    tube.fullLength = ui->spnLength->value();
    tube.moundHeight = ui->spnMoundHeight->value();
    tube.skew = ui->spnSkew->value();
    
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
    
    ui->wdgPortalIn->setPortal(t.in);
    ui->wdgPortalOut->setPortal(t.out);
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


