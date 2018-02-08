#include "connectdialog.h"
#include "ui_connectdialog.h"

#include <QMessageBox>
#include <QTcpServer>

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog),
    m_type(Const::Server)
{
    ui->setupUi(this);

    QValidator* validator = new QIntValidator(0, 255, this);
    ui->ip_0->setValidator(validator);
    ui->ip_1->setValidator(validator);
    ui->ip_2->setValidator(validator);
    ui->ip_3->setValidator(validator);

    QValidator* validator_port = new QIntValidator(0, 65535, this);
    ui->port->setValidator(validator_port);

    onServer();
    ui->username->setFocus();

    connect(ui->server, &QRadioButton::clicked, this, &ConnectDialog::onServer);
    connect(ui->client, &QRadioButton::clicked, this, &ConnectDialog::onClient);
    connect(ui->create, &QPushButton::clicked, this, &ConnectDialog::onCreate);
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::onCreate()
{
    if (ui->username->text().isEmpty())
    {
        QMessageBox::critical(this, tr("Username Requested"), tr("Please input your username!"));
        ui->username->setFocus();
        return;
    }
    m_ip = QString("%1.%2.%3.%4").arg(ui->ip_0->text())
                                 .arg(ui->ip_1->text())
                                 .arg(ui->ip_2->text())
                                 .arg(ui->ip_3->text());
    m_port = ui->port->text().toInt();
    m_username = ui->username->text();

    if (m_type == Const::Server)
    {
        QTcpServer server;
        if (!server.listen(QHostAddress(m_ip), m_port))
        {
            QMessageBox::critical(this, tr("Create Server Failed"), QString(tr("Cannot create the server at %1:%2")).arg(m_ip).arg(m_port));
            ui->port->setFocus();
            return;
        }
    }

    this->accept();
}

void ConnectDialog::onServer()
{
    m_type = Const::Server;
    this->setWindowTitle(tr("Create Server"));
    ui->label_ip->setText(tr("Local &IP"));
    ui->create->setText(tr("&Create"));

    QStringList ip = Const::getLocalIP().split('.');
    if (ip.size() != 4)
    {
        QMessageBox::critical(this, tr("Invalid IP Address"), tr("Cannot get valid IP address.\nPlease check the network connection."));
        qApp->quit();
        return;
    }

    ui->ip_0->setText(ip[0]);
    ui->ip_1->setText(ip[1]);
    ui->ip_2->setText(ip[2]);
    ui->ip_3->setText(ip[3]);

    ui->port->setFocus();
    ui->port->selectAll();
}

void ConnectDialog::onClient()
{
    m_type = Const::Client;
    this->setWindowTitle(tr("Connect to Server"));
    ui->label_ip->setText(tr("Server &IP:"));
    ui->create->setText(tr("&Connect"));

    ui->ip_0->setFocus();
    ui->ip_0->selectAll();
}


