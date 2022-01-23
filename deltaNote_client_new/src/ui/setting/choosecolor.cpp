#include "choosecolor.h"
#include "ui_choosecolor.h"

chooseColor::chooseColor(QWidget *parent, SettingCtrl *setting_ctrl) :
    QDialog(parent),
    ui(new Ui::chooseColor),
    setting_ctrl(setting_ctrl) {
    ui->setupUi(this);

    m_desktop_rect = QGuiApplication::screens()[0]->availableGeometry();

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

chooseColor::~chooseColor() {
    delete ui;
}

void chooseColor::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - setting_ctrl->get_int(SETTING_TRAN_POS)));

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
    painter.setPen(setting_ctrl->get_color(SETTING_ICON_COLOR));
    QRect lineRect = this->rect();
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);
}


void chooseColor::on_color_1_1_clicked()
{
    // 红
    choosed_color = QColor(239, 41, 41);
    accept();
}

void chooseColor::on_color_1_2_clicked()
{
    // 橙
    choosed_color = QColor(252, 175, 62);
    accept();
}

void chooseColor::on_color_1_3_clicked()
{
    // 黄
    choosed_color = QColor(252, 233, 79);
    accept();
}

void chooseColor::on_color_2_1_clicked()
{
    // 绿
    choosed_color = QColor(188, 250, 109);
    accept();
}


void chooseColor::on_color_2_2_clicked()
{
    // 蓝
    choosed_color = QColor(114, 159, 207);
    accept();
}

void chooseColor::on_color_2_3_clicked()
{
    // 紫
    choosed_color = QColor(173, 127, 168);
    accept();
}


void chooseColor::on_color_1_4_clicked()
{
    // 白
    choosed_color = QColor(255, 255, 255);
    accept();
}

void chooseColor::on_color_2_4_clicked()
{
    // 黑
    choosed_color = QColor(0, 0, 0);
    accept();
}

QColor chooseColor::get_color(){
    return choosed_color;
}


void chooseColor::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPosition().x() - m_mouse_click_x, event->globalPosition().y() - m_mouse_click_y);
    }
}

void chooseColor::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_mouse_click_x = event->position().x();
        m_mouse_click_y = event->position().y();
    }
}
