#include "tubesmodel.h"
#include <QDebug>
#include <QFile>


TubesModel::TubesModel(QObject *parent) :
    QAbstractListModel(parent)
{
    activeRow = -1;
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
        if (index.row() == activeRow) {
            return tr("%1 <<<").arg(tubesDataLoader.tubesData[index.row()].number);
        } else {
            return tubesDataLoader.tubesData[index.row()].number;
        }
    case Qt::CheckStateRole:
        return tubesDataLoader.tubesData[index.row()].ready;
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
    connect(&tubesDataLoader, SIGNAL(finished()),SLOT(resetModel()));
    connect(&tubesDataLoader, SIGNAL(finished()),SIGNAL(afterLoad()));
    tubesDataLoader.loadTubesInfo();
}

void TubesModel::resetModel()
{
    beginResetModel();
    endResetModel();
}

void TubesModel::setActive(int row)
{
    beginResetModel();
    activeRow = row;
    endResetModel();
}

void TubesModel::updateAcitve()
{
    beginResetModel();
    TubesData::TubeEx &tube = tubesDataLoader.tubesData[activeRow];
    tube.ready = QFile(tube.xmlPath).exists();
    endResetModel();  
}

void TubesModel::generateSaveScript(GenerateScriptOptions &options)
{
    options.excludeZeroPosition = true;
    tubesDataLoader.tubesData.generateSaveScript(options);
}


