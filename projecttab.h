#ifndef PROJECTTAB_H
#define PROJECTTAB_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>

namespace Ui {
class ProjectTab;
}

class ProjectTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectTab(QWidget *parent = nullptr);
    ~ProjectTab();

    void initConnection();
    void initUI();

    //展示表格删选数据
	void showDataBaseTable(const QString& filename);
    //csv转换为xlsx格式
    bool convertCsvToXlsx(const QString& csvPath, const QString& xlsxPath);

	//从文件中获取模型数据
	void getModelDataFromFile(const QString& filename="./output.txt");

public slots:
    void onSetButtonStyleSheet(); 
    void onShowList();
    void onCloseCostWidget();

private:
    Ui::ProjectTab *ui;
    QPushButton* prev_btn = nullptr;
};

#endif // PROJECTTAB_H
