#ifndef SOUNDCOMPONENTGRAPHIC_H
#define SOUNDCOMPONENTGRAPHIC_H

#include "SoundComponent.h"
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QString>

class SoundComponentGraphic : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(SoundComponent *backend READ getSoundComponent)
public:
    explicit SoundComponentGraphic(QQuickItem *parent = nullptr);
    ~SoundComponentGraphic();

    void paint(QPainter *painter);

    QString name();
    void setName(const QString &name);

    void setSoundComponent(SoundComponent *sc);
    SoundComponent *getSoundComponent();

signals:
    void nameChanged();

public slots:

private:
    SoundComponent *m_sc;
    bool drawingChanged;
};

#endif // SOUNDCOMPONENTGRAPHIC_H
