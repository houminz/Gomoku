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
    Q_OBJECT

public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

    Const::HostType getType() const { return m_type; }
    QString getIP() const { return m_ip; }
    int getPort() const { return m_port; }
    QString getUsername() const { return m_username; }

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
