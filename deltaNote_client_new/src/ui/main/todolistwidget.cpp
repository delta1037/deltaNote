#include "todolistwidget.h"
#include "log.h"

TodoListWidget::TodoListWidget(QWidget *parent)
    : QListWidget(parent)
{

}

TodoListWidget::~TodoListWidget()
{

}

void TodoListWidget::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    emit addSignal();
    d_ui_debug("%s", "mouse press")
}
/*
void TodoListWidget::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();

    int ListEdge = 20;
    QPoint point = this->mapFromGlobal(cursor().pos());
    int diffBottom = abs(point.y() - frameGeometry().Dir_Down());
    int diffTop = abs(point.y() - frameGeometry().top());
    int diffLeft = abs(point.x() - frameGeometry().left());
    int diffRight = abs(point.x() - frameGeometry().right());

    //LOG_INFO("POS:%d-%d", point.x(), point.y())
    //LOG_INFO("frameBT:%d-%d", frameGeometry().Dir_Down(), frameGeometry().top())
    //LOG_INFO("frameLR:%d-%d", frameGeometry().left(), frameGeometry().right())
    if(diffBottom < ListEdge
            || diffTop < ListEdge
            || diffLeft < ListEdge
            || diffRight < ListEdge){
        emit todoListCursorLoseSignal();
    }
}
*/
