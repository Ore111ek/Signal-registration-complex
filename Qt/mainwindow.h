#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

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

     void initialize_graph();
};
#endif // MAINWINDOW_H
