#ifndef GROUPWIDGET_H
#define GROUPWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class GroupWidget;
}

class GroupWidget : public QWidget
{
    Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent*);

private:
    Ui::GroupWidget* ui;
    int id = -1;

public:
    explicit GroupWidget(QWidget *parent = nullptr);
    ~GroupWidget();

    void initData(int groupid, std::string groupname);
    int getId() { return id; };

signals:
    void groupPressed(GroupWidget* groupWidget);
};

#endif // GROUPWIDGET_H
