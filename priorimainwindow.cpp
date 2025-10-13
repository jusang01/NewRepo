#include "priorimainwindow.h"
#include "ui_priorimainwindow.h"
#include "projecttab.h"
#include "./request/GetCostHttpAction.h"
#include <QDebug>
#include <QTableWidget>
#include <QFile>
#include <QMessageBox>
#include "xlsxcellrange.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"

#pragma execution_character_set("utf-8")

PrioriMainWindow::PrioriMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PrioriMainWindow)
{
    ui->setupUi(this);
    initConnection();

    ui->tabWidget_all->clear();
    ui->tabWidget_all->setTabsClosable(true);
    connect(ui->tabWidget_all, &QTabWidget::tabCloseRequested,this, &PrioriMainWindow::on_tabWidget_tabCloseRequested);
}

PrioriMainWindow::~PrioriMainWindow()
{
    delete ui;
}

void PrioriMainWindow::initConnection()
{
    connect(ui->open_action, &QAction::triggered, this, &PrioriMainWindow::onOpenExistingFile);
    connect(ui->btn_save, &QPushButton::clicked, this, &PrioriMainWindow::onSaveProject);
    connect(ui->cost_action, &QAction::triggered, this, &PrioriMainWindow::onTestCostRequest);
}

void PrioriMainWindow::initUIAfterLoadFile(const QString& filename)
{
    addDuplicatePageFromUiFile(ui->tabWidget_all, filename);
}

//加载新项目时复制UI，添加响应
void PrioriMainWindow::addDuplicatePageFromUiFile(QTabWidget* tabWidget,const QString& filename)
{
    //QUiLoader loader;
    //QFile uiFile("F:/qt/qt_test/aPriori_01/tabPage.ui");
    //if (!uiFile.open(QIODevice::ReadOnly)) {
    //    qDebug() << "Cannot open UI file:" << uiFile.errorString();
    //    return;
    //}
    //QWidget* newPage = loader.load(&uiFile, tabWidget);

    ProjectTab* tabtmp = new ProjectTab(tabWidget);
	m_tabs.push_back(tabtmp);
    tabtmp->setAttribute(Qt::WA_DeleteOnClose);
    int cur = tabWidget->addTab(tabtmp,filename);
    tabWidget->setCurrentWidget(tabtmp);

    // 设置选项卡样式
    tabWidget->setStyleSheet(R"(
        QTabBar::tab:selected {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                        stop: 0 #B39DDB, stop: 1 #90CAF9);
        }
        QTabBar::tab:!selected {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                        stop: 0 #F5F5F5, stop: 1 #EEEEEE);
        }
    )");
}

void PrioriMainWindow::on_tabWidget_tabCloseRequested(int index)
{
    if (index < 0)
        return;
    auto aa = QMessageBox::question(this, "tips", "window will be closed", QMessageBox::Yes, QMessageBox::No);
    if (aa == QMessageBox::No)
    {
        return;
    }
    QWidget* aForm = ui->tabWidget_all->widget(index);
    aForm->close();
}

void PrioriMainWindow::saveDocToFile(const QString& text)
{
    QFile file("长桁.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QString correctedText = text;
        correctedText.replace("0", "NaN");
        qDebug() << correctedText;

        //QJsonDocument doc = QJsonDocument::fromJson(correctedText.toUtf8());
        QTextStream out(&file);
        out.setCodec("UTF-8");
        //out <<doc.toJson(QJsonDocument::Indented);
        out << correctedText;
        file.close();
        qDebug() << "文件保存成功";

        commandCalculatorOfCost(text);
    }
    else
        qDebug() << "文件保存失败：" << file.errorString();

}

void PrioriMainWindow::commandCalculatorOfCost(const QString& text)
{
    QString path = QApplication::applicationDirPath();
    QString python_path = path + "/venv/Scripts/python.exe";
    QString command = QString("\"%1\" ./main3.py").arg(python_path);
    int result = system(command.toUtf8().constData());
    if (result == 0) {
        qDebug()<< "Python 脚本执行成功！";
    }
    else {
        qDebug() << "Python 脚本执行失败！";
    }
}

void PrioriMainWindow::showCostTable(const QString& text)
{
    QXlsx::Document xlsx("calculated_result.csv");

    int rowCount = xlsx.dimension().lastRow()- xlsx.dimension().firstRow()+1;
	int colCount = xlsx.dimension().lastColumn()-xlsx.dimension().firstColumn()+1;






}

void PrioriMainWindow::onOpenExistingFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, u8"打开文件", "./", "All Files (*)");
    if (fileName.isEmpty()) 
        return;

    //ui初始化
   initUIAfterLoadFile(fileName);
}

void PrioriMainWindow::onSaveProject()
{
    QString fileName = QFileDialog::getSaveFileName(this, u8"保存文件", "./", u8"文本文件 (*.txt);;所有文件 (*)");
    if (fileName.isEmpty()) {
        qDebug() << u8"用户取消了保存操作";
        return;
    }
    if (!fileName.endsWith(".txt", Qt::CaseInsensitive)) {
        fileName += ".txt";
    }
}

void PrioriMainWindow::onTestCostRequest()
{
	//测试表单请求
    int type = 1;
    int number = 0;
    GetCostHttpAction* costhttp = new GetCostHttpAction(this);
    connect(costhttp, &GetCostHttpAction::sigSuccess, this, &PrioriMainWindow::onDataSuccess);
    connect(costhttp, &GetCostHttpAction::sigFailed, this, &PrioriMainWindow::onDataFailed);
    costhttp->costSendPostRequest(type,number);
}

void PrioriMainWindow::onDataSuccess(const QString& text)
{
   saveDocToFile(text);
}

void PrioriMainWindow::onDataFailed()
{
}
