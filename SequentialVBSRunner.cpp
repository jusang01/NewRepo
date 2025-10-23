#include "SequentialVBSRunner.h"

SequentialVBSRunner::SequentialVBSRunner(QObject *parent)
	: QObject(parent)
{}

SequentialVBSRunner::~SequentialVBSRunner()
{}

void SequentialVBSRunner::addTask(const QString & scriptPath, const QStringList & arguments)
{
	m_tasks.append(qMakePair(scriptPath, arguments));
}

void SequentialVBSRunner::startExecution()
{
	if(m_tasks.isEmpty())
		emit allTasksFinished();
	else
	{
		m_currentIndex = 0;
		executeCurrentTask();
	}
}

void SequentialVBSRunner::executeCurrentTask()
{
	if (m_currentIndex >= m_tasks.size()) return;
	auto task = m_tasks[m_currentIndex];
	QString scriptPath = task.first;
	QStringList arguments = task.second;

	QProcess* process = new QProcess(this);
	connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &SequentialVBSRunner::onProcessFinished);
	
	QStringList args;
	args << "//nologo" << scriptPath;
	args << arguments;
	qDebug() << "执行任务" << m_currentIndex << ":" << scriptPath << "参数:" << arguments;
	process->start("cscript", args);

}

void SequentialVBSRunner::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	QProcess* process = qobject_cast<QProcess*>(sender());
	if (!process) return;

	//获取输出结果
	QByteArray output = process->readAllStandardOutput();
	QByteArray error = process->readAllStandardError();
	QString outputStr = QString::fromLocal8Bit(output);

	bool success = (exitStatus == QProcess::NormalExit && exitCode == 0);
	emit taskFinished(m_currentIndex, outputStr, success);
	emit progressChanged(m_currentIndex + 1, m_tasks.size());

	process->deleteLater();

	//执行下一个任务
	m_currentIndex++;
	if (m_currentIndex < m_tasks.size()) {
		executeCurrentTask();
	}
	else {
		emit allTasksFinished();
	}
}

