#include "windowstate.h"

WindowStateSingleton *WindowStateSingleton::windowState;
int WindowStateSingleton::state;

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

WindowState::WindowState(QQuickItem *parent) : QQuickItem(parent)
{
    windowState = WindowStateSingleton::instance();
}

void WindowState::setState(const int &state) { windowState->setState(state); }

int WindowState::getState() { return windowState->getState(); }
