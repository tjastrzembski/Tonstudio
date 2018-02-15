#include "SoundComponentGraphic.h"
#include "GeneralConfigs.h"
#include <QBrush>
#include <QPainter>
#include <cpp_redis/cpp_redis>
#include <iostream>

SoundComponentGraphic::SoundComponentGraphic(QQuickItem *parent)
    : QQuickPaintedItem(parent), drawingChanged(true)
{
}

SoundComponentGraphic::~SoundComponentGraphic() {}

QString SoundComponentGraphic::name() { return m_name; }

void SoundComponentGraphic::setName(const QString &name)
{
    if (name == m_name)
        return;

    m_name = name;
    emit nameChanged();
}

int SoundComponentGraphic::length() { return m_Length; }

void SoundComponentGraphic::setLength(int len)
{
    if (len == m_name)
        return;

    m_Length = len;
    emit lengthChanged();
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
        std::vector<cpp_redis::reply> tmp;
        rClient.connect(REDIS_HOST, REDIS_PORT);

        // get all values for this stream

        auto wS = .1f;

        // draw groundlines
        QSizeF itemSize = size();
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

        std::vector<std::string> fields;
        for (int j = 0; j < m_Length; j++) {
            fields.push_back(std::to_string(j));
        }
        redisAnswer = rClient.hmget(m_name.toStdString(), fields);
        rClient.sync_commit();
        tmp = redisAnswer.get().as_array();

        // init drawCoordinates for soundbar
        std::vector<QPointF> pLeft;
        std::vector<QPointF> pRight;

        int i(0);
        for (auto iter = tmp.cbegin(); iter != tmp.cend();) {
            if (NUM_CHANNELS == 2) {

                float leftVal = std::stof((*iter++).as_string());
                float rightVal = std::stof((*iter++).as_string());

                // qDebug() << leftVal;
                // qDebug() << rightVal;
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
