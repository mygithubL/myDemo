#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("视频监控");

    tcpSocket = new QTcpSocket;
    tcpSocket->connectToHost("192.168.1.22",8888);

    connect(tcpSocket, SIGNAL(connected()), this, SLOT(show_connected()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(recv_data()));

     ui->label->setScaledContents(true);

    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(send_pic()));//一旦定时器超时，调用对应的槽函数,绑定信号和槽

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    flag = 0;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::show_connected()
{
    qDebug() << "connectnd";
    /*ui->lineEdit->setText("connected!");*/

}
void Widget::recv_data()
{
    if(0 == flag)
    {
        if (tcpSocket->bytesAvailable() < 20)  //
            return;
        tcpSocket->read(buf, sizeof(buf)); //piclen: "1234"
        piclen = atoi(buf); //"1234"   ---->1234

        qDebug() << "piclen:" << piclen;
        flag = 1;
    }
    if(1 == flag)
    {

        if(tcpSocket->bytesAvailable() < piclen)
            return;
        tcpSocket->read(picbuf, piclen);
        QPixmap pixmap;
        pixmap.loadFromData((uchar*)picbuf, piclen);//jia zai tu pian
        ui->label->setPixmap(pixmap);
        flag = 0;
    }
}
void Widget::send_pic()
{
    char order[5] = "pic";
    tcpSocket->write(order,sizeof(order));
}
void Widget::show_timeout()
{
    qDebug() << "timeout..........";
}


void Widget::on_pushButton_clicked()
{
    timer->start(50);

    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
}

void Widget::on_pushButton_2_clicked()
{
    timer->stop();

    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
}
