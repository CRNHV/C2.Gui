#ifndef LIST_CLIENT_REQUEST_H
#define LIST_CLIENT_REQUEST_H

#include "Request.h"
#include <QJsonObject>

namespace Gui::Core::Requests
{
	class ListClientsRequest : public Request
	{
	public:
		QJsonObject ToJson() override
		{
			QJsonObject json;
			json["Action"] = RequestAction::LIST_CLIENTS;
			return json;
		}
	};
}

#endif