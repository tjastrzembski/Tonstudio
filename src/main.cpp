#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <cpp_redis\cpp_redis>
#include <mongoc.h>
#include <portaudio.h>

#include "GeneralConfigs.h"
#include "ProjectManager.h"
#include "SoundComponentGraphic.h"

bool initNeccessaryAPIs()
{
#if _WIN32
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return false;
    }
#endif // _WIN32

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    auto err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return false;
    }

    cpp_redis::client rClient;
    // check, if redis is available
    std::cout << "check for Redis availability" << std::endl;
    try {
        rClient.connect("127.0.0.1", 7000);
        rClient.disconnect();
        std::cout << "Redis ready." << std::endl;
    } catch (const std::exception &ex) {
        std::cerr << "Can't connect to Redis: " << ex.what() << std::endl;
        std::cerr << "Make sure, that Redis is Running at " << REDIS_HOST << ":"
                  << REDIS_PORT << std::endl;
    }

    std::cout << "init Mongo C" << std::endl;
    mongoc_init();
    std::cout << "mongo C initialized" << std::endl;
    return true;
}

bool terminateAPIs()
{
    auto err = Pa_Terminate();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        return false;
    }

    mongoc_cleanup();
    std::cout << "wut" << std::endl;
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

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Tonstudio.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    auto err = app.exec();
    terminateAPIs();
    return err;
}
