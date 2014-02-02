#ifndef TUBESMODEL_H
#define TUBESMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "tube.h"
#include "tubesdata.h"

class TubesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit TubesModel(QObject *parent = 0);
    TubesData tubesData;
    
signals:
    
public slots:
    
    
    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    
    TubesData::TubeEx &data(const QModelIndex &index);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    
public slots:
    void loadFromDir(QString imageDirPath);

};

#endif // TUBESMODEL_H
