#ifndef UPGRADER_H
#define UPGRADER_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QDebug>

class Upgrader : public QObject
{
    Q_OBJECT
//    Q_PROPERTY(QString progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)
//    Q_PROPERTY(QString operateType READ operateType NOTIFY operateTypeChanged)
public:
    explicit Upgrader(QObject *parent = nullptr);

//    QString progress() const { return QString::number(m_progress,'f',2); }
    QString message() {return m_message; }

    void setMessage(QString msg){
        if (msg!= m_message){
            m_message = msg;
            emit messageChanged();
        }
    }

    void updateKernel();
//    QString operateType() const {return m_operateType;}

//    void updateKernel();
    //void updateServo();
    //void updateApplication();
    void updateDatabase();
    //void backupFactoryApplication();
    //void recoveryFactoryApplication();
    void backupDatabase();

    Q_INVOKABLE void start();
    //bool initDatabaseInterface();

signals:
    void messageChanged();
    //void progressChanged(float);
    void marqueeStart();
    void marqueeFinish();
    //void operateTypeChanged(QString);

    /*
protected:
    void exitCount();
    */

protected slots:
    //void messageChanged(QString);
    //void updateMessage(QString msg);
    //void updateProgress();
    //void updateFinished(int exitCode);
    //void processStarted();
    //void processError(QProcess::ProcessError error);
    void rebootSystem();
    void restartApp();
    void updateStatus();

private:
    float m_progress=0;
    QString m_message;
    int m_exitCounter;
    int m_rebootTimerId=-1;

    QString m_operateType,m_operateDir,m_operateFilesName;
    QString m_operateStatus;
    QProcess m_process;
    QByteArray m_shell;
    QTimer *rebootTimer, *restartAppTimer,*statusTimer;
};

#endif // UPGRADER_H
