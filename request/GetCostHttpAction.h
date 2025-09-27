#pragma once

#include "HttpNetActionBase.h"

class GetCostHttpAction  : public HttpNetActionBase
{
	Q_OBJECT

public:
	GetCostHttpAction(QObject *parent);
	~GetCostHttpAction();

	void costSendPostRequest(int type,int number);
	
protected:
	void parseAndProcessResponse(const QByteArray& responseData) override;
	void ParseError(const QString& errorString, int statusCode);

signals:
	void sigSuccess(const QString& text);
	void sigFailed();

};
