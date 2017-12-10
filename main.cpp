#include "mainwindow.h"
#include <QApplication>
#include <cpp_redis/cpp_redis>

int main(int argc, char *argv[])
{
    std::cout << "Test" << std::endl;

    cpp_redis::client rClient;
    std::cout << "Test2" << std::endl;
    rClient.connect("127.0.0.1", 7000);
    std::cout << rClient.set("hello", "42").get() << std::endl;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
