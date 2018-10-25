#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
    void show_connected();
    void recv_data();
    void show_timeout();
    void send_pic();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;

    QTcpSocket *tcpSocket;
    QTimer *timer;

    char buf[20];
    char picbuf[1024*1024];
    unsigned int piclen;
    int flag;

};

#endif // WIDGET_H
