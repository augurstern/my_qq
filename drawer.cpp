#include "drawer.h"
#include<QGroupBox>
#include<QVBoxLayout>

Drawer::Drawer(QWidget *parent)
    :QToolBox(parent)
{
    setWindowTitle("My QQ 2024");
    setWindowIcon(QPixmap(":/images/qq.png"));
    toolBtn1 = new QToolButton;
    connect(toolBtn1,&QToolButton::clicked,this,&Drawer::showChatWidget1);
    toolBtn1->setText("水漂起源");
    toolBtn1->setIcon(QPixmap(":/images/spqy.png"));
    toolBtn1->setIconSize(QPixmap(":/images/spqy.png").size());
    toolBtn1->setAutoRaise(true);
    toolBtn1->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn2 = new QToolButton;
    connect(toolBtn2,&QToolButton::clicked,this,&Drawer::showChatWidget2);
    toolBtn2->setText("忆梦如澜");
    toolBtn2->setIcon(QPixmap(":/images/ymrl.png"));
    toolBtn2->setIconSize(QPixmap(":/images/ymrl.png").size());
    toolBtn2->setAutoRaise(true);
    toolBtn2->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn3 = new QToolButton;
    connect(toolBtn3,&QToolButton::clicked,this,&Drawer::showChatWidget3);
    toolBtn3->setText("北京");
    toolBtn3->setIcon(QPixmap(":/images/qq.png"));
    toolBtn3->setIconSize(QPixmap(":/images/qq.png").size());
    toolBtn3->setAutoRaise(true);
    toolBtn3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn4 = new QToolButton;
    connect(toolBtn4,&QToolButton::clicked,this,&Drawer::showChatWidget4);
    toolBtn4->setText("樱桃");
    toolBtn4->setIcon(QPixmap(":/images/Cherry.png"));
    toolBtn4->setIconSize(QPixmap(":/images/Cherry.png").size());
    toolBtn4->setAutoRaise(true);
    toolBtn4->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn5 = new QToolButton;
    connect(toolBtn5,&QToolButton::clicked,this,&Drawer::showChatWidget5);
    toolBtn5->setText("淡然");
    toolBtn5->setIcon(QPixmap(":/images/dr.png"));
    toolBtn5->setIconSize(QPixmap(":/images/dr.png").size());
    toolBtn5->setAutoRaise(true);
    toolBtn5->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn6 = new QToolButton;
    connect(toolBtn6,&QToolButton::clicked,this,&Drawer::showChatWidget6);
    toolBtn6->setText("娇娇");
    toolBtn6->setIcon(QPixmap(":/images/jj.png"));
    toolBtn6->setIconSize(QPixmap(":/images/jj.png").size());
    toolBtn6->setAutoRaise(true);
    toolBtn6->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn7 = new QToolButton;
    connect(toolBtn7,&QToolButton::clicked,this,&Drawer::showChatWidget7);
    toolBtn7->setText("落水无痕");
    toolBtn7->setIcon(QPixmap(":/images/lswh.png"));
    toolBtn7->setIconSize(QPixmap(":/images/lswh.png").size());
    toolBtn7->setAutoRaise(true);
    toolBtn7->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn8 = new QToolButton;
    connect(toolBtn8,&QToolButton::clicked,this,&Drawer::showChatWidget8);
    toolBtn8->setText("青墨暖暖");
    toolBtn8->setIcon(QPixmap(":/images/qmnn.png"));
    toolBtn8->setIconSize(QPixmap(":/images/qmnn.png").size());
    toolBtn8->setAutoRaise(true);
    toolBtn8->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    toolBtn9 = new QToolButton;
    connect(toolBtn9,&QToolButton::clicked,this,&Drawer::showChatWidget9);
    toolBtn9->setText("无语");
    toolBtn9->setIcon(QPixmap(":/images/wy.png"));
    toolBtn9->setIconSize(QPixmap(":/images/wy.png").size());
    toolBtn9->setAutoRaise(true);
    toolBtn9->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *layout = new QVBoxLayout(groupBox);

    layout->setAlignment(Qt::AlignLeft);
    layout->setContentsMargins(20,20,20,20);
    layout->addWidget(toolBtn1);
    layout->addWidget(toolBtn2);
    layout->addWidget(toolBtn3);
    layout->addWidget(toolBtn4);
    layout->addWidget(toolBtn5);
    layout->addWidget(toolBtn6);
    layout->addWidget(toolBtn7);
    layout->addWidget(toolBtn8);
    layout->addWidget(toolBtn9);
    layout->addStretch();
    this->addItem(groupBox,"群成员");
}

void Drawer::showChatWidget1()
{
    chatWidget1 = new Widget(0,toolBtn1->text());
    chatWidget1->setWindowTitle(toolBtn1->text());
    chatWidget1->setWindowIcon(toolBtn1->icon());
    chatWidget1->show();
}
void Drawer::showChatWidget2()
{
    chatWidget2 = new Widget(0,toolBtn2->text());
    chatWidget2->setWindowTitle(toolBtn2->text());
    chatWidget2->setWindowIcon(toolBtn2->icon());
    chatWidget2->show();
}
void Drawer::showChatWidget3()
{
    chatWidget3 = new Widget(0,toolBtn3->text());
    chatWidget3->setWindowTitle(toolBtn3->text());
    chatWidget3->setWindowIcon(toolBtn3->icon());
    chatWidget3->show();
}
void Drawer::showChatWidget4()
{
    chatWidget4 = new Widget(0,toolBtn4->text());
    chatWidget4->setWindowTitle(toolBtn4->text());
    chatWidget4->setWindowIcon(toolBtn4->icon());
    chatWidget4->show();
}
void Drawer::showChatWidget5()
{
    chatWidget5 = new Widget(0,toolBtn5->text());
    chatWidget5->setWindowTitle(toolBtn5->text());
    chatWidget5->setWindowIcon(toolBtn5->icon());
    chatWidget5->show();
}
void Drawer::showChatWidget6()
{
    chatWidget6 = new Widget(0,toolBtn6->text());
    chatWidget6->setWindowTitle(toolBtn6->text());
    chatWidget6->setWindowIcon(toolBtn6->icon());
    chatWidget6->show();
}
void Drawer::showChatWidget7()
{
    chatWidget7 = new Widget(0,toolBtn7->text());
    chatWidget7->setWindowTitle(toolBtn7->text());
    chatWidget7->setWindowIcon(toolBtn7->icon());
    chatWidget7->show();
}
void Drawer::showChatWidget8()
{
    chatWidget8 = new Widget(0,toolBtn8->text());
    chatWidget8->setWindowTitle(toolBtn8->text());
    chatWidget8->setWindowIcon(toolBtn8->icon());
    chatWidget8->show();
}
void Drawer::showChatWidget9()
{
    chatWidget9 = new Widget(0,toolBtn9->text());
    chatWidget9->setWindowTitle(toolBtn9->text());
    chatWidget9->setWindowIcon(toolBtn9->icon());
    chatWidget9->show();
}
