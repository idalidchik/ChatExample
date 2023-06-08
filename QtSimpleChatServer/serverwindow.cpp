#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "chatserver.h"
#include <QMessageBox>
ServerWindow::ServerWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWindow)
    , m_chatServer(new ChatServer(this))
{
    ui->setupUi(this);
    connect(ui->startStopButton, &QPushButton::clicked, this, &ServerWindow::toggleStartServer);
    connect(m_chatServer, &ChatServer::logMessage, this, &ServerWindow::logMessage);
    // connect the click of the "send" button and the press of the enter while typing to the slot that sends the message
    connect(ui->sendButton, &QPushButton::clicked, this, &ServerWindow::sendMessage);
    connect(ui->messageEdit, &QLineEdit::returnPressed, this, &ServerWindow::sendMessage);
    ui->sendButton->setEnabled(false);
    ui->messageEdit->setEnabled(false);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::toggleStartServer()
{
    if (m_chatServer->isListening()) {
        m_chatServer->stopServer();
        ui->startStopButton->setText(tr("Start Server"));
        logMessage(QStringLiteral("Server Stopped"));
        ui->sendButton->setEnabled(false);
        ui->messageEdit->setEnabled(false);
    } else {
        if (!m_chatServer->listen(QHostAddress::Any, 1967)) {
            QMessageBox::critical(this, tr("Error"), tr("Unable to start the server"));
            return;
        }
        logMessage(QStringLiteral("Server Started"));
        ui->startStopButton->setText(tr("Stop Server"));
        ui->sendButton->setEnabled(true);
        ui->messageEdit->setEnabled(true);
    }
}

void ServerWindow::logMessage(const QString &msg)
{
    ui->logEditor->appendPlainText(msg + QLatin1Char('\n'));
}

void ServerWindow::sendMessage()
{
    // we use the client to send the message that the user typed
    m_chatServer->sendMessage(ui->messageEdit->text());
    // clear the content of the message editor
    ui->messageEdit->clear();
}
