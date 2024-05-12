#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MessageWidget(QWidget *parent = nullptr);
    ~MessageWidget();
    void SetMessageText(QString Message);
    void SetUsername(QString name);

    virtual void showEvent(QShowEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    Ui::MessageWidget *ui;

    void setBoxsizeToTextsize();
};

#endif // MESSAGEWIDGET_H
