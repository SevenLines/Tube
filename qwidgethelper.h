#ifndef QWIDGETHELPER_H
#define QWIDGETHELPER_H

#include <QComboBox>

class QWidgetHelper
{
public:
    /// устанавливает значение комбобокса на text, 
    /// и добавляет к списку айтемов если стоит флаг addIfNotExistsы
    static void setEditText(QComboBox *comboBox, QString text, bool addIfNotExists = true);
};

#endif // QWIDGETHELPER_H
