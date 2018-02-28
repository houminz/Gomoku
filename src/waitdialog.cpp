#include "waitdialog.h"
#include "ui_waitdialog.h"

#include <QCloseEvent>

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog),
    m_allowed(false)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    this->setFixedSize(240, 100);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::closeEvent(QCloseEvent* event)
{
    if (!ui->buttonBox->isEnabled()) event->ignore();
}



void WaitDialog::onUndoAllowed()
{
    m_allowed = true;
    ui->label->setText(tr("Undo request was allowed."));
    ui->buttonBox->setEnabled(true);
    this->setWindowTitle(tr("Allowed"));
    this->activateWindow();
    emit undoAllowed();
}

void WaitDialog::onUndoDisallowed()
{
    m_allowed = false;
    ui->label->setText(tr("Undo request was not allowed."));
    ui->buttonBox->setEnabled(true);
    this->setWindowTitle(tr("Not Allowed"));
    this->activateWindow();
    emit undoDisallowed();
}

