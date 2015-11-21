#ifndef DIALOGREPORT_H
#define DIALOGREPORT_H

#include <QDialog>
#include "tubesdata.h"
#include <QCompleter>
#include <QList>

namespace Ui {
class DialogReport;
}

class DialogReport : public QDialog
{
    Q_OBJECT
    QList<TubesData::TubeEx> *tubes;
    
public:
    explicit DialogReport(QWidget *parent = 0);
    ~DialogReport();
    QString format();
    void setTubeList(QList<TubesData::TubeEx> *tubes);
    
public slots:
    QString report();
    void showAutoComplete();
    void loadTemplates();
    
private:
    Ui::DialogReport *ui;
    QCompleter *completer;

};

#endif // DIALOGREPORT_H
