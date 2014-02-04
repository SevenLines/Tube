#ifndef QLABELINFO_H
#define QLABELINFO_H

#include <QLabel>
#include <QMovie>

class QLabelInfo : public QLabel
{
    Q_OBJECT
public:
    explicit QLabelInfo(QWidget *parent = 0);
    
signals:
    
public slots:
    void showProcess(int length = 1000);
    void hideProcess();
    

    // QObject interface
protected:
    QMovie processAnimation;
    int timerShowInfo;
    void killShowInfoTimer();
    void timerEvent(QTimerEvent *);
};

#endif // QLABELINFO_H
