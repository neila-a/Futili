#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QTranslator>

#define BACKUPSDIRNAME "." + QStringLiteral(NAME) + "_backup"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setApplicationName(NAME);
    QApplication::setApplicationVersion(VERSION);

    QTranslator translator;
    if (translator.load(QLocale(), NAME, "_", ":/i18n"))
        a.installTranslator(&translator);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(a.translate("main", DESCRIPTION));
    parser.addPositionalArgument(a.translate("cli", "action"),
                                 a.translate("cli", "\"create\" or \"load\"."));
    parser.addPositionalArgument(a.translate("cli", "file"),
                                 a.translate("cli", "URL of file to create or load backup."));
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if (args.size() < 2) {
        qCritical().noquote() << a.translate("cli", "Too few arguments.");
        return EXIT_FAILURE;
    }
    const QString action = args.first();
    const QUrl filePathUrl(args.at(1));
    const QString filePath = filePathUrl.toLocalFile();
    QFile file(filePath);
    if (!file.exists()) {
        qCritical().noquote() << a.translate("cli", "The file \"%1\" does not exist.")
                                     .arg(filePath);
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
        const QString backupName = QInputDialog::
            getText(nullptr,
                    a.translate("dialog", "Create backup of %1").arg(filePath),
                    a.translate("dialog", "Backup name:"),
                    QLineEdit::Normal,
                    QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"),
                    &ok,
                    Qt::Window,
                    Qt::ImhNoAutoUppercase);
        if (ok && !backupName.isEmpty()) {
            thisBackupDir.remove(backupName);
            file.copy(thisBackupDir.path() + QDir::separator() + backupName);
        }
    } else if (action == "load") {
        const QString backupToLoad = QInputDialog::
            getItem(nullptr,
                    a.translate("dialog", "Load backup of %1").arg(filePath),
                    a.translate("dialog", "Select the backup to load"),
                    thisBackupDir
                        .entryList(QDir::Files | QDir::Readable | QDir::NoDotDot, QDir::Time),
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
        qCritical().noquote() << a.translate("cli", "The action \"%1\" does not exist.")
                                     .arg(filePath);
        return EXIT_FAILURE;
    }

    return a.exec();
}
