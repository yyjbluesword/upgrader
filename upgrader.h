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
    MyThread(QByteArray cmd);
    void run();
    void executeShell(const char *shell);
    void upgradeDatabase();
    void backupDatabase();
    void backupFactoryApplication();
    void recoveryDatabase();
    void showTranslate(QString source);
    bool upgradeApplication;
signals:
    void statusChanged(QString status);
    void progressChanged(QString progress);
private:
    float m_progress = 0;
    QString m_operateType;
};

class Upgrader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QString progress READ progress WRITE setProgress NOTIFY progressChanged)
public:
    explicit Upgrader(QByteArray cmd, QObject *parent = nullptr);
    QString message() const {return m_message; }
    QString progress() const {return m_progress; }
    Q_INVOKABLE void start();
    void setMessage(const QString msg){
        if(msg != m_message){
            m_message = msg;
            emit messageChanged();
        }
    }
    void setProgress(const QString progress){
        if(progress != m_progress){
            m_progress = progress;
            emit progressChanged();
        }
    }

    bool upgradeApplication;

signals:
    void messageChanged();
    void progressChanged();
    void marqueeStart();
    void marqueeFinish();
protected slots:
    void rebootSystem();
    void restartApp();
    void updateStatus(QString status);
    void updateProgress(QString progress);

private:
    QString m_progress = "0.00";
    QString m_message;
    int m_exitCounter;
    int m_rebootTimerId=-1;

    QString m_operateType,m_operateDir,m_operateFilesName;
    QString m_operateStatus;
    QProcess m_process;
    QTimer *rebootTimer, *restartAppTimer,*statusTimer;
    MyThread *statusThread;
    QByteArray m_cmd;
};

#endif // UPGRADER_H
