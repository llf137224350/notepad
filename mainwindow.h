#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include "QtDebug"
#include "QTranslator"
#include "QFileDialog"
#include "QFile"
#include "QDir"
#include "QTextStream"
#include <QPrinter>
#include <QPrintDialog>
#include <QTextCursor>
#include <QDateTime>
#include <QFontDialog>
#include <QFont>
#include <QLabel>
#include <QDesktopServices>
#include <QTextBlock>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QSettings>
#include "aboutdialog.h"
#include "gotodialog.h"
#include "finddialog.h"
#include "replacedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void replace(QString value, bool isChecked);

private:
    Ui::MainWindow *ui;
private slots:
    //新文件槽函数
    void newFileSolt();
    //退出软件槽函数
    void exitAppSlot();
    //打开文件槽函数
    void openFileSlot();
    //文本变化槽函数
    void textChangeSlot();
    //保存文件槽函数
    void saveFileSlot();
    //另存
    void saveOtherFileSlot();
    //打印
    void doPrintSlot();
    //编辑
    void editSolt();
    //撤销
    void undoSolt();
    //剪切
    void cutSolt();
    //复制
    void copySolt();
    //粘贴
    void pasteSolt();
    //删除
    void deleteSolt();
    //转到
    void goToSlot();
    //到某一行
    void goToLineSlot(QString line);
    //查找
    void findSlot();
    //查找下一个
    void findNextSlot();
    //查找
    void find(QString value,bool isChecked,bool isUp);
    //替换
    void replaceSlot();
    //查找 - 替换
    void findForReplaceSlot(QString value,bool isChecked);
    //替换或者全替换
    void doReplaceSlot(QString target,QString value,bool isChecked,bool isReplaceAll);
    //全选
    void selectAllSlot();
    //时间或者日期
    void timeOrDateSlot();
    //自动换行
    void autoLineSlot();
    //字体选择
    void fontChooseSlot();
    //光标变化
    void cursorChangeSlot();
    //状态栏
    void statusBarSlot();
    //帮助
    void helpSlot();
    //关于
    void aboutSlot();



private:
    QString fileName;//文件路径
    QString fileContent;//读到的文件内容
    bool autoLine;//自动换行
    bool statusBar;//显示状态栏
    QLabel *change;//行和列
    GoToDialog goToDialot;//跳转对话框
    FindDialog findDialog;//查找
    ReplaceDialog replaceDialog;//替换
    bool isFirstFind;
     QSettings *setting;//配置文件
    //保存文本到文件
    void saveTextToFile();
    //读取文件
    void readFile();
    //显示提示消息
    void showMessage(QString);
    //拖动文件进入窗口
    void dragEnterEvent(QDragEnterEvent *e);
    //释放对方时，执行的操作
    void dropEvent(QDropEvent *e);
    //初始化一下参数
    void init();
    //关联信号和草
    void conn();
    //设置快捷键
    void setShortcut();
    //关闭事件
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
