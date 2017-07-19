#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setWindowTitle("关于\"记事本\"");

}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_okButton_clicked()
{
    this->close();
}
