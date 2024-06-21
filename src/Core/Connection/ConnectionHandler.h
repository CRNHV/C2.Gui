#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <QObject>
#include <QString>
#include "Http.h"
#include <QtWebSockets/qwebsocket.h>
#include <QtNetwork/qnetworkaccessmanager.h>
#include "WebsocketConnection.h"
#include "Core/Requests/Request.h"

using namespace Gui::Core::Requests;

namespace Gui::Core::Connection
{
	class ConnectionHandler : public QObject
	{
		Q_OBJECT

	public:
		ConnectionHandler();
		void AttemptServerConnection(QString domain, QString username, QString password);
		void DisconnectFromServer();
		bool IsConnected();
		void SendRequest(Request* request);
	private:
		QString domain;
		Http* http = nullptr;
		WebsocketConnection* websocketConnection = nullptr;
	private slots:		
		void HandleJwtRequestFinished(QJsonObject jsonObject);
		void HandleConnectionChanged(bool connected);
		void HandleMessageReceived(QString message);
	signals:
		void MessageReceived(QJsonDocument json);
		void WebsocketConnectionChangedSignal(bool connected);
		void ConnectionLog(QString message, int level);
	};
}

#endif // !CONNECTION_MANAGER_H