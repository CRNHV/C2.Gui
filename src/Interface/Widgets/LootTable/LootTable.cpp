#include "LootTable.h"

namespace Gui::Interface::Widgets
{
	void LootTable::SetupUI(QWidget* parent)
	{
		setParent(parent);
		setColumnCount(3);
		setHorizontalHeaderLabels(QStringList() << "LootId" << "CommandName" << "FilePath");
		horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		setEditTriggers(QAbstractItemView::NoEditTriggers);

		// Connect the double-click signal to the slot
		connect(this, &QTableWidget::cellDoubleClicked, this, &LootTable::HandleDoubleClick);
	}

	void LootTable::ClearTable()
	{
		this->setRowCount(0);
	}

	void LootTable::HandleListLoot(QJsonDocument json)
	{
		int lootCount = json["Data"]["Amount"].toInt();
		QJsonArray dataArray = json["Data"]["Loot"].toArray();

		// Loop through each item in the array
		for (int i = 0; i < lootCount; ++i)
		{
			QJsonValue item = dataArray.at(i);

			// Check if the item is an object (assuming valid JSON)
			if (!item.isObject())
			{
				continue; // Skip non-object elements
			}

			QJsonObject lootObject = item.toObject();

			int lootId = lootObject["LootId"].toInt();
			QString commandName = lootObject["CommandName"].toString();
			QString filePath = lootObject["FilePath"].toString();

			// Add a new row to the table
			int row = rowCount();
			insertRow(row);

			// Set the table items
			setItem(row, 0, new QTableWidgetItem(QString::number(lootId)));
			setItem(row, 1, new QTableWidgetItem(commandName));
			setItem(row, 2, new QTableWidgetItem(filePath));
		}
	}

	void LootTable::HandleLootReceived(QJsonDocument json)
	{
		QJsonObject dataObject = json["Data"].toObject();

		int lootId = dataObject["LootId"].toInt();
		QString commandName = dataObject["CommandName"].toString();
		QString filePath = dataObject["FilePath"].toString();

		int row = rowCount();
		insertRow(row);
		setItem(row, 0, new QTableWidgetItem(QString::number(lootId)));
		setItem(row, 1, new QTableWidgetItem(commandName));
		setItem(row, 2, new QTableWidgetItem(filePath));
	}

	void LootTable::HandleLootDownloaded(QJsonDocument json)
	{
		QJsonObject dataObject = json["Data"].toObject();

		QString fileBase64 = dataObject["Base64File"].toString();
		QString filePath = dataObject["FilePath"].toString();

		// Decode the base64 string to bytes
		QByteArray fileBytes = QByteArray::fromBase64(fileBase64.toUtf8());

		// Write the bytes to the specified file
		QFile file(filePath);
		if (file.open(QIODevice::WriteOnly))
		{
			file.write(fileBytes);
			file.close();
			qDebug() << "File successfully written to" << filePath;

			QUrl fileUrl = QUrl::fromLocalFile(filePath);
			QDesktopServices::openUrl(fileUrl);
		}
	}

	void LootTable::HandleDoubleClick(int row, int column)
	{
		Q_UNUSED(column);

		QString lootId = item(row, 0)->text();
		QString commandName = item(row, 1)->text();
		QString filePath = item(row, 2)->text();

		emit LogMessage("Downloading loot with Id: " + lootId, 1);

		Gui::Core::Requests::Request* downloadLootRequest = new Gui::Core::Requests::DownloadLootRequest(lootId.toInt());
		emit DownloadLoot(downloadLootRequest);
	}
}