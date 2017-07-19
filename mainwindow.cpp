#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->init();  //进行初始化

    this->conn(); //连接信号与槽

    this->setShortcut(); //设置快捷键

    //是否传入了文件路径
    QStringList arguments = QCoreApplication::arguments();
    if(arguments.length() > 1){
        fileName = arguments[1];
        this->readFile();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
//初始化一下参数
void MainWindow::init(){
    this->setWindowTitle("无标题 - 记事本");
    setting = new QSettings("config.ini",QSettings::IniFormat);
    //初始化默认值
    autoLine = true;//是否自动换行
    statusBar = true;//是否显示状态栏
    isFirstFind = true;//是否第一次搜索 替换时用
    ui->textEdit->setAcceptDrops(false); //设置文本输入框不接受鼠标放下
    this->setAcceptDrops(true);  //设置窗口接受鼠标放下

    setting->beginGroup("config");//beginGroup与下面endGroup 相对应，“config”是标记
    QString family =setting->value("family").toString();
    if(!family.isEmpty()){//设置了字体
        QFont font;
        font.setFamily(family);
        font.setPointSize(setting->value("point_size").toInt());
        font.setItalic(setting->value("italic").toBool());
        font.setBold(setting->value("bold").toBool());
        font.setOverline(setting->value("overline").toBool());
        font.setUnderline(setting->value("underline").toBool());
        ui->textEdit->setFont(font);
    }
    statusBar = setting->value("status_bar").toInt() == 1?true : false;//获取存储的值
    autoLine = setting->value("auto_go_line").toInt() == 1?true : false;
    setting->endGroup();

    //设置是否显示状态栏
    ui->statusBar->setVisible(statusBar);
    //是否显示显示状态栏勾中图标
    ui->actionStatusBar->setIconVisibleInMenu(statusBar);
    //设置QTextEdit自动换行
    ui->actionAutoLine->setIconVisibleInMenu(autoLine);
    if(autoLine){
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    }else{
        ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
    }
    //设置状态栏
    ui->statusBar->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 设置不显示label的边框
    ui->statusBar->setSizeGripEnabled(false); //设置是否显示右边的大小控制点
    change = new QLabel("第 1 行，第 1 列   字数 0   ", this);
    ui->statusBar->addPermanentWidget(change); //显示永久信息
    //查找和查找下一个不可用
    ui->actionFind->setEnabled(false);
    ui->actionFindNext->setEnabled(false);
    ui->actionRevoke->setEnabled(false);

}
//关联信号和草
void MainWindow::conn(){
    //添加处理 文件
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFileSolt()));
    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(exitAppSlot()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(openFileSlot()));
    connect(ui->textEdit,SIGNAL(textChanged()),this,SLOT(textChangeSlot()));
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(saveFileSlot()));
    connect(ui->actionOtherSave,SIGNAL(triggered()),this,SLOT(saveOtherFileSlot()));
    connect(ui->actionPrint,SIGNAL(triggered()),this,SLOT(doPrintSlot()));
    //添加处理 编辑
    connect(ui->menuEdit,SIGNAL(aboutToShow()),this,SLOT(editSolt()));
    connect(ui->actionRevoke,SIGNAL(triggered()),this,SLOT(undoSolt()));
    connect(ui->actionCut,SIGNAL(triggered()),this,SLOT(cutSolt()));
    connect(ui->actionCopy,SIGNAL(triggered()),this,SLOT(copySolt()));
    connect(ui->actionPaste,SIGNAL(triggered()),this,SLOT(pasteSolt()));
    connect(ui->actionDelete,SIGNAL(triggered()),this,SLOT(deleteSolt()));

    connect(ui->actionGoTO,SIGNAL(triggered()),this,SLOT(goToSlot()));
    connect(&goToDialot, SIGNAL(goToLine(QString)), this,SLOT(goToLineSlot(QString)));
    connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(findSlot()));
    connect(&findDialog, SIGNAL(find(QString,bool,bool)), this,SLOT(find(QString,bool,bool)));
    connect(ui->actionFindNext,SIGNAL(triggered()),this,SLOT(findNextSlot()));
    connect(ui->actionReplace,SIGNAL(triggered()),this,SLOT(replaceSlot()));
    connect(&replaceDialog, SIGNAL(find(QString,bool)), this,SLOT(findForReplaceSlot(QString,bool)));
    connect(&replaceDialog, SIGNAL(replace(QString,QString,bool,bool)), this,SLOT(doReplaceSlot(QString,QString,bool,bool)));

    connect(ui->actionTimeOrDate,SIGNAL(triggered()),this,SLOT(timeOrDateSlot()));
    connect(ui->actionSelectAll,SIGNAL(triggered()),this,SLOT(selectAllSlot()));

    //格式
    connect(ui->actionAutoLine,SIGNAL(triggered()),this,SLOT(autoLineSlot()));
    connect(ui->actionFont,SIGNAL(triggered()),this,SLOT(fontChooseSlot()));
    connect(ui->textEdit,SIGNAL(cursorPositionChanged()),this,SLOT(cursorChangeSlot()));
    //查看
    connect(ui->actionStatusBar,SIGNAL(triggered()),this,SLOT(statusBarSlot()));
    //帮助
    connect(ui->actionHelp,SIGNAL(triggered()),this,SLOT(helpSlot()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(aboutSlot()));

}
//设置快捷键
void MainWindow::setShortcut(){
    //快捷键
    ui->actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    ui->actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    ui->actionOtherSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    ui->actionPrint->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));

    ui->actionRevoke->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
    ui->actionCut->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
    ui->actionCopy->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    ui->actionPaste->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    ui->actionFind->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    ui->actionFindNext->setShortcut(QKeySequence(Qt::Key_F3));
    ui->actionReplace->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    ui->actionDelete->setShortcut(QKeySequence(Qt::Key_Delete));
    ui->actionGoTO->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_G));
    ui->actionSelectAll->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    ui->actionTimeOrDate->setShortcut(QKeySequence(Qt::Key_F5));
}

