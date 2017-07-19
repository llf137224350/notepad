#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("替换");

    connect(ui->findEdit,SIGNAL(textChanged(QString)),this,SLOT(textChangeSlot()));


    ui->findButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    ui->replaceButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    ui->replaceAllButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
    ui->checkBox->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_cancelButton_clicked()
{
    this->close();
}
//文本改变
void ReplaceDialog::textChangeSlot(){
    if(ui->findEdit->text().trimmed().isEmpty()){
        ui->findButton->setEnabled(false);
        ui->replaceButton->setEnabled(false);
        ui->replaceAllButton->setEnabled(false);
    }else{
        ui->findButton->setEnabled(true);
        ui->replaceButton->setEnabled(true);
        ui->replaceAllButton->setEnabled(true);
    }
}

void ReplaceDialog::on_findButton_clicked()
{
    bool checkbox = ui->checkBox->isChecked();
    QString value = ui->findEdit->text().trimmed();
    //回调事件
    emit this->find(value,checkbox);
}

void ReplaceDialog::on_replaceButton_clicked()
{
    bool checkbox = ui->checkBox->isChecked();
    QString target = ui->findEdit->text().trimmed();
    QString value = ui->replaceEdit->text().trimmed();
    //回调事件
    emit this->replace(target,value,checkbox,false);
}

void ReplaceDialog::on_replaceAllButton_clicked()
{
    bool checkbox = ui->checkBox->isChecked();
    QString target = ui->findEdit->text().trimmed();
    QString value = ui->replaceEdit->text().trimmed();
    //回调事件
    emit this->replace(target,value,checkbox,true);
}
