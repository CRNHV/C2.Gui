#include "MainWindow.h"


namespace Gui::Interface
{
	void MainWindow::SetupUi(QMainWindow* mainWindow)
	{
		if (mainWindow->objectName().isEmpty())
			mainWindow->setObjectName("mainWindow");
		mainWindow->resize(1280, 720);

		AddMenuItems(mainWindow);

		centralwidget = new QWidget(mainWindow);
		centralwidget->setObjectName("centralwidget");
		centralGridLayout = new QGridLayout(centralwidget);
		centralGridLayout->setObjectName("centralGridLayout");
		mainWindow->setCentralWidget(centralwidget);

		botTable = new Widgets::BotTable();
		botTable->SetupUi(centralwidget);
		botTable->setSelectionBehavior(QAbstractItemView::SelectRows);
		botTable->setSelectionMode(QAbstractItemView::SingleSelection);
		botTable->horizontalHeader()->setVisible(true);
		botTable->verticalHeader()->setVisible(true);
		botTable->horizontalHeader()->setStretchLastSection(true);
		botTable->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		centralGridLayout->addWidget(botTable, 0, 0, 1, 1);

		eventDock = new Widgets::GridLayoutDock();
		eventViewer = new Widgets::EventViewer();
		eventDock->SetupUi(eventViewer);
		eventDock->setWindowTitle("Events");
		eventViewer->SetupUi(new QWidget());
		mainWindow->addDockWidget(Qt::BottomDockWidgetArea, eventDock);

		tabDock = new Widgets::GridLayoutDock();
		tabDock->setWindowTitle("Tabs");
		tab = new Widgets::TabWidget(this);

		lootTable = new Widgets::LootTable();
		lootTable->SetupUI(this);

		tab->AddTab(lootTable, "Loot");

		tabDock->SetupUi(tab);
		mainWindow->addDockWidget(Qt::BottomDockWidgetArea, tabDock);

		QMetaObject::connectSlotsByName(mainWindow);
	}

	void MainWindow::AddMenuItems(QMainWindow* mainWindow)
	{
		// Team server
		QMenu* teamServerMenu = new QMenu();
		teamServerMenu->setTitle("Team server");
		connectServer = teamServerMenu->addAction("Connect");
		disconnectServer = teamServerMenu->addAction("Disconnect");
		menuBar()->addMenu(teamServerMenu);

		disconnectServer->setVisible(false);

		// Toggle views
		QMenu* viewMenu = new QMenu();
		viewMenu->setTitle("View");
		QAction* toggleEvents = viewMenu->addAction(" ✓ Events");
		QAction* toggleTabDock = viewMenu->addAction(" ✓ Bottom tabs");
		menuBar()->addMenu(viewMenu);

		connect(toggleTabDock, &QAction::triggered, this, &MainWindow::HandleToggleTabsMenuClick);
		connect(connectServer, &QAction::triggered, this, &MainWindow::HandleConnectTeamserverClick);
		connect(disconnectServer, &QAction::triggered, this, &MainWindow::HandleDisconnectTeamserverClick);
		connect(toggleEvents, &QAction::triggered, this, &MainWindow::HandleToggleEventMenuClick);
	}

	void MainWindow::HandleToggleEventMenuClick()
	{
		if (this->eventDock != nullptr)
		{
			bool isVisible = this->eventDock->isVisible();
			this->eventDock->setVisible(!isVisible);

			QAction* button = qobject_cast<QAction*>(sender());

			QString visibleText = eventDock->isVisible() == true ? "✓" : "х";
			QString newText = visibleText + " Events";

			button->setText(newText);
		}
	}

	void MainWindow::HandleToggleTabsMenuClick()
	{
		if (this->tabDock != nullptr)
		{
			bool isVisible = this->tabDock->isVisible();
			this->tabDock->setVisible(!isVisible);

			QAction* button = qobject_cast<QAction*>(sender());

			QString visibleText = tabDock->isVisible() == true ? "✓" : "х";
			QString newText = visibleText + " Bottom tabs";

			button->setText(newText);
		}
	}

	void MainWindow::HandleConnectTeamserverClick()
	{
		if (this->teamServerDialog != nullptr)
		{
			this->teamServerDialog->show();
		}
	}

	void MainWindow::HandleDisconnectTeamserverClick()
	{
		if (this->connectionHandler != nullptr)
		{
			this->connectionHandler->DisconnectFromServer();
		}
	}

	void MainWindow::HandleSaveConnectionClick()
	{
		if (this->teamServerDialog == nullptr)
		{
			return;
		}

		this->teamServerDialog->close();

		QString hostname = teamServerDialog->hostNameEdit->text();
		QString username = teamServerDialog->usernameEdit->text();
		QString password = teamServerDialog->passwordEdit->text();

		if (hostname == "" || username == "" || password == "")
		{
			QMessageBox* message = new QMessageBox();
			message->setText("Incorrect input in connection settings");
			message->show();
			return;
		}

		QString eventString = "Connecting to domain: " + hostname + " with username:" + username;
		eventViewer->AppendText(eventString, 1);

		this->connectionHandler->AttemptServerConnection(hostname, username, password);
	}

	void MainWindow::HandleLogEvent(QString string, int errorLevel)
	{
		eventViewer->AppendText(string, errorLevel);
	}

	void MainWindow::HandleConnectionChanged(bool connected)
	{
		disconnectServer->setVisible(connected);
		connectServer->setVisible(!connected);

		this->setWindowTitle(connected ? "C2 UI - Connected" : "C2 UI - Disconnected");

		if (connected)
		{
			this->connectionHandler->SendRequest(new ListLootRequest());
			this->connectionHandler->SendRequest(new ListCommandsRequest());
			this->connectionHandler->SendRequest(new ListClientsRequest());
		}
		else
		{
			this->lootTable->ClearTable();
			this->botTable->ClearTable();
		}
	}

	void MainWindow::HandleMessageReceived(QJsonDocument json)
	{
		websocketMessageHandler->DispatchMessage(json);
	}

	void MainWindow::HandleMessageSent(Request* json)
	{
		connectionHandler->SendRequest(json);
	}
}