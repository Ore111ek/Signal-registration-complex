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

public slots:
    void on_math_request();
    void on_math_send_data(QVector <QVector <double>> graphs);

signals:
    void close_clicked();
    void math_update_settings();
    void math_data_request(QVector <int>);
    void math_result_graph(QVector <QVector <double>> graphs);
    void math_result_number(QVector <double> numbers);
};

#endif // MATHFORM_H
