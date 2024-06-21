#include "Interface/MainWindow.h"
#include <QtWidgets/QApplication>
#include <QCoreApplication>
#include <QDir>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QString dirPath = "./Loot/Screenshots/";
	if (!QDir().exists(dirPath))
	{
		QDir().mkpath(dirPath);
	}

	
	Gui::Interface::MainWindow w;
	w.show();
	return a.exec();
}
