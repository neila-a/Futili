#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QInputDialog>
#include <QLocale>
#include <QTranslator>
#define BACKUPSDIRNAME ".futili_backup"

int main(
    int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setApplicationName(NAME);
    QApplication::setApplicationVersion(VERSION);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = QString(NAME) + "_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QApplication::translate("main", DESCRIPTION));

    parser.addPositionalArgument(QApplication::translate("main", "action"),
                                 QApplication::translate("main", "\"create\" or \"load\"."));
    parser.addPositionalArgument(QApplication::translate("main", "file"),
                                 QApplication::translate("main",
                                                         "URL of file to create or load backup."));
    parser.process(a);

    const QStringList args = parser.positionalArguments();
    if (args.size() < 2) {
        qCritical().noquote() << QApplication::translate("main", "Too few arguments.");
        return EXIT_FAILURE;
    }
    const QString action = args.first();
    const QString filePath = args.at(1);
    QFile file(filePath);
    if (!file.exists()) {
        qCritical().noquote() << QApplication::translate("main", "The file \"%1\" does not exist.")
                                     .arg(filePath);
        return EXIT_FAILURE;
    }

    QStringList filePathList = filePath.split(QDir::separator());
    filePathList.removeLast();
    QDir fileDir(filePathList.join(QDir::separator()));
    fileDir.mkdir(".futili_backup");
    QDir backupsDir(fileDir.path() + QDir::separator() + BACKUPSDIRNAME);
    const QFileInfo fileInfo(file);
    backupsDir.mkdir(fileInfo.fileName());
    QDir thisBackupDir(backupsDir.path() + QDir::separator() + fileInfo.fileName());

    bool ok{};
    if (action == "create") {
        const QString backupName = QInputDialog::getText(
            nullptr,
            QApplication::translate("dialog", "Create backup of %1").arg(filePath),
            QApplication::translate("dialog", "Backup name:"),
            QLineEdit::Normal,
            QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss"),
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
        const QString backupToLoad = QInputDialog::getItem(
            nullptr,
            QApplication::translate("dialog", "Load backup of %1").arg(filePath),
            QApplication::translate("dialog", "Select the backup to load"),
            thisBackupDir.entryList(QDir::Files | QDir::Readable | QDir::NoDotDot, QDir::Time),
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
        qCritical().noquote() << QApplication::translate("main", "The action \"%1\" does not exist.")
                                     .arg(action);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
