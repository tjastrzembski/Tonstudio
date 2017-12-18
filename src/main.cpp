#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <cpp_redis/cpp_redis>
#include <libbson-1.0\bson.h>
#include <libbson-1.0\bcon.h>
#include <libmongoc-1.0\mongoc.h>


int main(int argc, char *argv[])
{
#if _WIN32
    WSADATA wsaData;
    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
#endif // _WIN32

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    cpp_redis::client rClient;
    std::future<cpp_redis::reply> redisAnswer;

    rClient.connect("127.0.0.1", 6379);
    std::cout << "set hello 42" << std::endl;
    redisAnswer = rClient.set("hello", "42");
    rClient.sync_commit();

    std::cout << redisAnswer.get() << std::endl;
    std::cout << "get hello" << std::endl;
    rClient.get("hello", [](cpp_redis::reply &reply) {
        std::cout << reply << std::endl;
    });

    //! also support std::future
    //! std::future<cpp_redis::reply> get_reply = client.get("hello");

    rClient.sync_commit();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/mainWindow.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
