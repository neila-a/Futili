#include "dirfile.h"
#include <QDir>
#include <QIcon>
#include <QPixmap>
#include <KDesktopFile>
#include <KIO/CopyJob>
#include <KIO/FileUndoManager>
#include <KIconLoader>
#include <KJobWidgets>

DirFile::DirFile(const QString _path, QWidget *_parentWidget)
    : QFileInfo(_path) {
    path = _path;
    parentWidget = _parentWidget;
}

const QIcon DirFile::mimeTypeIcon() const {
    QString iconName = "inode-directory";
    if (isDir()) {
        QDir dir(path);
        if (dir.exists(DIRECTORYFILE)) {
            KDesktopFile directoryFile(dir.filePath(DIRECTORYFILE));
            const QString iconStr = directoryFile.readIcon();
            if (!iconStr.isEmpty()) {
                iconName = iconStr;
            }
        }
    } else if (isFile()) {
        QFile file(path);
        QMimeType mimeType = QMimeDatabase().mimeTypeForFileNameAndData(path, &file);
        iconName = mimeType.genericIconName();
    }
    KIconLoader loader(NAME);
    QPixmap pixmap = loader.loadIcon(iconName, KIconLoader::NoGroup);
    return QIcon(pixmap);
}

void DirFile::copy(const QString desination) {
    KIO::CopyJob *job = KIO::
        copyAs(QUrl::fromLocalFile(path), QUrl::fromLocalFile(desination), KIO::Overwrite);
    KJobWidgets::setWindow(job, parentWidget);
    KIO::FileUndoManager::self()->recordCopyJob(job);
}

const QString DirFile::location(const QString path) {
    QStringList pathList = path.split(QDir::separator());
    pathList.removeLast();
    return pathList.join(QDir::separator());
}