//新建文件槽函数
void MainWindow::newFileSolt(){
    //文档已经修改
    if(ui->textEdit->document()->isModified()&& !ui->textEdit->document()->isEmpty() && fileContent != ui->textEdit->document()->toPlainText()){
        QMessageBox box(QMessageBox::Question,"记事本","是否将更改保存到 无标题");
        box.setIcon(QMessageBox::NoIcon);
        box.setStandardButtons (QMessageBox::Ok|QMessageBox::Ignore|QMessageBox::Cancel);
        box.setButtonText (QMessageBox::Ok,QString("保存"));
        box.setButtonText (QMessageBox::Ignore,QString("不保存"));
        box.setButtonText (QMessageBox::Cancel,QString("取消"));
        int result = box.exec();
        if(result == QMessageBox::Ok){
            if(fileName.isEmpty()){//新建
                //弹出保存文件对话框
                fileName = QFileDialog::getSaveFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
                if(!fileName.isEmpty()){
                    //保存文件
                    this->saveTextToFile();
                }
            }else{//读取的文本
                this->saveTextToFile();
            }
            ui->textEdit->clear();
            setWindowTitle("无标题 - 记事本");
        }else  if(result == QMessageBox::Ignore){
            //不保存
            ui->textEdit->clear();
            setWindowTitle("无标题 - 记事本");
            ui->statusBar->showMessage("");
        }
    }else{
        //文档未修改
        ui->textEdit->clear();
        setWindowTitle("无标题 - 记事本");
    }
    //路径置空
    fileName = "";
}

