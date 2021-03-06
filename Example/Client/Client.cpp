#include "Client.h"
#include "ui_Client.h"

Client::Client(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Client)
{
	ui->setupUi(this);

	wsSocket = new QWsSocket( this );

	socketStateChanged( wsSocket->state() );

	QObject::connect( ui->sendButton, SIGNAL(pressed()), this, SLOT(sendMessage()) );
	QObject::connect( ui->textLineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()) );
	QObject::connect( ui->connectButton, SIGNAL(pressed()), this, SLOT(connectSocket()) );
	QObject::connect( ui->disconnectButton, SIGNAL(pressed()), this, SLOT(disconnectSocket()) );
	QObject::connect( wsSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(socketStateChanged(QAbstractSocket::SocketState)) );
	QObject::connect( wsSocket, SIGNAL(frameReceived(QString)), this, SLOT(displayMessage(QString)) );
	QObject::connect( wsSocket, SIGNAL(connected()), this, SLOT(socketConnected()) );
	QObject::connect( wsSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()) );
}

Client::~Client()
{
	delete ui;
}

void Client::sendMessage()
{
	QString message = ui->pseudoLineEdit->text() + QLatin1String(": ") + ui->textLineEdit->text();
	ui->textLineEdit->clear();
	wsSocket->write( message );
}

void Client::displayMessage( QString message )
{
	ui->chatTextEdit->append( message );
}

void Client::connectSocket()
{
	wsSocket->connectToHost( QLatin1String("127.0.0.1"), 1337 );
}

void Client::disconnectSocket()
{
	wsSocket->disconnectFromHost();
}

void Client::socketConnected()
{	
	displayMessage( tr("CONNECTED") );
}

void Client::socketDisconnected()
{
	displayMessage( tr("DISCONNECTED") );
}

void Client::socketStateChanged(QAbstractSocket::SocketState socketState)
{
	switch ( socketState )
	{
		case QAbstractSocket::UnconnectedState:
			ui->socketStateLabel->setText(tr("Unconnected"));
			break;
		case QAbstractSocket::HostLookupState:
			ui->socketStateLabel->setText(tr("HostLookup"));
			break;
		case QAbstractSocket::ConnectingState:
			ui->socketStateLabel->setText(tr("Connecting"));
			break;
		case QAbstractSocket::ConnectedState:
			ui->socketStateLabel->setText("Connected");
			break;
		case QAbstractSocket::BoundState:
			ui->socketStateLabel->setText(tr("Bound"));
			break;
		case QAbstractSocket::ClosingState:
			ui->socketStateLabel->setText(tr("Closing"));
			break;
		case QAbstractSocket::ListeningState:
			ui->socketStateLabel->setText(tr("Listening"));
			break;
		default:
			ui->socketStateLabel->setText(tr("Unknown"));
			break;
	}
}
