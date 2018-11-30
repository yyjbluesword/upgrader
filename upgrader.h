#ifndef UPGRADER_H
#define UPGRADER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    void run();
    void upgradeDatabase();
    void backupDatabase();
signals:
    void statusChanged(QString status);
};

class Upgrader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
public:
    explicit Upgrader(QObject *parent = nullptr);
    QString message() const {return m_message; }
    void updateKernel();
    //void updateServo();
    //void updateApplication();
    //void upgradeDatabase();
    //void backupDatabase();
    //void backupFactoryApplication();
    //void recoveryDatabase();
    //void recoveryFactoryApplication();
    Q_INVOKABLE void start();
    void setMessage(const QString msg){
        if(msg != m_message){
            m_message = msg;
            emit messageChanged();
        }
    }

signals:
    void messageChanged();
    void marqueeStart();
    void marqueeFinish();
protected slots:
    void rebootSystem();
    void restartApp();
    void updateStatus(QString status);

private:
    QString m_message;
    int m_exitCounter;
    int m_rebootTimerId=-1;

    QString m_operateType,m_operateDir,m_operateFilesName;
    QString m_operateStatus;
    QProcess m_process;
    QTimer *rebootTimer, *restartAppTimer,*statusTimer;
    MyThread statusThread;
};

#endif // UPGRADER_H
