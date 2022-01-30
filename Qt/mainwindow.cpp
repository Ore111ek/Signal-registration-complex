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

    for(int j = 0; j < NUM_OF_COLORS; j++){
        graph.ch[j].x.clear();
        graph.ch[j].y.clear();
        for(double i = 0; i <= 100; i += 0.1){
            graph.ch[j].x.append(i);
            graph.ch[j].y.append(sin(i + j) + j/2);
        }
    }

    initialize_graph();
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

