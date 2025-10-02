#include "futili.h"
#include <QDir>
#include <QInputDialog>
#include <QMenu>
#include <QMimeDatabase>
#include "backupconfig.h"
#include "createbackupdialog.h"
#include "dirfile.h"
#include <KActionMenu>
#include <KFileItemListProperties>
#include <KLocalizedString>
#include <KPluginFactory>

#define BACKUPSDIRNAME "." + QStringLiteral(NAME) + "_backup"
#define PREPAREDIRS() \
const QUrl filePathUrl(fileItemInfos.urlList().at(0)); \
const QString filePath = filePathUrl.toLocalFile(); \
DirFile file(filePath, parentWidget); \
QDir fileDir(filePathUrl.adjusted(QUrl::RemoveFilename).toLocalFile()); \
bool backupsDirNoExists = fileDir.mkdir(BACKUPSDIRNAME); \
QDir backupsDir(fileDir.filePath(BACKUPSDIRNAME)); \
if (backupsDirNoExists) { \
    QFile backupDirIconFile(":/createDirectory/backupDirectory.desktop"); \
    backupDirIconFile.copy(backupsDir.filePath(DIRECTORYFILE)); \
} \
backupsDir.mkdir(file.fileName()); \
QDir thisBackupDir(backupsDir.filePath(file.fileName())); \
thisBackupDir.mkdir("backups"); \
QDir thisBackupStorageDir(thisBackupDir.filePath("backups"));

K_PLUGIN_CLASS_WITH_JSON(Futili, "futili.json")

Futili::Futili(QObject *parent, const QVariantList &args)
    : KAbstractFileItemActionPlugin(parent) {
    Q_UNUSED(args);
}

Futili::~Futili() {}

QList<QAction *> Futili::actions(const KFileItemListProperties &fileItemInfos,
                                 QWidget *parentWidget) {
    if (!(fileItemInfos.supportsDeleting() && fileItemInfos.supportsReading()
          && fileItemInfos.supportsWriting() && fileItemInfos.urlList().length() == 1)) {
        return {};
    }

    const QIcon backupIcon = QIcon::fromTheme("backup");
    const QIcon addIcon = QIcon::fromTheme("bqm-add");

    QAction *backupAction = new QAction(i18n("Backup"), parentWidget);
    backupAction->setIcon(backupIcon);
    QMenu *backupsMenu = new QMenu(parentWidget);
    backupAction->setMenu(backupsMenu);

    PREPAREDIRS();

    QAction *createBackupAction = new QAction(i18n("Create backup"), parentWidget);
    createBackupAction->setIcon(addIcon);
    connect(createBackupAction, &QAction::triggered, this, [=]() {
        PREPAREDIRS();
        CreateBackupDialog dialog(parentWidget);
        dialog.setFile(file);
        dialog.setBackupStorageDir(thisBackupStorageDir);
        dialog.exec();
    });
    backupsMenu->addAction(createBackupAction);

    backupsMenu->addSection(i18n("Load backup"));
    BackupConfig backupConfig(thisBackupDir);
    const BackupInfoWithPathList savedBackups = backupConfig.dedup();
    for (const auto &thisSavedBackup : savedBackups) {
        const QString selectedBackupPath = thisBackupStorageDir.filePath(thisSavedBackup.path);
        QAction *thisSavedBackupAction = new QAction(thisSavedBackup.name);
        connect(thisSavedBackupAction, &QAction::triggered, this, [=]() {
            PREPAREDIRS();
            DirFile selectedBackup(selectedBackupPath, parentWidget);
            selectedBackup.copy(filePath, false);
        });

        KIconLoader loader(NAME);
        QPixmap pixmap = loader.loadIcon(thisSavedBackup.icon, KIconLoader::NoGroup);
        thisSavedBackupAction->setIcon(QIcon(pixmap));

        backupsMenu->addAction(thisSavedBackupAction);
    }
    if (savedBackups.isEmpty()) {
        QAction *noCreatedBackupAction = new QAction(i18n("No backups have been created"));
        noCreatedBackupAction->setDisabled(true);
        backupsMenu->addAction(noCreatedBackupAction);
    }

    return {backupAction};
}

#include "futili.moc"
