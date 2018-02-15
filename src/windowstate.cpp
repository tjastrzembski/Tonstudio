#include "windowstate.h"

WindowStateSingleton *WindowStateSingleton::windowState;
int WindowStateSingleton::state;
QQmlApplicationEngine *WindowStateSingleton::engine;

WindowStateSingleton::~WindowStateSingleton()
{
    if (windowState)
        delete windowState;
    windowState = nullptr;
}

WindowStateSingleton *WindowStateSingleton::instance()
{
    if (!windowState)
        windowState = new WindowStateSingleton();
    return windowState;
}

void WindowStateSingleton::setState(const int &nState) { state = nState; }

int WindowStateSingleton::getState() const { return state; }

void WindowStateSingleton::setEngine(QQmlApplicationEngine *e) { engine = e; }

QQmlApplicationEngine *WindowStateSingleton::getEngine() { return engine; }

WindowState::WindowState(QQuickItem *parent) : QQuickItem(parent)
{
    windowState = WindowStateSingleton::instance();
}

void WindowState::setState(const int &state) { windowState->setState(state); }

int WindowState::getState() { return windowState->getState(); }
