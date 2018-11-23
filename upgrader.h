#ifndef UPGRADER_H
#define UPGRADER_H


#include <QObject>
#include <QProcess>
#include <QString>
#include <QTimer>

class Updater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
public:
    explicit Updater(QObject *parent = nullptr);

    QString progress() const { return QString::number(m_progress,'f',2); }
    QString message() const { return m_message; }

    void updateKernel();
    void updateServo();
    void updateApplication();
    void updateDatabase();

    Q_INVOKABLE void start();

signals:
    void progressChanged(float);
    void messageChanged(QString);
    void marqueeStart();
    void marqueeFinish();

protected:
    void exitCount();

protected slots:
    void updateMessage(const QString &msg);
    void updateProgress();
    void updateFinished(int exitCode);
    void processStarted();
    void processError(QProcess::ProcessError error);
    void rebootSystem();

private:
    float m_progress=0; //100.00%
    QString m_message;
    int m_exitCounter;
    int m_rebootTimerId=-1;

    QString m_operateType,m_operateDir,m_operateFilesName;
    QProcess m_process;
    QByteArray m_shell;
    QTimer *rebootTimer;
};

#endif // UPGRADER_H
