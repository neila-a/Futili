#ifndef FUTILI_H
#define FUTILI_H

#include <QFile>
#include <QUrl>
#include <KAbstractFileItemActionPlugin>

#define TRANSLATION_DOMAIN "futili"

class Futili : public KAbstractFileItemActionPlugin {
public:
    Futili(QObject *parent, const QVariantList &args);
    ~Futili() override;

    QList<QAction *> actions(const KFileItemListProperties &fileItemInfos,
                             QWidget *parentWidget) override;

private:
};

#endif // FUTILI_H
