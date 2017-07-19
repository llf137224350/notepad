#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class GoToDialog;
}

class GoToDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoToDialog(QWidget *parent = 0);
    ~GoToDialog();
signals:
    void goToLine(QString);

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::GoToDialog *ui;
    void showMessage(QString title);
};

#endif // GOTODIALOG_H
