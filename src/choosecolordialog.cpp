#include "choosecolordialog.h"
#include "ui_choosecolordialog.h"

#include <QPainter>

ChooseColorDialog::ChooseColorDialog(const QString& username, Const::HostType type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseColorDialog),
    m_username(username),
    m_type(type),
    m_is_disconnected(false)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::Dialog | Qt::WindowCloseButtonHint);

    drawIcon();
    this->setFixedHeight(this->sizeHint().height());
    if (type == Const::Client)
        ui->start->hide();
    m_state[0] = m_state[1] = -1;

    connect(ui->player0, &QRadioButton::clicked, this, &ChooseColorDialog::onFirst);
    connect(ui->player1, &QRadioButton::clicked, this, &ChooseColorDialog::onSecond);
    connect(ui->start, &QRadioButton::clicked, this, &ChooseColorDialog::onStart);
}

ChooseColorDialog::~ChooseColorDialog()
{
    delete ui;
}

void ChooseColorDialog::drawIcon()
{
    QPixmap pixmap(24, 24);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(12, 12), 10, 10);
    ui->player0->setIcon(pixmap);

    pixmap.fill(Qt::transparent);
    painter.setBrush(Qt::white);
    painter.drawEllipse(QPoint(12, 12), 10, 10);
    ui->player1->setIcon(pixmap);
}
