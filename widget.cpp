#include "widget.h"
#include "ui_widget.h"
#include"server.h"
#include"client.h"
#include<QFileDialog>
#include<QUdpSocket>
#include<QHostInfo>
#include<QMessageBox>
#include<QScrollBar>
#include<QDateTime>
#include<QNetworkInterface>
#include<QProcess>
#include<QColorDialog>

Widget::Widget(QWidget *parent,QString username)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    uName = username;
    udpSocket = new QUdpSocket(this);
    port = 23232;
    udpSocket->bind(port,QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::processPendingDatagrams);
    sndMsg(UserEnter);

    srv = new server(this);
    connect(srv,&server::sndFileName,this,&Widget::getFileName);
    connect(ui->msgTextEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(curFmtChanged(const QTextCharFormat)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sndMsg(MsgType type,QString srvaddr)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString address = getIP();
    out<<type<<getUser();
    switch (type) {
    case Msg:
        if(ui->msgTextEdit->toPlainText() == ""){
            QMessageBox::warning(0,"警告","发送内容不能为空",QMessageBox::Ok);
            return;
        }
        out<<address<<getMsg();
        ui->msgBrowser->verticalScrollBar()->setValue(ui->msgBrowser->verticalScrollBar()->maximum());
        break;
    case UserEnter:
        out<<address;
        break;
    case UserLeft:
        break;
    case FileName:
        {
            int row = ui->UserTblWidget->currentRow();
            QString clntaddr = ui->UserTblWidget->item(row,1)->text();
            out<<address<<clntaddr<<fileName;
            break;
        }
    case Refuse:
        out<<srvaddr;
        break;
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast,port);
}

void Widget::processPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int msgType;
        in>>msgType;
        QString userName,ipAddr,msg;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-ddhh:mm:ss");
        switch (msgType) {
        case Msg:
            in>>userName>>ipAddr>>msg;
            ui->msgBrowser->setTextColor(Qt::blue);
            ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
            ui->msgBrowser->append("[ " +userName+" ] "+ time);
            ui->msgBrowser->append(msg);
            break;
        case UserEnter:
            in>>userName>>ipAddr;
            userEnter(userName,ipAddr);
            break;
        case UserLeft:
            in>>userName;
            userLeft(userName,time);
            break;
        case FileName:
        {
            in>>userName>>ipAddr;
            QString fileName,clntAddr;
            hasPendingFile(userName,ipAddr,clntAddr,fileName);
            break;
        }
        case Refuse:
            in>>userName;
            QString srvAddr;
            in>>srvAddr;
            QString ipAddr = getIP();
            if(ipAddr == srvAddr)
            {
                srv->refused();
            }
            break;
        }
    }
}

void Widget::userEnter(QString username,QString ipaddr)
{
    bool isEmpty = ui->UserTblWidget->findItems(username,Qt::MatchExactly).isEmpty();
    if(isEmpty)
    {
        QTableWidgetItem *user = new QTableWidgetItem(username);
        QTableWidgetItem *ip = new QTableWidgetItem(ipaddr);
        ui->UserTblWidget->insertRow(0);
        ui->UserTblWidget->setItem(0,0,user);
        ui->UserTblWidget->setItem(0,1,ip);
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
        ui->msgBrowser->append(tr("%1 在线! ").arg(username));
        ui->userNumLbl->setText(tr("在线人数: %1").arg(ui->UserTblWidget->rowCount()));
        sndMsg(UserEnter);
    }
}

void Widget::userLeft(QString username,QString time)
{
    int rowNum = ui->UserTblWidget->findItems(username,Qt::MatchExactly).first()->row();
    ui->UserTblWidget->removeRow(rowNum);
    ui->msgBrowser->setTextColor(Qt::gray);
    ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
    ui->msgBrowser->append(tr("%1 于 %2 离开！").arg(username).arg(time));
    ui->userNumLbl->setText(tr("在线人数： %1").arg(ui->UserTblWidget->rowCount()));
}

QString Widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, list) {
        if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            return addr.toString();
    }
    return 0;
}

