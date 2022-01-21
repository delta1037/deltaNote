#include "ui/checkupdate.h"
#include "ui_checkupdate.h"
#include "log.h"

CheckUpdate::CheckUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckUpdate)
{
    ui->setupUi(this);
    downloadFinish = false;

    updateFile = nullptr;//文件指针
    updateReply = nullptr;
    updateManager = nullptr;

    ui->abort->setEnabled(false);

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif

    QPalette palette = ui->message->palette();
    palette.setColor(QPalette::Text, fontColor);
    ui->progressBar->setPalette(palette);
    ui->message->setPalette(palette);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    // 将界面移到中心
    move(desktopRect.width()/2 - this->width()/2, desktopRect.height()/2 - this->height()/2);
}

CheckUpdate::~CheckUpdate()
{
    delete updateManager;
    delete updateFile;
    delete ui;
}

void CheckUpdate::setUpdateLink(QString &link){
    updateLink = link;
}

void CheckUpdate::setUpdateMessage(QString &msg){
    QTextDocument *doucument = new QTextDocument();
    doucument->setHtml(msg);
    ui->message->setDocument(doucument);
}

bool CheckUpdate::createNewFile(){
    //下载文件之前先在本地创建一个文件
    QString downloadPath = QCoreApplication::applicationDirPath() + "/temp/";
    QString downloadFile = downloadPath + QFileInfo(updateLink).fileName();

    ui_debug("downloadFile:%s", downloadFile.toLatin1().data());

    QDir file(downloadPath);
    if(!file.exists()){
        ui_debug("%s", "create download path");
        file.mkpath(downloadPath);
    }

    QFileInfo fileInfo(downloadFile);
    if(fileInfo.isFile()){
        ui_debug("%s", "delete already download file");
        QFile tempFile(downloadFile);
        tempFile.remove();
    }

    updateFile = new QFile(downloadFile);
    //判断文件是否可写入 不可写删除 指针赋值0
    if(!updateFile->open(QIODevice::WriteOnly)){
        ui_debug("%s", "file cant write");

       delete updateFile;
       updateFile = nullptr;

       return false;
    }
    return true;
}

void CheckUpdate::getUpdateFile(){
    if(!createNewFile()){
        ui_debug("%s", "create new file error");
        return;
    }

    ui_debug("start download:%s", updateLink.toLatin1().data());

    //开始请求 下载文件
    QUrl serviceUrl = QUrl(updateLink);

    updateManager = new QNetworkAccessManager(this);
    updateReply = updateManager->get(QNetworkRequest(serviceUrl));
    if(updateReply != nullptr){
        ui_debug("update reply : %p", updateReply);
        connect(updateReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(processError(QNetworkReply::NetworkError)));
        connect(updateReply,SIGNAL(readyRead()),this,SLOT(fileDowload()));          //数据写入
        connect(updateReply,SIGNAL(finished()),this,SLOT(fileDowloadFinished()));   //请求完成
        connect(updateReply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateDownloadProgress(qint64,qint64)));
    }else{
        ui_debug("%s", "connect error");

        ui->cancel->setEnabled(true);
        ui->abort->setEnabled(false);
        ui->yes->setEnabled(true);
        ui->yes->setText("重试");

        MessagesBox::error(this, UPDATE_CONNECT_ERROR);
    }
}

void CheckUpdate::processError(QNetworkReply::NetworkError error){
    ui_debug("%s", "error code : %d", error);
}

void CheckUpdate::fileDowload(){
    if(updateFile){
         updateFile->write(updateReply->readAll());
    }
}
void CheckUpdate::fileDowloadFinished(){
    //刷新文件
    updateFile->flush();
    updateFile->close();

    downloadFinish = true;

    ui->cancel->setEnabled(true);
    ui->abort->setEnabled(false);
    ui->yes->setEnabled(true);
    ui->yes->setText("重启软件");

    ui_debug("%s", "download finish");
}

void CheckUpdate::updateDownloadProgress(qint64 recv, qint64 total){
    ui_debug("recv:%d total:%d", recv, total);
    ui->progressBar->setValue(int(recv * 100 / total));
}

void CheckUpdate::paintEvent(QPaintEvent *event)
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

void CheckUpdate::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalPosition().x()-m_nMouseClick_X_Coordinate, event->globalPosition().y()-m_nMouseClick_Y_Coordinate);
    }
}

void CheckUpdate::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_nMouseClick_X_Coordinate = event->position().x();
        m_nMouseClick_Y_Coordinate = event->position().y();
    }
}

void CheckUpdate::on_cancel_clicked(){
    reject();
}

void CheckUpdate::on_abort_clicked(){
    reject();
}

void CheckUpdate::on_yes_clicked(){
    if(downloadFinish){
       accept();
       return;
    }

    ui->cancel->setEnabled(false);
    ui->abort->setEnabled(true);
    ui->yes->setEnabled(false);
    getUpdateFile();
}

