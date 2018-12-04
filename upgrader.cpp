#include "upgrader.h"
#include <QRegularExpression>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QLibrary>
#include <QDir>
#include "include/cJSON.h"
#include "include/db_manager.h"
#include "include/db_context.h"

Upgrader::Upgrader(QByteArray cmd,QObject *parent) : QObject(parent),m_cmd(cmd)
{

    rebootTimer = new QTimer(this);
    restartAppTimer = new QTimer(this);
    statusThread = new MyThread(cmd);
    if(cmd.length()!=0){
        QString tmp(cmd);
        if(tmp.compare("upgradeKernel") == 0 ||
                tmp.compare("upgradeDatabase") == 0 ||
                tmp.compare("upgradeApplication") == 0 )
            m_operateType = tmp;
        else
            m_operateType = "upgradeApplication";
    }else{
        m_operateType = qgetenv("OPERATE_TYPE");
    }
    m_exitCounter = 4;

    connect(rebootTimer, SIGNAL(timeout()),
            this, SLOT(rebootSystem()));
    connect(restartAppTimer, SIGNAL(timeout()),
            this, SLOT(restartApp()));
    connect(statusThread,SIGNAL(statusChanged(QString)),
            this,SLOT(updateStatus(QString)));
    connect(statusThread,SIGNAL(progressChanged(QString)),
            this,SLOT(updateProgress(QString)));
}

//function : QML创建完成后调用程序
void Upgrader::start()
{
    marqueeStart();
    if(m_operateType.compare("upgradeKernel") == 0 ||
            m_operateType.compare("upgradeDatabase") == 0 ||
            m_operateType.compare("backupDatabase") == 0 ||
            m_operateType.compare("backupFactoryApplication") == 0 ||
            m_operateType.compare("recoveryDatabase") == 0 ||
            m_operateType.compare("upgradeApplication") == 0){
        statusThread->start();
    }
}

void Upgrader::updateStatus(QString status){
    if(status.compare("success") == 0){
        if(m_operateType.compare("upgradeKernel") == 0 ||
                m_operateType.compare("upgradeDatabase") == 0 ||
                m_operateType.compare("backupFactoryApplication") == 0 ||
                m_operateType.compare("recoveryDatabase") == 0){
            marqueeFinish();
            rebootTimer->start(5000);
        }else if(m_operateType.compare("backupDatabase") == 0){
            marqueeFinish();
            restartAppTimer->start(5000);
        }
        return;
    }else if(status.compare("failed") == 0){
        marqueeFinish();
        return;
    }
    setMessage(status);
}

void Upgrader::updateProgress(QString progress){
    if(progress.compare("100.00") == 0){
        marqueeFinish();
        if(m_operateType.compare("upgradeApplication") == 0){
            setMessage(tr("Upgrade Application success."));
        }
        rebootTimer->start(5000);
    }
    setProgress(progress);
}

//function : 系统重启函数
void Upgrader::rebootSystem()
{

    rebootTimer->setInterval(1000);
    m_exitCounter--;
    if(m_exitCounter == 0){
        setMessage(tr("System rebooting"));
        rebootTimer->stop();
        if(m_operateType.compare("upgradeDatabase") == 0){
            ::system("/update/upgradeDatabase.sh");
        }else if(m_operateType.compare("recoveryDatabase") == 0){
            ::system(("/update/recoveryDatabase.sh"));
        }else{
            ::system("reboot");
        }
        return;
    }
    setMessage(tr("System will reboot in %1s").arg(m_exitCounter));

}

//function : 应用重启程序函数
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

//function
MyThread::MyThread(QByteArray cmd)
{
    qDebug()<<"MyThread cmd.length = "<<cmd.length();
    if(cmd.length()!=0){
        QString tmp(cmd);
        if(tmp.compare("upgradeKernel") == 0 ||
                tmp.compare("upgradeDatabase") == 0 ||
                tmp.compare("upgradeApplication") == 0){
            m_operateType = tmp;
        }else
            m_operateType = "upgradeApplication";
    }else{
        m_operateType = qgetenv("OPERATE_TYPE");
    }
}

void MyThread::run(){
    if(m_operateType.compare("upgradeKernel") == 0){
        executeShell("/update/upgradeKernel.sh");
    }else if(m_operateType.compare("upgradeApplication") == 0){
        executeShell("/mnt/tmp/update-final.sh");
    }else if(m_operateType.compare("upgradeDatabase") == 0){
        upgradeDatabase();
    }else if(m_operateType.compare("backupDatabase") == 0){
        backupDatabase();
    }else if(m_operateType.compare("backupFactoryApplication") == 0){
        executeShell("/update/backupFactoryApplication.sh");
    }else if(m_operateType.compare("recoveryDatabase") == 0){
        recoveryDatabase();
    }
}

