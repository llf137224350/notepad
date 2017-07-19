#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("查找");

    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangeSlot()));

    ui->findButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    ui->radioUpButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
    ui->radioDownButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
    ui->checkBox->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

}

FindDialog::~FindDialog()
{
    delete ui;
}
void FindDialog::showMessage(QString title){
    QMessageBox box(QMessageBox::Question,"记事本 - 查找",title);
    box.setIcon(QMessageBox::NoIcon);
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确定"));
    box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
    box.exec();
}

void FindDialog::on_pushButton_2_clicked()
{
    this->close();

}

void FindDialog::on_findButton_clicked()
{
    bool checkbox = ui->checkBox->isChecked();
    QString value = ui->lineEdit->text().trimmed();
    bool isUp = false;
    if(ui->radioUpButton->isChecked()){
        isUp = true;
    }
    if(value.isEmpty()){
        this->showMessage("请输入查找关键字");
        return;
    }
    //回调事件
    emit this->find(value,checkbox,isUp);
}

void FindDialog::textChangeSlot(){
    if(ui->lineEdit->text().trimmed().isEmpty()){
        ui->findButton->setEnabled(false);
    }else{
        ui->findButton->setEnabled(true);
    }
}
