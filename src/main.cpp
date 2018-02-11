#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <cpp_redis/cpp_redis>
#include <mongoc.h>
#include <portaudio.h>

#include "GeneralConfigs.h"
#include "ProjectManager.h"
#include "SoundComponentGraphic.h"
#include "cassandra.h"
#include "windowstate.h"

// cassandra related
static CassCluster *cluster;
static CassSession *session;
static CassFuture *connect_future;

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
        rClient.connect(REDIS_HOST, REDIS_PORT);
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

    std::cout << "init Cassandra" << std::endl;
    // Setup and connect to cluster
    cluster = cass_cluster_new();
    session = cass_session_new();

    // Add contact points
    cass_cluster_set_contact_points(cluster, "127.0.0.1");

    // Provide the cluster object as configuration to connect the session
    connect_future = cass_session_connect(session, cluster);

    // This operation will block until the result is ready
    CassError rc = cass_future_error_code(connect_future);

    std::cout << "Connect result: " << cass_error_desc(rc) << std::endl;

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

    // Release/clean cassandra stuff
    cass_future_free(connect_future);
    cass_session_free(session);
    cass_cluster_free(cluster);

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
    QQmlApplicationEngine engine;

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
    return err;
}
