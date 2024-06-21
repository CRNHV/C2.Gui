#ifndef LIST_LOOT_H
#define LIST_LOOT_H

#include "Request.h"
#include <QJsonObject>

namespace Gui::Core::Requests
{
	class ListLootRequest : public Request
	{
	public:
		QJsonObject ToJson() override
		{
			QJsonObject json;
			json["Action"] = RequestAction::LIST_LOOT;
			return json;
		}
	};
}

#endif