#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

#include "messagewidget.h"
#include "groupwidget.h"

#include <sqlite3.h>
#include <spdlog/spdlog.h>

#include <QThread>

#include "settingsform.h"

namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
        
public:
    explicit MainWindow(QWidget *parent = nullptr);
    SettingsForm* settingsForm;
    ~MainWindow();

private:
    QString currentName = "NoName";
    Ui::MainWindow *ui;
    QTcpSocket* socket = nullptr;
    QByteArray Data;
    void setPixmapToIcons();

    int currentDialogid = -1;

    std::vector<GroupWidget*> groupWidgets;
    void clearLayout(QLayout* layout, bool deleteWidgets);

private slots:
    void on_buttonSendMes_clicked();
    void on_buttonSettings_clicked();

public slots:
    void SetUsername(QString username);
    void ShowMessage(QString username, quint16 groupid, const QString& message);
    void ShowGroup(int groupid, std::string groupname);
    void OnClickGroup(GroupWidget* groupWidget);
    void onReceiveMessages(std::string sender, std::string message);

signals:
    void messageSended(QString currentname, int groupid, const QString str);
    void groupSelected(int groupid);
};

#endif
