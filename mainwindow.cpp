#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tubesmodel.h"
#include "tubesdata.h"

#include <QSettings>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    connect(ui->lstTubes, SIGNAL(doubleClicked(QModelIndex)),
            SLOT(selectTube(QModelIndex)));
    connect(&tubesModel, SIGNAL(afterLoad()),
            ui->lblProcess, SLOT(hideProcess()));
    connect(ui->btnReload, SIGNAL(clicked()),
            SLOT(openDir()));
    
    connect(ui->actionTurnHelper, SIGNAL(triggered(bool)),
            ui->wdgTubeInfo, SLOT(useHelper(bool)));
    
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
    ui->wdgTubeInfo->setTube(tubesModel.data(index));
    ui->wdgTubeInfo->setEnabled(true);
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
    settings.endGroup();
    
    settings.beginGroup("Files");
        settings.setValue("lastDir", lastDir);
    settings.endGroup();
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
    settings.endGroup();
    
    settings.beginGroup("Files");
         openDir(settings.value("lastDir", QString()).toString());
         settings.endGroup();
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

void MainWindow::on_action_exit_triggered()
{
    close();
}
