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
    
    /// creates local copy of tube using info from tubeEx
    void synchronizeWithTube(Tube t);
   
public slots:    
    void setInImage(QString path);
    void setInImage(QPixmap pixmap);
    void setOutImage(QString path);
    void setOutImage(QPixmap pixmap);
    void setImage(QPixmap &pixmap, QLabelImage *label);
    
    void save();
    
private:
    Ui::TubeForm *ui;


};

#endif // TUBEFORM_H
