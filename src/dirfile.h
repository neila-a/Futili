#ifndef DIRFILE_H
#define DIRFILE_H

#include <QFileInfo>
#include <QMimeDatabase>
#include <QString>

#define DIRECTORYFILE ".directory"

class DirFile : public QFileInfo {
    public:
    DirFile(const QString _path, QWidget *_parentWidget);

    void copy(const QString desination, bool confirmOverride = true);
    QString path;

    private:
    QWidget *parentWidget;
};

#endif // DIRFILE_H
