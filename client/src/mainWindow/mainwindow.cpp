#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <qresource.h>
#include <string>
#include <QGraphicsDropShadowEffect>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	setPixmapToIcons();

	ui->layoutLeftMenu->setAlignment({ Qt::AlignLeft });
	ui->layoutHeader->setAlignment({ Qt::AlignLeft });
	ui->layoutParams->setAlignment(Qt::AlignBottom);

	sqlite3* db = 0; 
	char* err = 0;

	const char* SQL = "CREATE TABLE IF NOT EXISTS foo(a,b,c); INSERT INTO FOO VALUES(1,2,3); INSERT INTO FOO SELECT * FROM FOO;";

	if (sqlite3_open("test.test", &db))
		fprintf(stderr, "Ошибка открытия/создания БД: %s\n", sqlite3_errmsg(db));
	else if (sqlite3_exec(db, SQL, 0, 0, &err))
	{
		fprintf(stderr, "Ошибка SQL: %sn", err);
		sqlite3_free(err);
	}
	sqlite3_close(db);

	settingsForm = new SettingsForm;
	connect(settingsForm, &SettingsForm::nameSaved, this, &MainWindow::SetUsername);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSendMes_clicked()
{
	try
	{
		emit messageSended(currentName, currentDialogid, ui->lineEdit->text());
	}
	catch (const std::exception& ex)
	{
		spdlog::error(ex.what());
	}
}
 
void MainWindow::setPixmapToIcons()
{
	std::map<std::string, QPushButton*> mapIcons {
		{std::string(":/img/search_icon.png"), ui->srch_icon},
		{std::string(":/img/settings_img.png"), ui->buttonSettings},
		{std::string(":/img/send_img.png"), ui->buttonSendMes},
		{std::string(":/img/main_icon.png"), ui->buttonConnect}
	};

	for (auto it = mapIcons.begin(); it != mapIcons.end(); it++) {
		QPixmap pix(it->first.c_str());
		it->second->setIcon(pix);
	}
}

void MainWindow::ShowGroup(int groupid, std::string groupname)
{
	GroupWidget* groupWidget = new GroupWidget(this);
	groupWidget->initData(groupid, groupname);
	ui->groupLayout->setAlignment({ Qt::AlignLeft, Qt::AlignTop });
	ui->groupLayout->addWidget(groupWidget);

	groupWidgets.push_back(groupWidget);

	connect(groupWidget, &GroupWidget::groupPressed, this, &MainWindow::OnClickGroup);
}

void MainWindow::OnClickGroup(GroupWidget* groupWidget)
{
	clearLayout(ui->layoutContent->layout(), true);

	for (auto it = groupWidgets.begin(); it != groupWidgets.end(); it++)
	{
		(*it)->setStyleSheet("background-color: #FFFFFF");
	}
	groupWidget->setStyleSheet("background-color: #ffb8c6");
	currentDialogid = groupWidget->getId();
	emit groupSelected(currentDialogid);
}

void MainWindow::onReceiveMessages(std::string sender, std::string message)
{
	QString buttonText = tr("%1").arg(QString::fromStdString(message));
	MessageWidget* messageWidget = new MessageWidget(this);
	messageWidget->SetMessageText(buttonText);
	messageWidget->SetUsername(QString::fromStdString(sender));
	ui->layoutContent->setAlignment({ Qt::AlignLeft, Qt::AlignTop });
	ui->layoutContent->addWidget(messageWidget);
}

void MainWindow::ShowMessage(QString username, quint16 groupid, const QString& message)
{
	if (groupid == currentDialogid)
	{
		QString buttonText = tr("%1").arg(message);
		MessageWidget* messageWidget = new MessageWidget(this);
		messageWidget->SetMessageText(buttonText);
		messageWidget->SetUsername(username);
		ui->layoutContent->setAlignment({ Qt::AlignLeft, Qt::AlignTop });
		ui->layoutContent->addWidget(messageWidget);
	}
}

void MainWindow::on_buttonSettings_clicked()
{
	settingsForm->show();
}

void MainWindow::SetUsername(QString username)
{
	currentName = username;
}

void MainWindow::clearLayout(QLayout* layout, bool deleteWidgets = true)
{
	while (QLayoutItem* item = layout->takeAt(0))
	{
		if (deleteWidgets)
		{
			if (QWidget* widget = item->widget())
				widget->deleteLater();
		}
		if (QLayout* childLayout = item->layout())
			clearLayout(childLayout, deleteWidgets);
		delete item;
	}
}