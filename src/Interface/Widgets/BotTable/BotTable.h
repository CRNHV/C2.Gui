#ifndef BOT_TABLE_H
#define BOT_TABLE_H

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include "BotTableModel.h"
#include <qjsondocument.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMenu>
#include "Core/Requests/Request.h"
#include "Core/Requests/SendCommandRequest.h"
#include <qheaderview.h>
#include "Core/Requests/Request.h"

namespace Gui::Interface::Widgets
{
	class BotTable : public QTableView
	{
		Q_OBJECT

	public:
		void SetupUi(QWidget* widget);
		void ClearTable();
	private:
		BotTableModel* tableModel = nullptr;
		QGridLayout* layout = nullptr;
		QJsonArray contextMenuData;
		void buildContextMenu(const QJsonArray& menuArray, QMenu* parentMenu, uint32_t botId);
		void LoadClientData(QJsonDocument& json);
		void UpdateClient(QJsonDocument& json);
		void LoadNewClient(QJsonDocument& json);
		void MapJsonToDto(QJsonObject& botObj, BotDto* bot);
	public slots:
		void ShowContextMenu(const QPoint& pos);
		void HandleTableCommandReceived(QJsonDocument json);
		void LoadContextMenuData(QJsonDocument& json);
		void HandleContextMenuClick(int commandId, uint32_t botId);
	signals:
		void CommandSent(Core::Requests::Request* request);
	};
}

#endif // !BOT_TABLE_H
