#pragma once

#include "HttpNetActionBase.h"
#include <QDebug>

class PostAccountHttpAction  : public HttpNetActionBase
{
	Q_OBJECT

public:
	PostAccountHttpAction(QObject *parent);
	~PostAccountHttpAction();
	void accountsendPostRequest(QString account,QString password);
	void accountLoginOutPostRequest(QString account, QString password);

protected:
	void ParseError(const QString& errorString, int statusCode);
	void ParseResult(const QJsonValue& result);

signals:
	void sigSuccess();
	void sigFailed();


};
