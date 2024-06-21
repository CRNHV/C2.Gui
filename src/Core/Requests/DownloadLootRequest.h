#ifndef DOWNLOAD_LOOT_REQUEST_H
#define DOWNLOAD_LOOT_REQUEST_H

#include "Request.h"
#include <QJsonObject>

namespace Gui::Core::Requests
{
	class DownloadLootRequest : public Request
	{
	public:

		DownloadLootRequest(int lootId)
		{
			this->lootId = lootId;
		}

		QJsonObject ToJson() override
		{
			QJsonObject dataObj;
			dataObj["LootId"] = lootId;

			QJsonObject mainObj;
			mainObj["Action"] = 8;
			mainObj["Data"] = dataObj;

			return mainObj;
		}

	private:
		int lootId;
	};
}

#endif