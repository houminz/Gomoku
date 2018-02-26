#include "connection.h"

#include <QDataStream>

Connection::Connection(QObject *parent) :
    QTcpSocket(parent)
{
    connect(this, &QTcpSocket::readyRead, this, &Connection::onReceivedData);
    connect(this, &QTcpSocket::connected, this, &Connection::greeting);
}

void Connection::greeting()
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("GREETING") << m_greeting_message;
    this->write(array);
}

void Connection::sendMove(int row, int col, Piece::PieceColor color)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("MOVE");
    out << row << col << (int)color;
    this->write(array);
}

void Connection::sendMessage(const QString &message)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("MESSAGE") << message;
    this->write(array);
}

void Connection::sendPrepareState(bool isAccepted, int state0, int state1, const QString &username)
{
    QByteArray array;
    QDataStream out(&array, QIODevice::WriteOnly);
    out << QString("PREPARE");
    out << isAccepted << state0 << state1 << username;
    this->write(array);
}

void Connection::onReceivedData()
{
    QByteArray array = this->readAll();
    QDataStream in(array);
    for (QString type; !in.atEnd();)
    {
        in >> type;
        if (type == "GREETING")
        {
            QString username;
            in >> username;
            emit connectionReady(username);
        }
        else if (type == "PREPARE")
        {
            bool isAccepted;
            int state0, state1;
            QString username;
            in >> isAccepted >> state0 >> state1 >> username;
            emit prepareStateReceived(isAccepted, state0, state1, username);
        }
        else if (type == "MOVE")
        {
            int row, col, color;
            in >> row >> col >> color;
            emit moveReceived(row, col, (Piece::PieceColor)color);
        }
        else if (type == "MESSAGE")
        {
            QString message;
            in >> message;
            if (message == "start")
                emit gameStartedReceived();
            else if (message == "pause")
                emit pauseReceived();
            else if (message == "continue")
                emit continueReceived();
            else if (message == "undo")
                emit opponentUndoRequest();
            else if (message == "allowundo")
                emit undoAllowed();
            else if (message == "disallowundo")
                emit undoDisallowed();
            else if (message == "drop")
                emit opponentDropReceived();
        }
    }
}
