#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //设置无边框和设置隐藏下部图标
#ifdef LINUX_CLIENT
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
#endif
#ifdef WINDOW_CLIENT
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::SubWindow);
#endif

    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color:transparent");

    ui->displayData->setText("更新ing");
    ui->displayData->setStyleSheet("color:" + QColor(252, 175, 62).name() + ";");//橙色
    ui->ok->setEnabled(false);

    srcPath = QCoreApplication::applicationDirPath() + "/temp/";
    srcFile = srcPath + UPDATE_FILE;

    desPath = QCoreApplication::applicationDirPath();
    desFile = desPath + "/" + UPDATE_FILE;
    backFile = srcPath + "/" + UPDATE_FILE + ".back";

    // 定时更新，等待主线程结束
    QTimer::singleShot(1000 * 2, this, SLOT(doUpdate()));
    startFailCount = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::doUpdate(){
    /* do back */
    QFile::remove(backFile);
    QFile::copy(desFile, backFile);
    LogCtrl::debug("src:%s dest:%s", srcFile.toLatin1().data(), desFile.toLatin1().data());

    /* 若拷贝成功 */
    QFile::remove(desFile);
    if(QFile::copy(srcFile, desFile)){
#ifdef LINUX_CLIENT
        QFile(desFile).setPermissions(QFileDevice::ReadUser | QFileDevice::ReadGroup | QFileDevice::ReadOther
                                      | QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ExeOther
                                      | QFileDevice::WriteUser | QFileDevice::WriteGroup);
#endif
#ifdef WINDOW_CLIENT
        QFile(desFile).setPermissions(QFileDevice::ReadUser | QFileDevice::ReadGroup | QFileDevice::ReadOther
                                      | QFileDevice::ExeUser | QFileDevice::ExeGroup | QFileDevice::ExeOther
                                      | QFileDevice::WriteUser | QFileDevice::WriteGroup);
#endif
        if(QFileInfo(desFile).isExecutable()){
            QProcess *deltaNote = new QProcess();
            deltaNote->start(desFile, QStringList());

            LogCtrl::debug("start code : %d", deltaNote->state());
            if(deltaNote->waitForStarted() == true){
                LogCtrl::debug("start success");
                qApp->quit();
                return;
            }
        }
    }

    LogCtrl::debug("back to old file");
    QFile::remove(desFile);
    QFile::copy(backFile, desFile);

    ui->displayData->setStyleSheet("color:red;");
    ui->displayData->setText("更新失败,回退版本");

    ui->ok->setEnabled(true);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    event->ignore();
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(255, 255, 255, 35));

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
    painter.setPen(QColor(252, 175, 62));
    QRect lineRect = this->rect();
    lineRect.setHeight(lineRect.height() - 1);
    lineRect.setWidth(lineRect.width() - 1);
    painter.drawRoundedRect(lineRect, 10, 10, Qt::AbsoluteSize);

    QPalette palette = ui->displayData->palette();
    palette.setColor(QPalette::Text, QColor(252, 175, 62));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->buttons() & Qt::LeftButton){
        move(event->globalX()-m_nMouseClick_X_Coordinate, event->globalY()-m_nMouseClick_Y_Coordinate);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    event->ignore();
    if (event->button() == Qt::LeftButton)  //每当按下鼠标左键就记录一下位置
    {
        m_nMouseClick_X_Coordinate = event->x();
        m_nMouseClick_Y_Coordinate = event->y();
    }
}

void MainWindow::updateFail(){
    qApp->quit();
}
void MainWindow::on_ok_clicked()
{
    if(startFailCount > 1){
        ui->displayData->setText("请重装软件");
        ui->displayData->setFocus();
        ui->ok->setEnabled(false);
        QTimer::singleShot(1000 * 3, this, SLOT(updateFail()));
        return;
    }

    /* start new/old process*/
    QProcess *deltaNote = new QProcess();
    if(QFileInfo(desFile).isFile() && QFileInfo(desFile).isExecutable()){
        deltaNote->start(desFile, QStringList());

        if(deltaNote->waitForStarted() == true){
            qApp->quit();
        }
    }

    ++startFailCount;
    ui->displayData->setText("启动失败，重试" + QString(startFailCount + '0') + "次");
}
