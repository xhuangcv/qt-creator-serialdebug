#include "version.h"
#include "ui_version.h"

version::version(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::version)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("版本信息"));
    this->setWindowIcon(QIcon(":/icon/icons/version.ico"));
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

version::~version()
{
    delete ui;
}
