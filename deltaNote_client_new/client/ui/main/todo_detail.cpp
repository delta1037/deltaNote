#include <QListView>
#include "todo_detail.h"
#include "ui_todo_detail.h"
#include "log.h"
#include "svg_color.h"

TodoDetail::TodoDetail(QWidget *parent, SettingCtrl *setting_ctrl) :
    QDialog(parent),
    ui(new Ui::TodoDetail),
    setting_ctrl(setting_ctrl) {
    ui->setupUi(this);

    // 初始化时生成指定颜色icon，这里icon只需要在显示的时候生成一次
    QColor t_icon_color = setting_ctrl->get_color(SETTING_ICON_COLOR);
    SvgColor svg_c_b(":/resource/clock.svg");
    ui->c_b->setIcon(svg_c_b.setColor(t_icon_color));

    SvgColor svg_i_b(":/resource/important.svg");
    ui->i_b->setIcon(svg_i_b.setColor(t_icon_color));

    // 设置内容信息
    QPalette palette;
    palette.setColor(QPalette::Text, setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->tag->setPalette(palette);
    ui->ddl->setPalette(palette);
    palette.setColor(QPalette::PlaceholderText, setting_ctrl->get_color(SETTING_FONT_COLOR));
    ui->text_line->setPalette(palette);

    // DDL 设置为现在的时间（相当于没有设置）
    ui->ddl->setDateTime(QDateTime::currentDateTime());
    ui->ddl->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
    ui->ddl->setSelectedSection(QDateTimeEdit::DaySection);
    // 保存当前时间字符串作为对比，如果编辑结束还是当前时间，就认为该项未编辑，返回空内容
    init_time = ui->ddl->dateTime().toString(REMINDER_FORMAT);

    // 标签设置
    ui->tag->addItems({"低", "中", "高"});
    ui->tag->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
    // List View
    auto *tag_list = new QListView(ui->tag);
    tag_list->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
    tag_list->setSpacing(setting_ctrl->get_int(SETTING_FONT_SIZE)/4);
    tag_list->setStyleSheet("background-color:#F0F8FF;selection-background-color: #FFB7DD;border-radius: 0px;");
    ui->tag->setView(tag_list);

    // 设置透明
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);

    ui->label->setVisible(false);
    ui->radioButton->setVisible(false);

    QString styleSheet = "QPushButton:pressed{\n	  background-color:rgba(68, 91, 237, 10);\n}";
    ui->exit->setStyleSheet(styleSheet);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif
    // 获取界面信息
    m_desktop_rect = QGuiApplication::screens()[0]->availableGeometry();
    // 将界面移到中心
    d_ui_debug("login desktop w:%d h:%d", m_desktop_rect.width(), m_desktop_rect.height())
    move(m_desktop_rect.width()/2 - this->width()/2, m_desktop_rect.height()/2 - this->height()/2);
}

TodoDetail::~TodoDetail() {
    delete ui;
}

void TodoDetail::set_item_data(const TodoItem &todo_item){
    this->item_data = todo_item;
    // 刷新该条目
    render_text();
}

TodoItem TodoDetail::get_item_data(){
    return this->item_data;
}

QString TodoDetail::tag_to_string(TagType tag_type){
    if(tag_type == TagType_high){
        return "高";
    }else if(tag_type == TagType_mid){
        return "中";
    }else{
        return "低";
    }
}

void TodoDetail::render_text(){
    ui->text_line->setFont(QFont("黑体", setting_ctrl->get_int(SETTING_FONT_SIZE)));
    if(!item_data.data.empty()){
        ui->text_line->setText(QString::fromStdString(item_data.data));
    }else{
        ui->text_line->setPlaceholderText("TODO");
    }

    ui->tag->setCurrentText(tag_to_string(item_data.tag_type));
    if(!item_data.reminder.empty()){
        ui->ddl->setDateTime(QDateTime::fromString(QString::fromStdString(item_data.reminder), REMINDER_FORMAT));
        ui->ddl->setSelectedSection(QDateTimeEdit::DaySection);
    }
}

void TodoDetail::on_cancel_clicked() {
    reject();
}

void TodoDetail::on_accept_clicked() {
    // 内容
    item_data.data = ui->text_line->text().toStdString();
    // 标签
    QString tag = ui->tag->currentText();
    if(tag == "低"){
        item_data.tag_type = TagType_low;
    }else if(tag == "中"){
        item_data.tag_type = TagType_mid;
    }else if(tag == "高"){
        item_data.tag_type = TagType_high;
    }else{
        // 默认是中等类型
        item_data.tag_type = TagType_mid;
    }
    // 时间
    QString edit_time = ui->ddl->dateTime().toString(REMINDER_FORMAT);
    if(init_time != edit_time){
        item_data.reminder = edit_time.toStdString();
    }else{
        item_data.reminder = "";
    }
    accept();
}

void TodoDetail::paintEvent(QPaintEvent *event){
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 255 - setting_ctrl->get_int(SETTING_TRAN_POS)));

    // 主界面位置
    QRect new_rect;
    int bound = 10;
    new_rect.setX(rect().x() + bound);
    new_rect.setY(rect().y() + bound);
    new_rect.setWidth(this->rect().width() - 2*bound);
    new_rect.setHeight(this->rect().height() - 2*bound);

    // 设置主界面圆角蒙版
    QBitmap bmp(rect().size());
    bmp.fill();
    QPainter painter_bmp(&bmp);
    painter_bmp.setRenderHint(QPainter::Antialiasing, true);
    painter_bmp.setPen(Qt::NoPen);
    painter_bmp.setBrush(Qt::black);
    painter_bmp.drawRoundedRect(new_rect, 10, 10, Qt::AbsoluteSize);
    setMask(bmp);

    // 画边界的框
    painter.setPen(setting_ctrl->get_color(SETTING_ICON_COLOR));
    QRect lineRect = new_rect;
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);
}

void TodoDetail::mouseMoveEvent(QMouseEvent *event){
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPosition().x() - m_mouse_click_x, event->globalPosition().y() - m_mouse_click_y);
    }
}

void TodoDetail::mousePressEvent(QMouseEvent *event){
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_mouse_click_x = event->position().x();
        m_mouse_click_y = event->position().y();
    }
}

