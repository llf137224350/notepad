#include "gotodialog.h"
#include "ui_gotodialog.h"

GoToDialog::GoToDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoToDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("转到指定行");

}

GoToDialog::~GoToDialog()
{
    delete ui;
}

void GoToDialog::on_cancelButton_clicked()
{
    this->close();
}

void GoToDialog::on_okButton_clicked()
{
    QString value = ui->lineEdit->text().trimmed();
    if(value.isEmpty()){
        this->showMessage("请输入要跳到的行数");
        return;
    }
    QRegExp rx("(\\d+)");
    if(!rx.exactMatch(value)){
        this->showMessage("请输入正整数");
        return;
    }
    emit goToLine(value);

}
void GoToDialog::showMessage(QString title){
    QMessageBox box(QMessageBox::Question,"记事本 - 跳行",title);
    box.setIcon(QMessageBox::NoIcon);
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确定"));
    box.setWindowFlags( Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint );
    box.exec();
}
