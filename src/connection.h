#ifndef CONNECTION_H
#define CONNECTION_H

#include "piece.h"

#include <QTcpSocket>

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    Connection(QObject *parent = nullptr);

    void setGreetingMessage(const QString& message) { m_greeting_message = message; }

public slots:
    void greeting();
    void sendMove(int row, int col, Piece::PieceColor color);
    void sendMessage(const QString &message);
    void sendPrepareState(bool isAccepted, int state0, int state1, const QString& username);

private:
    QString m_greeting_message;

private slots:
    void onReceivedData();

signals:
    void connectionReady(const QString& username);
    void prepareStateReceived(bool isAccepted, int state0, int state1, const QString& username);

    void gameStartedReceived();
    void pauseReceived();
    void continueReceived();

    void moveReceived(int row, int col, Piece::PieceColor color);
    void opponentUndoRequest();
    void undoAllowed();
    void undoDisallowed();
    void opponentDropReceived();
};

#endif // CONNECTION_H
