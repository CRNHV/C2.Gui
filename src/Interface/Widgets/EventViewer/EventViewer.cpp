#include "EventViewer.h"

namespace Gui::Interface::Widgets
{
	void EventViewer::SetupUi(QWidget* parent)
	{
		this->eventViewerConsole = new QTextBrowser(this);
		this->eventViewerConsole->setObjectName("EventViewerConsole");
		this->gridLayout = new QGridLayout(this);

		gridLayout->addWidget(eventViewerConsole);
		setLayout(gridLayout);
	}

	void EventViewer::AppendText(const char* text, int errorLevel)
	{
		EventViewer::AppendText(QString(text), errorLevel);
	}

	void EventViewer::AppendText(QString text, int errorLevel)
	{
		// Get current date and time
		QDateTime dateTime = QDateTime::currentDateTime();
		QString dateTimeText = dateTime.toString("dd/MM/yyyy : hh:mm:ss");

		// Set text color based on error level
		QColor textColor;
		switch (errorLevel)
		{
		case 1:
			textColor = Qt::green;
			break;
		case 2:
			textColor = Qt::darkYellow;
			break;
		case 3:
			textColor = Qt::red;
			break;
		}

		// Prepend formatted date time and colored text
		QString formattedText = "<font color=\"" + textColor.name() + "\">" + "[" + dateTimeText + "] " + text + "</font>";

		// Append the formatted text to the console
		eventViewerConsole->append(formattedText);
	}
}