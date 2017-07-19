#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = 0);
    ~ReplaceDialog();
signals:
    void find(QString,bool);
    void replace(QString,QString,bool,bool);
private slots:
    void on_cancelButton_clicked();
    //文本改变
    void textChangeSlot();
    void on_findButton_clicked();

    void on_replaceButton_clicked();

    void on_replaceAllButton_clicked();

private:
    Ui::ReplaceDialog *ui;

};

#endif // REPLACEDIALOG_H
