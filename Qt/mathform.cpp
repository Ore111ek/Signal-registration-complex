#include "mathform.h"
#include "ui_mathform.h"


MathForm::MathForm(int m_number, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MathForm)
{
    number = m_number;
    ui->setupUi(this);
    ui->label_m_number->setText("M" + QString::number(m_number));
    on_comboBox_type_currentIndexChanged(0);
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
    //channels.append(1);
    //channels.append(2);

    switch (ui->comboBox_type->currentIndex()){
        case 0:// Разность каналов
            channels.append(combo.at(0)->currentIndex());
            channels.append(combo.at(1)->currentIndex());
        break;
        case 1:// Сумма каналов
            channels.append(combo.at(0)->currentIndex());
            channels.append(combo.at(1)->currentIndex());
        break;
        case 2:// Умножить канал
            channels.append(combo.at(0)->currentIndex());
        break;
        case 3:// Амплитуда канала

        break;
        default:// Частота канала
        break;
    }
    emit math_data_request(number, channels);
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
            emit math_result_graph(number, result_graph);
        break;
        case 1:// Сумма каналов
            for(int i = 0; i<graphs.at(0).count(); i++){
                one_graph.append(graphs.at(0).at(i)+graphs.at(1).at(i));
            }
            result_graph.append(one_graph);
            emit math_result_graph(number, result_graph);
        break;
        case 2:// Умножить канал
            for(int i = 0; i<graphs.at(0).count(); i++){
                one_graph.append(graphs.at(0).at(i)*combo.at(1)->currentText().toInt());
            }
            result_graph.append(one_graph);
            emit math_result_graph(number, result_graph);
        break;
        case 3:// Амплитуда канала
            emit math_result_number(number, result_number);
        break;
        default:// Частота канала
        break;
    }

}


void MathForm::on_comboBox_type_currentIndexChanged(int index)
{
    delete ui->groupBox_settings->layout();
    for(int i = 0; i < combo.count(); i++){
        delete combo.at(i);
    }
    for(int i = 0; i < label.count(); i++){
        delete label.at(i);
    }
    //for(int i = 0; i < lay.count(); i++){
      //  lay.at(i).;
    //}
    combo.clear();
    label.clear();
    lay.clear(); // Возможно удалять только layout и тогда удалятся все его элементы
    combo.squeeze();
    label.squeeze();
    lay.squeeze();
    switch (index){
        case 0:
            combo.append(new QComboBox());
            combo.append(new QComboBox());
            //combo[0] = new QComboBox();
            //combo[1] = new QComboBox();
            for(int i = 1; i <= NUM_OF_CHANNELS; i++){
                //combo[0]->addItem(QString::number(i));
                //combo[1]->addItem(QString::number(i));
                combo.at(0)->addItem("К" + QString::number(i));
                combo.at(1)->addItem("К" + QString::number(i));
            }
            label.append(new QLabel());
            label.at(0)->setText("-");
            label.at(0)->setAlignment(Qt::AlignHCenter);
            label.at(0)->setFixedHeight(20);
            //label[0] = new QLabel();
            //label1->setText("-");
            lay.append(new QHBoxLayout());
            //QHBoxLayout *lay = new QHBoxLayout();
            lay.at(0)->addWidget(combo.at(0));
            lay.at(0)->addWidget(label.at(0));
            lay.at(0)->addWidget(combo.at(1));
            //lay->addWidget(combo1);
            //lay->addWidget(label1);
            //lay->addWidget(combo2);
            ui->groupBox_settings->setLayout(lay.at(0));
            //ui->groupBox_settings->setLayout(lay);
        break;
        case 1:
            combo.append(new QComboBox());
            combo.append(new QComboBox());
            for(int i = 1; i <= NUM_OF_CHANNELS; i++){
                combo.at(0)->addItem("К" + QString::number(i));
                combo.at(1)->addItem("К" + QString::number(i));
            }
            label.append(new QLabel());
            label.at(0)->setText("+");
            label.at(0)->setAlignment(Qt::AlignHCenter);
            label.at(0)->setFixedHeight(20);
            lay.append(new QHBoxLayout());
            lay.at(0)->addWidget(combo.at(0));
            lay.at(0)->addWidget(label.at(0));
            lay.at(0)->addWidget(combo.at(1));
            ui->groupBox_settings->setLayout(lay.at(0));
        break;
        case 2:
            combo.append(new QComboBox());
            combo.append(new QComboBox());
            for(int i = 1; i <= NUM_OF_CHANNELS; i++){
                combo.at(0)->addItem("К" + QString::number(i));
            }
            for(int i = 1; i <= 10; i++){
                combo.at(1)->addItem(QString::number(i));
            }
            label.append(new QLabel());
            label.at(0)->setText("*");
            label.at(0)->setAlignment(Qt::AlignHCenter);
            label.at(0)->setFixedHeight(20);
            lay.append(new QHBoxLayout());
            lay.at(0)->addWidget(combo.at(0));
            lay.at(0)->addWidget(label.at(0));
            lay.at(0)->addWidget(combo.at(1));
            ui->groupBox_settings->setLayout(lay.at(0));
        break;
        case 3:
        break;
        case 4:
        break;
        case 5:
        break;
        case 6:
        break;
        case 7:
        break;
        case 8:
        break;
        case 9:
        break;
        case 10:
        break;
        case 11:
        break;
        case 12:
        break;
        case 13:
        break;
        case 14:
        break;
        case 15:
        break;
        default:
        break;
    }
}

