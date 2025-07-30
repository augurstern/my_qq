#include "server.h"
#include "ui_server.h"
#include<QFile>
#include<QTcpServer>
#include<QTcpSocket>
#include<QMessageBox>
#include<QFileDialog>
#include<QDebug>

server::server(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::server)
{
    ui->setupUi(this);

    setFixedSize(400,207);
    tPort = 5555;
    tSrv = new QTcpServer(this);
    connect(tSrv,&QTcpServer::newConnection,this,&server::sndMsg);
    initSrv();
}

server::~server()
{
    delete ui;
}

void server::initSrv()
{
    payloadSize = 64*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesTobeWrite = 0;
    ui->progressBar->reset();
    ui->sOpenBtn->setEnabled(true);
    ui->sSendBtn->setEnabled(false);
    tSrv->close();
}

void server::sndMsg()
{
    ui->sSendBtn->setEnabled(false);
    clntConn = tSrv->nextPendingConnection();
    connect(clntConn,&QTcpSocket::bytesWritten,this,&server::updClntProgress);
    ui->sStatusLbl->setText(tr("开始传送文件%1!").arg(theFileName));
    locFile = new QFile(fileName);
    if(!locFile->open(QFile::ReadOnly))
    {
        QMessageBox::warning(this,"应用程序",tr("无法读取文件%1\n%2").arg(fileName).arg(locFile->errorString()));
        return;
    }
    totalBytes = locFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_8);
    timer.start();
    QString curFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    sendOut<<qint64(0)<<qint64(0)<<curFile;
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64(outBlock.size() - sizeof(qint64)*2);
    bytesTobeWrite = totalBytes - clntConn->write(outBlock);
    outBlock.resize(0);
}

void server::updClntProgress(qint64 numBytes)
{
    qApp->processEvents();
    bytesWritten += (int)numBytes;
    if(bytesTobeWrite > 0)
    {
        outBlock = locFile->read(qMin(bytesTobeWrite,payloadSize));
        bytesTobeWrite -=(int)clntConn->write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        locFile->close();
    }
    ui->progressBar->setMaximum(totalBytes);
    ui->progressBar->setValue(bytesWritten);
    float useTime = timer.elapsed();
    double speed = bytesWritten / useTime;
    ui->sStatusLbl->setText(tr("已发送 %1MB（%2MB/s)\n共%3MB 已用时：%4秒\n估计剩余时间： %5秒").arg(bytesWritten / (1024*1024))
                                .arg(speed * 1000 / (1024*1024),0,'f',2)
                                .arg(totalBytes / (1024*1024))
                                .arg(useTime / 1000,0,'f',0)
                                .arg(totalBytes/speed/1000 - useTime/1000,0,'f',0));
    if(bytesWritten == totalBytes)
    {
        locFile->close();
        tSrv->close();
        ui->sStatusLbl->setText(tr("传送文件%1成功").arg(theFileName));
    }
}
void server::on_sOpenBtn_clicked()
{
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
        theFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
        ui->sStatusLbl->setText(tr("要传送的文件为：%1").arg(theFileName));
        ui->sSendBtn->setEnabled(true);
        ui->sOpenBtn->setEnabled(false);
    }
}


void server::on_sSendBtn_clicked()
{
    if(!tSrv->listen(QHostAddress::Any,tPort))
    {
        qDebug()<<tSrv->errorString();
        close();
        return;
    }
    ui->sStatusLbl->setText("等待对方接受... ...");
    emit sndFileName(theFileName);
}


void server::on_sCloseBtn_clicked()
{
    if(tSrv->isListening())
    {
        tSrv->close();
        if(locFile->isOpen())
        {
            locFile->close();
        }
        clntConn->abort();
    }
    close();
}

void server::closeEvent(QCloseEvent *)
{
    on_sCloseBtn_clicked();
}

void server::refused()
{
    tSrv->close();
    ui->sStatusLbl->setText("对方拒绝接受!");
}
