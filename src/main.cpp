#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <KLocalizedString>

#define BACKUPSDIRNAME "." + QStringLiteral(NAME) + "_backup"

int main(
    int argc, char *argv[]) {
    qDebug() << 3;
    QApplication a(argc, argv);
    QApplication::setApplicationName(NAME);
    QApplication::setApplicationVersion(VERSION);
    KLocalizedString::setApplicationDomain(QByteArrayLiteral(
        NAME)); // https://api.kde.org/frameworks/ki18n/html/prg_guide.html#link_prog
    qDebug() << KLocalizedString::availableDomainTranslations(NAME)
             << KLocalizedString::availableApplicationTranslations()
             << KLocalizedString::languages()
             << KLocalizedString::isApplicationTranslatedInto("zh_CN") << i18n("action");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(i18n(DESCRIPTION));

    parser.addPositionalArgument(i18n("action"), i18n("\"create\" or \"load\"."));
    parser.addPositionalArgument(i18n("file"), i18n("URL of file to create or load backup."));
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if (args.size() < 2) {
        qCritical().noquote() << i18n("Too few arguments.");
        return EXIT_FAILURE;
    }
    const QString action = args.first();
    const QUrl filePathUrl(args.at(1));
    const QString filePath = filePathUrl.toLocalFile();
    QFile file(filePath);
    qDebug() << filePathUrl;
    if (!file.exists()) {
        qCritical().noquote() << i18n("The file \"%1\" does not exist.", filePath);
        return EXIT_FAILURE;
    }

    QStringList filePathList = filePath.split(QDir::separator());
    filePathList.removeLast();
    QDir fileDir(filePathList.join(QDir::separator()));
    bool backupsDirNoExists = fileDir.mkdir(BACKUPSDIRNAME);
    QDir backupsDir(fileDir.path() + QDir::separator() + BACKUPSDIRNAME);
    if (backupsDirNoExists) {
        QFile backupDirIconFile(":/createDirectory/backupDirectory.desktop");
        backupDirIconFile.copy(backupsDir.path() + QDir::separator() + ".directory");
    }
    const QFileInfo fileInfo(file);
    backupsDir.mkdir(fileInfo.fileName());
    QDir thisBackupDir(backupsDir.path() + QDir::separator() + fileInfo.fileName());

    bool ok{};
    if (action == "create") {
        const QString backupName = QInputDialog::getText(nullptr,
                                                         i18n("Create backup of %1", filePath),
                                                         i18n("Backup name:"),
                                                         QLineEdit::Normal,
                                                         QDateTime::currentDateTime().toString(
                                                             "yyyyMMdd-HHmmss"),
                                                         &ok,
                                                         Qt::Window,
                                                         Qt::ImhNoAutoUppercase);
        if (ok && !backupName.isEmpty()) {
            thisBackupDir.remove(backupName);
            file.copy(thisBackupDir.path() + QDir::separator() + backupName);
            qDebug() << filePathList << fileDir.path() << backupsDir.path() << thisBackupDir.path()
                     << thisBackupDir.path() + QDir::separator() + backupName;
        }
    } else if (action == "load") {
        const QString backupToLoad
            = QInputDialog::getItem(nullptr,
                                    i18n("Load backup of %1", filePath),
                                    i18n("Select the backup to load"),
                                    thisBackupDir.entryList(QDir::Files | QDir::Readable
                                                                | QDir::NoDotDot,
                                                            QDir::Time),
                                    0,
                                    true,
                                    &ok,
                                    Qt::Window,
                                    Qt::ImhNoAutoUppercase);
        if (ok) {
            file.remove();
            QFile selectedBackup(thisBackupDir.path() + QDir::separator() + backupToLoad);
            selectedBackup.copy(filePath);
        }
    } else {
        qCritical().noquote() << i18n("The action \"%1\" does not exist.", action);
        return EXIT_FAILURE;
    }

    return a.exec();
}
