#ifndef LIST_COMMANDS_H
#define LIST_COMMANDS_H

#include "Request.h"
#include <QJsonObject>

namespace Gui::Core::Requests
{
	class ListCommandsRequest : public Request
	{
	public:
		QJsonObject ToJson() override
		{
			QJsonObject json;
			json["Action"] = RequestAction::LIST_COMMANDS;
			return json;
		}
	};
}

#endif