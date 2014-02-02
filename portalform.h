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
};

#endif // PORTALFORM_H
