#ifndef SOUNDLISTMODEL_H
#define SOUNDLISTMODEL_H

#include "SoundComponentGraphic.h"
#include <QList>
#include <QQmlContext>

// Forward declaration

class SoundListModel
{
public:
    SoundListModel(QList<SoundComponentGraphic *> &sounds);
    ~SoundListModel() = default;

    void update();

private:
    QQmlContext *m_QmlCtxt;
    QList<SoundComponentGraphic *> &m_sounds;
};

#endif // SOUNDLISTMODEL_H
