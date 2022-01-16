#include "ui/choosecolor.h"
#include "ui_choosecolor.h"

chooseColor::chooseColor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::chooseColor)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif
}

chooseColor::~chooseColor()
{
    delete ui;
}

void chooseColor::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - transparentPos));

    // 设置主界面圆角蒙版
    QBitmap bmp(rect().size());
    bmp.fill();
    QPainter painter_bmp(&bmp);
    painter_bmp.setRenderHint(QPainter::Antialiasing, true);
    painter_bmp.setPen(Qt::NoPen);
    painter_bmp.setBrush(Qt::black);
    painter_bmp.drawRoundedRect(rect(), 10, 10, Qt::AbsoluteSize);
    setMask(bmp);

    // 画边界的框
    painter.setPen(iconColor);
    QRect lineRect = this->rect();
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);
}


void chooseColor::on_color_1_1_clicked()
{
    // 红
    choosedColor = QColor(239, 41, 41);
    accept();
}

void chooseColor::on_color_1_2_clicked()
{
    // 橙
    choosedColor = QColor(252, 175, 62);
    accept();
}

void chooseColor::on_color_1_3_clicked()
{
    // 黄
    choosedColor = QColor(252, 233, 79);
    accept();
}

void chooseColor::on_color_2_1_clicked()
{
    // 绿
    choosedColor = QColor(188, 250, 109);
    accept();
}


void chooseColor::on_color_2_2_clicked()
{
    // 蓝
    choosedColor = QColor(114, 159, 207);
    accept();
}

void chooseColor::on_color_2_3_clicked()
{
    // 紫
    choosedColor = QColor(173, 127, 168);
    accept();
}


void chooseColor::on_color_1_4_clicked()
{
    // 白
    choosedColor = QColor(255, 255, 255);
    accept();
}

void chooseColor::on_color_2_4_clicked()
{
    // 黑
    choosedColor = QColor(0, 0, 0);
    accept();
}

QColor chooseColor::getCurColor(){
    return choosedColor;
}


void chooseColor::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalX()-m_nMouseClick_X_Coordinate, event->globalY()-m_nMouseClick_Y_Coordinate);
    }
}

void chooseColor::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_nMouseClick_X_Coordinate = event->x();
        m_nMouseClick_Y_Coordinate = event->y();
    }
}
