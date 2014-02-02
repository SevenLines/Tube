#include "tubesmodel.h"
#include <QDebug>
#include <QFile>


TubesModel::TubesModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


int TubesModel::rowCount(const QModelIndex &) const
{
    return tubesData.count();
}

QVariant TubesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    switch(role) {
    case Qt::DisplayRole:
        return tubesData[index.row()].number;
    case Qt::CheckStateRole:
        return QFile(tubesData[index.row()].xmlPath).exists();
    }
    
    return QVariant();
}

TubesData::TubeEx &TubesModel::data(const QModelIndex &index)
{
    return tubesData[index.row()];
}


Qt::ItemFlags TubesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    
    return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled
            | Qt::ItemIsSelectable;
}


void TubesModel::loadFromDir(QString imageDirPath)
{
    beginResetModel();
    tubesData.loadFromDir(imageDirPath);
    endResetModel();
}


