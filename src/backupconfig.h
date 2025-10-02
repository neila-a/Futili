#ifndef BACKUPCONFIG_H
#define BACKUPCONFIG_H

#include <QDir>
#include <QFile>

struct backupInfo {
    QString name;
    QString icon;
};
Q_DECLARE_METATYPE(backupInfo)
typedef QList<backupInfo> BackupInfoList;
struct backupInfoWithPath : backupInfo {
    QString path;
};
typedef QList<backupInfoWithPath> BackupInfoWithPathList;

class BackupConfig : public QFile {
    public:
    BackupConfig(QDir baseDir);

    BackupInfoList read();
    void append(const backupInfo info);
    BackupInfoWithPathList dedup();
};

#endif // BACKUPCONFIG_H
