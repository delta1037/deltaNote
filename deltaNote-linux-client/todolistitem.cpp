#include "todolistitem.h"
#include "ui_todolistitem.h"

ToDoListItem::ToDoListItem(QWidget *parent, const QString &displayData, char *_opTime, char *_createTime, char _check, char *_data) :
    QWidget(parent),
    ui(new Ui::ToDoListItem)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setColor(QPalette::Text, fontColor);
    ui->dataLine->setPalette(palette);

    ui->dataLine->setFont(QFont("黑体",16));

    if(_opTime != nullptr){
        strcpy(opTime, _opTime);
        strcpy(createTime, _createTime);
        isCheck = _check;
        isChoose = (isCheck == Checked);

        if(!isChoose){
            GraphicsColorSvgItem svg_check(":/resource/unChoose.svg");
            ui->choose->setIcon(svg_check.setColor(iconColor));
        }else{
            GraphicsColorSvgItem svg_check(":/resource/choose.svg");
            ui->choose->setIcon(svg_check.setColor(iconColor));
        }

        op = 0;
        strcpy(data, _data);
        ui->dataLine->setText(QString(data));
        oldData = QString(QLatin1String(data));
    }else{
        memset(opTime, 0, sizeof(opTime));
        memset(createTime, 0, sizeof(createTime));
        memset(data, 0, sizeof(data));
        ui->dataLine->setText(displayData);
        ui->dataLine->setPlaceholderText("TODO");
        isChoose = false;
        oldData = nullptr;

        op = 0;
        isCheck = UnCheck;

        GraphicsColorSvgItem svg_check(":/resource/unChoose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }
}
void ToDoListItem::refreshColor(){
    QPalette palette;
    palette.setColor(QPalette::Text, fontColor);
    ui->dataLine->setPalette(palette);

    if(isChoose){
        GraphicsColorSvgItem svg_check(":/resource/choose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }else{
        GraphicsColorSvgItem svg_check(":/resource/unChoose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }
}

ToDoListItem::~ToDoListItem()
{
    delete ui;
}

void ToDoListItem::on_choose_clicked()
{
    isChoose = !isChoose;
    isCheck = isChoose == true ? Checked:UnCheck;
    sprintf(opTime, "%ld", std::time(nullptr));
    if(isLogin) {
        MSG_PACK synPack{};
        makeSocketPack(synPack, 1, MSG_FULL, Push);
        makeDataPack(synPack.msgQueue[0], opTime, createTime, CHECK, isCheck, data);

        if(PushError == synMsgToServer(synPack)){
            QMessageBox::warning(nullptr, tr("Warning"), tr("修改便签同步错误!"), QMessageBox::Yes);
        }
    }else{
        MSG_OP_PACK pack{};
        ClientSqlite clientSqlite;
        makeDataPack(pack, opTime, createTime, CHECK, isCheck, data);
        clientSqlite.insertChangeItem(pack);
    }

    if(!isChoose){
        GraphicsColorSvgItem svg_check(":/resource/unChoose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }else{
        GraphicsColorSvgItem svg_check(":/resource/choose.svg");
        ui->choose->setIcon(svg_check.setColor(iconColor));
    }
}

void ToDoListItem::on_dataLine_editingFinished()
{
    // edit success start process
    QString newData = ui->dataLine->text().trimmed();
    if(oldData == nullptr && newData != nullptr){
        // add
        strcpy(data, newData.toUtf8().data());
        oldData = newData;
        sprintf(createTime, "%ld", std::time(nullptr));
        sprintf(opTime, "%ld", std::time(nullptr));

        if(isLogin) {
            MSG_PACK synPack{};
            makeSocketPack(synPack, 1, MSG_FULL, Push);
            makeDataPack(synPack.msgQueue[0], opTime, createTime, ADD, isCheck, data);

            if(PushError == synMsgToServer(synPack)){
                QMessageBox::warning(nullptr, tr("Warning"), tr("新建便签同步错误!"), QMessageBox::Yes);
            }
        }else{
            MSG_OP_PACK pack{};
            ClientSqlite clientSqlite;
            makeDataPack(pack, opTime, createTime, ADD, isCheck, data);
            clientSqlite.insertChangeItem(pack);
        }
    } else if(oldData.compare(newData) != 0){
        // alter
        memset(opTime, 0, sizeof(opTime));
        sprintf(opTime, "%ld", std::time(nullptr));
        strcpy(data, newData.toUtf8().data());
        oldData = newData;

        if(isLogin) {
            MSG_PACK synPack{};
            makeSocketPack(synPack, 1, MSG_FULL, Push);
            makeDataPack(synPack.msgQueue[0], opTime, createTime, ALTER, isCheck, data);

            if(PushError == synMsgToServer(synPack)){
                QMessageBox::warning(nullptr, tr("Warning"), tr("修改便签同步错误!"), QMessageBox::Yes);
            }
        }else{
            MSG_OP_PACK pack{};
            ClientSqlite clientSqlite;
            makeDataPack(pack, opTime, createTime, ALTER, isCheck, data);
            clientSqlite.insertChangeItem(pack);
        }
    }
}

