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
    Q_PROPERTY(QString progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
public:
    explicit Upgrader(QObject *parent = nullptr);

    QString progress() const { return QString::number(m_progress,'f',2); }
    QString message() const {return m_message; }

    void updateKernel();
    void updateServo();
    void updateApplication();
    void updateDatabase();
    void backupFactoryApplication();
    void recoveryFactoryApplication();

    Q_INVOKABLE void start();
    bool initDatabaseInterface();

signals:
    void progressChanged(float);
    void messageChanged(QString);
    void marqueeStart();
    void marqueeFinish();

protected:
    void exitCount();

protected slots:
    void updateMessage(QString msg);
    void updateProgress();
    void updateFinished(int exitCode);
    void processStarted();
    void processError(QProcess::ProcessError error);
    void rebootSystem();

private:
    float m_progress=0;
    QString m_message;
    int m_exitCounter;
    int m_rebootTimerId=-1;

    QString m_operateType,m_operateDir,m_operateFilesName;
    QProcess m_process;
    QByteArray m_shell;
    QTimer *rebootTimer;
};

#endif // UPGRADER_H
