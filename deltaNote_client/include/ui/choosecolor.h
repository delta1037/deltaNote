#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include <QDialog>
#include <QPaintEvent>
#include <QPainter>
#include <QBitmap>

#include "common/untils.h"

namespace Ui {
class chooseColor;
}

class chooseColor : public QDialog
{
    Q_OBJECT

public:
    explicit chooseColor(QWidget *parent = nullptr);
    ~chooseColor();
    QColor getCurColor();

private slots:
    void on_color_1_1_clicked();

    void on_color_1_2_clicked();

    void on_color_1_3_clicked();

    void on_color_2_1_clicked();

    void on_color_2_2_clicked();

    void on_color_2_3_clicked();

    void on_color_1_4_clicked();

    void on_color_2_4_clicked();

private:
    Ui::chooseColor *ui;
    QColor choosedColor;

    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // CHOOSECOLOR_H
