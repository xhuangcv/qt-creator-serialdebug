#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QDragEnterEvent>
#include <QUrl>
#include <QMimeData>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    this->setWindowIcon(QIcon(":/icon/icons/mainWindow.ico"));
    this->setWindowTitle("SerialDebug1.0");
    serialPort = new QSerialPort(this);

    //时间更新
    currentTimeLabel = new QLabel();
    timerClock = new QTimer(this);
    timeUpdate();
    timerClock->start(1000);
    connect(timerClock, SIGNAL(timeout()), this, SLOT(timeUpdate()));

    //每0.5秒检测端口
    timerPort = new QTimer(this);
    timerPort->start(500);
    connect(timerPort, SIGNAL(timeout()), this, SLOT(comUpdate()));

    //实例化自动发送时钟
    timerCycle = new QTimer(this);

    //串口设置初始化
    QStringList list1, list2, list3, list4;
    list1 << "300" << "600" << "1200" << "2400" << "4800" << "9600" << "19200"
          << "38400" << "43000" << "56000" << "57600" << "115200" << "1000000";
    list2 << "1" << "1.5" << "2";
    list3 << "5" << "6" << "7" << "8";
    list4 << "无" << "奇校检" << "偶校检";
    ui->cbBaud->addItems(list1);
    ui->cbStop->addItems(list2);
    ui->cbData->addItems(list3);
    ui->cbCheck->addItems(list4);

    //串口设置初值
    ui->cbBaud->setCurrentText("1000000");
    ui->cbStop->setCurrentText("1");
    ui->cbData->setCurrentText("8");
    ui->cbCheck->setCurrentText("无");

    //接收&发送设置初值
    ui->rowcheckBox->setCheckState(Qt::Checked);
    ui->newcheckBox->setCheckState(Qt::Checked);

    ui->cycle->setValue(1000);
    ui->angle->setValue(500);
    ui->sp->setValue(500);


}

MainWindow::~MainWindow()
{
    delete ui;
}

//检测端口并显示信息
void MainWindow::comUpdate()
{

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        serialPort->setPort(info);
        if(ui->cbPort->currentText() != info.portName())
        {
            if (serialPort->open(QIODevice::ReadWrite))
            {
                ui->cbPort->addItem(info.portName());
                ui->receiveTextBrowser->append(QStringLiteral("检测到端口列表"));
                ui->receiveTextBrowser->append(info.portName());
                ui->receiveTextBrowser->append(info.description());
                ui->receiveTextBrowser->append(info.manufacturer()+"\n");
                serialPort->close();
            }
            else
            {
                ui->cbPort->clear();
            }
        }
    }
}

//接收信息
void MainWindow::readDate(){
    QByteArray byteArray = serialPort->readAll();

    if (!byteArray.isEmpty())
    {
        //判断是否十六进制显示
        if(!ui->hexcheckBox->isChecked())
        {
            QString str = QString::fromLocal8Bit(byteArray.constData());
            ui->receiveTextBrowser->moveCursor(QTextCursor::End);
            ui->receiveTextBrowser->insertPlainText(str);
            //添加时间
            ui->receiveTextBrowser->insertPlainText(str);
            if(ui->timecheckBox->isChecked())
            {
                QDateTime currentTime =QDateTime::currentDateTime();
                QString strTime =currentTime.toString("yyyy.MM.dd hh:mm:ss\n");
                ui->receiveTextBrowser->insertPlainText(strTime);
            }
            //添加换行
            if(ui->rowcheckBox->isChecked())
            {
                ui->receiveTextBrowser->insertPlainText("\n");
            }
        }
        else
        {
            ui->receiveTextBrowser->moveCursor(QTextCursor::End);
            ui->receiveTextBrowser->insertPlainText(byteArray.constData());
            //添加时间
            if(ui->timecheckBox->isChecked())
            {
                QDateTime currentTime =QDateTime::currentDateTime();
                QString strTime =currentTime.toString("yyyy.MM.dd hh:mm:ss\n");
                ui->receiveTextBrowser->insertPlainText(strTime);
            }
            //添加换行
            if(ui->rowcheckBox->isChecked())
            {
                ui->receiveTextBrowser->insertPlainText("\n");
            }
        }

        //ui->receiveTextBrowser->append(str);

    }
}

QSerialPort::StopBits MainWindow::StopBits(int index)
{
    QSerialPort::StopBits stopbits;

    switch (index) {
    case 0:
        stopbits = QSerialPort::OneStop;
        break;
    case 1:
        stopbits = QSerialPort::OneAndHalfStop;
        break;
    case 2:
        stopbits = QSerialPort::TwoStop;
        break;
    default:
        stopbits = QSerialPort::UnknownStopBits;
        break;
    }

    return stopbits;
}

