#include "groupwidget.h"
#include "ui_groupwidget.h"
#include "QFile"
#include "QFontDatabase"
#include <iostream>

void GroupWidget::mousePressEvent(QMouseEvent*)
{
    emit groupPressed(this);
}

GroupWidget::GroupWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/img/fonts/ubuntu.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont monospace(family, QFont::Bold, true);

    ui->groupName->setFont(monospace);
    ui->groupDescription->setFont(monospace);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}

void GroupWidget::initData(int groupid, std::string groupname)
{
    id = groupid;
    ui->groupName->setText(QString::fromStdString(groupname));
}
