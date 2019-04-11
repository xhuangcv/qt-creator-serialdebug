#include "ascii.h"
#include "ui_ascii.h"

#include <QLabel>

ascii::ascii(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ascii)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("ASCII码表"));
    this->setWindowIcon(QIcon(":/icon/icons/ASCII.ico"));
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    //显示图片
    QImage *image=new QImage(":/image/images/ASCII.jpg");

    QLabel *label=new QLabel(this);
    label->resize(image->width(), image->height());
    label->setPixmap(QPixmap::fromImage(*image));
    label->show();
}

ascii::~ascii()
{
    delete ui;
}
