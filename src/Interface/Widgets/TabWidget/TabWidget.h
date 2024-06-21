#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

namespace Gui::Interface::Widgets
{
	class TabWidget : public QTabWidget
	{
		Q_OBJECT

	public:
		TabWidget(QWidget* parent) : QTabWidget(parent)
		{
			setCurrentIndex(0);
		};
		void AddTab(QWidget* tabContent, const char* name);
		void AddTabLayout(QLayout* layout, const char* name);
	};
}

#endif // !TAB_WIDGET_H
