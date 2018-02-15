#include "SoundListModel.h"
#include <QVariant>
#include <windowstate.h>

SoundListModel::SoundListModel(QList<SoundComponent *> &sounds, QObject *parent)
    : QAbstractListModel(parent), m_sounds(sounds)
{
    m_QmlCtxt = WindowStateSingleton::instance()->getEngine()->rootContext();
}

void SoundListModel::update()
{
    m_QmlCtxt->setContextProperty("SoundListModel", QVariant::fromValue(this));
}

void SoundListModel::addSound(SoundComponent *sound)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_sounds << sound;
    endInsertRows();
}

int SoundListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_sounds.count();
}

QVariant SoundListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_sounds.count())
        return QVariant();

    SoundComponent *sound = m_sounds[index.row()];
    if (role == NameRole)
        return QString(sound->getName().c_str());
    if (role == SizeRole)
        return sound->getLength();
    if (role == MuteRole)
        return sound->isMuted();
    return QVariant();
}

QHash<int, QByteArray> SoundListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "Name";
    roles[SizeRole] = "Size";
    roles[MuteRole] = "isMuted";
    return roles;
}
