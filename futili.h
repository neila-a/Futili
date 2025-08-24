#ifndef FUTILI_H
#define FUTILI_H

#include <KAbstractFileItemActionPlugin>
#include <KFileItemListProperties>
#include <KPluginFactory>

class Futili : public KAbstractFileItemActionPlugin {
    Q_OBJECT
public:
    explicit Futili(QObject *parent = nullptr);
    QList<QAction *> actions(const KFileItemListProperties &fileItemInfos, QWidget *parentWidget);
};

#endif // FUTILI_H
