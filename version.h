#ifndef VERSION_H
#define VERSION_H

#include <QDialog>

namespace Ui {
class version;
}

class version : public QDialog
{
    Q_OBJECT

public:
    explicit version(QWidget *parent = 0);
    ~version();

private:
    Ui::version *ui;
};

#endif // VERSION_H
