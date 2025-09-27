#pragma once
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QDebug>

class HttpNetActionBase  : public QObject
{
	Q_OBJECT

public:
    // 请求方法枚举
    enum class Method {
        GET,
        POST
    };

	explicit HttpNetActionBase(QObject *parent=nullptr);
	virtual ~HttpNetActionBase();

protected:
    virtual void sendGetRequest(const QUrl& url, const QHash<QString, QString>& queryParams, const QHash<QString, QString>& headers = {});
    virtual void sendPostRequest(const QUrl& url, const QByteArray& data, const QHash<QString, QString>& headers = {});
    virtual void ParseError(const QString& errorString, int statusCode);
    virtual void ParseResult(const QJsonValue& result);
    virtual void parseAndProcessResponse(const QByteArray& responseData);
    QNetworkAccessManager* networkManager() const;

public slots:
    void onFinished(QNetworkReply* reply);

private:
    void sendRequest(Method method, const QUrl& url, const QByteArray& data, const QHash<QString, QString>& headers);
    void setDefaultHeaders(QNetworkRequest& request);
    void setupRequest(QNetworkRequest& request, const QHash<QString, QString>& headers);
    QUrl buildUrl(const QString& baseUrl, const QHash<QString, QString>& queryParams = QHash<QString, QString>());

private:
	QNetworkAccessManager* m_manager;
    QNetworkReply* m_currentReply;
};
