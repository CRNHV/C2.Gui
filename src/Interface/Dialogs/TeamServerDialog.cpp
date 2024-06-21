#include "TeamServerDialog.h"

namespace Gui::Interface::Dialogs
{
	TeamServerDialog::TeamServerDialog(QWidget* parent) : QDialog(parent)
	{
		setParent(parent);

		this->ui = this;
		setWindowTitle("Tartarus | Teamserver connection");

		formLayout = new QFormLayout(this);

		hostNameEdit = new QLineEdit();
		formLayout->addRow("Hostname:", hostNameEdit);

		usernameEdit = new QLineEdit();
		formLayout->addRow("Username:", usernameEdit);

		passwordEdit = new QLineEdit();
		formLayout->addRow("Password:", passwordEdit);

		formLayout->addRow(" ", new QWidget());

		saveButton = new QPushButton("Connect", this);
		formLayout->addRow("Connect", saveButton);

		hostNameEdit->setText("192.168.68.120:5012");
		usernameEdit->setText("root");
		passwordEdit->setText("Root12345!");

		this->adjustSize();
		this->resize(this->width() + 30, this->height());
	}
}

