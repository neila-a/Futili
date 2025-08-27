#include "futili.h"
#include <QInputDialog>
#include <QMenu>
#include <QMimeDatabase>
#include <KActionMenu>
#include <KFileItemListProperties>
#include <KLocalizedString>
#include <KPluginFactory>

#define BACKUPSDIRNAME "." + QStringLiteral(NAME) + "_backup"
#define PREPAREDIRS() \
const QUrl filePathUrl(fileItemInfos.urlList().at(0)); \
const QString filePath = filePathUrl.toLocalFile(); \
QFile file(filePath); \
QStringList filePathList = filePath.split(QDir::separator()); \
filePathList.removeLast(); \
QDir fileDir(filePathList.join(QDir::separator())); \
bool backupsDirNoExists = fileDir.mkdir(BACKUPSDIRNAME); \
QDir backupsDir(fileDir.path() + QDir::separator() + BACKUPSDIRNAME); \
if (backupsDirNoExists) { \
    QFile backupDirIconFile(":/createDirectory/backupDirectory.desktop"); \
    backupDirIconFile.copy(backupsDir.path() + QDir::separator() + ".directory"); \
} \
const QFileInfo fileInfo(file); \
backupsDir.mkdir(fileInfo.fileName()); \
QDir thisBackupDir(backupsDir.path() + QDir::separator() + fileInfo.fileName());

K_PLUGIN_CLASS_WITH_JSON(Futili, "futili.json")

Futili::Futili(QObject *parent, const QVariantList &args)
    : KAbstractFileItemActionPlugin(parent) {
    Q_UNUSED(args);
}

Futili::~Futili() {}

QList<QAction *> Futili::actions(const KFileItemListProperties &fileItemInfos,
                                 QWidget *parentWidget) {
    if (!(fileItemInfos.isFile() && fileItemInfos.supportsDeleting()
          && fileItemInfos.supportsReading() && fileItemInfos.supportsWriting()
          && fileItemInfos.urlList().length() == 1)) {
        return {};
    }

    const QIcon backupIcon = QIcon::fromTheme("backup");
    const QIcon addIcon = QIcon::fromTheme("bqm-add");

    QAction *backupAction = new QAction(i18n("Backup"), parentWidget);
    backupAction->setIcon(backupIcon);
    QMenu *actionsMenu = new QMenu(parentWidget);
    backupAction->setMenu(actionsMenu);

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
            thisBackupDir.remove(backupName);
            file.copy(thisBackupDir.path() + QDir::separator() + backupName);
        }
    });
    actionsMenu->addAction(createBackupAction);

    QAction *loadBackupAction = new QAction(i18n("Load backup"), parentWidget);
    loadBackupAction->setIcon(backupIcon);
    actionsMenu->addAction(loadBackupAction);

    QMenu *savedBackupsMenu = new QMenu(parentWidget);
    const QList savedBackups = thisBackupDir
                                   .entryList(QDir::Files | QDir::Readable | QDir::NoDotDot,
                                              QDir::Time);
    QListIterator savedBackupsIterator(savedBackups);
    loadBackupAction->setMenu(savedBackupsMenu);
    while (savedBackupsIterator.hasNext()) {
        const QString thisSavedBackup = savedBackupsIterator.next();
        const QString selectedBackupPath = thisBackupDir.path() + QDir::separator()
                                           + thisSavedBackup;
        QAction *thisSavedBackupAction = new QAction(thisSavedBackup);
        connect(thisSavedBackupAction, &QAction::triggered, this, [=]() {
            PREPAREDIRS();
            file.remove();
            QFile selectedBackup(selectedBackupPath);
            selectedBackup.copy(filePath);
        });
        QFile selectedBackup(selectedBackupPath);
        const QMimeType selectedMimeType = QMimeDatabase()
                                               .mimeTypeForFileNameAndData(selectedBackupPath,
                                                                           &selectedBackup);
        const QIcon selectedIcon = QIcon::fromTheme(selectedMimeType.genericIconName());
        thisSavedBackupAction->setIcon(selectedIcon);
        savedBackupsMenu->addAction(thisSavedBackupAction);
    }

    return {backupAction};
}

#include "futili.moc"
