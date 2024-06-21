#ifndef CONNECTION_COMMAND_H
#define CONNECTION_COMMAND_H

#include <QJsonObject>

namespace Gui::Core::Requests
{
	enum RequestAction
	{
		CLOSE = -1,
		SEND_COMMAND = 1,
		LIST_CLIENTS = 2,
		NEW_CLIENT = 3,
		REMOVE_CLIENT = 4,
		UPDATE_CLIENT = 5,
		LIST_COMMANDS = 6,
		LOOT_RECEIVED = 7,
		DOWNLOAD_LOOT = 8,
		LIST_LOOT = 9,
	};

	class Request
	{
	public:
		virtual QJsonObject ToJson() = 0;
		virtual ~Request() = default;
	};

}
#endif