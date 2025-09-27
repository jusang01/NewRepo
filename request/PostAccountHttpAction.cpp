#include "PostAccountHttpAction.h"

PostAccountHttpAction::PostAccountHttpAction(QObject *parent)
	: HttpNetActionBase(parent)
{}

PostAccountHttpAction::~PostAccountHttpAction()
{}

void PostAccountHttpAction::accountsendPostRequest(QString account, QString password)
{
	//’À∫≈∫Õ√‹¬Î
	QString url = "http://localhost:5000/login";
	QJsonObject jsonData;
	jsonData["userName"] = account;
	jsonData["password"] = password;
	QByteArray jsondata = QJsonDocument(jsonData).toJson();
	sendPostRequest(url, jsondata);
}

void PostAccountHttpAction::accountLoginOutPostRequest(QString account, QString password)
{
	QString url = "http://localhost:5000/logout";
	QJsonObject jsonData;
	jsonData["userName"] = account;
	jsonData["password"] = password;
	QByteArray jsondata = QJsonDocument(jsonData).toJson();
	sendPostRequest(url, jsondata);
}

void PostAccountHttpAction::ParseResult(const QJsonValue& result)
{
	if (!result.isObject()) 
		return;

	QString message = result["message"].toString();
	QString role = result["role"].toString();
	emit sigSuccess();
}

void PostAccountHttpAction::ParseError(const QString& errorString, int statusCode)
{
	qDebug() << errorString << " statusCode:" << statusCode;
	emit sigFailed();
}
