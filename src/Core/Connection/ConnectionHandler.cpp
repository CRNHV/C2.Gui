#include "ConnectionHandler.h"

namespace Gui::Core::Connection
{
	ConnectionHandler::ConnectionHandler()
	{
		if (this->http == nullptr)
		{
			this->http = new Http();
		}

		if (this->websocketConnection == nullptr)
		{
			this->websocketConnection = new WebsocketConnection();
		}

		connect(websocketConnection, &WebsocketConnection::MessageReceived, this, &ConnectionHandler::HandleMessageReceived);
		connect(websocketConnection, &WebsocketConnection::ConnectionChanged, this, &ConnectionHandler::HandleConnectionChanged);
		connect(http, &Http::JwtRequestFinished, this, &ConnectionHandler::HandleJwtRequestFinished);
	}

	void ConnectionHandler::AttemptServerConnection(QString domain, QString username, QString password)
	{
		this->domain = domain;
		http->GetJWTToken(domain, username, password);
	}

	void ConnectionHandler::DisconnectFromServer()
	{
		this->websocketConnection->DisconnectWebsocket();
	}

	bool ConnectionHandler::IsConnected()
	{
		return this->websocketConnection->IsConnected();
	}

	void ConnectionHandler::SendRequest(Request* request)
	{
		if (request == nullptr)
			return;

		this->websocketConnection->SendJson(request->ToJson());
		delete request;
	}

	void ConnectionHandler::HandleConnectionChanged(bool connected)
	{
		emit ConnectionLog(connected ? "Websocket connected" : "Websocket disconnected", connected ? 1 : 3);

		emit WebsocketConnectionChangedSignal(connected);
	}

	void ConnectionHandler::HandleJwtRequestFinished(QJsonObject jsonObject)
	{
		if (jsonObject.isEmpty())
		{
			emit ConnectionLog("Failed to authenticate.", 3);
			HandleConnectionChanged(false);
			return;
		}

		emit ConnectionLog("Authentication succesfull.", 1);

		QString authenticationToken = jsonObject["token"].toString();
		if (authenticationToken.isNull() || authenticationToken.isEmpty())
		{
			emit ConnectionLog("Failed to parse JSON response.", 3);
			HandleConnectionChanged(false);
			return;
		}

		emit ConnectionLog("Starting websocket connection.", 1);
		this->websocketConnection->ConnectWebsocket(this->domain, authenticationToken);
		return;
	}

	void ConnectionHandler::HandleMessageReceived(QString message)
	{
		qDebug() << "Websocket message received: " << message;

		QJsonDocument jsonMessage = QJsonDocument::fromJson(message.toUtf8());
		emit MessageReceived(jsonMessage);
	}
}