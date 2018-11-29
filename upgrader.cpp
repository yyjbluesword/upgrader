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
    restartAppTimer = new QTimer(this);
    statusTimer = new QTimer(this);
    m_operateType = qgetenv("OPERATE_TYPE");
    m_exitCounter = 4;
    qDebug()<<"m_operateType = "<<m_operateType;
    m_operateStatus = "NOOPERATE";
    connect(rebootTimer, SIGNAL(timeout()),
            this, SLOT(rebootSystem()));
    connect(restartAppTimer, SIGNAL(timeout()),
            this, SLOT(restartApp()));
    connect(statusTimer,SIGNAL(timeout()),
            this, SLOT(updateStatus()));
    statusTimer->start(5000);
    statusTimer->setInterval(100);
}

//function : QML创建完成后调用程序
void Upgrader::start(){
    if(m_operateType.compare("upgradeKernel") == 0){
        setMessage(tr("Start to upgrade Kernel."));
        updateKernel();
    }else if(m_operateType.compare("upgradeServo") == 0){
        //updateServo();
    }else if(m_operateType.compare("upgradeApplication") == 0){
        //updateApplication();
    }else if(m_operateType.compare("upgradeDatabase") == 0){
        setMessage(tr("Strart to upgrade Database."));
        updateDatabase();
    }else if(m_operateType.compare("backupDatabase") == 0){
        setMessage(tr("Start to backup Database"));
        backupDatabase();
    }else if(m_operateType.compare("backupFactoryApplication") == 0){
        setMessage(tr("Start to backup Factory application."));
        backupFactoryApplication();
    }else if(m_operateType.compare("recoveryFactoryApplication") == 0){
        recoveryFactoryApplication();
    }
}

//function : 升级内核程序
void Upgrader::updateKernel(){
    marqueeStart();
    system("/update/upgradeKernel.sh");
}

/*
void Upgrader::updateServo(){
    qDebug()<<"updateServo was invoked!!!!!\n";
    m_shell = "echo update servo";
    //m_process.start(m_shell);
}
*/

/*
void Upgrader::updateApplication(){
    m_shell = "echo update Application";
    //m_process.start(m_shell.data());
}
*/

//function : 升级数据库函数接口
void Upgrader::updateDatabase(){
    marqueeStart();
    db_manager db_mrg;
    const char *upgrade_pkg = "/update/elibotDB.upgrade.pkg";
    const char *CONN_STRINGS = "/rbctrl/db/elibotDB.db";
    const char *DB_DIR = "/rbctrl/db";
    int ok = new_upgrade_db_manager(CONN_STRINGS, DB_DIR, upgrade_pkg, &db_mrg);
    if(ok == DB_OK){
        ok = db_mrg.execute(&db_mrg, NULL);
        if(ok == DB_OK){
            setMessage(tr("Update Database Success!"));
            marqueeFinish();
            rebootTimer->start(5000);
            return;
        }
    }
    setMessage(tr("Update Database Failed!"));
    marqueeFinish();
    rebootTimer->start(5000);
}

//function : 备份数据库数据
void Upgrader::backupDatabase()
{
    marqueeStart();
    db_manager db_mrg;
    const char *CONN_STRINGS = "/rbctrl/db/elibotDB.db";
    const char *DB_DIR = "/mnt/udisk/db";
    const char *ELIBOT_BAK_BACKUP_PARAMS = "elibot.bak.backupParams";
    char bak[256] = {0};
    char cmds[256] = {0};
    sprintf(cmds, "%s", ELIBOT_BAK_BACKUP_PARAMS);
    int ok = new_backup_db_manager(CONN_STRINGS, DB_DIR, &db_mrg);
    if(ok == DB_OK){
        set_cmds(&db_mrg, cmds);
        ok = db_mrg.execute(&db_mrg, bak);
        if(ok == DB_OK){
            qDebug()<<"bak = "<<bak;
            setMessage(tr("backup Database Success"));
            marqueeFinish();
            restartAppTimer->start(5000);
            return;
        }
    }
    setMessage(tr("backup Database failed"));
    marqueeFinish();
    restartAppTimer->start(5000);
    return;
}

//function : 备份出厂应用程序
void Upgrader::backupFactoryApplication()
{
    marqueeStart();
    ::system("/update/backupFactoryApplication.sh");
}
void Upgrader::recoveryFactoryApplication()
{
    //m_shell = "/update/recovery.sh";
    //m_process.start(m_shell);
}

/*void Upgrader::updateProgress(){
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

}*/
/*
void Upgrader::updateMessage(const QString msg){
    if(msg==m_message)
        return;
    m_message = msg;
    //emit messageChanged(m_message);
}
*/

//function : 系统重启函数
void Upgrader::rebootSystem()
{
    //marqueeFinish();
    rebootTimer->setInterval(1000);
    m_exitCounter--;
    if(m_exitCounter == 0){
        setMessage(tr("System rebooting"));
        rebootTimer->stop();
        if(m_operateType == "upgradeDatabase"){
            ::system("/update/upgradeDatabase.sh");
        }else{
            ::system("reboot");
        }
        return;
    }
    setMessage(tr("System will reboot in %1s").arg(m_exitCounter));
}

//function : 重启应用程序函数
void Upgrader::restartApp(){
    restartAppTimer->setInterval(1000);
    m_exitCounter--;
    if(m_exitCounter == 0){
        setMessage(tr("App rebooting"));
        restartAppTimer->stop();
        system("/update/restartCobot.sh");
        m_exitCounter = 4;
        return;
    }
    setMessage(tr("App will reboot in %1s").arg(m_exitCounter));
}

//function: 监事系统环境变量，显示当前状态
void Upgrader::updateStatus(){
    QString operateStatus = qgetenv("OPERATE_STATUS");
    //qDebug()<<"operateStatus = "<<operateStatus;
    if(m_operateStatus.compare(operateStatus) == 0)
        return;
    m_operateStatus = operateStatus;
    if(operateStatus.compare("OPERATEING")){
        if(m_operateType.compare("upgradeKernel") == 0){
            setMessage(tr("upgarding kernel"));
        }else if(m_operateType.compare("backupFactoryApplication") == 0){
            setMessage(tr("backup Factory Application"));
        }
    }else if(operateStatus.compare("OPERATEOVER") == 0){
        marqueeFinish();
        if(m_operateType.compare("upgradeKernel") == 0){
            setMessage(tr("upgrading kernel finished"));
        }else if(m_operateType.compare("backupFactoryApplication") == 0){
            setMessage(tr("backup Factory Application finished."));
        }
        qputenv("OPERATE_STATUS","NOOPERATE");
        m_exitCounter = 4;
        rebootTimer->start(5000);
    }
}

/*void Upgrader::processStarted(){
    emit marqueeStart();
}*/


/*
void Upgrader::updateFinished(int exitCode){
    emit marqueeFinish();
    m_exitCounter = 4;
    rebootTimer->start(5000);
}*/

/*
void Upgrader::processError(QProcess::ProcessError error)
{
    qDebug()<<"processError error num is "<<error;
    updateMessage(tr("Failure to start an external process!!!"));
}
*/
