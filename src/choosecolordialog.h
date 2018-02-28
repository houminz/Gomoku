#ifndef CHOOSECOLORDIALOG_H
#define CHOOSECOLORDIALOG_H

#include "const.h"
#include "connection.h"

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class ChooseColorDialog;
}

class ChooseColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseColorDialog(const QString& username, Const::HostType type, QWidget *parent = nullptr);
    ~ChooseColorDialog();

    Const::HostType firstPlayer() const { return m_first_player; }
    QString opponentUsername() const { return m_opp_username; }
    bool isDisconnected() const { return m_is_disconnected; }

public slots:
    void onUpdateState(bool isAccepted, int state0, int state1, const QString& oppUsername);
    void onDisconnected();

private:
    Ui::ChooseColorDialog *ui;
    QString m_username, m_opp_username;
    Const::HostType m_type, m_first_player;
    bool m_is_disconnected;
    int m_state[2];

    void drawIcon();
    void changeInfo();
    void sendData(bool isOk);

private slots:
    void onFirst(bool checked);
    void onSecond(bool checked);
    void onStart();

signals:
    void prepareStateChanged(bool isAccepted, int state0, int state1, const QString& username);

};

#endif // CHOOSECOLORDIALOG_H
