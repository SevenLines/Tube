#ifndef DATABASEROADINFODIALOG_H
#define DATABASEROADINFODIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class DatabaseRoadInfoDialog;
}

class DatabaseRoadInfoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DatabaseRoadInfoDialog(QWidget *parent = 0);
    ~DatabaseRoadInfoDialog();
    
    
    int NumDataSource();
    int NumRoad();
    
    void saveIni(QSettings *ini);
    void loadIni(QSettings *ini);
    
private slots:
    void on_DatabaseRoadInfoDialog_accepted();
    
private:
    Ui::DatabaseRoadInfoDialog *ui;
};

#endif // DATABASEROADINFODIALOG_H
