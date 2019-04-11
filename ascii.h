#ifndef ASCII_H
#define ASCII_H

#include <QDialog>

namespace Ui {
class ascii;
}

class ascii : public QDialog
{
    Q_OBJECT

public:
    explicit ascii(QWidget *parent = 0);
    ~ascii();

private:
    Ui::ascii *ui;
};

#endif // ASCII_H
