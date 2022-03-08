#ifndef MATHFORM_H
#define MATHFORM_H

#include <QWidget>

namespace Ui {
class MathForm;
}

class MathForm : public QWidget
{
    Q_OBJECT

public:
    explicit MathForm(int m_number, QWidget *parent = nullptr);
    ~MathForm();
    void change_number(int new_number);

private:
    Ui::MathForm *ui;
    int number;

private slots:
    void on_btn_close_clicked();

signals:
    void close_clicked();
};

#endif // MATHFORM_H
