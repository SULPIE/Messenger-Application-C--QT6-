#include "messagewidget.h"
#include "ui_messagewidget.h"
#include "QFile"
#include "QFontDatabase"

MessageWidget::MessageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);

    int id = QFontDatabase::addApplicationFont(":/img/fonts/ubuntu.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont monospace(family, QFont::Bold, true);

    ui->name->setFont(monospace);
    ui->textEdit->setFont(monospace);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

void MessageWidget::SetMessageText(QString message)
{
    ui->textEdit->setPlainText(message);
}
void MessageWidget::SetUsername(QString name)
{
    ui->name->setText(name);
}

void MessageWidget::resizeEvent(QResizeEvent* event)
{
    setBoxsizeToTextsize();
}

void MessageWidget::showEvent(QShowEvent* showEvent)
{     
    setBoxsizeToTextsize();
}

void MessageWidget::setBoxsizeToTextsize()
{
    QSize size = ui->textEdit->document()->size().toSize();
    ui->textEdit->setFixedHeight(size.height()+10);
}
