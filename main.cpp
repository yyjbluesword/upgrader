#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include "upgrader.h"
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QTranslator translator;
    translator.load(":/zh_CN.qm");
    app.installTranslator(&translator);

    Upgrader *upgrader = new Upgrader();
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("upgrader",upgrader);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
