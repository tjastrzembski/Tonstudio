#include "SoundComponentGraphic.h"
#include "GeneralConfigs.h"
#include < QBrush >
#include <QPainter>
#include <cpp_redis\cpp_redis>
#include <iostream>

SoundComponentGraphic::SoundComponentGraphic(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_sc(nullptr), drawingChanged(true)
{
}

SoundComponentGraphic::~SoundComponentGraphic() { delete m_sc; }

QString SoundComponentGraphic::name()
{
    return QString(m_sc->getName().c_str());
}

void SoundComponentGraphic::setName(const QString &name)
{
    if (name.toStdString() == m_sc->getName())
        return;

    m_sc->setName(name.toStdString());
    emit nameChanged();
}

void SoundComponentGraphic::paint(QPainter *painter)
{
    if (drawingChanged) {
        drawingChanged = false;
        // prepare Brush
        QBrush brush(QColor("#007430"));
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen); //(Qt::NoPen);Qt::SolidLine)
        painter->setRenderHint(QPainter::Antialiasing);

        // setup cpp_redis
        cpp_redis::client rClient;
        std::future<cpp_redis::reply> redisAnswer;
        rClient.connect(REDIS_HOST, REDIS_PORT);

        // get all values for this stream
        redisAnswer = rClient.hlen(m_sc->getName());
        rClient.sync_commit();

        auto wS = .1f;
        // this->setProperty("width", wS * redisAnswer.get().as_integer());

        // draw groundlines
        QSizeF &itemSize = size();
        auto ihMmono = itemSize.height() * 0.5f;
        auto ihMleft = itemSize.height() * 0.25f;
        auto ihMRight = itemSize.height() * 0.75f;

        const QPointF middleLineLeft[5] = {
            QPointF(0, ihMleft - 0.5), QPointF(itemSize.width(), ihMleft - 0.5),
            QPointF(itemSize.width(), ihMleft + 0.5),
            QPointF(0, ihMleft + 0.5)};
        const QPointF middleLine[5] = {QPointF(0, ihMmono - 0.5),
                                       QPointF(itemSize.width(), ihMmono - 0.5),
                                       QPointF(itemSize.width(), ihMmono + 0.5),
                                       QPointF(0, ihMmono + 0.5)};
        const QPointF middleLineRight[5]
            = {QPointF(0, ihMRight - 0.5),
               QPointF(itemSize.width(), ihMRight - 0.5),
               QPointF(itemSize.width(), ihMRight + 0.5),
               QPointF(0, ihMRight + 0.5)};

        painter->drawPolygon(middleLineLeft, 4);
        painter->drawPolygon(middleLine, 4);
        painter->drawPolygon(middleLineRight, 4);

        redisAnswer = rClient.hvals(m_sc->getName());
        rClient.sync_commit();
        std::vector<cpp_redis::reply> tmp = redisAnswer.get().as_array();

        // init drawCoordinates for soundbar
        std::vector<QPointF> pLeft;
        std::vector<QPointF> pRight;
        long i(0);
        for (auto iter = tmp.cbegin(); iter != tmp.cend();) {
            if (NUM_CHANNELS == 2) {

                float leftVal = std::stof((*iter++).as_string());
                float rightVal = std::stof((*iter++).as_string());

                // Left Stream / Upper Bar
                pLeft.push_back(QPointF(i * wS, ihMleft + leftVal * ihMleft));
                pLeft.push_back(QPointF(i * wS, ihMleft - leftVal * ihMleft));
                pLeft.push_back(
                    QPointF(i * wS + wS, ihMleft - leftVal * ihMleft));
                pLeft.push_back(
                    QPointF(i * wS + wS, ihMleft + leftVal * ihMleft));

                // Right Stream / Bottom Bar
                pRight.push_back(
                    QPointF(i * wS, ihMRight + rightVal * ihMleft));
                pRight.push_back(
                    QPointF(i * wS, ihMRight - rightVal * ihMleft));
                pRight.push_back(
                    QPointF(i * wS + wS, ihMRight - rightVal * ihMleft));
                pRight.push_back(QPointF(
                    i * wS + wS, ihMRight + rightVal * ihMleft)); /* right */
                i++;
                painter->drawPolygon(pLeft.data(), 4);
                painter->drawPolygon(pRight.data(), 4);
                pLeft.clear();
                pRight.clear();
            }
            // todo: drawing for mono
        }
        rClient.disconnect();
    }
}

void SoundComponentGraphic::setSoundComponent(SoundComponent *sc)
{
    if (m_sc)
        delete m_sc;
    m_sc = sc;
}
SoundComponent *SoundComponentGraphic::getSoundComponent() { return m_sc; }
