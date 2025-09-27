#include "GetCostHttpAction.h"

GetCostHttpAction::GetCostHttpAction(QObject *parent)
	: HttpNetActionBase(parent)
{}

GetCostHttpAction::~GetCostHttpAction()
{
}

void GetCostHttpAction::costSendPostRequest(int type, int number)
{
	QString url = "http://localhost:5000/readPartDatas";
	QJsonObject jsonData;
	jsonData["type"] = type;
	jsonData["number"] = number;
	QByteArray jsondata = QJsonDocument(jsonData).toJson();
	sendPostRequest(url, jsondata);
}

void GetCostHttpAction::parseAndProcessResponse(const QByteArray& responseData)
{
	QString text = QString::fromUtf8(responseData);
	qDebug() << "Response Data:" << text;
	emit sigSuccess(text);
}

void GetCostHttpAction::ParseError(const QString& errorString, int statusCode)
{
	qDebug() << errorString << " statusCode:" << statusCode;
	emit sigFailed();
}
