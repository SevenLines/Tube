#include "qwidgethelper.h"


void QWidgetHelper::setEditText(QComboBox *comboBox, QString text, bool addIfNotExists)
{
    int index = comboBox->findText(text);
    if (index!=-1) {
        comboBox->setCurrentIndex(index);
    } else {
        if (addIfNotExists) {
            comboBox->addItem(text);
        }
    }
}

void QWidgetHelper::fillComboBox(QComboBox *comboBox, QStringList items)
{
    foreach(QString str, items) {
        comboBox->addItem(str);
    }
}
