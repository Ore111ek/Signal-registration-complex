#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMoveEvent>
#include <QUdpSocket>
#include "connectionform.h"
#include "chminiform.h"
#include "chanform.h"

#define NUM_OF_COLORS 13
#define NUM_OF_CHANNELS 8
#define MAX_MEASUREMENTS 4

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
    QPushButton *button[12];
    QWidget *chmini[8];
    QWidget *chan[8];
    QPushButton *chbutton[12];
    QUdpSocket *socket;

    QHostAddress *destIP;

    void initialize_graph();
    void graph_setDark();
    void graph_setLight();

protected:
    void keyPressEvent(QKeyEvent * e) override; //А НУЖНЫ ЛИ
    void keyReleaseEvent(QKeyEvent * e) override; // НАЖАТИЯ КНОПОК КЛАВИАТУРЫ?? ПРИДУМАТЬ ГОРЯЧИЕ КЛАВИШИ
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void moveEvent(QMoveEvent * event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_btn_conForm_clicked();
    void on_btn_addMath_clicked();
    void on_mini_ch_close_clicked();
    void on_btn_ch_clicked();
    void on_btn_graph_theme_clicked();
    void on_btn_save_graph_pdf_clicked();
    void on_btn_save_graph_png_clicked();
    void on_btn_save_graph_jpg_clicked();
    void on_btn_save_graph_bmp_clicked();
    void on_btn_graph_zone_clicked();
    void on_btn_graph_zoominX_clicked();
    void on_btn_graph_zoomoutX_clicked();
    void on_btn_graph_zoominY_clicked();
    void on_btn_graph_zoomoutY_clicked();
    void on_btn_graph_zoomin_clicked();
    void on_btn_graph_zoomout_clicked();
    void readyReadUDP();
    void on_btn_start_reg_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_mousePress_subwidget(QMouseEvent *event);

public slots:
    void ping_device(QString ip_str);
    void hide_subwidgets();

signals:
    void ping_response(bool state);
};
#endif // MAINWINDOW_H
