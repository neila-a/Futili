#ifndef DIRFILE_H
#define DIRFILE_H

#include <QFileInfo>
#include <QMimeDatabase>
#include <QString>

#define DIRECTORYFILE ".directory"

class DirFile : public QFileInfo {
    public:
    DirFile(const QString _path);

    const QIcon mimeTypeIcon() const;
    void copy(const QString desination);

    static const QString location(const QString path);

    private:
    QString path;
};

#endif // DIRFILE_H
