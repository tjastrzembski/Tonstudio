#include "SoundListModel.h"
#include <QQuickView>

#include "SoundComponentPersistent.h"

SoundListModel::SoundListModel(QList<SoundComponentGraphic *> &sounds)
    : m_sounds(sounds)
{
    SoundComponentGraphic *scg = new SoundComponentGraphic();
    scg->setSoundComponent(new SoundComponentPersistent(nullptr));
    scg->getSoundComponent()->setName("Test2");
    m_sounds.append(scg);
}

void SoundListModel::update()
{
    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    m_QmlCtxt = view.rootContext();
    m_QmlCtxt->setContextProperty("myModel", QVariant::fromValue(m_sounds));
}
