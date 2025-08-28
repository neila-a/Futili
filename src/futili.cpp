#include "futili.h"
#include <QInputDialog>
#include <QMenu>
#include <QMimeDatabase>
#include "dirfile.h"
#include <KActionMenu>
#include <KFileItemListProperties>
#include <KLocalizedString>
#include <KPluginFactory>

#define BACKUPSDIRNAME "." + QStringLiteral(NAME) + "_backup"
#define PREPAREDIRS() \
const QUrl filePathUrl(fileItemInfos.urlList().at(0)); \
const QString filePath = filePathUrl.toLocalFile(); \
DirFile file(filePath); \
QDir fileDir(DirFile::location(filePath)); \
bool backupsDirNoExists = fileDir.mkdir(BACKUPSDIRNAME); \
QDir backupsDir(fileDir.filePath(BACKUPSDIRNAME)); \
if (backupsDirNoExists) { \
    QFile backupDirIconFile(":/createDirectory/backupDirectory.desktop"); \
    backupDirIconFile.copy(backupsDir.filePath(DIRECTORYFILE)); \
} \
backupsDir.mkdir(file.fileName()); \
QDir thisBackupDir(backupsDir.filePath(file.fileName()));

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
        bool ok{};
        PREPAREDIRS();
        const QString backupName = QInputDialog::
            getText(parentWidget,
                    i18n("Create backup of %1", filePath),
                    i18n("Backup name:"),
                    QLineEdit::Normal,
                    QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"),
                    &ok,
                    Qt::Window,
                    Qt::ImhNoAutoUppercase);
        if (ok && !backupName.isEmpty()) {
            file.copy(thisBackupDir.filePath(backupName));
        }
    });
    backupsMenu->addAction(createBackupAction);

    backupsMenu->addSection(i18n("Load backup"));
    const QFileInfoList savedBackups = thisBackupDir
                                           .entryInfoList(QDir::Files | QDir::Dirs | QDir::Readable
                                                              | QDir::NoDotAndDotDot,
                                                          QDir::Time);
    QListIterator savedBackupsIterator(savedBackups);
    while (savedBackupsIterator.hasNext()) {
        const QFileInfo thisSavedBackup = savedBackupsIterator.next();
        const QString selectedBackupPath = thisSavedBackup.filePath();
        QAction *thisSavedBackupAction = new QAction(thisSavedBackup.fileName());
        connect(thisSavedBackupAction, &QAction::triggered, this, [=]() {
            PREPAREDIRS();
            DirFile selectedBackup(selectedBackupPath);
            selectedBackup.copy(filePath);
        });
        DirFile selectedBackup(selectedBackupPath);
        const QIcon selectedIcon = selectedBackup.mimeTypeIcon();
        thisSavedBackupAction->setIcon(selectedIcon);
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
