#include "futili.h"
#include <QAction>

Futili::Futili(
    QObject *parent)
    : KAbstractFileItemActionPlugin{parent} {}

QList<QAction *> Futili::actions(
    const KFileItemListProperties &fileItemInfos, QWidget *parentWidget) {
    QAction d("114");
    return QList<QAction *>() << &d;
}

K_PLUGIN_CLASS_WITH_JSON(
    Futili, "futili.json")

#include "futili.moc"
