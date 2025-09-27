#include "HttpNetActionBase.h"
#include <QDebug>
#include <QJsonArray>

#pragma execution_character_set("utf-8")

HttpNetActionBase::HttpNetActionBase(QObject *parent)
	: QObject(parent)
{
	m_manager = new QNetworkAccessManager(this);
	m_currentReply = nullptr;
	connect(m_manager, &QNetworkAccessManager::finished,this, &HttpNetActionBase::onFinished);
}

HttpNetActionBase::~HttpNetActionBase()
{}

void HttpNetActionBase::sendRequest(Method method, const QUrl& url, const QByteArray& data, const QHash<QString, QString>& headers)
{
	QNetworkRequest request;
	request.setUrl(QUrl(url));

	//设置完头部
	setupRequest(request, headers);

	switch (method)
	{
	case Method::GET:
		m_currentReply = m_manager->get(request);
		break;

	case Method::POST:
		m_currentReply = m_manager->post(request, data);
		break;
	}
}

void HttpNetActionBase::parseAndProcessResponse(const QByteArray& responseData)
{
	if (responseData.isEmpty()) {
		qDebug() << "Empty response data";
		return;
	}

	QJsonParseError parseError;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData, &parseError);

	if (parseError.error != QJsonParseError::NoError) {
		QString errorMsg = QString("JSON parse error: %1").arg(parseError.errorString());
		qDebug() << errorMsg << "Response:" << responseData;
		return;
	}

	if (jsonDoc.isNull() || !jsonDoc.isObject()) {
		qWarning() << "Invalid JSON document or not a JSON object";
		return;
	}

	QJsonValue jsonValue;
	if (jsonDoc.isObject()) {
		jsonValue = QJsonValue(jsonDoc.object());
	}
	else if (jsonDoc.isArray()) {
		jsonValue = QJsonValue(jsonDoc.array());
	}
	else {
		// 处理空文档或其他情况
		jsonValue = QJsonValue();
	}

	ParseResult(jsonValue);
}

void HttpNetActionBase::sendGetRequest(const QUrl& url, const QHash<QString, QString>& queryParams, const QHash<QString, QString>& headers)
{
	QUrl fullUrl = buildUrl(url.toString(), queryParams);
	qDebug() << fullUrl.toString();
	sendRequest(Method::GET, fullUrl, QByteArray(), headers);
}

void HttpNetActionBase::sendPostRequest(const QUrl& url, const QByteArray& data, const QHash<QString, QString>& headers)
{
	sendRequest(Method::POST, url, data, headers);
}

void HttpNetActionBase::setDefaultHeaders(QNetworkRequest & request)
{
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
}

void HttpNetActionBase::ParseError(const QString& errorString, int statusCode)
{
	qDebug() << errorString;
}

void HttpNetActionBase::setupRequest(QNetworkRequest& request, const QHash<QString, QString>& headers)
{
	setDefaultHeaders(request);

	for (auto it = headers.begin(); it != headers.end(); ++it)
	{
		request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
	}
}

QNetworkAccessManager* HttpNetActionBase::networkManager() const
{
	return m_manager;
}

QUrl HttpNetActionBase::buildUrl(const QString& baseUrl, const QHash<QString, QString>& queryParams)
{
	QUrl url(baseUrl);
	if (!queryParams.isEmpty())
	{
		QUrlQuery query;
		for (auto it = queryParams.begin(); it != queryParams.end(); ++it)
		{
			query.addQueryItem(it.key(), it.value());
		}
		url.setQuery(query);
	}
	return url;
}

void HttpNetActionBase::ParseResult(const QJsonValue& result)
{

}

void HttpNetActionBase::onFinished(QNetworkReply* reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if (reply->error() == QNetworkReply::NoError)
	{
		QByteArray responseData = reply->readAll();
		parseAndProcessResponse(responseData);
	}
	else
	{
		QString errorString = reply->errorString();
		ParseError(errorString, statusCode);
	}

}
