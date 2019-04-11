#include "log.h"
#include "ui_log.h"

#include <QFile>
#include <QTextStream>

log::log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("日志"));
    this->setWindowIcon(QIcon(":/icon/icons/log.ico"));
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    QFile file("log.txt");
    //判断打开文件是否成功
    if (file.open(QIODevice ::ReadOnly | QIODevice ::Text))
    {
        QTextStream textStream(&file);
        while (!textStream.atEnd())
        {
            //QtextEdit按行显示文件内容
            ui->textEdit->append(textStream.readLine());
        }
    }
}

log::~log()
{
    delete ui;
}
