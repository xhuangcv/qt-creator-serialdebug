#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QLabel>

#include "about.h"
#include "version.h"
#include "ascii.h"
#include "log.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void readDate();

    void comUpdate();

    void timeUpdate();

    void saveLog();

    void on_openFile_triggered();

    void on_versionInf_triggered();

    void on_aboutUs_triggered();

    void on_start_triggered();

    void on_sendInfo_clicked();

    void on_clearSend_clicked();

    void on_turn_clicked();

    void on_modify_clicked();

    void on_autocheckBox_clicked();

    void on_stop_triggered();

    void on_clear_triggered();

    void on_exit_triggered();

    void on_caculate_triggered();

    void on_ASCII_triggered();

    void on_lookLog_triggered();

    void on_log_triggered();

    void on_wordcheckBox_clicked();

private:
    Ui::MainWindow *ui;

    //各种定义
    QSerialPort *serialPort;
    QTimer *timerPort;
    QTimer *timerCycle;
    QTimer *timerClock;
    version *myVersion;
    about *myAbout;
    ascii *myAscii;
    log *myLog;
    QByteArray makeOrder;
    QLabel *currentTimeLabel;
    int len;

    //通过索引找到对应数值
    QSerialPort::BaudRate BaudRate(int index);
    QSerialPort::StopBits StopBits(int index);
    QSerialPort::DataBits DataBits(int index);
    QSerialPort::Parity Parity(int index);

    //转16进制函数
    void StringToHex(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);

    //转动&修改ID函数
    unsigned char axSendPosition(unsigned char ID, unsigned short int target_pos, unsigned short int target_speed);
    unsigned char axModifyID(unsigned char ID, unsigned char trgID);
    void axSendPack(unsigned char ID, char order, unsigned char param[], unsigned short int length);

    //拖动操作
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MAINWINDOW_H
