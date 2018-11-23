#include "upgrader.h"
#include <QRegularExpression>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QLibrary>

Updater::Updater(QObject *parent) : QObject(parent)
{
    rebootTimer = new QTimer(this);
    m_shell = "./updater.sh";
    m_operateType = qgetenv("oType");
    m_operateDir = qgetenv("oDir");
    m_operateFilesName = qgetenv("oFilesName");

    connect(&m_process, &QProcess::readyReadStandardOutput,
            this, &Updater::updateProgress, Qt::DirectConnection);
    connect(&m_process, static_cast<void (QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished),
            this,&Updater::updateFinished);
    connect(&m_process,&QProcess::started,
            this, &Updater::processStarted);
    connect(rebootTimer, SIGNAL(timeout()),
            this, SLOT(rebootSystem()));
    connect(&m_process, SIGNAL(error(QProcess::ProcessError)),
            this,SLOT(processError(QProcess::ProcessError)));

    if(m_operateType.compare("updateKernel") == 0){
        updateKernel();
    }else if(m_operateType.compare("updateServo") == 0){
        updateServo();

    }else if(m_operateType.compare("updateApplication") == 0){
        updateApplication();

    }else if(m_operateType.compare("updateDatabase") == 0){
        updateDatabase();
    }
    //m_process.start(m_shell);
}

void Updater::updateKernel(){
    qDebug()<<"updateKernel was invoked!!!!!\n";
}

void Updater::updateServo(){
    qDebug()<<"updateServo was invoked!!!!!\n";
}

void Updater::updateApplication(){
    qDebug()<<"updateApplication was invoked!!!!!\n";
}

void Updater::updateDatabase(){
    qDebug()<<"updateDatabase was invoked!!!!!\n";
}

void Updater::updateProgress(){

    QByteArray line;
    while(!m_process.atEnd()){
        line = m_process.readLine();
        QRegularExpression re("progress ([0-9]*\\.?[0-9]+)%");
        QRegularExpressionMatch match = re.match(line);
        if(!match.hasMatch()){
            updateMessage(line);
            continue;
        }
        bool ok;
        double progress = match.capturedRef(1).toDouble(&ok);
        if(!ok)
            continue;
        if(qAbs(progress-m_progress) > 0.0001){
            m_progress = progress;
            emit progressChanged(progress);
        }
    }

}

void Updater::updateFinished(int exitCode){
    emit marqueeFinish();
    m_exitCounter = 4;
    rebootTimer->start(5000);
}

void Updater::updateMessage(const QString &msg){
    if(msg==m_message)
        return;
    m_message = msg;
    emit messageChanged(msg);
}

void Updater::rebootSystem()
{
    rebootTimer->setInterval(1000);
    m_exitCounter--;
    if(m_exitCounter == 0){
        updateMessage(tr("System rebooting"));
        rebootTimer->stop();
        //::system("reboot");
        return;
    }
    updateMessage(tr("System will reboot in %1s").arg(m_exitCounter));
}

void Updater::processStarted(){
    qDebug()<<"yyj output processStarted\n";
    emit marqueeStart();
}

void Updater::start(){
    qDebug()<<"Updater::start function was invoked!!!!!\n";
    m_process.start(m_shell);
}

void Updater::processError(QProcess::ProcessError error)
{
    qDebug()<<"processError error num is "<<error;
    updateMessage(tr("Failure to start an external process!!!"));
}
