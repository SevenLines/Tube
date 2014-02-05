#include "tubesdataloader.h"

TubesDataLoader::TubesDataLoader(QObject *parent) :
    QThread(parent)
{
}

QString TubesDataLoader::dirWithImages()
{
    return mDirWithImages;
}

void TubesDataLoader::setDirWithImages(QString dirWithImages)
{
    mDirWithImages = dirWithImages;
}

void TubesDataLoader::loadTubesInfo()
{
    start();
}

void TubesDataLoader::run()
{
    tubesData.loadFromDir(mDirWithImages);
}
