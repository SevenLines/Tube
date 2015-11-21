#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tubesmodel.h"
#include "tubesdata.h"
#include "dialogreport.h"

#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDesktopServices>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    connect(ui->lstTubes, SIGNAL(activated(QModelIndex)),
            SLOT(selectTube(QModelIndex)));
    connect(&tubesModel, SIGNAL(afterLoad()),
            ui->lblProcess, SLOT(hideProcess()));
    connect(ui->btnReload, SIGNAL(clicked()),
            SLOT(openDir()));
    
    connect(ui->actionTurnHelper, SIGNAL(triggered(bool)),
            ui->wdgTubeInfo, SLOT(useHelper(bool)));
    connect(ui->wdgTubeInfo, SIGNAL(saved(TubesData::TubeEx&)),
            SLOT(updateTube(TubesData::TubeEx&)));
    
    connect(ui->actionGenerateSaveScript, SIGNAL(triggered()),
            SLOT(generateSaveScript()));
            
    connect(ui->actionReport, SIGNAL(triggered()),
            SLOT(report()));
    
    ui->lstTubes->setModel(&tubesModel);
    ui->wdgTubeInfo->setEnabled(false);
    
    loadIni();
}

MainWindow::~MainWindow()
{
    saveIni();
    delete ui;
}

void MainWindow::openDir(QString dirPath)
{
    lastDir = dirPath;
    ui->lblProcess->showProcess(0);
    tubesModel.loadFromDir(dirPath);
    setWindowTitle(lastDir);
}

void MainWindow::openDir()
{
    openDir(lastDir);
}

void MainWindow::selectTube(QModelIndex index)
{
    tubesModel.setActive(index.row());
    ui->wdgTubeInfo->setImagesOrder(ui->chkAscImagesOrder->isChecked());
    ui->wdgTubeInfo->setTube(tubesModel.data(index));
    ui->wdgTubeInfo->setEnabled(true);
}

void MainWindow::generateSaveScript()
{
    QString path = QFileDialog::getSaveFileName(this, 
                                 "Select script path", 
                                 lastDir, 
                                 "SQL script file (*.sql)");
    if (path.isNull()) {
        return;
    }
    
    if (!path.endsWith(".sql")) path.append(".sql");
    
    if (databaseRoadInfoDialog.exec()!=QDialog::Accepted) {
        return;
    }
    
    GenerateScriptOptions options(path, databaseRoadInfoDialog.NumDataSource());
    tubesModel.generateSaveScript(options);
    
    if (QMessageBox::question(this, "Потдверждение", "Открыть сгенерированный файл?") != QMessageBox::Yes) {
        return;
    }

    QDesktopServices::openUrl("file:///"+path);
}

void MainWindow::saveIni()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    
    settings.beginGroup("MainForm");
        settings.setValue("geometry", saveGeometry());
        settings.setValue("state", saveState());
        settings.setValue("fullscreen", isMaximized());
    settings.endGroup();
    
    settings.beginGroup("Settings");
        settings.setValue("useHelper", ui->actionTurnHelper->isChecked());
        settings.setValue("imagesOrder", ui->chkAscImagesOrder->isChecked());
    settings.endGroup();
    
    settings.beginGroup("Files");
        settings.setValue("lastDir", lastDir);
    settings.endGroup();
    
    databaseRoadInfoDialog.saveIni(&settings);
}

void MainWindow::loadIni()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    
    settings.beginGroup("MainForm");
        restoreGeometry(settings.value("geometry", QByteArray()).toByteArray());
        restoreState(settings.value("state", QByteArray()).toByteArray());
        if (settings.value("fullscreen", false).toBool()) {
            showNormal();
            QApplication::processEvents();
            showMaximized();
        }
    settings.endGroup();   
    
    settings.beginGroup("Settings");
        useHelper(settings.value("useHelper", true).toBool());
        ui->chkAscImagesOrder->setChecked(settings.value("imagesOrder", true).toBool());
    settings.endGroup();
    
    settings.beginGroup("Files");
         openDir(settings.value("lastDir", QString()).toString());
    settings.endGroup();
         
    databaseRoadInfoDialog.loadIni(&settings);
}

void MainWindow::useHelper(bool use)
{
     ui->actionTurnHelper->setChecked(use);
     ui->wdgTubeInfo->useHelper(use);
}

void MainWindow::on_action_OpenDir_triggered()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "", lastDir);
    if (dirPath.isNull())
        return;
    
    openDir(dirPath);
}

void MainWindow::report()
{
    dialogReport.setTubeList(&tubesModel.tubesDataLoader.tubesData.tubes);
    if (dialogReport.exec() != QDialog::Accepted) {
        return;
    }
    
    
    
//    QString format = dialogReport.format(); 
    
//    foreach(TubesData::TubeEx tube, tubesModel.tubesDataLoader.tubesData.tubes) {
//        Tube t = Tube::readFromFile(tube.xmlPath);
//        QString out  = format;
        
//        out.replace("{number}", QString::number(t.number==-1?tube.number:t.number));
//        out.replace("{position}", QString(t.position));
//        out.replace("{obstacle}", QString(t.obstacle));
//        out.replace("{waterCourse}", QString(t.waterCourse));
//        out.replace("{schedule}", QString(t.schedule));
//        out.replace("{fullLength}", QString::number(t.fullLength));
//        out.replace("{moundHeight}", QString::number(t.moundHeight));
//        out.replace("{skew}", QString::number(t.skew));
        
//        out.replace("{in.widthPortal}", QString::number(t.in.widthPortal));
//        out.replace("{in.widthTrumpet}", QString::number(t.in.widthTrumpet));
//        out.replace("{in.thickness}", QString::number(t.in.thickness));
//        out.replace("{in.materialPortal}", QString(t.in.materialPortal));
//        out.replace("{in.materialBody}", QString(t.in.materialBody));
//        out.replace("{in.typeCut}", QString(t.in.typeCut));
//        out.replace("{in.type}", QString(t.in.type));
//        out.replace("{in.diameter}", QString::number(t.in.size.diameter));
//        out.replace("{in.height}", QString::number(t.in.size.height));
//        out.replace("{in.width}", QString::number(t.in.size.width));
        
//        out.replace("{out.widthPortal}", QString::number(t.out.widthPortal));
//        out.replace("{out.widthTrumpet}", QString::number(t.out.widthTrumpet));
//        out.replace("{out.thickness}", QString::number(t.out.thickness));
//        out.replace("{out.materialPortal}", QString(t.out.materialPortal));
//        out.replace("{out.materialBody}", QString(t.out.materialBody));
//        out.replace("{out.typeCut}", QString(t.out.typeCut));
//        out.replace("{out.type}", QString(t.out.type));
//        out.replace("{out.diameter}", QString::number(t.out.size.diameter));
//        out.replace("{out.height}", QString::number(t.out.size.height));
//        out.replace("{out.width}", QString::number(t.out.size.width));
        
//        out.replace(QRegularExpression("{.*?}"), "");
        
//        qDebug() << out;
//    }
}

void MainWindow::on_action_exit_triggered()
{
    close();
}

void MainWindow::updateTube(TubesData::TubeEx &tube)
{
    tubesModel.updateAcitve();
}

