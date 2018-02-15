#ifndef SOUNDLISTMODEL_H
#define SOUNDLISTMODEL_H

#include "SoundComponent.h"
#include <QAbstractListModel>
#include <QQmlContext>

// Forward declaration

class SoundListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SoundRoles { NameRole, SizeRole, MuteRole };

    SoundListModel(QList<SoundComponent *> &sounds, QObject *parent = nullptr);
    ~SoundListModel() = default;
    void update();

    void addSound(SoundComponent *sound);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QQmlContext *m_QmlCtxt;
    QList<SoundComponent *> &m_sounds;
};

#endif // SOUNDLISTMODEL_H
