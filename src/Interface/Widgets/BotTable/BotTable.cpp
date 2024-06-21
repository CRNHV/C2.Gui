#include "BotTable.h"

namespace Gui::Interface::Widgets
{
	void BotTable::SetupUi(QWidget* parent)
	{
		setParent(parent);
		tableModel = new BotTableModel();
		setModel(tableModel);

		setContextMenuPolicy(Qt::CustomContextMenu);
		connect(this, &QTableView::customContextMenuRequested, this, &BotTable::ShowContextMenu);

		setVerticalScrollMode(QAbstractItemView::ScrollPerItem);
		setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setSelectionMode(QAbstractItemView::MultiSelection);
		setSelectionBehavior(QAbstractItemView::SelectRows);
		setAlternatingRowColors(true);
	}

	void BotTable::ClearTable()
	{
		if (tableModel != nullptr)
		{
			tableModel->clear();
		}
	}

	void BotTable::ShowContextMenu(const QPoint& pos)
	{
		QModelIndex index = indexAt(pos);
		if (!index.isValid())
			return;

		int botId = model()->data(model()->index(index.row(), 0)).toInt();
		QMenu contextMenu(tr("Context Menu"), this);
		buildContextMenu(contextMenuData, &contextMenu, botId);
		QPoint adjustedPos = pos + QPoint(0, 20);
		contextMenu.exec(mapToGlobal(adjustedPos));
	}

	void BotTable::buildContextMenu(const QJsonArray& menuArray, QMenu* parentMenu, uint32_t botId)
	{
		for (const QJsonValue& value : menuArray)
		{
			QJsonObject item = value.toObject();
			int type = item["Type"].toInt();
			QString text = item["Text"].toString();

			if (type == 0)
			{
				int id = item["CommandId"].toInt();
				QAction* action = new QAction(text, this);
				connect(action, &QAction::triggered, this, [this, id, botId]() {
					HandleContextMenuClick(id, botId);
					});
				parentMenu->addAction(action);
			}
			else if (type == 1)
			{
				QMenu* subMenu = new QMenu(text, this);
				QJsonArray subActions = item["Actions"].toArray();
				buildContextMenu(subActions, subMenu, botId);
				parentMenu->addMenu(subMenu);
			}
		}
	}

	void BotTable::HandleTableCommandReceived(QJsonDocument json)
	{
		int action = json["Action"].toInt();

		switch (action)
		{
		case Core::Requests::RequestAction::UPDATE_CLIENT:
		{
			UpdateClient(json);
			break;
		}
		case Core::Requests::RequestAction::NEW_CLIENT:
		{
			LoadNewClient(json);
			break;
		}
		case Core::Requests::RequestAction::LIST_CLIENTS:
		{
			LoadClientData(json);
			break;
		}
		case Core::Requests::RequestAction::LIST_COMMANDS:
		{
			LoadContextMenuData(json);
			break;
		}
		}
	}

	void BotTable::LoadContextMenuData(QJsonDocument& json)
	{
		QJsonObject rootObj = json.object();
		if (rootObj.contains("Data") && rootObj["Data"].isObject())
		{
			QJsonObject dataObj = rootObj["Data"].toObject();

			if (dataObj.contains("Commands") && dataObj["Commands"].isArray())
			{
				contextMenuData = dataObj["Commands"].toArray();
			}
		}
	}

	void BotTable::LoadClientData(QJsonDocument& json)
	{
		tableModel->clear();

		QJsonObject rootObj = json.object();

		if (!rootObj.contains("Data") || !rootObj["Data"].isObject())
		{
			return;
		}

		QJsonObject dataObj = rootObj["Data"].toObject();

		if (!dataObj.contains("Bots") || !dataObj["Bots"].isArray())
		{
			return;
		}

		QJsonArray botsArray = dataObj["Bots"].toArray();
		std::vector<BotDto> newBots;
		newBots.reserve(botsArray.size());

		for (const QJsonValue& botValue : botsArray)
		{
			if (!botValue.isObject())
			{
				continue;
			}

			QJsonObject botObj = botValue.toObject();

			BotDto bot;

			this->MapJsonToDto(botObj, &bot);

			newBots.push_back(bot);
		}

		tableModel->addBots(newBots);

	}

	void BotTable::UpdateClient(QJsonDocument& json)
	{
		QJsonObject rootObj = json.object();

		if (!rootObj.contains("Data") || !rootObj["Data"].isObject())
		{
			return;
		}

		QJsonObject dataObj = rootObj["Data"].toObject();

		if (!dataObj.contains("Agent") || !dataObj["Agent"].isObject())
		{
			return;
		}

		QJsonObject botObj = dataObj["Agent"].toObject();

		BotDto bot;

		this->MapJsonToDto(botObj, &bot);
		this->tableModel->updateBot(bot);
	}

	void BotTable::LoadNewClient(QJsonDocument& json)
	{
		QJsonObject rootObj = json.object();

		if (!rootObj.contains("Data") || !rootObj["Data"].isObject())
		{
			return;
		}

		QJsonObject dataObj = rootObj["Data"].toObject();

		if (!dataObj.contains("Agent") || !dataObj["Agent"].isObject())
		{
			return;
		}

		QJsonObject botObj = dataObj["Agent"].toObject();

		BotDto bot;

		this->MapJsonToDto(botObj, &bot);
		tableModel->addBot(bot);
	}

	void BotTable::MapJsonToDto(QJsonObject& botObj, BotDto* bot)
	{
		bot->Id = botObj["Id"].toDouble();
		bot->Host = botObj["Host"].toString();
		bot->Username = botObj["Username"].toString();
		bot->ExternalIp = botObj["ExternalIp"].toString();
		bot->InternalIp = botObj["InternalIp"].toString();
		bot->Architecture = botObj["Architecture"].toString();
		bot->OsVersion = botObj["OsVersion"].toString();

		QDateTime parsedDateTime = QDateTime::fromString(botObj["LastPingUtc"].toString(), Qt::ISODate);
		QString formattedDateTime = parsedDateTime.toString("yyyy-MM-dd HH:mm:ss");

		bot->LastPingUtc = formattedDateTime;
	}

	void BotTable::HandleContextMenuClick(int commandId, uint32_t botId)
	{
		qDebug() << "Send Command ID:" << commandId << "to Bot ID:" << botId;

		emit CommandSent(new Core::Requests::SendCommandRequest(commandId, botId));
	}
}

