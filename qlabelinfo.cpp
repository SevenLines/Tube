#include "qlabelinfo.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimerEvent>
#include <QMovie>

QLabelInfo::QLabelInfo(QWidget *parent) :
    QLabel(parent)
{
    timerShowInfo = 0;
    
    processAnimation.setFileName(":/images/assets/waiting_process.gif");
    processAnimation.start();
    processAnimation.stop();
}

void QLabelInfo::showProcess(int length)
{
    processAnimation.start();
    setMovie(&processAnimation);
    show();
    if (timerShowInfo) {
        killShowInfoTimer();
    }
    if (length) {
        timerShowInfo = startTimer(1000);
    }
}

void QLabelInfo::hideProcess()
{
    hide();
    processAnimation.stop();
    if (timerShowInfo) {
        killShowInfoTimer();
    }    
}

void QLabelInfo::killShowInfoTimer()
{
    killTimer(timerShowInfo);
    timerShowInfo = 0;   
}

void QLabelInfo::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == timerShowInfo) {
        hideProcess();
    }
}
