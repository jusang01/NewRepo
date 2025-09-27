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

public slots:
    void onSetButtonStyleSheet(); 
    void onShowList();
    void onCloseCostWidget();

private:
    Ui::ProjectTab *ui;
    QPushButton* prev_btn = nullptr;
};

#endif // PROJECTTAB_H
