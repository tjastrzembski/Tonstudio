#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <portaudio.h>

#include "GeneralConfigs.h"
#include "ProjectManager.h"
#include "RedisInitiator.h"
#include "SoundComponentGraphic.h"
#include "cassandra_handler.h"
#include "windowstate.h"
#include <iostream>

#include <SoundComponentPersistent.h>
#include <sounddevicesettings.h>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

bool initNeccessaryAPIs()
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    std::cout << "init portaudio" << std::endl;
    auto err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return false;
    }
    std::cout << "portaudio initialized" << std::endl;
    RedisInitiator::initRedis();

    std::cout << "init Mongo C" << std::endl;
    mongocxx::instance instance{};
    std::cout << "mongo C initialized" << std::endl;

   Cassandra_Handler::instance()->initCassandra();
    return true;
}

bool terminateAPIs()
{
    auto err = Pa_Terminate();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return false;
    }
    Cassandra_Handler::instance()->terminateCassandra();

    return true;
}

int main(int argc, char *argv[])
{

    if (!initNeccessaryAPIs()) {
        return 1;
    }

    // init Qt
    std::cout << "init QT" << std::endl;
    QGuiApplication app(argc, argv);

    qmlRegisterType<SoundComponentGraphic>(
        "io.qt.examples.SoundComponentGraphic", 1, 0, "SoundComponentGraphic");
    qmlRegisterType<ProjectManager>("io.qt.examples.ProjectManager", 1, 0,
                                    "ProjectManager");
    qmlRegisterType<WindowState>("io.qt.examples.WindowState", 1, 0,
                                 "WindowState");

    // create Connection between Gui and main for window decision issues
    WindowStateSingleton *ws = WindowStateSingleton::instance();
    ws->setState(WindowStateSingleton::SOUNDSTUDIO);
    QQmlApplicationEngine engine;
    ws->setEngine(&engine);
    int err;
    while (ws->getState() != WindowStateSingleton::EXIT) {
        switch (ws->getState()) {
        case WindowStateSingleton::LOGIN: {
            engine.load(QUrl(QStringLiteral("qrc:/Login.qml")));
            break;
        }
        case WindowStateSingleton::REGISTER: {
            engine.load(QUrl(QStringLiteral("qrc:/Register.qml")));
            break;
        }
        case WindowStateSingleton::SOUNDSTUDIO: {
            engine.load(QUrl(QStringLiteral("qrc:/Tonstudio.qml")));
            break;
        }
        default: {
            std::cerr << "unexpected case" << std::endl;
            exit(0);
        }
        }
        if (engine.rootObjects().isEmpty())
            return -1;
        ws->setState(WindowStateSingleton::EXIT);
        err = app.exec();
    }

    terminateAPIs();
    return 0;
}
