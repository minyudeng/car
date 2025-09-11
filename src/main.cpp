#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "car_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    QFile styleFile(":/styles/style.qss");

    
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
       QString style = styleFile.readAll();
        a.setStyleSheet(style);
        qDebug() << "样式表style加载成功！";
        styleFile.close();
    }else {
        qDebug() << "无法打开style文件:" << styleFile.errorString();
        qDebug() << "使用默认样式";
    }
    

    MainWindow w;
    w.show();
    return a.exec();
}
