#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <QJsonObject>
#include <qjsondocument.h>

namespace Gui::Core::RequestHandler
{
	class WebsocketMessageHandler : public QObject
	{
		Q_OBJECT

	public:
		void DispatchMessage(QJsonDocument json);
	signals:
		void LogMessage(QString message, int level);
		void TableCommandReceived(QJsonDocument json);
		void LootReceived(QJsonDocument json);
		void LootDownloaded(QJsonDocument json);
		void ListLoot(QJsonDocument json);
	};
}

#endif