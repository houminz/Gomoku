#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include "const.h"

#include <QDialog>

namespace Ui
{
class ConnectDialog;
}

class ConnectDialog : public QDialog
{
public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

private slots:
    void onCreate();
    void onServer();
    void onClient();

private:
    Ui::ConnectDialog *ui;
    Const::HostType m_type;
    QString m_username;
    QString m_ip;
    int m_port;
};

#endif // CONNECTDIALOG_H
