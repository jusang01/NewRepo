#include "aPriori_01.h"
#include "./request/PostAccountHttpAction.h"

#pragma execution_character_set("utf-8")

aPriori_01::aPriori_01(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    init();
}

aPriori_01::~aPriori_01()
{}

void aPriori_01::init()
{
    this->setFixedSize(300, 150);
    connect(ui.btn_login_in, &QPushButton::clicked, this, [this]() {
        loginAccount();
    });
}

void aPriori_01::onShowMainWindow()
{
    m_window = new PrioriMainWindow;
    m_window->setAttribute(Qt::WA_DeleteOnClose);
    m_window->setWindowModality(Qt::ApplicationModal);
    m_window->show();
    this->close();
}

void aPriori_01::loginAccount()
{
    QString account = ui.lineEdit_account->text();
    QString password = ui.lineEdit_password->text();

    PostAccountHttpAction* postAccount = new PostAccountHttpAction(this);
    connect(postAccount, &PostAccountHttpAction::sigSuccess, this, &aPriori_01::onLoginSuccess);
    connect(postAccount, &PostAccountHttpAction::sigFailed, this, &aPriori_01::onLoginFailed);
    postAccount->accountsendPostRequest(account,password);
}

void aPriori_01::onLoginSuccess()
{
    onShowMainWindow();
}

void aPriori_01::onLoginFailed()
{
    ui.label_error->setText("µÇÂ¼Ê§°Ü");
}
