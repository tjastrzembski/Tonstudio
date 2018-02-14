#include "SoundListModel.h"
#include <QQuickView>

#include "SoundComponentPersistent.h"

SoundListModel::SoundListModel(QList<SoundComponentGraphic *> &sounds)
    : m_sounds(sounds)
{
}

void SoundListModel::update()
{
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    m_QmlCtxt = view.rootContext();
    m_QmlCtxt->setContextProperty("myModel", QVariant::fromValue(m_sounds));
}
