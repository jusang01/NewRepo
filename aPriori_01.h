#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_aPriori_01.h"
#include "priorimainwindow.h"

class aPriori_01 : public QMainWindow
{
    Q_OBJECT

public:
    aPriori_01(QWidget *parent = nullptr);
    ~aPriori_01();

    void init();
    //µÇÂ¼ÕËºÅ
    void loginAccount();

public slots:
    void onShowMainWindow();
    void onLoginSuccess();
    void onLoginFailed();

private:
    Ui::aPriori_01Class ui;
    PrioriMainWindow* m_window;
};
