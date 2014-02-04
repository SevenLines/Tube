#include "tubesmodel.h"
#include <QDebug>
#include <QFile>


TubesModel::TubesModel(QObject *parent) :
    QAbstractListModel(parent)
{
}


int TubesModel::rowCount(const QModelIndex &) const
{
    if (tubesDataLoader.isRunning()) {
        return 0;
    }
    return tubesDataLoader.tubesData.count();
}

QVariant TubesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    
    if (tubesDataLoader.isRunning()) {
        return QVariant();
    }
    
    switch(role) {
    case Qt::DisplayRole:
        return tubesDataLoader.tubesData[index.row()].number;
    case Qt::CheckStateRole:
        return QFile(tubesDataLoader.tubesData[index.row()].xmlPath).exists();
    }
    
    return QVariant();
}

TubesData::TubeEx &TubesModel::data(const QModelIndex &index)
{
    if (tubesDataLoader.isRunning()) {
        TubesData::TubeEx tube;
        return tube;
    }
    
    return tubesDataLoader.tubesData[index.row()];
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
    if (tubesDataLoader.isRunning()) {
        return;
    }
    
    tubesDataLoader.setDirWithImages(imageDirPath);
    connect(&tubesDataLoader, SIGNAL(finished()),
            SLOT(resetModel()));
    connect(&tubesDataLoader, SIGNAL(finished()),
            SIGNAL(afterLoad()));
    tubesDataLoader.loadTubesInfo();
}

void TubesModel::resetModel()
{
    beginResetModel();
    endResetModel();
}


