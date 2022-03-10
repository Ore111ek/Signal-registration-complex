#include "mathform.h"
#include "ui_mathform.h"

MathForm::MathForm(int m_number, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MathForm)
{
    number = m_number;
    ui->setupUi(this);
    ui->label_m_number->setText("M" + QString::number(m_number));
}

MathForm::~MathForm()
{
    delete ui;
}

void MathForm::change_number(int new_number)
{
    number = new_number;
    ui->label_m_number->setText("M" + QString::number(new_number));
}


void MathForm::on_btn_close_clicked()
{
    emit close_clicked();
}

void MathForm::on_math_request()
{
    QVector <int> channels;
    //Поменять
    channels.append(1);
    channels.append(2);

    emit math_data_request(channels);
}

void MathForm::on_math_send_data(QVector<QVector<double>> graphs)
{
    QVector <QVector <double>> result_graph;
    QVector <double> one_graph;
    QVector <double> result_number;
    switch (ui->comboBox_type->currentIndex()){
        case 0:// Разность каналов
            for(int i = 0; i<graphs.at(0).count(); i++){
                one_graph.append(graphs.at(0).at(i)-graphs.at(1).at(i));
            }
            result_graph.append(one_graph);
            emit math_result_graph(result_graph);
        break;
        case 1:// Сумма каналов
            for(int i = 0; i<graphs.at(0).count(); i++){
                one_graph.append(graphs.at(0).at(i)+graphs.at(1).at(i));
            }
            result_graph.append(one_graph);
            emit math_result_graph(result_graph);
        break;
        case 2:// Амплитуда канала
            emit math_result_number(result_number);
        break;
        default:// Частота канала
        break;
    }

}

