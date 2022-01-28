#ifndef TODO_DETAIL_H
#define TODO_DETAIL_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QString>

#include "inter_var.h"
#include "setting_ctrl.h"

namespace Ui {
class TodoDetail;
}

enum AltType{
    Alt_nul = 0,
    Alt_add = 1,
    Alt_alt = 2,
    Alt_chk = 3,
};

enum ItemStatus{
    Item_old = 0,
    Item_new = 1,
};

class TodoDetail : public QDialog
{
    Q_OBJECT

public:
    explicit TodoDetail(QWidget *parent = nullptr, SettingCtrl *setting_ctrl = nullptr);
    ~TodoDetail();

    void set_item_data(const TodoItem &data, ItemStatus status=Item_old);
    TodoItem get_item_data();
private:
    void render_text(ItemStatus status);
    static QString tag_to_string(TagType tag_type);
private slots:

    void on_cancel_clicked();

    void on_accept_clicked();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::TodoDetail *ui;

    // 数据
    TodoItem item_data;
    // 配置
    SettingCtrl *setting_ctrl;
    // 环境
    QRect    m_desktop_rect;

    QString init_time;

    int m_mouse_click_x;
    int m_mouse_click_y;
};

#endif // TODO_DETAIL_H
