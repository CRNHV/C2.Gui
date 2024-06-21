#ifndef EVENT_VIEWER_H
#define EVENT_VIEWER_H

#include <QDateTime>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

namespace Gui::Interface::Widgets
{
	class EventViewer : public QWidget
	{
	public:
		void SetupUi(QWidget* widget);
		void AppendText(const char* text, int errorLevel);
		void AppendText(QString text, int errorLevel);
	private:
		QGridLayout* gridLayout = nullptr;
		QTextBrowser* eventViewerConsole = nullptr;
	};
}
#endif // !EVENT_VIEWER_H

