#include "backupconfig.h"

BackupConfig::BackupConfig(QDir baseDir)
    : QFile(baseDir.filePath("backupsinfo")) {}

BackupInfoList BackupConfig::read() {
    open(QIODevice::ReadOnly | QIODevice::Text);

    int lineIndex = 0;
    backupInfo info;
    BackupInfoList list;

    QTextStream in(this);
    QString line = in.readLine().trimmed();
    while (!line.isNull()) {
        switch (lineIndex % 2) {
        case 0: // name
            info = {line};
            break;
        case 1: // icon
            info.icon = line;
            list.append(info);
            break;
        }
        line = in.readLine();
        lineIndex++;
    }
    close();
    return list;
}
void BackupConfig::append(const backupInfo info) {
    open(QIODevice::Append | QIODevice::WriteOnly | QIODevice::Text);
    write(info.name.toUtf8());
    write("\n");
    write(info.icon.toUtf8());
    write("\n");
    close();
}
BackupInfoWithPathList BackupConfig::dedup() {
    const BackupInfoList backupInfoList = read();
    QMap<QString, backupInfoWithPath> displayedBackupsMap;
    int path = 0;
    for (const auto &thisBackupInfo : backupInfoList) {
        backupInfoWithPath withPath = {thisBackupInfo, QString::number(path, 36)};
        displayedBackupsMap.insert(thisBackupInfo.name, withPath);
        path++;
    }
    BackupInfoWithPathList displayedBackupsList;
    for (const auto [name, info] : displayedBackupsMap.asKeyValueRange()) {
        displayedBackupsList.append(info);
    }
    return displayedBackupsList;
}
