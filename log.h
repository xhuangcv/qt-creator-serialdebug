#ifndef LOG_H
#define LOG_H

#include <QDialog>

namespace Ui {
class log;
}

class log : public QDialog
{
    Q_OBJECT

public:
    explicit log(QWidget *parent = 0);
    ~log();

private:
    Ui::log *ui;
};

#endif // LOG_H
