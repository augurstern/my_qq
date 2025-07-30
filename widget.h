#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QTextCharFormat>

QT_BEGIN_NAMESPACE
class QUdpSocket;
class server;
namespace Ui {
class Widget;
}
enum MsgType{Msg,UserEnter,UserLeft,FileName,Refuse};   //消息类型
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString username);
    ~Widget();

protected:
    void userEnter(QString username,QString ipaddr);            //处理新用户加入
    void userLeft(QString username,QString time);               //处理用户离开
    void sndMsg(MsgType type,QString srvaddr = "");             //广播UDP消息
    QString getIP();                                            //获取IP地址
    QString getUser();                                          //获取用户名
    QString getMsg();                                           //获取聊天信息
    bool saveFile(const QString &filename);
    void closeEvent(QCloseEvent *);

    void hasPendingFile(QString username,QString srvaddr,QString clntaddr,QString filename);

private:
    Ui::Widget *ui;

    QUdpSocket *udpSocket;
    qint16 port;
    QString uName;
    QString fileName;
    server *srv;
    QColor color;

private slots:
    void processPendingDatagrams();     //接受udp消息
    void on_sendBtn_clicked();
    void getFileName(QString);
    void on_sendTBtn_clicked();
    void on_fontCbx_currentFontChanged(const QFont &f);
    void on_sizeCbx_currentIndexChanged(int index);
    void on_boldTBtn_clicked(bool checked);
    void on_italicTBtn_clicked(bool checked);
    void on_underlineTBtn_clicked(bool checked);
    void on_colorTBtn_clicked();
    void curFmtChanged(const QTextCharFormat &fmt);
    void on_saveTBtn_clicked();
    void on_clearTBtn_clicked();
    void on_exitBtn_clicked();
};
#endif // WIDGET_H