QSerialPort::DataBits MainWindow::DataBits(int index)
{
    QSerialPort::DataBits databits;

    switch (index) {
    case 0:
        databits = QSerialPort::Data5;
        break;
    case 1:
        databits = QSerialPort::Data6;
        break;
    case 2:
        databits = QSerialPort::Data7;
        break;
    case 3:
        databits = QSerialPort::Data8;
        break;
    default:
        databits = QSerialPort::UnknownDataBits;
        break;
    }

    return databits;
}

QSerialPort::Parity MainWindow::Parity(int index)
{
    QSerialPort::Parity parity;

    switch (index) {
    case 0:
        parity = QSerialPort::NoParity;
        break;
    case 1:
        parity = QSerialPort::OddParity;
        break;
    case 2:
        parity = QSerialPort::EvenParity;
        break;
    default:
        parity = QSerialPort::UnknownParity;
        break;
    }

    return parity;
}

//打开串口
void MainWindow::on_start_triggered()
{

    serialPort->setPortName(ui->cbPort->currentText());
    if(serialPort->open(QIODevice::ReadWrite))
    {
        serialPort->setBaudRate(ui->cbBaud->currentText().toInt());
        serialPort->setDataBits(DataBits(ui->cbData->currentIndex()));
        serialPort->setStopBits(StopBits(ui->cbStop->currentIndex()));
        serialPort->setParity(Parity(ui->cbCheck->currentIndex()));
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData())); //读取数据
        ui->start->setEnabled(false);
        ui->sendInfo->setEnabled(true);
        ui->turn->setEnabled(true);
        ui->modify->setEnabled(true);
    }
    else
    {
        QMessageBox::warning(NULL, tr("错误"), tr("串口打开失败"), QMessageBox::Retry);
    }

}

//打开文件
void MainWindow::on_openFile_triggered()
{
    //获取文件名
    QString fileName = QFileDialog::getOpenFileName(this, NULL, NULL);
    QFile file(fileName);

    //判断打开文件是否成功
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text))
    {
        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            //QtextEdit按行显示文件内容
            ui->writeInfo->append(textStream.readLine());
        }
    }
    /*
    else
    {

        QMessageBox::information(NULL, "提示", "打开文件错误");
    }
    */
}

//string转16进制
void MainWindow::StringToHex(QString str, QByteArray &senddata)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        //qDebug("%d ",(int)senddata[hexdatalen]);
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return (-1);
}

//发送信息
void MainWindow::on_sendInfo_clicked()
{
    QByteArray senddata;
    QString order = ui->writeInfo->toPlainText();
    //发送新行
    if(ui->newcheckBox->isChecked())
    {
        order = order + " 0d 0a";
    }

    StringToHex(order,senddata);
    //qDebug(senddata);
    serialPort->write(senddata);
}

//清空发送
void MainWindow::on_clearSend_clicked()
{
    ui->writeInfo->clear();
}

//转动&修改ID的一个函数
void MainWindow::axSendPack(unsigned char ID, char order, unsigned char param[], unsigned short int length)
{
    unsigned short int count = 0;
    unsigned int check = 0;
    unsigned char ax_txbuf[256] = {0};
    unsigned char i=0;
    if(length + 6 > 256)
        return;
    ax_txbuf[0] = 0xff;
    ax_txbuf[1] = 0xff;
    ax_txbuf[2] = ID;
    ax_txbuf[3] = length + 2;
    ax_txbuf[4] = order;

    check = ax_txbuf[2] + ax_txbuf[3] + ax_txbuf[4];

    if(length != 0)
    {
        for (count = 0; count <= length - 1; count++)
        {
            ax_txbuf[count + 5] = param[count];
            check = check + param[count];
        }
    }
    ax_txbuf[length + 5] = (~check) & 0x000000ff;
    makeOrder.clear();
    makeOrder.resize(length+8);
    for(i=0;i<=length+5;i++)
    {
        makeOrder[i] = (char)ax_txbuf[i];
        //qDebug("%x ", ax_txbuf[i]);
    }
    len = length+5;
}

//转动函数
unsigned char MainWindow::axSendPosition(unsigned char ID, unsigned short int target_pos, unsigned short int target_speed)
{
    unsigned char param[8] = {0};
    param[0] = 30;
    param[1] = target_pos;
    param[2] = target_pos >> 8;
    param[3] = target_speed;
    param[4] = target_speed >> 8;
    axSendPack(ID, 0x03, param, 5);
    return 0x00;
}

