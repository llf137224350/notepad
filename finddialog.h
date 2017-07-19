#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = 0);
    ~FindDialog();
signals:
    void find(QString,bool,bool);
private slots:
    void on_pushButton_2_clicked();

    void on_findButton_clicked();
    //文本改变
    void textChangeSlot();

private:
    Ui::FindDialog *ui;
    //显示消息
    void showMessage(QString title);
};

#endif // FINDDIALOG_H
