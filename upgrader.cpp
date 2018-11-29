#include "upgrader.h"
#include <QRegularExpression>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QLibrary>
#include <QDir>
#include "cJSON.h"
#include "db_manager.h"
#include "db_context.h"

Upgrader::Upgrader(QObject *parent) : QObject(parent)
{
    rebootTimer = new QTimer(this);
    m_operateType = qgetenv("OPERATE_TYPE");
    m_operateDir = qgetenv("oDir");
    m_operateFilesName = qgetenv("oFilesName");

    connect(&m_process, &QProcess::readyReadStandardOutput,
            this, &Upgrader::updateProgress, Qt::DirectConnection);
    connect(&m_process, static_cast<void (QProcess::*)(int,QProcess::ExitStatus)>(&QProcess::finished),
            this,&Upgrader::updateFinished);
    connect(&m_process,&QProcess::started,
            this, &Upgrader::processStarted);
    connect(rebootTimer, SIGNAL(timeout()),
            this, SLOT(rebootSystem()));
    connect(&m_process, SIGNAL(error(QProcess::ProcessError)),
            this,SLOT(processError(QProcess::ProcessError)));
}

void Upgrader::updateKernel(){
    m_shell = "/rbctrl/updater.sh";
    system(m_shell);
}

void Upgrader::updateServo(){
    qDebug()<<"updateServo was invoked!!!!!\n";
    m_shell = "echo update servo";
    m_process.start(m_shell);
}

void Upgrader::updateApplication(){
    qDebug()<<"updateApplication was invoked!!!!!\n";
    m_shell = "echo update Application";
    //m_shell = "";
    m_process.start(m_shell.data());
}

void Upgrader::updateDatabase(){
    updateMessage("start to update Database");
    db_manager db_mrg;
    const char *upgrade_pkg = "/update/elibotDB.upgrade.pkg";
    const char *CONN_STRINGS = "/rbctrl/db/elibotDB.db";
    const char *DB_DIR = "/rbctrl";
    int ok = new_upgrade_db_manager(CONN_STRINGS, DB_DIR, upgrade_pkg, &db_mrg);
    if(ok == DB_OK){
        ok = db_mrg.execute(&db_mrg, NULL);
        if(ok == DB_OK){
            updateMessage(tr("update datebase success!"));
            return;
        }
    }
    updateMessage(tr("update database fail.\n"));
}

void Upgrader::updateProgress(){
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

void Upgrader::updateMessage(const QString msg){
    if(msg==m_message)
        return;
    m_message = msg;
    emit messageChanged(m_message);
}

void Upgrader::rebootSystem()
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

void Upgrader::processStarted(){
    emit marqueeStart();
}


void Upgrader::updateFinished(int exitCode){
    emit marqueeFinish();
    m_exitCounter = 4;
    rebootTimer->start(5000);
}

void Upgrader::processError(QProcess::ProcessError error)
{
    qDebug()<<"processError error num is "<<error;
    updateMessage(tr("Failure to start an external process!!!"));
}

void Upgrader::start(){
    if(m_operateType.compare("updateKernel") == 0){
        updateKernel();
    }else if(m_operateType.compare("updateServo") == 0){
        updateServo();
    }else if(m_operateType.compare("updateApplication") == 0){
        updateApplication();
    }else if(m_operateType.compare("updateDatabase") == 0){
        updateDatabase();
    }else if(m_operateType.compare("backupFactoryApplication") == 0){
        backupFactoryApplication();
    }else if(m_operateType.compare("recoveryFactoryApplication") == 0){
        recoveryFactoryApplication();
    }
}

void Upgrader::backupFactoryApplication()
{
    m_shell = "/update/backupData.sh";
    m_process.start(m_shell);
}

void Upgrader::recoveryFactoryApplication()
{
    m_shell = "/update/recovery.sh";
    m_process.start(m_shell);
}
