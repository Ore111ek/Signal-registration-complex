#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QProgressDialog>
#include <QProgressBar>
#include <QLayout>
#include <QBoxLayout>
#include <QVector>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMoveEvent>
#include <QUdpSocket>

#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include "connectionform.h"
#include "settingsform.h"
#include "chminiform.h"
#include "chanform.h"
#include "mathform.h"
#include "DeviceConstants.h"
#include "qcustomplot.h"

#define NUM_OF_COLORS 13
#define MAX_MEASUREMENTS 10

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GraphData{
    QVector<double> x_, y_; // в точках и отсчётах(кодах) АЦП
    QVector<double> x_ns_, x_mcs_, x_ms_, x_s_; // нс, мкс, мс, с
    QVector<double> y_mv_, y_v_; // мВ, В
    double sample_rate = 1600000000;
    // Расчёт для всех единиц измерения
    void add_x(QVector<double> x){ // x в точках
        for(int i = 0; i < x.size(); i++){
            x_.append(x.at(i));
            x_s_.append(x.at(i)/sample_rate);
            x_ms_.append(x.at(i)/(sample_rate/1000));
            x_mcs_.append(x.at(i)/(sample_rate/1000000));
            x_ns_.append(x.at(i)/(sample_rate/1000000000));
        }
    };
    void add_y(QVector<double> y){ // y в отсчётах АЦП ( от 0 до 4096)
        for(int i = 0; i < y.size(); i++){
            y_.append(y.at(i));
            y_v_.append(y.at(i)*cal[20*ch + 4*(4-range) + 2*resist] + cal[20*ch + 4*(4-range) + 2*resist +1]);
            y_mv_.append((y.at(i)*cal[20*ch + 4*(4-range) + 2*resist] + cal[20*ch + 4*(4-range) + 2*resist +1]) * 1000 );
        }
    };
public:
    int resist; // 1 - 1 МОм, 0 - 50 Ом
    int range; // 0 - +-5В, .. , 4 - +-0.1В
    int ch; // 0 - 1 канал
    // Доступно чтение в любой единице измерения
    QVector<double> x(){return x_;};
    QVector<double> y(){return y_;};
    QVector<double> x_ns(){return x_ns_;};
    QVector<double> x_mcs(){return x_mcs_;};
    QVector<double> x_ms(){return x_ms_;};
    QVector<double> x_s(){return x_s_;};
    QVector<double> y_mv(){return y_mv_;};
    QVector<double> y_v(){return y_v_;};
    // Загрузка данных
    void clear_data(){
        x_.clear(); x_s_.clear(); x_ms_.clear(); x_mcs_.clear(); x_ns_.clear();
        y_.clear(); y_mv_.clear(); y_v_.clear();
    };
    void set_data(QVector<double> x, QVector<double> y){
        clear_data();
        add_data(x,y);
    }
    void add_data(QVector<double> x, QVector<double> y){
        add_x(x);
        add_y(y);
    }
    // Установка частоты дискретизации
    void set_sample_rate(double f){sample_rate = f;};
};

class Graph{
public:
    GraphData data;
    double offsetx = 0, offsety = 0;
    double factorx = 1, factory = 1;
    double stepx = 1, stepy = 1; // Единицы измерения Отсчёты/мкс/нс КодАЦП/В/мВ
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProgressBar *progressBar;
    QProgressDialog *progressDialog;
    QVector<Graph> chGraphs, mathGraphs;
    GraphData chGrData[NUM_OF_CHANNELS];
    QVector <QVector<double>> mathGrData;
    ConnectionForm *ConnectForm;
    SettingsForm *SettingForm;
    QWidget *ChSettingsForm;
    QPushButton *button[12];
    QWidget *chmini[NUM_OF_CHANNELS];
    ChanForm *chan[NUM_OF_CHANNELS];
    QVector <MathForm *> mathForms;
    QPushButton *chbutton[12];
    QVBoxLayout *lay_math;
    QLabel *label_logo;
    QUdpSocket *socket, *getSocket;
    QHostAddress *destIP;
    QByteArray *datagram;
    bool ConnectionSet; // Флаг установки соединения, 1 - связь установлена
    bool badResponse; // В ответ на запрос приходит сообщение об ошибке 03 xx EE
    bool goodResponse;  // В ответ на запрос приходит сообщение о подтверждении выполнения 03 xx FF
    bool wrongCommand;  // Если запрос был повторён и ответ всё ещё 03 xx EE, то отправляемая команда не корректна
    int delayTime = 100; // Задержка между отправкой двух последовательных команд
    double sample_rate = 1600000000;
    QCPItemTracer *tracer1 = nullptr;
    QCPItemTracer *tracer2 = nullptr;

