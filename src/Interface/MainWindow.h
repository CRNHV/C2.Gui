#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>
#include <QJsonObject>

#include "Interface/Widgets/BotTable/BotTable.h"
#include "Interface/Widgets/EventViewer/EventViewer.h"
#include "Interface/Widgets/GridLayoutDock/GridLayoutDock.h"
#include "Interface/Widgets/TabWidget/TabWidget.h"
#include "Widgets/BotTable/BotTableModel.h"
#include "Widgets/LootTable/LootTable.h"
#include "Interface/Dialogs/TeamServerDialog.h"
#include "Core/Connection/ConnectionHandler.h"
#include "Core/Requests/Request.h"
#include "Core/Requests/ListLootRequest.h"
#include "Core/Requests/ListClientsRequest.h"
#include "Core/Requests/ListCommandsRequest.h"
#include "Core/RequestHandler/WebsocketMessageHandler.h"


using namespace Gui::Interface;
using namespace Gui::Core;
using namespace Gui::Core::RequestHandler;
using namespace Gui::Interface::Widgets;

namespace Gui::Interface
{
	class MainWindow : public QMainWindow
	{
	public:
		MainWindow(QWidget* parent = nullptr) : QMainWindow(parent)
		{
			this->setWindowTitle("C2 UI");
			SetupUi(this);
			this->mainWindow = this;

			this->websocketMessageHandler = new WebsocketMessageHandler();
			this->teamServerDialog = new Dialogs::TeamServerDialog(this);
			this->connectionHandler = new Connection::ConnectionHandler();

			connect(websocketMessageHandler, &WebsocketMessageHandler::LogMessage, this, &MainWindow::HandleLogEvent);
			connect(connectionHandler, &Connection::ConnectionHandler::ConnectionLog, this, &MainWindow::HandleLogEvent);
			connect(lootTable, &LootTable::LogMessage, this, &MainWindow::HandleLogEvent);

			connect(botTable, &BotTable::CommandSent, this, &MainWindow::HandleMessageSent);
			connect(lootTable, &LootTable::DownloadLoot, this, &MainWindow::HandleMessageSent);

			connect(teamServerDialog->saveButton, &QPushButton::released, this, &MainWindow::HandleSaveConnectionClick);
			connect(connectionHandler, &Connection::ConnectionHandler::WebsocketConnectionChangedSignal, this, &MainWindow::HandleConnectionChanged);

			connect(connectionHandler, &Connection::ConnectionHandler::MessageReceived, this, &MainWindow::HandleMessageReceived);
			connect(websocketMessageHandler, &WebsocketMessageHandler::TableCommandReceived, botTable, &BotTable::HandleTableCommandReceived);
			connect(websocketMessageHandler, &WebsocketMessageHandler::LootReceived, lootTable, &LootTable::HandleLootReceived);
			connect(websocketMessageHandler, &WebsocketMessageHandler::LootDownloaded, lootTable, &LootTable::HandleLootDownloaded);
			connect(websocketMessageHandler, &WebsocketMessageHandler::ListLoot, lootTable, &LootTable::HandleListLoot);
		}
		void SetupUi(QMainWindow* mainWindow);
	private:
		// Layout
		QMainWindow* mainWindow = nullptr;
		QWidget* centralwidget = nullptr;
		QGridLayout* centralGridLayout = nullptr;

		// Widgets
		Dialogs::TeamServerDialog* teamServerDialog = nullptr;
		Widgets::GridLayoutDock* eventDock = nullptr;
		Widgets::GridLayoutDock* tabDock = nullptr;
		Widgets::TabWidget* tab = nullptr;

		Widgets::BotTable* botTable = nullptr;
		Widgets::EventViewer* eventViewer = nullptr;
		Widgets::LootTable* lootTable = nullptr;

		QAction* connectServer;
		QAction* disconnectServer;
		// Networking
		Connection::ConnectionHandler* connectionHandler = nullptr;
		WebsocketMessageHandler* websocketMessageHandler = nullptr;

		void AddMenuItems(QMainWindow* mainWindow);
	private slots:
		void HandleLogEvent(QString string, int errorLevel);
		void HandleToggleEventMenuClick();
		void HandleDownloadLoot();
		void HandleToggleTabsMenuClick();
		void HandleConnectTeamserverClick();
		void HandleDisconnectTeamserverClick();
		void HandleSaveConnectionClick();
		void HandleConnectionChanged(bool connected);
		void HandleMessageReceived(QJsonDocument json);
		void HandleMessageSent(Request* request);
	};
}

#endif // !MAINWINDOW_H