//function : 执行Shell接口
void MyThread::executeShell(const char *shell)
{
    FILE *fpRead;
    fpRead = popen(shell,"r");
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    while(fgets(buf,1024-1,fpRead) != NULL)
    {
        QString buffer(buf);
        //////////////////////////////////////////
        QRegularExpression re("progress ([0-9]*\\.?[0-9]+)%");
        QRegularExpressionMatch match = re.match(buffer);
        if (!match.hasMatch()){
            showTranslate(buffer);
        }else{
            bool ok;
            float progress = match.capturedRef(1).toFloat(&ok);
            if(ok){
                if(qAbs(progress-m_progress)>0.0001){
                    m_progress = progress;
                    emit progressChanged(QString::number(m_progress,'f',2));
                }
            }
        }
        memset(buf,0x00,sizeof(buf));
    }
    if(fpRead != NULL)
        pclose(fpRead);
}

//function : 升级数据库接口
void MyThread::upgradeDatabase()
{
    emit statusChanged(tr("start to upgrade database."));
    db_manager db_mrg;
    const char *upgrade_pkg = "/update/elibotDB.upgrade.pkg";
    const char *CONN_STRINGS = "/rbctrl/db/elibotDB.db";
    const char *DB_DIR = "/rbctrl/db";
    int ok = new_upgrade_db_manager(CONN_STRINGS, DB_DIR, upgrade_pkg, &db_mrg);
    if(ok == DB_OK){
        ok = db_mrg.execute(&db_mrg, NULL);
        if(ok == DB_OK){
            emit statusChanged(tr("Upgrade Database Success!"));
            emit statusChanged("success");
            return;
        }
    }
    emit statusChanged(tr("Upgrade Database Failed!"));
    emit statusChanged("failed");
}

//function : 备份数据库数据
void MyThread::backupDatabase()
{
    emit statusChanged(tr("start to backup database."));
    db_manager db_mrg;
    const char *CONN_STRINGS = "/rbctrl/db/elibotDB.db";
    const char *DB_DIR = "/mnt/udisk/rbctrl/db";
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
            emit statusChanged(tr("Backup Database Success!"));
            emit statusChanged("success");
            return;
        }
    }
    emit statusChanged(tr("Backup Database Failed!"));
    emit statusChanged("failed");
}

//function : 恢复数据库数据
void MyThread::recoveryDatabase()
{
    emit statusChanged(tr("start to recovery database."));
    db_manager db_mrg;
    const char *bak="/mnt/udisk/rbctrl/db/parameters.bak";
    char cmds[256] = {0};
    const char *CONN_STRINGS = "/rbctrl/db/elibotDB.db";
    const char *DB_DIR = "/rbctrl/db";
    sprintf(cmds, "%s", "elibot.bak.backupParams");
    int ok = new_restore_db_manager(CONN_STRINGS, DB_DIR, bak, &db_mrg, 0);
    if (ok == DB_OK) {
        ok = db_mrg.execute(&db_mrg, NULL);
        if (ok == DB_OK) {
            emit statusChanged(tr("Recovery Database Success!"));
            emit statusChanged("success");
            return;
        }
    }
    emit statusChanged(tr("Recovery Database Failed!"));
    emit statusChanged("failed");
}

//function : 转换Shell内文本

void MyThread::showTranslate(QString source)
{
    source.chop(1);
    qDebug()<<"source.chop(1) = "<<source;
    if(source.compare("Prepare to upgrade kernel.") == 0)
        emit statusChanged(tr("Prepare to upgrade kernel."));
    else if(source.compare("Prepare upgrade kernel failed.") == 0)
        emit statusChanged(tr("Prepare upgrade kernel failed."));
    else if(source.compare("Start to upgrade kernel.") == 0)
        emit statusChanged(tr("Start to upgrade kernel."));
    else if(source.compare("Upgrade kernel failed.") == 0)
        emit statusChanged(tr("Upgrade kernel failed."));
    else if(source.compare("now,let's waiting for update!") == 0)
        emit statusChanged(tr("now,let's waiting for update!"));
    else if(source.compare("Update firmware") == 0)
        emit statusChanged(tr("Update firmware"));
    else if(source.compare("Please wait.") == 0)
        emit statusChanged(tr("Upgrading Application,please wait."));
    else if(source.compare("Delete temporary data.") == 0)
        emit statusChanged(tr("Delete temporary data."));
    else if(source.compare("Delete temporary data failed.") == 0)
        emit statusChanged(tr("Delete temporary data failed."));
    else if(source.compare("Upgrade kernel success.") == 0)
        emit statusChanged(tr("Upgrade kernel success."));
    else if(source.compare("Start to back up Factory Application") == 0)
        emit statusChanged(tr("Start to back up Factory Application"));
    else if(source.compare("Backup Factory Application Failed!") == 0)
        emit statusChanged(tr("Backup Factory Application Failed!"));
    else if(source.compare("Backup Factory Application success.") == 0)
        emit statusChanged(tr("Backup Factory Application success."));
    else
        emit statusChanged(source);
}