//退出应用槽函数
void MainWindow::exitAppSlot(){
    //文档已经修改
    if(ui->textEdit->document()->isModified()&& !ui->textEdit->document()->isEmpty() && fileContent != ui->textEdit->document()->toPlainText()){
        QMessageBox box(QMessageBox::Question,"记事本","是否将更改保存到 无标题");
        box.setIcon(QMessageBox::NoIcon);
        box.setStandardButtons (QMessageBox::Ok|QMessageBox::Ignore|QMessageBox::Cancel);
        box.setButtonText (QMessageBox::Ok,QString("保存"));
        box.setButtonText (QMessageBox::Ignore,QString("不保存"));
        box.setButtonText (QMessageBox::Cancel,QString("取消"));
        int result = box.exec();
        if(result == QMessageBox::Ok){
            if(fileName.isEmpty()){//新建
                //弹出保存文件对话框
                fileName = QFileDialog::getSaveFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
                if(!fileName.isEmpty()){
                    //保存文件
                    this->saveTextToFile();
                }
            }else{//读取的文本
                this->saveTextToFile();
            }

        }else  if(result == QMessageBox::Ignore){
            //不保存 关闭
            this->close();
        }
    }else{
        this->close();
    }
}
//另存文件槽函数
void MainWindow::saveOtherFileSlot(){
    //弹出保存文件对话框
    fileName = QFileDialog::getSaveFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
    if(!fileName.isEmpty()){
        //保存文件
        this->saveTextToFile();
    }
}
//保存文件槽函数
void MainWindow::saveFileSlot(){
    //判断是新建还是读取的文本
    if(fileName.isEmpty()){//新建
        //弹出保存文件对话框
        fileName = QFileDialog::getSaveFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
        if(!fileName.isEmpty()){
            //保存文件
            this->saveTextToFile();
        }
    }else{//读取的文本
        this->saveTextToFile();
    }
}
//保存文件
void MainWindow::saveTextToFile(){
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)){
        QTextStream out(&file);
        out << ui->textEdit->document()->toPlainText();
        file.close();
        this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本");
        fileContent = ui->textEdit->document()->toPlainText();
        ui->statusBar->showMessage("已保存",3000);
    }else{
        QMessageBox box(QMessageBox::Question,"提示","保存文件失败！");
        box.setIcon(QMessageBox::Warning);
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确定"));
        box.exec();
    }

}
//打开文件槽函数
void MainWindow::openFileSlot(){
    //文档已经修改
    if(ui->textEdit->document()->isModified() && !ui->textEdit->document()->isEmpty() && fileContent != ui->textEdit->document()->toPlainText()){
        QMessageBox box(QMessageBox::Question,"记事本","是否将更改保存到 无标题");
        box.setIcon(QMessageBox::NoIcon);
        box.setStandardButtons (QMessageBox::Ok|QMessageBox::Ignore|QMessageBox::Cancel);
        box.setButtonText (QMessageBox::Ok,QString("保存"));
        box.setButtonText (QMessageBox::Ignore,QString("不保存"));
        box.setButtonText (QMessageBox::Cancel,QString("取消"));
        int result = box.exec();
        if(result == QMessageBox::Ok){
            //保存文件
            this->saveTextToFile();
        }else  if(result == QMessageBox::Ignore){
            //不保存
            //打开文件
            fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
            this->readFile();
        }
    }else{
        fileName = QFileDialog::getOpenFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
        this->readFile();
    }
}
//真正打开文件
void MainWindow::readFile(){

    //得到路径不为空
    if(!fileName.isEmpty()){
        QFile *file = new QFile;
        file->setFileName(fileName);

        bool isOpen = file->open(QIODevice::ReadOnly);
        if(isOpen){
            ui->textEdit->clear();
            QTextStream in(file);

            while (!in.atEnd()) {
                ui->textEdit->append(in.readLine());
                //光标移动到开始位置
                ui->textEdit->moveCursor(QTextCursor::Start);
            }
            //已读完
            fileContent = ui->textEdit->document()->toPlainText();

            if(fileName.lastIndexOf("\\") != -1){
                //设置标题
                this->setWindowTitle(fileName.mid(fileName.lastIndexOf('\\')+1)+" - 记事本");
            }else{
                //设置标题
                this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本");
            }

            file->close();
            ui->statusBar->showMessage("");
        }else{
            QMessageBox box(QMessageBox::Question,"提示","打开文件失败！");
            box.setIcon(QMessageBox::Warning);
            box.setStandardButtons (QMessageBox::Ok);
            box.setButtonText (QMessageBox::Ok,QString("确定"));
            box.exec();
        }
    }
}
//文本 变化槽函数
void MainWindow::textChangeSlot(){
    ui->statusBar->showMessage("");
    if(ui->textEdit->document()->isEmpty()){
        if(fileName.isEmpty()){//没有保存到文件
            this->setWindowTitle("无标题 - 记事本");

        }else{//有文件
            this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本 *");
            if( fileContent != ui->textEdit->document()->toPlainText())
                ui->statusBar->showMessage("未保存");
        }
    }else if(ui->textEdit->document()->isModified()){
        if(fileName.isEmpty()){//没有保存到文件
            this->setWindowTitle("无标题 - 记事本 *");
            if( fileContent != ui->textEdit->document()->toPlainText())
                ui->statusBar->showMessage("未保存");
        }else{//有文件
            this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本 *");
            if( fileContent != ui->textEdit->document()->toPlainText())
                ui->statusBar->showMessage("未保存");
        }
        if(fileContent == ui->textEdit->document()->toPlainText()){
            this->setWindowTitle(fileName.mid(fileName.lastIndexOf('/')+1)+" - 记事本");
        }
    }
    if(ui->textEdit->document()->isEmpty()){
        ui->actionFind->setEnabled(false);
        ui->actionFindNext->setEnabled(false);
    }else{
        ui->actionFind->setEnabled(true);
        ui->actionFindNext->setEnabled(true);
    }
    //撤销可用
    ui->actionRevoke->setEnabled(true);
}

