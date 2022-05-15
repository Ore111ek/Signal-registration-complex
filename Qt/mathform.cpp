#include "mathform.h"
#include "ui_mathform.h"

const double TwoPi = 6.283185307179586;
// AVal - массив анализируемых данных, Nvl - длина массива должна быть кратна степени 2.
// FTvl - массив полученных значений, Nft - длина массива должна быть равна Nvl.
void FFTAnalysis(QVector<double> AVal, QVector<double> *FTvl, int Nvl, int Nft) {
  int i, j, n, m, Mmax, Istp;
  double Tmpr, Tmpi, Wtmp, Theta;
  double Wpr, Wpi, Wr, Wi;
  double *Tmvl;

  n = Nvl * 2; Tmvl = new double[n];
  for (i = 0; i < n; i+=2) {
   Tmvl[i] = 0;
   Tmvl[i+1] = AVal[i/2];
  }
  i = 1; j = 1;
  while (i < n) {
    if (j > i) {
      Tmpr = Tmvl[i]; Tmvl[i] = Tmvl[j]; Tmvl[j] = Tmpr;
      Tmpr = Tmvl[i+1]; Tmvl[i+1] = Tmvl[j+1]; Tmvl[j+1] = Tmpr;
    }
    i = i + 2; m = Nvl;
    while ((m >= 2) && (j > m)) {
      j = j - m; m = m >> 1;
    }
    j = j + m;
  }
  Mmax = 2;
  while (n > Mmax) {
    Theta = -TwoPi / Mmax; Wpi = sin(Theta);
    Wtmp = sin(Theta / 2); Wpr = Wtmp * Wtmp * 2;
    Istp = Mmax * 2; Wr = 1; Wi = 0; m = 1;
    while (m < Mmax) {
      i = m; m = m + 2; Tmpr = Wr; Tmpi = Wi;
      Wr = Wr - Tmpr * Wpr - Tmpi * Wpi;
      Wi = Wi + Tmpr * Wpi - Tmpi * Wpr;
      while (i < n) {
        j = i + Mmax;
        Tmpr = Wr * Tmvl[j] - Wi * Tmvl[j-1];
        Tmpi = Wi * Tmvl[j] + Wr * Tmvl[j-1];

        Tmvl[j] = Tmvl[i] - Tmpr; Tmvl[j-1] = Tmvl[i-1] - Tmpi;
        Tmvl[i] = Tmvl[i] + Tmpr; Tmvl[i-1] = Tmvl[i-1] + Tmpi;
        i = i + Istp;
      }
    }
    Mmax = Istp;
  }
  for (i = 0; i < Nft; i++) {
    j = i * 2; FTvl->append(2*sqrt(pow(Tmvl[j],2) + pow(Tmvl[j+1],2))/Nvl);
  }
  delete []Tmvl;
}

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
        case 5:// Фурье
            channels.append(combo.at(0)->currentIndex());
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
                one_graph.append((graphs.at(0).at(i))*combo.at(1)->currentText().toInt());
            }
            result_graph.append(one_graph);
            emit math_result_graph(number, result_graph);
        break;
        case 3:// Амплитуда канала
            emit math_result_number(number, result_number);
        break;
        case 4:// Амплитуда канала
            emit math_result_number(number, result_number);
        break;
        case 5:// Фурье
            //double sample_rate = 1600000000; Вроде уже mainwindow будет выставлять измерение на yAxis2
        {//Без скобок ошибка из-за объявления переменных внутри case
            int num = graphs.at(0).length();
            int power = -1;
            while(num > 0){
                num /= 2;
                power++;
            }
            num = pow(2,power);
            // graphs.at(0) - массив анализируемых данных, num - длина массива должна быть кратна степени 2.
            // fftgraph - массив полученных значений, num - длина массива должна быть равна Nvl.
            QVector <double> fftgraph;
            FFTAnalysis(graphs.at(0), &fftgraph, num, num);
            QVector <double> fftresult;
            int size = graphs.at(0).length();
            for(int i = 0; i < size; i++){
                fftresult.append(fftgraph.at((int)(i*(num/2-1)/(size-1))));
            }
            result_graph.append(fftresult);
            emit math_result_graph(number, result_graph);
        }
        break;
        default:// Частота канала
        break;
    }

}


void MathForm::on_comboBox_type_currentIndexChanged(int index)
{
    delete ui->groupBox_settings->layout();
    for(int i = 0; i < combo.count(); i++){
        delete combo[i];
    }
    for(int i = 0; i < label.count(); i++){
        delete label[i];
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
                combo[0]->addItem("К" + QString::number(i));
                combo[1]->addItem("К" + QString::number(i));
            }
            label.append(new QLabel());
            label[0]->setText("-");
            label[0]->setAlignment(Qt::AlignHCenter);
            label[0]->setFixedHeight(20);
            //label[0] = new QLabel();
            //label1->setText("-");
            lay.append(new QHBoxLayout());
            //QHBoxLayout *lay = new QHBoxLayout();
            lay[0]->addWidget(combo[0]);
            lay[0]->addWidget(label[0]);
            lay[0]->addWidget(combo[1]);
            //lay->addWidget(combo1);
            //lay->addWidget(label1);
            //lay->addWidget(combo2);
            ui->groupBox_settings->setLayout(lay[0]);
            //ui->groupBox_settings->setLayout(lay);
        break;
        case 1:
            combo.append(new QComboBox());
            combo.append(new QComboBox());
            for(int i = 1; i <= NUM_OF_CHANNELS; i++){
                combo[0]->addItem("К" + QString::number(i));
                combo[1]->addItem("К" + QString::number(i));
            }
            label.append(new QLabel());
            label[0]->setText("+");
            label[0]->setAlignment(Qt::AlignHCenter);
            label[0]->setFixedHeight(20);
            lay.append(new QHBoxLayout());
            lay[0]->addWidget(combo[0]);
            lay[0]->addWidget(label[0]);
            lay[0]->addWidget(combo[1]);
            ui->groupBox_settings->setLayout(lay[0]);
        break;
        case 2:
            combo.append(new QComboBox());
            combo.append(new QComboBox());
            for(int i = 1; i <= NUM_OF_CHANNELS; i++){
                combo[0]->addItem("К" + QString::number(i));
            }
            for(int i = 1; i <= 50; i++){
                combo[1]->addItem(QString::number(i));
            }
            label.append(new QLabel());
            label[0]->setText("*");
            label[0]->setAlignment(Qt::AlignHCenter);
            label[0]->setFixedHeight(20);
            lay.append(new QHBoxLayout());
            lay[0]->addWidget(combo[0]);
            lay[0]->addWidget(label[0]);
            lay[0]->addWidget(combo[1]);
            ui->groupBox_settings->setLayout(lay[0]);
        break;
        case 3:
        break;
        case 4:
        break;
        case 5:
            combo.append(new QComboBox());
            for(int i = 1; i <= NUM_OF_CHANNELS; i++){
                combo[0]->addItem("К" + QString::number(i));
            }
            lay.append(new QHBoxLayout());
            lay[0]->addWidget(combo[0]);
            ui->groupBox_settings->setLayout(lay[0]);
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

