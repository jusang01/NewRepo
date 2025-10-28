#include "projecttab.h"
#include "ui_projecttab.h"
#include <QDialog>
#include <QFile>
#include "xlsxcellrange.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"

#pragma execution_character_set("utf-8")

ProjectTab::ProjectTab(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProjectTab)
{
    ui->setupUi(this);
    initConnection();
    initUI();
}

ProjectTab::~ProjectTab()
{
    delete ui;
}

void ProjectTab::initConnection()
{
    connect(ui->btn_close_cost, &QPushButton::clicked, this, &ProjectTab::onCloseCostWidget);
    connect(ui->btn_manufacture, &QPushButton::clicked, this, &ProjectTab::onSetButtonStyleSheet);
    connect(ui->btn_GCD, &QPushButton::clicked, this, &ProjectTab::onSetButtonStyleSheet);
    connect(ui->btn_art, &QPushButton::clicked, this, &ProjectTab::onSetButtonStyleSheet);
    connect(ui->btn_list, &QPushButton::clicked, this, &ProjectTab::onShowList);
}

void ProjectTab::initUI()
{
    prev_btn = ui->btn_manufacture;  //初始化为第一项
    prev_btn->setStyleSheet("QPushButton{background-color:#FF4500;border:1px solid white;}");

    //成本总结初始化 测试
    ui->tableWidget_cost->setRowCount(2);
    ui->tableWidget_cost->setColumnCount(7);
    ui->tableWidget_cost->verticalHeader()->setVisible(false);
    ui->tableWidget_cost->horizontalHeader()->setVisible(false);
    ui->tableWidget_cost->setUpdatesEnabled(false);

    for (int row = 1; row <= 2; row++)
    {
        for (int col = 1; col <= 7; col++)
        {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(u8"测试");
            ui->tableWidget_cost->setItem(row - 1, col - 1, item);
        }
    }

    ui->tableWidget_detail->setRowCount(7);
    ui->tableWidget_detail->setColumnCount(7);
    ui->tableWidget_detail->verticalHeader()->setVisible(false);
    ui->tableWidget_detail->horizontalHeader()->setVisible(false);
    ui->tableWidget_detail->setUpdatesEnabled(false);

    for (int row = 1; row <= 7; row++)
    {
        for (int col = 1; col <= 7; col++)
        {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setText(u8"测试");
            ui->tableWidget_detail->setItem(row - 1, col - 1, item);
        }
    }

    ui->tableWidget_cost->setUpdatesEnabled(true);
    ui->tableWidget_detail->setUpdatesEnabled(true);

    QPixmap pixmap(":/aPriori_01/icon/成本.png");

    ui->label_cost->setPixmap(pixmap);
    ui->label_cost->setScaledContents(true);

}

void ProjectTab::showDataBaseTable(const QString& filename)
{
    
}

bool ProjectTab::convertCsvToXlsx(const QString& csvPath, const QString& xlsxPath)
{
    QFile csvFile(csvPath);
    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开 CSV 文件:" << csvPath;
        return false;
    }
    QXlsx::Document xlsx;
    QTextStream in(&csvFile);
    int row = 1;

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');

        for (int col = 0; col < fields.size(); ++col) {
            // 移除可能的引号
            QString value = fields[col];
            if (value.startsWith('"') && value.endsWith('"')) {
                value = value.mid(1, value.length() - 2);
            }
            xlsx.write(row, col + 1, value);
        }
        row++;
    }

    csvFile.close();
    return xlsx.saveAs(xlsxPath);

}

void ProjectTab::getModelDataFromFile(const QString& filename)
{
	QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "无法打开文件：" << filename;
        return;
	}

    QByteArray text = file.readAll();
    QString output = QString::fromLocal8Bit(text);
    QStringList info = output.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
    for (const QString& line : info) {
        if (line.contains("材料"))
        {
			QString material = line.section(':', 1).trimmed();
            ui->lineEdit_2->setText(material);  //材料
        }

    }
}

void ProjectTab::onSetButtonStyleSheet()
{
    QPushButton* btn_tmp = qobject_cast<QPushButton*>(QObject::sender());
    qDebug() << btn_tmp->objectName();
    if (prev_btn == btn_tmp)
        return;

    prev_btn->setStyleSheet("QPushButton{background-color:#FFFACD;border:1px solid white;}");
    prev_btn = btn_tmp;
    btn_tmp->setStyleSheet("QPushButton{background-color:#FF4500;border:1px solid white;}");
    if (btn_tmp == ui->btn_manufacture)
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else if (btn_tmp == ui->btn_GCD)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (btn_tmp == ui->btn_art)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
}


void ProjectTab::onShowList()
{
	//显示成本总结表单
    //待完成，根据不同的页面进行成本计算结果筛选

	QDialog* dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setWindowModality(Qt::ApplicationModal);
    dlg->setWindowTitle("成本总结");
    dlg->resize(800, 600);
    dlg->setStyleSheet("background-color: white;");
	QVBoxLayout* layout = new QVBoxLayout(dlg);

    QString csvPath = QApplication::applicationDirPath() + "/calculated_result.csv";
    QString xlsxPath = QApplication::applicationDirPath() + "/converted_result.xlsx";

    if (!convertCsvToXlsx(csvPath, xlsxPath)) {
        return;
    }

    

    QXlsx::Document xlsx(xlsxPath);
    int rowCount = xlsx.dimension().lastRow() - xlsx.dimension().firstRow() + 1;
    int colCount = xlsx.dimension().lastColumn() - xlsx.dimension().firstColumn() + 1;

    QTableWidget* table = new QTableWidget(dlg);
	table->setRowCount(rowCount);
	table->setColumnCount(colCount);
	layout->addWidget(table);
    table->setUpdatesEnabled(false);

    //添加数据
    for(int row = 1; row <= rowCount; row++)
    {
        for(int col = 1; col <= colCount; col++)
        {
            QTableWidgetItem* item = new QTableWidgetItem();

            QVariant cellvalue = xlsx.read(row, col);
            if (cellvalue.type() == QVariant::Double)
            {
                item->setData(Qt::DisplayRole, cellvalue);
            }
            else if (cellvalue.type() == QVariant::String)
            {
                item->setText(cellvalue.toString());
            }
            table->setItem(row - 1, col - 1, item);
        }
	}

    table->setUpdatesEnabled(true);
    dlg->show();
}

void ProjectTab::onCloseCostWidget()
{
    ui->cost_widget->close();
}