// 打印槽函数
void MainWindow::doPrintSlot()
{
    // 创建打印机对象
    QPrinter printer;
    // 创建打印对话框
    QPrintDialog dlg(&printer, this);
    //如果编辑器中有选中区域，则打印选中区域
    if (ui->textEdit->textCursor().hasSelection())
        dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    // 如果在对话框中按下了打印按钮，则执行打印操作
    if (dlg.exec() == QDialog::Accepted) {
        ui->textEdit->print(&printer);
    }
}
// 撤销槽函数
void MainWindow::editSolt()
{
    QTextCursor cursor= ui->textEdit->textCursor();
    if(cursor.hasSelection()){
        //剪切复制删除可用
        ui->actionCut->setEnabled(true);
        ui->actionCopy->setEnabled(true);
        ui->actionDelete->setEnabled(true);

    }else{
        //剪切复制删除不可用
        ui->actionCut->setEnabled(false);
        ui->actionCopy->setEnabled(false);
        ui->actionDelete->setEnabled(false);

    }
}
// 撤销槽函数
void MainWindow::undoSolt()
{
    ui->textEdit->undo();
}
//剪切
void MainWindow::cutSolt()
{
    ui->textEdit->cut();
}
//复制
void MainWindow::copySolt()
{
    ui->textEdit->copy();
}
//粘贴
void MainWindow::pasteSolt()
{
    ui->textEdit->paste();
}
//删除
void MainWindow::deleteSolt()
{
    QTextCursor cursor= ui->textEdit->textCursor();
    if(cursor.hasSelection()){
        cursor.deleteChar();
    }
}
//全选
void MainWindow::selectAllSlot()
{
    ui->textEdit->selectAll();
}
//时间和日期
void MainWindow::timeOrDateSlot()
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    ui->textEdit->append(time.toString("hh:mm yyyy-MM-dd"));
}
//自动换行
void MainWindow::autoLineSlot(){
    if(autoLine){
        ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
        autoLine = false;

    }else{
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        autoLine = true;
    }
    ui->actionAutoLine->setIconVisibleInMenu(autoLine);

    setting->beginGroup("config");//beginGroup与下面endGroup 相对应，“config”是标记
    setting->setValue("auto_go_line",QVariant(autoLine?"1":"0"));
    setting->endGroup();
}
//字体选择对话框
void MainWindow::fontChooseSlot(){
    QFont textFont=  ui->textEdit->font();
    bool ok;
    QFont font = QFontDialog::getFont(&ok, textFont,this,tr("字体选择"));
    if (ok) {
        ui->textEdit->setFont(font);

        setting->beginGroup("config");//beginGroup与下面endGroup 相对应，“config”是标记
        setting->setValue("family",QVariant(font.family()));
        setting->setValue("point_size",QVariant(font.pointSize()));
        setting->setValue("italic",QVariant(font.italic() == true ?"1":"0"));
        setting->setValue("bold",QVariant(font.bold()== true ?"1":"0"));
        setting->setValue("overline",QVariant(font.overline()== true ?"1":"0"));
        setting->setValue("underline",QVariant(font.underline()== true ?"1":"0"));
        setting->endGroup();
    }

}
//光标变化
void MainWindow::cursorChangeSlot(){
    QTextCursor cursor = ui->textEdit->textCursor();
    int column = cursor.columnNumber()+1;
    int block = cursor.blockNumber()+1;
    int count = ui->textEdit->document()->toPlainText().length();
    change->setText("第 "+QString::number(block)+" 行，第 "+QString::number(column)+" 列   字数 "+QString::number(count)+"   ");
}
//状态栏
void MainWindow::statusBarSlot(){

    statusBar = !statusBar;
    ui->actionStatusBar->setIconVisibleInMenu(statusBar);
    ui->statusBar->setVisible(statusBar);

    setting->beginGroup("config");//beginGroup与下面endGroup 相对应，“config”是标记
    setting->setValue("status_bar",QVariant(statusBar?"1":"0"));
    setting->endGroup();
}
//关于记事本
void MainWindow::aboutSlot(){
    AboutDialog aboutDialog;

    aboutDialog.exec();
}
//跳转
void MainWindow::goToSlot(){

    goToDialot.exec();
}
void MainWindow::goToLineSlot(QString line){
    QTextCursor cursor = ui->textEdit->textCursor();
    if(line.toInt() > cursor.blockNumber()+1){
        QMessageBox box(QMessageBox::Question,"记事本 - 跳行","行数超过了总行数");
        box.setIcon(QMessageBox::NoIcon);
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确定"));
        box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
        box.exec();
        return;
    }
    goToDialot.close();
    int position = ui->textEdit->document()->findBlockByLineNumber(line.toInt()-1 ).position();
    cursor.setPosition(position,QTextCursor::MoveAnchor);
    ui->textEdit->setTextCursor( cursor );
}
//帮助
void MainWindow::helpSlot(){
    QDesktopServices::openUrl(QUrl("https://go.microsoft.com/fwlink/?LinkId=834783"));
}
//查找槽函数
void MainWindow::findSlot(){
    ui->textEdit->moveCursor(QTextCursor::Start);
    findDialog.exec();
}
//查找槽函数
void MainWindow::findNextSlot(){
    QTextCursor tc= ui->textEdit->textCursor();
    if(tc.selectedText().isEmpty()){
        findDialog.exec();
    }else{
        find(tc.selectedText(),false,false);
    }
}
//查找
void MainWindow::find(QString value,bool isChecked,bool isUp){
    if(isUp){
        if(!isChecked){
            if(!ui->textEdit->find(value,QTextDocument::FindBackward)){
                showMessage("找不到\""+value+"\"");
            }
        }else{
            if(!ui->textEdit->find(value,QTextDocument::FindBackward|QTextDocument::FindCaseSensitively)){
                showMessage("找不到\""+value+"\"");
            }
        }
    }else{
        if(!isChecked){
            if(!ui->textEdit->find(value)){
                showMessage("找不到\""+value+"\"");
            }
        }else{
            if(!ui->textEdit->find(value,QTextDocument::FindCaseSensitively)){
                showMessage("找不到\""+value+"\"");
            }
        }
    }
    QPalette palette =  ui->textEdit->palette();
    palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
    ui->textEdit->setPalette(palette);
}
//替换
void MainWindow::replaceSlot(){
    isFirstFind = true;
    replaceDialog.exec();
}
//进行替换
void MainWindow::replace(QString value, bool isChecked)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    //替换单个值
    cursor.insertText(value);
    //光标移动到前一位
    ui->textEdit->moveCursor(cursor.PreviousCharacter);
    //是否区分大小写 查找替换后的值高亮
    if(!isChecked){
        ui->textEdit->find(value);
    }else{
        ui->textEdit->find(value,QTextDocument::FindCaseSensitively);
    }
}

