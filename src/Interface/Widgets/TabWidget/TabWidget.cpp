#include "TabWidget.h"

namespace Gui::Interface::Widgets
{
	void TabWidget::AddTab(QWidget* tabContent, const char* name)
	{
		QWidget* tabPage = new QWidget();
		QGridLayout* pageLayout = new QGridLayout(tabPage);
		pageLayout->addWidget(tabContent);
		tabPage->setLayout(pageLayout);
		addTab(tabPage, name);
	}

	void TabWidget::AddTabLayout(QLayout* layout, const char* name)
	{
		QWidget* tabPage = new QWidget();
		layout->setParent(tabPage);
		tabPage->setLayout(layout);
		addTab(tabPage, name);
	}
}