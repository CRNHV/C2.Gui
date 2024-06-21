#include "WebsocketConnection.h"

namespace Gui::Core::Connection
{
	WebsocketConnection::WebsocketConnection()
	{
		if (this->websocket == nullptr)
		{
			this->websocket = new QWebSocket();

			this->websocket->connect(this->websocket, &QWebSocket::connected, this, &WebsocketConnection::HandleWebsocketConnected);
			this->websocket->connect(this->websocket, &QWebSocket::disconnected, this, &WebsocketConnection::HandleWebsocketDisconnected);
			this->websocket->connect(this->websocket, &QWebSocket::textMessageReceived, this, &WebsocketConnection::HandleTextMessageReceived);
		}
	}

	void WebsocketConnection::ConnectWebsocket(QString domain, QString token)
	{
		QNetworkRequest request = QNetworkRequest();
		request.setRawHeader(QByteArray("Authorization"), "Bearer " + token.toUtf8());
		request.setUrl("ws://" + domain + "/ws");
		this->websocket->open(request);
	}

	void WebsocketConnection::DisconnectWebsocket()
	{
		if (this->websocket == nullptr)
		{
			return;
		}

		if (!this->websocket->isValid())
		{
			return;
		}

		websocket->close(QWebSocketProtocol::CloseCodeNormal, "");
	}

	void WebsocketConnection::SendJson(QJsonObject data)
	{
		if (this->websocket == nullptr)
		{
			return;
		}

		if (!this->websocket->isValid())
		{
			return;
		}

		QJsonDocument jsonDocument(data);
		QByteArray byteData = jsonDocument.toJson();
		websocket->sendBinaryMessage(byteData);
	}

	bool WebsocketConnection::IsConnected()
	{
		return this->websocket->isValid();
	}

	void WebsocketConnection::HandleWebsocketConnected()
	{
		emit WebsocketConnection::ConnectionChanged(true);
	}

	void WebsocketConnection::HandleWebsocketDisconnected()
	{
		emit WebsocketConnection::ConnectionChanged(false);
	}

	void WebsocketConnection::HandleTextMessageReceived(QString message)
	{
		emit MessageReceived(message);
	}
}