QString Widget::getUser()
{
    return uName;
}

QString Widget::getMsg()
{
    QString msg = ui->msgTextEdit->toHtml();
    ui->msgTextEdit->clear();
    ui->msgTextEdit->setFocus();
    return msg;
}


void Widget::on_sendBtn_clicked()
{
    sndMsg(Msg);
}

void Widget::getFileName(QString name)
{
    fileName = name;
    sndMsg(FileName);
}

void Widget::on_sendTBtn_clicked()
{
    if(ui->UserTblWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(0,"选择用户","请先选择目标用户!");
        return;
    }
    srv->show();
    srv->initSrv();
}

void Widget::hasPendingFile(QString username,QString srvaddr,QString clntaddr,QString filename)
{
    QString ipAddr = getIP();
    if(ipAddr == clntaddr)
    {
        int btn = QMessageBox::information(this,"接收文件",tr("来自%1(%2)的文件: %3,是否接收？").arg(username).arg(srvaddr).arg(filename),
                                           QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,"保存文件",filename);
            if(!name.isEmpty())
            {
                Client *clnt = new Client(this);
                clnt->setFileName(name);
                clnt->setHostAddr(QHostAddress(srvaddr));
                clnt->show();
            }
        }
        else
        {
            sndMsg(Refuse,srvaddr);
        }
    }
}

void Widget::on_fontCbx_currentFontChanged(const QFont &f)
{
    ui->msgTextEdit->setCurrentFont(f);
    ui->msgTextEdit->setFocus();
}


void Widget::on_sizeCbx_currentIndexChanged(int index)
{
    ui->msgTextEdit->setFontPointSize(index);
    ui->msgTextEdit->setFocus();
}


void Widget::on_boldTBtn_clicked(bool checked)
{
    if(checked)
    {
        ui->msgTextEdit->setFontWeight(QFont::Bold);
    }
    else
    {
        ui->msgTextEdit->setFontWeight(QFont::Normal);
    }
    ui->msgTextEdit->setFocus();
}


void Widget::on_italicTBtn_clicked(bool checked)
{
    ui->msgTextEdit->setFontItalic(checked);
    ui->msgTextEdit->setFocus();
}


void Widget::on_underlineTBtn_clicked(bool checked)
{
    ui->msgTextEdit->setFontUnderline(checked);
    ui->msgTextEdit->setFocus();
}


void Widget::on_colorTBtn_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->msgTextEdit->setTextColor(color);
        ui->msgTextEdit->setFocus();
    }
}

void Widget::curFmtChanged(const QTextCharFormat &fmt)
{
    ui->fontCbx->setCurrentFont(fmt.font());
    if(fmt.fontPointSize() < 8)
    {
        ui->sizeCbx->setCurrentIndex(4);
    }
    else
    {
        ui->sizeCbx->setCurrentIndex(ui->sizeCbx->findText(QString::number(fmt.fontPointSize())));
    }
    ui->boldTBtn->setChecked(fmt.font().bold());
    ui->italicTBtn->setChecked(fmt.font().italic());
    ui->underlineTBtn->setChecked(fmt.font().underline());
    color = fmt.foreground().color();
}

void Widget::on_saveTBtn_clicked()
{
    if(ui->msgBrowser->document()->isEmpty())
    {
        QMessageBox::warning(0,"警告","聊天记录为空，无法保存");
    }
    else
    {
        QString fname = QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","文本(*.txt);;所有文件(*.*");
        if(!fname.isEmpty())
        {
            saveFile(fname);
        }
    }
}

bool Widget::saveFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"保存文件",tr("无法保存文件%1:\n%2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out<<ui->msgBrowser->toPlainText();
    return true;
}
void Widget::on_clearTBtn_clicked()
{
    ui->msgBrowser->clear();
}


void Widget::on_exitBtn_clicked()
{
    close();
}

void Widget::closeEvent(QCloseEvent *e)
{
    sndMsg(UserLeft);
    QWidget::closeEvent(e);
}
