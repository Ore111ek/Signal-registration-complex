#ifndef MATHFORM_H
#define MATHFORM_H

#include <QWidget>
#include "DeviceConstants.h"
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>

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
    QVector <QComboBox *> combo;
    QVector <QLabel *> label;
    QVector <QHBoxLayout *> lay;

private slots:
    void on_btn_close_clicked();

    void on_comboBox_type_currentIndexChanged(int index);

public slots:
    void on_math_request();
    void on_math_send_data(QVector <QVector <double>> graphs);

signals:
    void close_clicked();
    void math_update_settings(); //используется вообще?
    void math_data_request(int num_math, QVector <int> channels);
    void math_result_graph(int num_math, QVector <QVector <double>> graphs);
    void math_result_number(int num_math, QVector <double> numbers);
};

#endif // MATHFORM_H
