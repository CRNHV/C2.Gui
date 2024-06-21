#pragma once
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <QObject>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <Core/Requests/Request.h>
#include <Core/Requests/DownloadLootRequest.h>
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QJsonArray>

namespace Gui::Interface::Widgets
{
	class LootTable : public QTableWidget
	{
		Q_OBJECT

	public:
		void SetupUI(QWidget* parent);
		void ClearTable();
	public slots:
		void HandleLootDownloaded(QJsonDocument json);
		void HandleLootReceived(QJsonDocument json);
		void HandleListLoot(QJsonDocument json);
		void HandleDoubleClick(int row, int column);
	signals:
		void DownloadLoot(Gui::Core::Requests::Request* request);
		void LogMessage(QString message, int level);
	};
}