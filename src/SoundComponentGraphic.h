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
    Q_PROPERTY(int len READ length WRITE setLength NOTIFY lengthChanged)
public:
    explicit SoundComponentGraphic(QQuickItem *parent = nullptr);
    ~SoundComponentGraphic();

    void paint(QPainter *painter);

    QString name();
    void setName(const QString &name);
    int length();
    void setLength(int len);

signals:
    void nameChanged();
    void lengthChanged();

public slots:

private:
    QString m_name;
    int m_Length;
    bool drawingChanged;
};

#endif // SOUNDCOMPONENTGRAPHIC_H
