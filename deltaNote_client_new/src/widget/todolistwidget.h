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

public:


signals:
    void addTodoSignal();
    void todoListCursorLoseSignal();

private:
    void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);
};

#endif // TODOLISTWINWIDGET_H
