#ifndef GRID_LAYOUT_DOCK_H
#define GRID_LAYOUT_DOCK_H

#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainwindow>
#include <QtWidgets/QWidget>

namespace Gui::Interface::Widgets
{
	class GridLayoutDock : public QDockWidget
	{
	public:
		void SetupUi(QWidget* parent);
	};
}
#endif // !GRID_LAYOUT_DOCK_H

