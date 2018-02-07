#include "SoundListModel.h"
#include <QQuickView>

SoundListModel::SoundListModel(QList<SoundComponentGraphic *> &sounds)
    : m_sounds(sounds)
{
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    m_QmlCtxt = view.rootContext();
}

void SoundListModel::update()
{
    m_QmlCtxt->setContextProperty("myModel", QVariant::fromValue(m_sounds));
}
