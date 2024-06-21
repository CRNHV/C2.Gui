#include "GridLayoutDock.h"

namespace Gui::Interface::Widgets
{
	void GridLayoutDock::SetupUi(QWidget* parent)
	{
		QWidget* dockWidgetContent = new QWidget();
		QGridLayout* gridLayout = new QGridLayout(dockWidgetContent);

		setWidget(dockWidgetContent);
		gridLayout->addWidget(parent);
		parent->setParent(dockWidgetContent);

		setContextMenuPolicy(Qt::PreventContextMenu);
	}
}

