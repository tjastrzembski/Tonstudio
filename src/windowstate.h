#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H

#include <QQmlApplicationEngine>
#include <QQuickItem>

class WindowStateSingleton
{
public:
    ~WindowStateSingleton();
    static WindowStateSingleton *instance();
    int getState() const;
    void setState(const int &);
    void setEngine(QQmlApplicationEngine *e);
    QQmlApplicationEngine *getEngine();

    enum Window { LOGIN = 0, REGISTER = 1, SOUNDSTUDIO = 2, EXIT = 3 };

private:
    WindowStateSingleton() { state = WindowStateSingleton::LOGIN; }
    WindowStateSingleton(const WindowStateSingleton &) {}
    WindowStateSingleton &operator=(const WindowStateSingleton &)
    {
        return *windowState;
    }

    static WindowStateSingleton *windowState;
    static int state;
    static QQmlApplicationEngine *engine;
};

class WindowState : public QQuickItem
{
    Q_OBJECT
public:
    WindowState(QQuickItem *parent = nullptr);
    ~WindowState() = default;
    Q_INVOKABLE void setState(const int &);
    Q_INVOKABLE int getState();

private:
    WindowStateSingleton *windowState;
};

#endif // WINDOWSTATE_H
