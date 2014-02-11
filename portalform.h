#ifndef PORTALFORM_H
#define PORTALFORM_H

#include <QWidget>
#include "tube.h"

namespace Ui {
class PortalForm;
}

class PortalForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit PortalForm(QWidget *parent = 0);
    
    void setPortal(Tube::Portal &portal);
    
    Tube::Portal portal();
    
    ~PortalForm();
    
private:
    Ui::PortalForm *ui;
    bool mUseHelper;
    float mPortalWidthAdd;

public slots:
    void typeCutChanged(QString value);
    void diameterChanged(double value);
    void eyesCountChanged(int value);
    void setPortalWidthAdd(double value);
    
    /// поставить на true если хочется использовать помощника в 
    /// заполнении полей
    void useHelper(bool use = true);
};

#endif // PORTALFORM_H
