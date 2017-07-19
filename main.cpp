#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QDesktopWidget>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置编码为utf-8
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    //加载Qt标准对话框的中文翻译文件
    QTranslator translator;
    if (translator.load(QString(":/qt_zh_CN"))){
        a.installTranslator(&translator);
    }
    QTranslator translator1;
    if (translator1.load(QString(":/widgets"))){
        a.installTranslator(&translator1);
    }
    QSettings setting("config.ini",QSettings::IniFormat);//生成配置文件
    setting.beginGroup("config");//beginGroup与下面endGroup 相对应，“config”是标记
    if(setting.value("status_bar").isNull())
        setting.setValue("status_bar",QVariant("1"));
    if(setting.value("auto_go_line").isNull())
        setting.setValue("auto_go_line",QVariant("1"));
    setting.endGroup();

    MainWindow w;
    QDesktopWidget *desktop=QApplication::desktop();
    //居中显示
    w.move((desktop->width()-w.width())/2,(desktop->height()-w.height())/2);
    w.show();
    return a.exec();
}
