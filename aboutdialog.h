#ifndef ABOUTNOTEPADDIALOG_H
#define ABOUTNOTEPADDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = 0);
    ~AboutDialog();

private slots:
    void on_okButton_clicked();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDDIALOG_H
