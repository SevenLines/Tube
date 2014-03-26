#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "databaseroadinfodialog.h"
#include "tubesmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString lastDir;
    DatabaseRoadInfoDialog databaseRoadInfoDialog;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void openDir(QString dirPath);
    void openDir();
    void selectTube(QModelIndex index);
    void generateSaveScript();
    
    void saveIni();
    void loadIni();
    void useHelper(bool use);
    
private slots:
    void on_action_OpenDir_triggered();
    void on_action_exit_triggered();
    void updateTube(TubesData::TubeEx& tube);
    
    
private:
    Ui::MainWindow *ui;
    TubesModel tubesModel;
    
    
};

#endif // MAINWINDOW_H
