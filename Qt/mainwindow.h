#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
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

#define NUM_OF_COLORS 13

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ChannelData{
public:
    QVector<double> x, y;
};

class Graph{
public:
    ChannelData ch[NUM_OF_COLORS];
    double x0 = 0, xn, step;
    int n;

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Graph graph;
    ConnectionForm *ConnectForm;
    SettingsForm *SettingForm;
    QWidget *ChSettingsForm;
    QPushButton *button[12];
    QWidget *chmini[NUM_OF_CHANNELS];
    ChanForm *chan[NUM_OF_CHANNELS];
    QVector <MathForm *> mathForms;
    QPushButton *chbutton[12];
    QVBoxLayout *lay_math;
    QUdpSocket *socket, *getSocket;
    QHostAddress *destIP;
    QByteArray *datagram;
    bool ConnectionSet; // Флаг установки соединения, 1 - связь установлена
    bool badResponse; // В ответ на запрос приходит сообщение об ошибке 03 xx EE
    bool goodResponse;  // В ответ на запрос приходит сообщение о подтверждении выполнения 03 xx FF
    bool wrongCommand;  // Если запрос был повторён и ответ всё ещё 03 xx EE, то отправляемая команда не корректна
    int delayTime = 100; // Задержка между отправкой двух последовательных команд

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

    void on_btn_start_reg_2_clicked();

    void on_btn_start_reg_3_clicked();

    void on_btn_start_reg_4_clicked();

    void on_btn_start_reg_5_clicked();

    void on_btn_start_reg_7_clicked();

    void on_btn_start_reg_6_clicked();

    void on_btn_start_reg_8_clicked();

    void on_btn_add_math_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();



    void on_pushButton_5_clicked();

    void on_btn_call_math_clicked();

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
