#include "windowstate.h"

WindowStateSingleton *WindowStateSingleton::windowState;
int WindowStateSingleton::state;
int WindowStateSingleton::dialog;

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

void WindowStateSingleton::setDialogState(const int &nState)
{
    dialog = nState;
}

int WindowStateSingleton::getDialogState() const { return dialog; }

WindowState::WindowState(QQuickItem *parent) : QQuickItem(parent)
{
    windowState = WindowStateSingleton::instance();
}

void WindowState::setState(const int &state) { windowState->setState(state); }

int WindowState::getState() { return windowState->getState(); }

void WindowState::setDialogState(const int &state)
{
    windowState->setDialogState(state);
}

int WindowState::getDialogState() { return windowState->getDialogState(); }
