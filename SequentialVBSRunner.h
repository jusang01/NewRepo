#pragma once

#include <QObject>
#include <QProcess>
#include <QList>
#include <QDebug>

class SequentialVBSRunner  : public QObject
{
	Q_OBJECT

public:
	explicit SequentialVBSRunner(QObject *parent = nullptr);
	~SequentialVBSRunner();

	//脚本路径和传递给脚本的参数
	void addTask(const QString& scriptPath, const QStringList& arguments = QStringList());

	//开始任务
	void startExecution();

private:
	void executeCurrentTask();

signals:
	void allTasksFinished();
	void taskFinished(int index, const QByteArray& output, bool success);
	void progressChanged(int current, int total);

private slots:
	void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
	int m_currentIndex = 0;  //当前任务号
	QList<QPair<QString, QStringList>> m_tasks;
};

