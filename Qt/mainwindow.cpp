#include "mainwindow.h"
#include "ui_mainwindow.h"

QColor graph_palette[NUM_OF_COLORS] = {
// 1-4 channels
    Qt::yellow,
    QColor(120, 250, 120), // Green
    QColor(79, 255, 249), //Electric Blue
    Qt::red,
// 5-8 channels
    QColor(250, 100, 50), // Orange
    QColor(21, 189, 9), // Islamic Green
    Qt::blue,
    QColor(188, 59, 227),  //Purple
//for measurements
    QColor(255, 178, 46), // Yellow Orange
    QColor(255, 36, 226), //Barbie Pink
    QColor(255, 182, 193),  //Light Pink
    QColor(160, 130, 100), // Brown
    QColor(200, 200, 180) // Grey
};



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ConnectForm = new ConnectionForm;
    connect(ui->btn_conForm, SIGNAL (clicked()), this, SLOT (on_btn_conForm_clicked()));

    for(int j = 0; j < NUM_OF_COLORS; j++){
        graph.ch[j].x.clear();
        graph.ch[j].y.clear();
        for(double i = 0; i <= 100; i += 0.1){
            graph.ch[j].x.append(i);
            graph.ch[j].y.append(sin(i + j) + j/2);
        }
    }

    initialize_graph();

    //test
/*
    // создаем кнопки
    for(int i = NUM_OF_CHANNELS+MAX_MEASUREMENTS-1; i >= 0; i--){
        if(i < NUM_OF_CHANNELS) button[i] = new QPushButton(QString::number(i+1), this);
        else button[i] = new QPushButton("Изм. " + QString::number(i+1 - NUM_OF_CHANNELS), this);
        ui->Layout_bottom->insertWidget(0,button[i]);
        button[i]->hide();
    }
    */

    for(int i = NUM_OF_CHANNELS-1; i >= 0; i--){
        chmini[i] = new ChMiniForm(i+1);
        ui->Layout_bottom->insertWidget(0,chmini[i]);
        chmini[i]->setProperty("objectName","ChMini" + QString::number(i+1));
        chmini[i]->hide();
    }
    chmini[1]->setStyleSheet("ChMiniForm{border: 1px solid #060606;}");//???Не работает((((
    chmini[3]->setStyleSheet("ChMiniForm{background-color: #dfdfdf;}");// ??? ((((
    /*
    chmini[1] = new ChMiniForm(2);
    ui->Layout_bottom->insertWidget(0,chmini[1]);
    chmini[0] = new ChMiniForm(1);
    ui->Layout_bottom->insertWidget(0,chmini[0]);
    chmini[0]->show();
  */
    chbutton[0] = ui->btn_ch1;
    chbutton[1] = ui->btn_ch2;
    chbutton[2] = ui->btn_ch3;
    chbutton[3] = ui->btn_ch4;
    chbutton[4] = ui->btn_ch5;
    chbutton[5] = ui->btn_ch6;
    chbutton[6] = ui->btn_ch7;
    chbutton[7] = ui->btn_ch8;
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        connect(chbutton[i], SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
        connect(chmini[i], SIGNAL (close_clicked()), this, SLOT (on_mini_ch_close_clicked()));
    }

/*    connect(ui->btn_ch1, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch2, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch3, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch4, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch5, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch6, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch7, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
    connect(ui->btn_ch8, SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked())); */

}

void MainWindow::initialize_graph(){
    // set some pens, brushes and backgrounds:
    ui->widget->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->widget->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->widget->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->widget->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->widget->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->widget->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->widget->xAxis->setTickLabelColor(Qt::white);
    ui->widget->yAxis->setTickLabelColor(Qt::white);

    ui->widget->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->widget->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    //ui->widget->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //ui->widget->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //ui->widget->xAxis->grid()->setSubGridVisible(true);
    //ui->widget->yAxis->grid()->setSubGridVisible(true);
    ui->widget->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->widget->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->widget->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->widget->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->widget->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->widget->axisRect()->setBackground(axisRectGradient);
   // customPlot->rescaleAxes();
   // customPlot->yAxis->setRange(0, 2);

    ui->widget->xAxis->setRange(0,100);
    ui->widget->yAxis->setRange(-1,4);
    for (int j = 0; j < NUM_OF_COLORS; j++){
        ui->widget->addGraph();
        ui->widget->graph(j)->addData(graph.ch[j].x, graph.ch[j].y);
        ui->widget->graph(j)->setPen(QPen(graph_palette[j], 1.2));
    }
    ui->widget->replot();

    ui->widget->setInteraction(QCP::iRangeZoom, true);
    ui->widget->setInteraction(QCP::iRangeDrag, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_conForm_clicked()
{
    ConnectForm->show();
}

void MainWindow::on_btn_ch_clicked()
{
    int n = sender()->property("objectName").toString()[6].unicode()-49;
    chmini[n]->show();
    sender()->setProperty("visible",false);
    ui->widget->graph(n)->setVisible(true);
    ui->widget->replot();
}

void MainWindow::on_mini_ch_close_clicked()
{
    sender()->setProperty("visible",false);
    int n = sender()->property("objectName").toString()[6].unicode()-49;
    chbutton[n]->show();
    ui->widget->graph(n)->setVisible(false);
    ui->widget->replot();
}

void MainWindow::on_btn_addMath_clicked()
{

}