    QStringList chansCSV[NUM_OF_CHANNELS];

    //Временно?
    int cur_fpga = 0;
    int cur_x[NUM_OF_CHANNELS];
    int b_channel=1;
    //


    void initialize_graph();
    void graph_setDark();
    void graph_setLight();

    void delayms();
protected:
    void keyPressEvent(QKeyEvent * e) override; //А НУЖНЫ ЛИ
    void keyReleaseEvent(QKeyEvent * e) override; // НАЖАТИЯ КНОПОК КЛАВИАТУРЫ?? ПРИДУМАТЬ ГОРЯЧИЕ КЛАВИШИ
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void moveEvent(QMoveEvent * event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void update_ranges_of_Graph();

    void on_btn_conForm_clicked();
    void on_mini_ch_close_clicked();
    void on_btn_ch_clicked();
    void on_btn_math_close_clicked();

    void on_btn_save_graph_pdf_clicked();
    void on_btn_save_graph_png_clicked();
    void on_btn_save_graph_jpg_clicked();
    void on_btn_save_graph_bmp_clicked();
    void on_btn_save_graph_csv_clicked();
    void on_btn_load_graph_csv_choice_clicked();
    void on_btn_load_graph_csv_default_clicked();
    void WriteToCSV(const QList<QStringList>& points);
    QList<QStringList> ReadCSV(QString filename);
    void load_graph_from_csv(QList<QStringList> points);

    void on_btn_graph_zone_clicked();
    void on_btn_graph_zoominX_clicked();
    void on_btn_graph_zoomoutX_clicked();
    void on_btn_graph_zoominY_clicked();
    void on_btn_graph_zoomoutY_clicked();
    void on_btn_graph_zoomin_clicked();
    void on_btn_graph_zoomout_clicked();
    void on_btn_graph_theme_clicked();

    void readyReadUDP();
    void on_btn_start_reg_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_mousePress_subwidget(QMouseEvent *event);

    void on_pushButton_clicked();

    void on_btn_settings_clicked();

    void on_btn_add_math_clicked();

    void on_btn_call_math_clicked();

    void update_measureXY();

    void on_btn_clear_RAM_clicked();

    void on_btn_set_pre_rec_clicked();

    void on_btn_read_part_clicked();

    void on_spinBox_3_textChanged(const QString &arg1);

    void on_btn_read_all_clicked();

    void on_btn_tracer_clicked();

    void on_btn_tracer2_clicked();

    void on_btn_tr_time_clicked();

    void on_btn_tr_freq_clicked();

    void on_btn_tr_max_min_clicked();

    void on_btn_tr_clear_clicked();

    void on_btn_tr_ampl_clicked();

    void on_comboBox_tr_ch_currentIndexChanged(int index);



    void on_combo_measureY_currentTextChanged(const QString &arg1);

    void on_combo_measureX_currentTextChanged(const QString &arg1);

    void on_comboBox_tr2_ch_currentIndexChanged(int index);

    void on_btn_tr_fourier_clicked();

public slots:
    void ping_device(QString ip_str);
    void handleChSettings(int channel, int range, bool resist, int offset, int filter);
    void hide_subwidgets();

    void on_math_update_settings();
    void on_math_data_request(int math_num, QVector<int> channels);
    void on_math_result_graph(int math_num, QVector<QVector<double>> graphs);
    void on_math_result_number(int math_num, QVector<double> numbers);

signals:
    void ping_response(bool state);
    void save_newChSettings(int channel,int range,bool resist,int offset,int filter);
    void math_request();
    void math_send_data(QVector<QVector<double>> graphs);
};
#endif // MAINWINDOW_H
