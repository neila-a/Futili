#include "createbackupdialog.h"
#include <QCompleter>
#include <QMap>
#include <KDesktopFile>
#include <KLocalizedString>

CreateBackupDialog::CreateBackupDialog(QWidget *parent)
    : QDialog(parent)
    , m_file("", parent) {
    ui.setupUi(this);
    ui.enterBackupNameLabel->setText(i18n("Backup name:"));
    ui.backupNameEdit->setText(QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"));

    connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &CreateBackupDialog::save);
}

CreateBackupDialog::~CreateBackupDialog() {
    delete backupConfig;
}

void CreateBackupDialog::save() {
    QString backupName = ui.backupNameEdit->text();

    BackupInfoList backupInfoList = backupConfig->read();
    backupConfig->append({backupName, ui.iconButton->icon()});

    m_file.copy(m_backupStorageDir.filePath(QString::number(backupInfoList.length(), 36)));
}

DirFile CreateBackupDialog::file() const {
    return m_file;
}
void CreateBackupDialog::setFile(DirFile newFile) {
    m_file = newFile;

    setWindowFilePath(m_file.path);
    setWindowTitle(i18n("Create backup of %1", m_file.path));

    QString defaultIconName = QStringLiteral("inode-directory");
    if (m_file.isDir()) {
        QDir dir(m_file.path);
        if (dir.exists(DIRECTORYFILE)) {
            KDesktopFile directoryFile(dir.filePath(DIRECTORYFILE));
            const QString iconStr = directoryFile.readIcon();
            if (!iconStr.isEmpty()) {
                defaultIconName = iconStr;
            }
        }
    } else if (m_file.isFile()) {
        QFile file(m_file.path);
        QMimeType mimeType = QMimeDatabase().mimeTypeForFileNameAndData(m_file.path, &file);
        defaultIconName = mimeType.genericIconName();
    }
    ui.iconButton->setIcon(defaultIconName);
}

QDir CreateBackupDialog::backupStorageDir() const {
    return m_backupStorageDir;
}
void CreateBackupDialog::setBackupStorageDir(QDir newBackupStorageDir) {
    m_backupStorageDir = newBackupStorageDir;

    QDir parentDir(m_backupStorageDir);
    parentDir.cdUp();
    backupConfig = new BackupConfig(parentDir);

    const BackupInfoWithPathList displayedBackupsList = backupConfig->dedup();
    QStringList backupNamesList;
    for (const auto &info : displayedBackupsList) {
        backupNamesList.append(info.name);
    }
    QCompleter *completer = new QCompleter(backupNamesList, this);
    ui.backupNameEdit->setCompleter(completer);
}
