#include "notify.h"
#include "ui_notify.h"

notify::notify(QWidget *parent, QString _message) :
    QDialog(parent),
    ui(new Ui::notify)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Text, fontColor);
    ui->message->setPalette(palette);
    ui->message->setText(_message);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    QFont font = ui->message->font();
    font.setPixelSize(fontSize-2);
    ui->message->setFont(font);

    //设置无边框和设置隐藏下部图标
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);

    if(parent != mainWindowWidget){
        parentWidget()->setWindowOpacity(0);
    }else{
        move(parent->x() - parent->width() / 2, parent->y());
    }
}

notify::~notify()
{
    if(parent() != mainWindowWidget){
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

void MessagesBox::warn(QWidget *parent, QString message){
    notify m(parent, WARNING_MESSAGE + message);
    m.exec();
}

void MessagesBox::error(QWidget *parent, QString message){
    notify m(parent, ERROR_MESSAGE + message);
    m.exec();
}
