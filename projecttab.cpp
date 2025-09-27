#include "projecttab.h"
#include "ui_projecttab.h"

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

}

void ProjectTab::onCloseCostWidget()
{
    ui->cost_widget->close();
}

