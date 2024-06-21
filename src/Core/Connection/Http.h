#ifndef HTTP_H
#define HTTP_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QtNetwork/qnetworkaccessmanager.h>
#include <QtNetwork/qnetworkreply.h>
#include <QObject>
#include <QString>

namespace Gui::Core::Connection
{
	class Http : public QObject
	{
		Q_OBJECT

	public:
		Http();
		void GetJWTToken(QString domain, QString username, QString password);
	private:
		QNetworkAccessManager* manager = nullptr;
		QNetworkRequest request;
		void SendAuthTokenRequest(QString domain, QString username, QString password);
	private slots:
		void RequestFinished(QNetworkReply* reply);
	signals:
		void JwtRequestFinished(QJsonObject jsonObject);
	};
}

#endif // !HTTP_H