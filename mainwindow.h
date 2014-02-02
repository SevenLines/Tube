#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tubesmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString lastDir;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void openDir(QString dirPath);
    void selectTube(QModelIndex index);
    
    void saveIni();
    void loadIni();
    
private slots:
    void on_action_OpenDir_triggered();
    
private:
    Ui::MainWindow *ui;
    TubesModel tubesModel;
    
    
};

#endif // MAINWINDOW_H
