#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include "connectionform.h"
#include "chminiform.h"

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
    QPushButton *chbutton[12];

    void initialize_graph();
private slots:
    void on_btn_conForm_clicked();
    void on_btn_addMath_clicked();
    void on_mini_ch_close_clicked();
    void on_btn_ch_clicked();
};
#endif // MAINWINDOW_H
