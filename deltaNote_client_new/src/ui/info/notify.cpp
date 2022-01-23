#include "notify.h"
#include "ui_notify.h"

notify::notify(QWidget *parent, QString _message, SettingCtrl *setting_ctrl, bool parent_main) :
    QDialog(parent),
    ui(new Ui::notify),
    m_setting_ctrl(setting_ctrl),
    m_parent_main(parent_main)
{
    ui->setupUi(this);
    m_desktop_rect = QGuiApplication::screens()[0]->availableGeometry();

    QPalette palette;
    palette.setColor(QPalette::Text, m_setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->message->setPalette(palette);
    ui->message->setText(_message);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    QFont font = ui->message->font();
    font.setPixelSize(m_setting_ctrl->get_int(SETTING_FONT_SIZE)-2);
    ui->message->setFont(font);

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif
    if(!m_parent_main){
        // 隐藏父窗口
        parentWidget()->setWindowOpacity(0);
    }
    // 将界面移到中心
    move(m_desktop_rect.width()/2 - this->width()/2, m_desktop_rect.height()/2 - this->height()/2);
}

notify::~notify()
{
    if(!m_parent_main){
        parentWidget()->setWindowOpacity(1);
    }
    delete ui;
}

void notify::on_OK_clicked()
{
    accept();
}

void notify::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - m_setting_ctrl->get_int(SETTING_TRAN_POS)));

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
    painter.setPen(m_setting_ctrl->get_color(SETTING_ICON_COLOR));
    QRect lineRect = this->rect();
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);
}

void notify::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPosition().x() - m_mouse_click_x, event->globalPosition().y() - m_mouse_click_y);
    }
}

void notify::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_mouse_click_x = event->position().x();
        m_mouse_click_y = event->position().y();
    }
}

void MessagesBox::warn(QWidget *parent, QString message, SettingCtrl *setting_ctrl, bool parent_main){
    notify m(parent, WARNING_MESSAGE + message, setting_ctrl, parent_main);
    m.exec();
}

void MessagesBox::error(QWidget *parent, QString message, SettingCtrl *setting_ctrl, bool parent_main){
    notify m(parent, ERROR_MESSAGE + message, setting_ctrl, parent_main);
    m.exec();
}

void MessagesBox::info(QWidget *parent, QString message, SettingCtrl *setting_ctrl, bool parent_main){
    notify m(parent, INFO_MESSAGE + message, setting_ctrl, parent_main);
    m.exec();
}
