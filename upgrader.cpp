#include "upgrader.h"
#include <QRegularExpression>
#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QLibrary>
#include <QDir>

Upgrader::Upgrader(QObject *parent) : QObject(parent)
{
    rebootTimer = new QTimer(this);
    m_shell = "./updater.sh";
    m_operateType = qgetenv("oType");
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
    initDatabaseInterface();

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

void Upgrader::updateKernel(){
    qDebug()<<"updateKernel was invoked!!!!!\n";
}

void Upgrader::updateServo(){
    qDebug()<<"updateServo was invoked!!!!!\n";
}

void Upgrader::updateApplication(){
    qDebug()<<"updateApplication was invoked!!!!!\n";
}

void Upgrader::updateDatabase(){
    qDebug()<<"updateDatabase was invoked!!!!!\n";
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

void Upgrader::updateFinished(int exitCode){
    emit marqueeFinish();
    m_exitCounter = 4;
    rebootTimer->start(5000);
}

void Upgrader::updateMessage(const QString &msg){
    if(msg==m_message)
        return;
    m_message = msg;
    emit messageChanged(msg);
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
    qDebug()<<"yyj output processStarted\n";
    emit marqueeStart();
}

void Upgrader::start(){
    qDebug()<<"Upgrader::start function was invoked!!!!!\n";
    m_process.start(m_shell);
}

void Upgrader::processError(QProcess::ProcessError error)
{
    qDebug()<<"processError error num is "<<error;
    updateMessage(tr("Failure to start an external process!!!"));
}

bool Upgrader::initDatabaseInterface()
{
    QDir dir("/rbctrl/mcserver_plugin/libsqlitedb.so");
    QString path = dir.absolutePath();
    QLibrary *databaseLibrary = new QLibrary(path);
    databaseLibrary->load();
    typedef int(*Test)();
    Test t = (Test)databaseLibrary->resolve("test_get_all_record");
    if(t){
        qDebug()<<"test_get_all_record function was invoked!!!"<<endl;
        t();
    }
    return true;
}
