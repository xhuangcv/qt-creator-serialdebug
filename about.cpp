#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("关于我们"));
    this->setWindowIcon(QIcon(":/icon/icons/us.ico"));
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

about::~about()
{
    delete ui;
}
