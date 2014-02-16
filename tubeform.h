#ifndef TUBEFORM_H
#define TUBEFORM_H

#include <QWidget>
#include <QLabel>
#include "tubesdata.h"
#include "qlabelimage.h"

namespace Ui {
class TubeForm;
}

class TubeForm : public QWidget
{
    Q_OBJECT
    TubesData::TubeEx tube;
    
public:
    explicit TubeForm(QWidget *parent = 0);
    ~TubeForm();
    
    void setTube(TubesData::TubeEx &tube);
    Tube getTube();
    QStringList deffectList();
    void setDeffects(QStringList &deffectsList);
    
    /// creates local copy of tube using info from tubeEx
    void synchronizeWithTube(Tube t);
   
public slots:    
    void setInImage();
    void setInImage(QString path);
    void setOutImage();
    void setOutImage(QString path);
    
    void save();
    void useHelper(bool use);
    
signals:
    void saved(TubesData::TubeEx &tube);
    
private:
    Ui::TubeForm *ui;


};

#endif // TUBEFORM_H
