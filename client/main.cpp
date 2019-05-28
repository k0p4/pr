#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "controller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    Controller::instance()->registerTypes();

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QSettings settings("server_settings.ini", QSettings::IniFormat);
    QString url = settings.value("url", "").toString();

    QUrl serverUrl = QUrl(url.isEmpty() ? QStringLiteral("local:registry") : url);

    Controller::instance()->connectToHost(serverUrl);

    return app.exec();
}
