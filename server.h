#ifndef SERVER_H
#define SERVER_H

#include<QDialog>
#include<QElapsedTimer>
class QFile;
class QTcpServer;
class QTcpSocket;

namespace Ui {
class server;
}

class server : public QDialog
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = nullptr);
    ~server();

    void initSrv();
    void refused();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::server *ui;

    qint16 tPort;
    QTcpServer *tSrv;
    QString fileName;
    QString theFileName;
    QFile *locFile;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesTobeWrite;
    qint64 payloadSize;
    QByteArray outBlock;
    QTcpSocket *clntConn;
    QElapsedTimer timer;

private slots:
    void sndMsg();
    void updClntProgress(qint64 numBytes);

    void on_sOpenBtn_clicked();

    void on_sSendBtn_clicked();

    void on_sCloseBtn_clicked();

signals:
    void sndFileName(QString fileName);
};

#endif // SERVER_H
