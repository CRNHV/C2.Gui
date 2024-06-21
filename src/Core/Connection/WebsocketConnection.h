#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <QObject>
#include <QString>
#include <qwebsocket.h>

namespace Gui::Core::Connection
{
	class WebsocketConnection : public QObject
	{
		Q_OBJECT

	public:
		WebsocketConnection();	
		void ConnectWebsocket(QString domain, QString token);
		void DisconnectWebsocket();
		void SendJson(QJsonObject data);
		bool IsConnected();
		void HandleWebsocketConnected();
		void HandleWebsocketDisconnected();
		void HandleTextMessageReceived(QString message);
	private:
		QWebSocket* websocket = nullptr;
	signals:
		void ConnectionChanged(bool connected);	
		void MessageReceived(QString message);
	};
}