void MainWindow::doReplaceSlot(QString target,QString value,bool isChecked,bool isReplaceAll){
    if(isFirstFind){
        ui->textEdit->moveCursor(QTextCursor::Start);
        isFirstFind = false;
    }
    if(!isChecked){
        if(!ui->textEdit->find(target)){
            showMessage("找不到\""+target+"\"");
            return;
        }
    }else{
        if(!ui->textEdit->find(target,QTextDocument::FindCaseSensitively)){
            showMessage("找不到\""+target+"\"");
            return;
        }
    }
    //选中高亮
    QPalette palette =  ui->textEdit->palette();
    palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
    ui->textEdit->setPalette(palette);
    //替换
    if(isReplaceAll){
        if(!ui->textEdit->textCursor().atEnd()){
            replace(value,isChecked);
            doReplaceSlot(target,value,isChecked,isReplaceAll);
        }
    }else{
        replace(value, isChecked);
    }
}
//查找
void MainWindow::findForReplaceSlot(QString value,bool isChecked){
    if(isFirstFind){
        ui->textEdit->moveCursor(QTextCursor::Start);
        isFirstFind = false;
    }

    this->find(value,isChecked,false);
}
void MainWindow::showMessage(QString title){
    QMessageBox box(QMessageBox::Question,"记事本 - 查找",title);
    box.setIcon(QMessageBox::NoIcon);
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确定"));
    box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
    box.exec();
}
//拖动文件进入窗口
void MainWindow::dragEnterEvent(QDragEnterEvent *even)
{
        even->acceptProposedAction(); //可以在这个窗口部件上拖放对象
}
//释放对方时，执行的操作
void MainWindow::dropEvent(QDropEvent *even)
{
    QList<QUrl> urls = even->mimeData()->urls();
    if(!urls.isEmpty()){
        this->fileName = urls.last().toLocalFile();
        if(!this->fileName.isEmpty()){
             this->readFile();//读文件
        }
    }
}
//关闭事件
void MainWindow::closeEvent(QCloseEvent *event){
    //文档已经修改
    if(ui->textEdit->document()->isModified() && !ui->textEdit->document()->isEmpty() && fileContent != ui->textEdit->document()->toPlainText()){
        QMessageBox box(QMessageBox::Question,"记事本","是否将更改保存到 无标题");
        box.setIcon(QMessageBox::NoIcon);
        box.setStandardButtons (QMessageBox::Ok|QMessageBox::Ignore|QMessageBox::Cancel);
        box.setButtonText (QMessageBox::Ok,QString("保存"));
        box.setButtonText (QMessageBox::Ignore,QString("不保存"));
        box.setButtonText (QMessageBox::Cancel,QString("取消"));
        int result = box.exec();
        if(result == QMessageBox::Ok){
            //保存文件
            if(fileName.isEmpty()){//新建
                //弹出保存文件对话框
                fileName = QFileDialog::getSaveFileName(this, tr("打开文件"),QDir::homePath(),tr("文本文件 (*.*);;"));
                if(!fileName.isEmpty()){
                    //保存文件
                    this->saveTextToFile();
                }
            }else{//读取的文本
                this->saveTextToFile();
            }
             event->accept();
        }else  if(result == QMessageBox::Ignore){
             event->accept();
        }else{
            event->ignore();
        }
    }else{
        event->accept();
    }
}