//修改ID函数
unsigned char MainWindow::axModifyID(unsigned char ID, unsigned char trgID)//ID:舵机原ID trgID：修改后的ID
{
    unsigned char param[8] = {0};

    param[0] = 3;
    param[1] = trgID;

    axSendPack(ID, 0x03, param, 2);
    return 0x00;
}

//转动按钮
void MainWindow::on_turn_clicked()
{
    short int id, pos, speed;
    id = ui->ID->text().toInt();
    pos = ui->angle->text().toInt();
    speed = ui->sp->text().toInt();
    axSendPosition(id,pos,speed);
    makeOrder[len+1] = '\r';
    makeOrder[len+2] = '\n';
    serialPort->write(makeOrder);
}

//修改按钮
void MainWindow::on_modify_clicked()
{
    int id1, id2;

    id1 = ui->ID->text().toInt();
    id2 = ui->ID2->text().toInt();

    axModifyID(id1,id2);
    makeOrder[len+1] = '\r';
    makeOrder[len+2] = '\n';
    serialPort->write(makeOrder);
}

void MainWindow::on_autocheckBox_clicked()
{
    if(ui->autocheckBox->isChecked())
    {
        connect(timerCycle,SIGNAL(timeout()),this,SLOT(on_sendInfo_clicked()));
        //qDebug("zou qi : %d ", ui->cycle->text().toInt());
        timerCycle->start(ui->cycle->text().toInt());

        ui->cycle->setEnabled(false);
    }
    else
    {
        timerCycle->stop();
        ui->cycle->setEnabled(true);
    }
}

//停止串口
void MainWindow::on_stop_triggered()
{
    serialPort->close();
    ui->start->setEnabled(true);
    ui->sendInfo->setEnabled(false);
    ui->turn->setEnabled(false);
    ui->modify->setEnabled(false);
}

//清空接收
void MainWindow::on_clear_triggered()
{
    ui->receiveTextBrowser->clear();
}

//退出
void MainWindow::on_exit_triggered()
{
    this->close();
}

//计算器
void MainWindow::on_caculate_triggered()
{
    system("calc");
}

void MainWindow::on_ASCII_triggered()
{
    myAscii = new ascii();
    myAscii->show();
}

void MainWindow::on_versionInf_triggered()
{
    //创建版本对象并打开子窗口
    myVersion = new version();
    myVersion->exec();
}

void MainWindow::on_aboutUs_triggered()
{
    //创建关于对象并打开子窗口
    myAbout = new about();
    myAbout->exec();
}

//查看日志
void MainWindow::on_lookLog_triggered()
{
    myLog = new log();
    myLog->show();
}

//记录日志
void MainWindow::saveLog()
{
    QByteArray byteArray = serialPort->readAll();
    QFile data("log.txt");
    if (data.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&data);
        if (!byteArray.isEmpty())
        {

            QString str = QString::fromLocal8Bit(byteArray.constData());
            out << str;

        }

    }
}

void MainWindow::on_log_triggered()
{
    if(ui->log->isChecked())
    {
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(saveLog()));

    }
}

//设置背景颜色
void MainWindow::on_wordcheckBox_clicked()
{
    if(ui->wordcheckBox->isChecked())
    {
        ui->receiveTextBrowser->setStyleSheet("background-color:black;");
        ui->receiveTextBrowser->setTextColor(Qt::green);
        QString str = ui->receiveTextBrowser->toPlainText();
        ui->receiveTextBrowser->clear();
        ui->receiveTextBrowser->append(str);
    }
    else
    {
        ui->receiveTextBrowser->setStyleSheet("background-color:white;");
        ui->receiveTextBrowser->setTextColor(Qt::black);
        QString str = ui->receiveTextBrowser->toPlainText();
        ui->receiveTextBrowser->clear();
        ui->receiveTextBrowser->append(str);
    }
}
//拖动事件
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
    {
        event->ignore();
    }
}

//放下事件
void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if(mimeData->hasUrls())
    {
        QList <QUrl> urlList = mimeData->urls();
        QString fileName = urlList.at(0).toLocalFile();
        if(! fileName.isEmpty())
        {
            QFile file(fileName);
            if(! file.open(QIODevice::ReadOnly))
            {
                return;
            }
            QTextStream in(&file);
            ui->writeInfo->setText(in.readAll());
        }
    }
}

//时间更新
void MainWindow::timeUpdate()
{
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString("yyyy年MM月dd日  hh:mm:ss"); // 设置显示的格式
    currentTimeLabel->setText(Timestr);
    ui->statusBar->addPermanentWidget(currentTimeLabel);
    if(serialPort->isOpen())
    {
        ui->statusBar->showMessage("The serial port has been opened",1000);
    }
    else
    {
        //QLabel *per = new QLabel();
        ui->statusBar->showMessage("Not open the serial port",1000);
    }
}




