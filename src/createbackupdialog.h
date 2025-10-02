#ifndef CREATEBACKUPDIALOG_H
#define CREATEBACKUPDIALOG_H

#define TRANSLATION_DOMAIN "futili"

#include <QDir>
#include "backupconfig.h"
#include "dirfile.h"
#include "ui_createbackupdialog.h"
#include <KConfig>
#include <KConfigGroup>

class CreateBackupDialog : public QDialog {
    Q_OBJECT
    Q_PROPERTY(DirFile file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(QDir backupStorageDir READ backupStorageDir WRITE setBackupStorageDir NOTIFY
                   backupStorageDirChanged)

    public:
    explicit CreateBackupDialog(QWidget *parent = nullptr);
    ~CreateBackupDialog();

    DirFile file() const;
    void setFile(DirFile newFile);

    QDir backupStorageDir() const;
    void setBackupStorageDir(QDir newSavedBackups);

    signals:
    void fileChanged();
    void backupStorageDirChanged();

    private slots:
    void save();

    private:
    Ui::CreateBackupDialog ui;

    DirFile m_file;
    BackupConfig *backupConfig = nullptr;
    QDir m_backupStorageDir;
};

#endif // CREATEBACKUPDIALOG_H
