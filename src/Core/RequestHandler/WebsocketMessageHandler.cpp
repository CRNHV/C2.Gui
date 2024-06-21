#include "WebsocketMessageHandler.h"
#include "Core/Requests/Request.h"

using namespace Gui::Core::Requests;

void Gui::Core::RequestHandler::WebsocketMessageHandler::DispatchMessage(QJsonDocument json)
{
	int action = json["Action"].toInt();

	switch (action)
	{
	case RequestAction::NEW_CLIENT:
	case RequestAction::UPDATE_CLIENT:
	case RequestAction::LIST_CLIENTS:
	case RequestAction::LIST_COMMANDS:
		qDebug() << ("Got command for table from sever", 1);
		emit TableCommandReceived(json);
		break;
	case RequestAction::LOOT_RECEIVED:
		emit LogMessage("Got new loot from server", 1);
		emit LootReceived(json);
		break;
	case RequestAction::LIST_LOOT:
		emit LogMessage("Got all loot from server", 1);
		emit ListLoot(json);
		break;
	case RequestAction::DOWNLOAD_LOOT:
		emit LogMessage("Got loot from server", 1);
		emit LootDownloaded(json);
		break;
	}
}
