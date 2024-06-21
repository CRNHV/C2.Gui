#ifndef SEND_COMMAND_H
#define SEND_COMMAND_H

#include "Request.h"
#include <QJsonObject>

namespace Gui::Core::Requests
{
	class SendCommandRequest : public Request
	{
	public:

		SendCommandRequest(int commandId, uint32_t botId)
		{
			this->commandId = commandId;
			this->botId = botId;
		}

		QJsonObject ToJson() override
		{
			QJsonObject dataObj;
			dataObj["BotId"] = (double)botId;
			dataObj["CommandId"] = commandId;

			QJsonObject mainObj;
			mainObj["Action"] = RequestAction::SEND_COMMAND;
			mainObj["Data"] = dataObj;

			return mainObj;
		}

	private:
		int commandId;
		uint32_t botId;
	};
}

#endif