#include "dirfile.h"
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <KIO/CopyJob>
#include <KIO/FileUndoManager>
#include <KIconLoader>
#include <KJobWidgets>

DirFile::DirFile(const QString _path, QWidget *_parentWidget)
    : QFileInfo(_path) {
    path = _path;
    parentWidget = _parentWidget;
}

void DirFile::copy(const QString desination, bool confirmOverride) {
    KIO::CopyJob *job = KIO::copyAs(QUrl::fromLocalFile(path),
                                    QUrl::fromLocalFile(desination),
                                    confirmOverride ? KIO::DefaultFlags : KIO::Overwrite);
    KJobWidgets::setWindow(job, parentWidget);
    KIO::FileUndoManager::self()->recordCopyJob(job);
}
