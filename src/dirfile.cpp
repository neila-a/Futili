#include "dirfile.h"
#include <QDir>
#include <QIcon>
#include <KDesktopFile>

DirFile::DirFile(const QString _path)
    : QFileInfo(_path) {
    path = _path;
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
    return QIcon::fromTheme(iconName);
}

void DirFile::copy(const QString desination) {
    if (isDir()) {
        QDir dir(path);
        QDir desinationLocationDir(location(desination));
        QFileInfo desinationInfo(desination);
        desinationLocationDir.mkdir(desinationInfo.fileName());
        QDir desinationDir(desination);
        const QFileInfoList fileInfos = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Readable
                                                          | QDir::NoDotAndDotDot | QDir::Hidden);
        QListIterator fileInfosIterator(fileInfos);
        while (fileInfosIterator.hasNext()) {
            QFileInfo fileInfo = fileInfosIterator.next();
            DirFile file(fileInfo.filePath());
            file.copy(desinationDir.filePath(fileInfo.fileName()));
        }
    } else if (isFile()) {
        QFile file(path);
        QFile desinationFile(desination);
        desinationFile.remove();
        file.copy(desination);
    }
}

const QString DirFile::location(const QString path) {
    QStringList pathList = path.split(QDir::separator());
    pathList.removeLast();
    return pathList.join(QDir::separator());
}
