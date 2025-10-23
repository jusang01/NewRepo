#ifndef PRIORIMAINWINDOW_H
#define PRIORIMAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QTabWidget>
#include <QVector>

class ProjectTab;

namespace Ui {
class PrioriMainWindow;
}

class PrioriMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrioriMainWindow(QWidget *parent = nullptr);
    ~PrioriMainWindow();
    
    void initConnection();
    void initUIAfterLoadFile(const QString& filename);
    void addDuplicatePageFromUiFile(QTabWidget* tabWidget, const QString& filename="test");
    void on_tabWidget_tabCloseRequested(int index);
    //请求后表单保存
    void saveDocToFile(const QString& text);
    //调用脚本去计算成本
    void commandCalculatorOfCost(const QString& text);
    //将表单进行显示
    void showCostTable(const QString& text);

    void getModolDataFromFile(const QString& filename);

public slots:
    void onOpenExistingFile();
    void onSaveProject();
    void onTestCostRequest();
    void onDataSuccess(const QString& text);
    void onDataFailed();

private:
    Ui::PrioriMainWindow *ui;
	QVector<ProjectTab*> m_tabs;
};

#endif // PRIORIMAINWINDOW_H
