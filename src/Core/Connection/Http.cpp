#include "Http.h"

namespace Gui::Core::Connection
{
	Http::Http()
	{
		if (manager == nullptr)
		{
			manager = new QNetworkAccessManager();
			connect(manager, &QNetworkAccessManager::finished, this, &Http::RequestFinished);
		}
	}

	void Http::GetJWTToken(QString domain, QString username, QString password)
	{
		SendAuthTokenRequest(domain, username, password);
	}

	void Http::RequestFinished(QNetworkReply* reply)
	{
		QJsonObject jsonObject;
		QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
		if (statusCode != 200)
		{
			emit JwtRequestFinished(jsonObject);
			return;
		}

		if (!reply->isReadable())
		{
			emit JwtRequestFinished(jsonObject);
			return;
		}

		QString replyString = reply->readAll();
		QJsonDocument replyDocument = QJsonDocument::fromJson(replyString.toUtf8());

		if (replyDocument.isNull() || !replyDocument.isObject())
		{
			emit JwtRequestFinished(jsonObject);
			return;
		}

		jsonObject = replyDocument.object();
		emit JwtRequestFinished(jsonObject);
	}

	void Http::SendAuthTokenRequest(QString domain, QString username, QString password)
	{
		QNetworkRequest request("http://" + domain + "/auth/login");
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

		QJsonObject jsonRequest;
		jsonRequest["username"] = username;
		jsonRequest["password"] = password;
		QJsonDocument jsonDocument(jsonRequest);
		QByteArray byteData = jsonDocument.toJson();

		this->manager->post(request, byteData);
	}
}