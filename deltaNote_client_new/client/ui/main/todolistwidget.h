#ifndef TODOLISTWINWIDGET_H
#define TODOLISTWINWIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QCursor>

class TodoListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit TodoListWidget(QWidget *parent = nullptr);
    ~TodoListWidget();

signals:
    void addSignal();
    void todoListCursorLoseSignal();

private:
    void mousePressEvent(QMouseEvent *event);
};

#endif // TODOLISTWINWIDGET_H
