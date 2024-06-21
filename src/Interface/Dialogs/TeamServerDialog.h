#ifndef TEAM_SERVER_DIALOG_H
#define TEAM_SERVER_DIALOG_H

#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>


namespace Gui::Interface::Dialogs
{
	class TeamServerDialog : public QDialog
	{

	public:
		TeamServerDialog(QWidget* parent = nullptr);
		QPushButton* saveButton;		
		QLineEdit* hostNameEdit;
		QLineEdit* usernameEdit;
		QLineEdit* passwordEdit;
	private:
		QFormLayout* formLayout = nullptr;
		Dialogs::TeamServerDialog* ui;
	signals:
		void TeamServerDialogEvent(QString string, int errorLevel);
		
	};
}
#endif // !TEAM_SERVER_DIALOG_H

