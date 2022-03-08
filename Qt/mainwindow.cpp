#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

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

void MainWindow::delayms()
{
    QTime dieTime= QTime::currentTime().addMSecs(delayTime);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

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

    connect(ui->customPlot, SIGNAL (mousePress(QMouseEvent *)), this, SLOT (on_mousePress_subwidget(QMouseEvent *)));
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

        // Новые строки
        chan[i] = new ChanForm(i+1);
        chan[i]->hide();
        chan[i]->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
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
        // Прятать настройки каналов при нажатии на кнопки
        connect(chbutton[i], SIGNAL (clicked()), this, SLOT (hide_subwidgets()));
        connect(chmini[i], SIGNAL (close_clicked()), this, SLOT (hide_subwidgets()));
        connect(chmini[i], SIGNAL (edit_clicked(QPoint)), this, SLOT (hide_subwidgets()));
        // Открытия мини форм и нажатие кнопок на них
        connect(chbutton[i], SIGNAL (clicked()), this, SLOT (on_btn_ch_clicked()));
        connect(chmini[i], SIGNAL (close_clicked()), this, SLOT (on_mini_ch_close_clicked()));
        connect(chmini[i], SIGNAL (edit_clicked(QPoint)), chan[i], SLOT (show_settings(QPoint)));
        // Обработка настроек канала
        connect(chan[i], SIGNAL (sendChSettings(int,int,bool,int,int)), this, SLOT (handleChSettings(int,int,bool,int,int)));
        connect(this, SIGNAL (save_newChSettings(int,int,bool,int,int)), chmini[i], SLOT (update_settings(int,int,bool,int,int)));
    }

    //lay_math = new QVBoxLayout();
    //QPushButton *btn_addMath = new QPushButton("Добавить измерение");
    //btn_addMath->setMinimumHeight(30);
    //btn_addMath->setStyleSheet("QPushButton {\n border: 1px solid #8f8f91;\n border-radius: 6px;\n background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n stop: 0 #f6f7fa, stop: 1 #dadbde);\n min-width: 60px;\n }\n\nQPushButton:hover {\n border: 1px solid #bc3be3;\n border-radius: 6px;\n background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n                                       stop: 0 #f6f7fa, stop: 1 #dadbde);\n     min-width: 60px;\n }\n\n QPushButton:pressed {\n     background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\n                                       stop: 0 #dadbde, stop: 1 #f6f7fa);\n }\n\n QPushButton:flat {\n     border: none; /* для плоской кнопки границы нет */\n }\n\n QPushButton:default {\n     border-color: navy; /* делаем кнопку по умолчанию выпуклой */\n }");
    //ui->scrollAreaWidgetContents->layout()->addWidget(btn_addMath);
    //lay_math->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Minimum, QSizePolicy::Expanding));
    //lay_math->addWidget(btn_addMath);
    //ui->scrollArea_math->setLayout(lay_math);
    //connect(btn_addMath, SIGNAL (clicked()), this, SLOT (on_btn_add_math_clicked()));


    SettingForm = new SettingsForm(nullptr, chan);

    //ChSettingsForm = new QWidget();
    //ChSettingsForm->setWindowTitle("Панель настроек");
    //ChSettingsForm->setAttribute(Qt::WA_DeleteOnClose,true);
    //connect(ChSettingsForm , SIGNAL(destroyed()), this,SLOT(releaseChFromLayout()));

    /*
    QGridLayout *layoutChSettings = new QGridLayout;
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        layoutChSettings->addWidget(chan[i], int(i/4), i%4);
        chan[i]->show();
    }
    ChSettingsForm->setLayout(layoutChSettings);
    ChSettingsForm->show();
    */

/*
        QHBoxLayout *hlayout=new QHBoxLayout;
        hlayout->setAlignment(Qt::AlignTop);
        hlayout->addWidget(x);
        layout->addChildLayout(hlayout);


    this->setLayout(layout);
        this->show();

    for(int i = 0; i < NUM_OF_CHANNELS; i++){

    }

    win = new QWidget();
    layout=new QGridLayout;
    //layout->setAlignment(Qt::AlignTop);

    for(int i =0; i <8; i++){
        //layout->addWidget(chform[i]);
        layout->addWidget(chform[i], int(i/4), i%4);
    }
    win->setLayout(layout);
   // win->setMinimumSize(600,600);
    win->show();

    */

    // Network
    socket = new QUdpSocket(this);
    getSocket = new QUdpSocket(this);
    getSocket->bind(4000, QUdpSocket::ShareAddress); // Прослушиваем порт 4000
    destIP = new QHostAddress("132.9.36.64");
    socket->connectToHost(*destIP,2054,QIODevice::WriteOnly);
    datagram = new QByteArray();
    ConnectionSet = true;//false на самом деле
    badResponse = false;
    goodResponse = false;
    wrongCommand = false;

    connect(getSocket,SIGNAL(readyRead()),this,SLOT(readyReadUDP()));// Функция обработки входящих пакетов от МК

    ConnectForm = new ConnectionForm;
    connect(ui->btn_conForm, SIGNAL (clicked()), this, SLOT (on_btn_conForm_clicked()));
    connect(ConnectForm, SIGNAL (ping(QString)), this, SLOT (ping_device(QString)));
    connect(this, SIGNAL (ping_response(bool)), ConnectForm, SLOT (handle_response(bool)));

    // CSV сохранение настроек каналов
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        chansCSV[i].append("Channel Number");
        chansCSV[i].append(QString::number(i+1));
        chansCSV[i].append("Range");
        chansCSV[i].append("");
        chansCSV[i].append("Input Resistance");
        chansCSV[i].append("");
        chansCSV[i].append("DAC code");
        chansCSV[i].append("");
        chansCSV[i].append("Filter");
        chansCSV[i].append("");
    }
    /*
    for(int i = 0; i < MAX_MEASUREMENTS; i++){
        mathForms[i] = new MathForm(i+1);
        ui->scrollAreaWidgetContents->layout()->addWidget(mathForms[i]);
    }*/

}
// Обработка входящих пакетов
void MainWindow::readyReadUDP()
{
    QByteArray buffer;
    buffer.resize(getSocket->pendingDatagramSize()); //320 байт, скорее всего
    QHostAddress sender;
    quint16 senderPort;

    getSocket->readDatagram(buffer.data(), buffer.size(),
                        &sender, &senderPort);

    qDebug() << "Message from: " << sender.toString();
    qDebug() << "Message port: " << senderPort;
    qDebug() << "Message: " << buffer;
    if((unsigned char)datagram->at(1)==0x9A){
        ui->spinBox_2->setValue((unsigned char)buffer.at(0));
    } else if(datagram->at(1)==0x70){
        for(int w = 0; w < 80; w++){
                ui->customPlot->graph(2*cur_fpga)->addData(cur_x[2*cur_fpga], (buffer.at(4*w+1)&0x0F)*256 + (unsigned char)buffer.at(4*w));
                ui->customPlot->graph(2*cur_fpga+1)->addData(cur_x[2*cur_fpga+1], (buffer.at(4*w+3)&0x0F)*256 + (unsigned char)buffer.at(4*w+2));
                cur_x[2*cur_fpga]+=1;
                cur_x[2*cur_fpga+1]+=1;
        }
        ui->customPlot->replot();
    }else{
    if(buffer.at(0)==0x02){        // Пришёл ответ из 2 байт
        if(datagram->at(1)==0x02){ // Если отправляли ping
            if(buffer.at(1)==0x03){ // Ответ правильный
                emit ping_response(true); // То связь установлена
                ConnectionSet = true;
            }else {
                emit ping_response(false); // Иначе связь не установлена
                ConnectionSet = false;
            }
        }
    }
    else {  // Пришёл ответ > 2 байт
        switch((unsigned char)buffer.at(1)){ // Аккуратнее, если код команды больше 127, то писать в обратном коде
            case 0x9A://Опрос квитанции FIFO??
                ui->spinBox_2->setValue((unsigned char)buffer.at(2));
            break;
            case 0x70://Чтение пакета FIFO?
/*
            for(int w = 0; w < 80; w++){
                    ui->customPlot->graph(2*cur_fpga)->addData(cur_x[2*cur_fpga], (buffer.at(4*w+1)&0x0F)*256 + buffer.at(4*w));
                    ui->customPlot->graph(2*cur_fpga+1)->addData(cur_x[2*cur_fpga+1], (buffer.at(4*w+3)&0x0F)*256 + buffer.at(4*w+2));
                    cur_x[2*cur_fpga]+=1;
                    cur_x[2*cur_fpga+1]+=1;
            }*/
            /*
                for(int w = 0; w < 160; w++){
                    //ui->customPlot->graph(cur_fpga + (buffer.at(2*w+1)&0xC0)/128)->addData(cur_x[cur_fpga+(buffer.at(2*w+1)&0xC0)/64], (buffer.at(2*w+1)&0x0F)*256 + buffer.at(2*w));
                    switch(buffer.at(2*w+1)&0xC0){
                        case 0x00:
                            ui->customPlot->graph(2*cur_fpga+b_channel)->addData(cur_x[2*cur_fpga+b_channel]+1, (buffer.at(2*w+1)&0x0F)*256 + buffer.at(2*w));
                        break;
                        case 0x40:
                            ui->customPlot->graph(2*cur_fpga+b_channel)->addData(cur_x[2*cur_fpga+b_channel], (buffer.at(2*w+1)&0x0F)*256 + buffer.at(2*w));
                        break;
                        case 0x80:
                            ui->customPlot->graph(2*cur_fpga+b_channel)->addData(cur_x[2*cur_fpga+b_channel]+3, (buffer.at(2*w+1)&0x0F)*256 + buffer.at(2*w));
                        break;
                        case 0xC0:
                            ui->customPlot->graph(2*cur_fpga+b_channel)->addData(cur_x[2*cur_fpga+b_channel]+2, (buffer.at(2*w+1)&0x0F)*256 + buffer.at(2*w));
                        break;
                         default:break;
                    }
                    if(b_channel) b_channel=0;
                    else b_channel=1;
                    if(w % 8 == 7){
                        cur_x[2*cur_fpga]+=4;
                        cur_x[2*cur_fpga+1]+=4;
                    }
                }*/
            break;
            default: // Ответ - это подтверждение выполнения команды
                if((unsigned char)buffer.at(2)==0xFF){
                    //Всё хорошо, обработка не требуется
                    qDebug() << "Message correct: " << buffer;
                    badResponse = false;
                    goodResponse = true;
                }else if((unsigned char)buffer.at(2)==0xEE){
                    //Ошибка
                    qDebug() << "Error: " << buffer;
                    if(!badResponse){
                        socket->write(*datagram); // Повторная отправка пакета
                        delayms();
                        badResponse = true;
                    }else{
                        wrongCommand = true;
                    }
                }
            break;
        }
    }
    }
}

void MainWindow::graph_setDark(){
    // set some pens, brushes and backgrounds:
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);

    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    //ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    //ui->customPlot->xAxis->grid()->setSubGridVisible(true);
    //ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);
}

void MainWindow::graph_setLight(){
    // set some pens, brushes and backgrounds:
    ui->customPlot->xAxis->setBasePen(QPen(Qt::black, 1));
    ui->customPlot->yAxis->setBasePen(QPen(Qt::black, 1));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::black, 1));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::black, 1));
    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::black, 1));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::black, 1));
    ui->customPlot->xAxis->setTickLabelColor(Qt::black);
    ui->customPlot->yAxis->setTickLabelColor(Qt::black);

    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(110, 110, 110), 1, Qt::DotLine));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(110, 110, 110), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(255, 255, 255));
    plotGradient.setColorAt(1, QColor(255, 255, 255));
    ui->customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(350, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(255, 255, 255));
    axisRectGradient.setColorAt(0.5, QColor(255, 255, 255));
    axisRectGradient.setColorAt(1, QColor(240, 240, 240));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Alt){
        //qDebug() << "Alt button pressed";
        ui->customPlot->setSelectionRectMode(QCP::srmZoom);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Alt){
       // qDebug() << "Alt button released";
        ui->customPlot->setSelectionRectMode(QCP::srmNone);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    hide_subwidgets();
    qDebug() << "Left Mouse button pressed";
    /*
    switch (event->button()) {
            case Qt::LeftButton:
            {
                //ui->customPlot->setSelectionRectMode(QCP::srmNone);

                qDebug() << "Left Mouse button pressed";
                break;
            }
            case Qt::RightButton:
            {
                //ui->customPlot->setSelectionRectMode(QCP::srmZoom);

                qDebug() << "Right Mouse button pressed";
                break;
            }
            case Qt::MiddleButton:
            {
                qDebug() << "Middle Mouse button pressed";
                break;
            }
            default:
            {
                qDebug() << "Other button pressed, id = "+QString::number(event->button());
                break;
            }
        } */
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
/*
    switch (event->button()) {
            case Qt::LeftButton:
            {
                qDebug() << "Left Mouse button released";
                break;
            }
            case Qt::RightButton:
            {
                qDebug() << "Right Mouse button released";
                break;
            }
            case Qt::MiddleButton:
            {
                qDebug() << "Middle Mouse button released";
                break;
            }
            default:
            {
                qDebug() << "Other button released, id = "+QString::number(event->button());
                break;
            }
    }
    */
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    //QPoint delta;
    //delta.setX(event->pos().x() - event->oldPos().x());
    //delta.setY(event->pos().y() - event->oldPos().y());
   /* for(int i = 0; i < NUM_OF_CHANNELS; i++){
        if(chan[i]->isVisible()){
            chan[i]->move(chan[i]->x()+delta.x(),chan[i]->y()+delta.y());
            chan[i]->hide();
            chan[i]->show();
        }
    }*/
    hide_subwidgets();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent";
    event->accept(); //delete this;
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        delete chmini[i];
        delete chan[i];
    }
    delete ConnectForm;

}

void MainWindow::initialize_graph(){
    graph_setDark();
   // customPlot->rescaleAxes();
   // customPlot->yAxis->setRange(0, 2);

    ui->customPlot->xAxis->setRange(0,300);
    ui->customPlot->yAxis->setRange(0,4096);
    for (int j = 0; j < NUM_OF_COLORS; j++){
        ui->customPlot->addGraph();
        ui->customPlot->graph(j)->addData(graph.ch[j].x, graph.ch[j].y);
        ui->customPlot->graph(j)->setPen(QPen(graph_palette[j], 1.2));
        ui->customPlot->graph(j)->setVisible(false); // Hide Graphs
    }
    ui->customPlot->replot();

    //Было так
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    //Пробуем приближение прямоугольником
    //ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables); //iSelectPlottables даёт выбрать график, выделить его
    //ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    //ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal); // только по горизонтали приближает и отдаляет колесиком
    //ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, NULL); //To block y axis zoom NULL axis as horizontal
    ui->customPlot->axisRect()->setRangeZoomAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->setSelectionRectMode(QCP::srmNone); //Отключить прямоугольное приближение по умолчанию
}

MainWindow::~MainWindow()
{
    /*for(int i = 0; i < NUM_OF_CHANNELS; i++){
        delete chmini[i];
        delete chan[i];
    }
    delete ConnectForm; */
    delete socket;
    delete destIP;
    delete ui;
    qDebug() << "Destructor";
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
    ui->customPlot->graph(n)->setVisible(true);
    ui->customPlot->replot();
}

void MainWindow::on_mini_ch_close_clicked()
{
    sender()->setProperty("visible",false);
    int n = sender()->property("objectName").toString()[6].unicode()-49;
    chbutton[n]->show();
    ui->customPlot->graph(n)->setVisible(false);
    ui->customPlot->replot();
}

void MainWindow::on_btn_graph_theme_clicked()
{
    if(ui->btn_graph_theme->text() == "Включить\nсветлую\nтему"){
        ui->btn_graph_theme->setText("Включить\nтёмную\nтему");
        graph_setLight();
        ui->customPlot->replot();
    }
    else {
        ui->btn_graph_theme->setText("Включить\nсветлую\nтему");
        graph_setDark();
        ui->customPlot->replot();
    }
}


void MainWindow::on_btn_save_graph_pdf_clicked()
{
    ui->customPlot->savePdf(QFileDialog::getSaveFileName(this,"Сохранить график", QDir::currentPath() + "/images_graphs", "PDF files (*.pdf);"));
}


void MainWindow::on_btn_save_graph_png_clicked()
{
    ui->customPlot->savePng(QFileDialog::getSaveFileName(this,"Сохранить график", QDir::currentPath() + "/images_graphs", "PNG files (*.png);"), 0, 0, 1, 100);
}


void MainWindow::on_btn_save_graph_jpg_clicked()
{
    ui->customPlot->saveJpg(QFileDialog::getSaveFileName(this,"Сохранить график", QDir::currentPath() + "/images_graphs", "JPEG files (*.jpg);"), 0, 0, 1, 100);
}


void MainWindow::on_btn_save_graph_bmp_clicked()
{
    ui->customPlot->saveBmp(QFileDialog::getSaveFileName(this,"Сохранить график", QDir::currentPath() + "/images_graphs", "BMP files (*.bmp);"), 0, 0, 1);
}


void MainWindow::on_btn_graph_zone_clicked()
{
    // Поменять на вызов диалогового окна с выбором отборажаемого диапазона
    if(ui->customPlot->selectionRectMode() == QCP::srmNone){
        ui->customPlot->setSelectionRectMode(QCP::srmZoom);}
    else {ui->customPlot->setSelectionRectMode(QCP::srmNone);}
}


void MainWindow::on_btn_graph_zoominX_clicked()
{
    double fivePercent = (ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower) / 20;
    ui->customPlot->xAxis->setRange(ui->customPlot->xAxis->range().lower + fivePercent, ui->customPlot->xAxis->range().upper - fivePercent);
    ui->customPlot->replot();
}


void MainWindow::on_btn_graph_zoomoutX_clicked()
{
    double fivePercent = (ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower) / 20;
    ui->customPlot->xAxis->setRange(ui->customPlot->xAxis->range().lower - fivePercent, ui->customPlot->xAxis->range().upper + fivePercent);
    ui->customPlot->replot();
}


void MainWindow::on_btn_graph_zoominY_clicked()
{
    double fivePercent = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower) / 20;
    ui->customPlot->yAxis->setRange(ui->customPlot->yAxis->range().lower + fivePercent, ui->customPlot->yAxis->range().upper - fivePercent);
    ui->customPlot->replot();
}


void MainWindow::on_btn_graph_zoomoutY_clicked()
{
    double fivePercent = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower) / 20;
    ui->customPlot->yAxis->setRange(ui->customPlot->yAxis->range().lower - fivePercent, ui->customPlot->yAxis->range().upper + fivePercent);
    ui->customPlot->replot();
}


void MainWindow::on_btn_graph_zoomin_clicked()
{
    double fivePercent = (ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower) / 20;
    ui->customPlot->xAxis->setRange(ui->customPlot->xAxis->range().lower + fivePercent, ui->customPlot->xAxis->range().upper - fivePercent);
    fivePercent = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower) / 20;
    ui->customPlot->yAxis->setRange(ui->customPlot->yAxis->range().lower + fivePercent, ui->customPlot->yAxis->range().upper - fivePercent);
    ui->customPlot->replot();
}


void MainWindow::on_btn_graph_zoomout_clicked()
{
    double fivePercent = (ui->customPlot->xAxis->range().upper - ui->customPlot->xAxis->range().lower) / 20;
    ui->customPlot->xAxis->setRange(ui->customPlot->xAxis->range().lower - fivePercent, ui->customPlot->xAxis->range().upper + fivePercent);
    fivePercent = (ui->customPlot->yAxis->range().upper - ui->customPlot->yAxis->range().lower) / 20;
    ui->customPlot->yAxis->setRange(ui->customPlot->yAxis->range().lower - fivePercent, ui->customPlot->yAxis->range().upper + fivePercent);
    ui->customPlot->replot();
}


void MainWindow::on_btn_start_reg_clicked()
{
    if(ConnectionSet){
        for (int j = 0; j < NUM_OF_COLORS; j++){
            ui->customPlot->graph(j)->data()->clear();
        }

        datagram->clear();
        datagram->append(QByteArray::fromHex("0299"));
        socket->write(*datagram);
        delayms();
        delayms();
        delayms();
        delayms();
        delayms();
        datagram->clear();
        datagram->append(QByteArray::fromHex("029F"));
        socket->write(*datagram);
    }
}

void MainWindow::ping_device(QString ip_str)
{
    *destIP = QHostAddress(ip_str);
    socket->disconnectFromHost();
    socket->connectToHost(*destIP,2054,QIODevice::ReadWrite);
    //сделать проверку связи
    //emit ping_response(ip_str=="192.168.0.104");
    datagram->clear();
    datagram->append(QByteArray::fromHex("0202"));
    emit ping_response(false); // Связь не установлена
    ConnectionSet = false;
    socket->write(*datagram);
    /*
    datagram->clear();
    datagram->append(socket->read(320));
    if(datagram[0].toInt()==2 && datagram[1].toInt()==3){
        emit ping_response(true);
    }else{
        emit ping_response(false);
    }
    */
}

void MainWindow::handleChSettings(int channel, int range, bool resist, int offset, int filter)
{
    if(ConnectionSet){
        wrongCommand = false;

        datagram->clear();
        badResponse = false;
        goodResponse = false;

        datagram->append(QByteArray::fromHex("03"));
        range > 2 ? datagram->append(QByteArray::fromHex("3C")) : datagram->append(QByteArray::fromHex("3D"));
        char ch = channel & 0xFF;
        datagram->append(ch);
        socket->write(*datagram); // set divider - Устанавливать Делитель и Усиление в зависимости от диапазона!!!

        delayms();
        if(!goodResponse) wrongCommand = true;
        datagram->clear();
        badResponse = false;
        goodResponse = false;

        datagram->append(QByteArray::fromHex("03"));
        resist == 0 ? datagram->append(QByteArray::fromHex("3B")) : datagram->append(QByteArray::fromHex("3A"));
        datagram->append(ch);
        socket->write(*datagram); // set resistance

        delayms();
        if(!goodResponse) wrongCommand = true;
        datagram->clear();
        badResponse = false;
        goodResponse = false;

        datagram->append(QByteArray::fromHex("050A"));
        datagram->append(ch);
        char offset1 = (offset>>8) & 0xFF;
        char offset2 = offset & 0xFF;
        datagram->append(offset2);
        datagram->append(offset1);
        socket->write(*datagram); // set offset

        /* Установка смещения сразу на 4 канала
        datagram->append(QByteArray::fromHex("06"));
        channel < 5 ? datagram->append(QByteArray::fromHex("12")) : datagram->append(QByteArray::fromHex("13"));
        datagram->append(QByteArray::fromHex("0300"));
        char offset1 = (offset>>8) & 0xFF;
        char offset2 = offset & 0xFF;
        datagram->append(offset1);
        datagram->append(offset2);
        socket->write(*datagram); // set offset
        datagram->clear();*/

        delayms();
        if(!goodResponse) wrongCommand = true;
        datagram->clear();
        badResponse = false;
        goodResponse = false;

        datagram->append(QByteArray::fromHex("050B")); //05 0B ch dd1 dd2
        datagram->append(ch);
        int lmhData;
        switch (range){
            case 0: lmhData = 0b1010;
            break;
            case 1: lmhData = 0b10110;
            break;
            case 2: lmhData = 0b10011;
            break;
            case 3: lmhData = 0b1010;
            break;
            case 4: lmhData = 0b10011;
            break;
            default: lmhData = 0b11010;
            break;
        }
        switch (filter){
            case 0: lmhData += 0x000;
            break;
            case 1: lmhData += 0x180;
            break;
            case 2: lmhData += 0x140;
            break;
            case 3: lmhData += 0x100;
            break;
            case 4: lmhData += 0x0C0;
            break;
            case 5: lmhData += 0x080;
            break;
            case 6: lmhData += 0x040;
            break;
            default: lmhData += 0x000;
            break;
        }
        char lmhData1 = (lmhData>>8) & 0xFF;
        char lmhData2 = lmhData & 0xFF;
        datagram->append(lmhData2);
        datagram->append(lmhData1);
        socket->write(*datagram); // set Attenuation, Gain and filter

        delayms();
        if(!goodResponse) wrongCommand = true;
        datagram->clear();

        // **** ТОЛЬКО ДЛЯ ОТЛАДКИ БЕЗ МК **** //
         if(!ui->checkBox->isChecked()) wrongCommand = false;
        // ****                    **** //
        if(wrongCommand){ // Если на один из запросов (команд) не пришёл ответ подтверждение или пришёл ответ - сообщение об ошибке
            QMessageBox::warning(this, "Ошибка выполнения","Одна из команд не была выполнена.    \n Проверьте вводимые данные и повторите операцию  \n");
            wrongCommand = false;
        }else{ // Команды успешно выполнены
            emit save_newChSettings(channel, range, resist, offset, filter);

            chansCSV[channel-1].replace(3,QString::number(range));
            chansCSV[channel-1].replace(5,QString::number(resist));
            chansCSV[channel-1].replace(7,QString::number(offset));
            chansCSV[channel-1].replace(9,QString::number(filter));
        }
    } else {
        QMessageBox::critical(this, "Ошибка соединения","Соединение с регистратором    \n        не установлено\n");
    }

}

void MainWindow::hide_subwidgets()
{
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        chan[i]->hide();
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(ui->tabWidget->tabText(index)=="Скрыть меню"){
        ui->tabWidget->setMaximumHeight(40);
    }else{
        ui->tabWidget->setMaximumHeight(130);
    }
}

void MainWindow::on_mousePress_subwidget(QMouseEvent *event)
{
    mousePressEvent(event);
}

void MainWindow::on_pushButton_clicked()
{
    //Убрать кнопку и spin
    delayTime = ui->spinBox->value();
}


void MainWindow::on_btn_settings_clicked()
{
    /*
    QGridLayout *layoutChSettings = new QGridLayout;
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        layoutChSettings->addWidget(chan[i], int(i/4), i%4);
    }
    ChSettingsForm->setLayout(layoutChSettings);
    ChSettingsForm->show();*/
    SettingForm->setChannelsGrid();
    SettingForm->move(5,5);
    SettingForm->show();


/*
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        chan[i]->show();
    }
*/


}


void MainWindow::on_btn_start_reg_2_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("030604"));
        socket->write(*datagram);
    }
}


void MainWindow::on_btn_start_reg_3_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("039A00"));
        socket->write(*datagram);
    }
}


void MainWindow::on_btn_start_reg_4_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("0219"));
        socket->write(*datagram);
    }
}


void MainWindow::on_btn_start_reg_5_clicked()
{

    if(ConnectionSet){
        for(cur_fpga = 0; cur_fpga < 4; cur_fpga++){
            datagram->clear();
            datagram->append(QByteArray::fromHex("0370")); // СЛОЖНО
            datagram->append(cur_fpga);
            for(int i = 0; i < ui->spinBox_3->value(); i++){
                socket->write(*datagram);
                delayms();
            }
        }
    }
}


void MainWindow::on_btn_start_reg_7_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("0220"));
        socket->write(*datagram);
    }
}


void MainWindow::on_btn_start_reg_6_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("0218"));
        socket->write(*datagram);
    }
}


void MainWindow::on_btn_start_reg_8_clicked()
{
    char ch; int Data;
    for(int i = 0; i < 8; i++){
        datagram->clear();
        ch = (i+1) & 0xFF;
        datagram->append(QByteArray::fromHex("051A")); //05 0B ch dd1 dd2
        datagram->append(ch);
        Data = ui->spinBox_4->value();
        char Data1 = (Data>>8) & 0xFF;
        char Data2 = Data & 0xFF;
        datagram->append(Data2);
        datagram->append(Data1);
        socket->write(*datagram); // set Attenuation, Gain and filter

        delayms();
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("031C00"));
        socket->write(*datagram);
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("031B01"));
        socket->write(*datagram);
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("064A01FF7F46"));
        socket->write(*datagram);
        delayms();
        datagram->clear();
        datagram->append(QByteArray::fromHex("064A02FF7F46"));
        socket->write(*datagram);
        delayms();
        datagram->clear();
        datagram->append(QByteArray::fromHex("064A03FF7F46"));
        socket->write(*datagram);
        delayms();
        datagram->clear();
        datagram->append(QByteArray::fromHex("064A04FF7F46"));
        socket->write(*datagram);
    }
}

void MainWindow::WriteToCSV(const QList<QStringList>& points)
{
    // Open csv-file

    QFile file(QFileDialog::getSaveFileName(this,"Сохранить график", QDir::currentPath() + "/CSV_graphs", "CSV files (*.csv);"));
   // QFile file("graph.csv");
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    // Write data to file
    QTextStream stream(&file);
    QString separator(",");
    for (int i = 0; i < points.size(); ++i)
    {
        stream << points.at(i).join(separator) << "\n";
    }

    stream.flush();
    file.close();
}

QList<QStringList> MainWindow::ReadCSV(QString filename)
{
    // Open csv-file
    //QFile file("graph.csv");
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    // Read data from file
    QTextStream stream(&file);
    QList<QStringList> data;
    QString separator(",");
    while (stream.atEnd() == false)
    {
        QString line = stream.readLine();
        data << line.split(separator);
    }

    file.close();
    return data;
}

void MainWindow::on_btn_save_graph_csv_clicked()
{
    QStringList row1 = {"Registrator","Name","Num of channels",QString::number(NUM_OF_CHANNELS)};
    QList<QStringList> points;
    points.append(row1);

    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        points.append(chansCSV[i]);
    }

    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        QStringList record;
        record.append(QString::number(ui->customPlot->graph(i)->data()->size()));
        for(int j = 0; j < ui->customPlot->graph(i)->data()->size(); j++){
            record.append(QString::number(ui->customPlot->graph(i)->data()->at(j)->mainKey()));
            record.append(QString::number(ui->customPlot->graph(i)->data()->at(j)->mainValue()));
        }
        points.append(record);
    }


    WriteToCSV(points);
}

void MainWindow::load_graph_from_csv(QList<QStringList> points){
    int num_of_chan = points[0][3].toInt();
    for(int i = 0; i < num_of_chan; i++){
        emit save_newChSettings(i+1,points[i+1][3].toInt(),points[i+1][5].toInt(),points[i+1][7].toInt(),points[i+1][9].toInt());
        ui->customPlot->graph(i)->data()->clear();
        for(int j = 0; j < points[1+num_of_chan+i][0].toInt(); j++){
            ui->customPlot->graph(i)->addData(points[1+num_of_chan+i][2*j+1].toDouble(), points[1+num_of_chan+i][2*(j+1)].toDouble());
        }
    }
    ui->customPlot->replot();
}

void MainWindow::on_btn_load_graph_csv_choice_clicked()
{
    QString csv_path = QFileDialog::getOpenFileName(this, "Открыть график", "C:/Users", "CSV files (*.csv);");
    if(csv_path != "") load_graph_from_csv(ReadCSV(csv_path));
}


void MainWindow::on_btn_load_graph_csv_default_clicked()
{
    QString csv_path = QFileDialog::getOpenFileName(this, "Открыть график", QDir::currentPath() + "/CSV_graphs", "CSV files (*.csv);");
    if(csv_path != nullptr) load_graph_from_csv(ReadCSV(csv_path));
}

void MainWindow::on_btn_add_math_clicked()
{
    int last_id = mathForms.count();
    mathForms.append(new MathForm(last_id+1));
    ui->scrollAreaWidgetContents->layout()->addWidget(mathForms.at(last_id));
    mathForms.at(last_id)->show();
    connect(mathForms.at(last_id), SIGNAL (close_clicked()), this, SLOT (on_btn_math_close_clicked()));

    QString m_number = last_id>8 ? QString::number(last_id+1):"0" + QString::number(last_id+1);
    mathForms.at(last_id)->setProperty("objectName","Measur" + m_number);
}

void MainWindow::on_btn_math_close_clicked()
{
    int n = (sender()->property("objectName").toString()[6].unicode()-48)*10;
    n = n + sender()->property("objectName").toString()[7].unicode()-48;
    delete mathForms.at(n-1);
    mathForms.removeAt(n-1);
    for(int i = n-1; i < mathForms.count(); i++){
        QString m_number = i>8 ? QString::number(i+1):"0" + QString::number(i+1);
        mathForms.at(i)->setProperty("objectName","Measur" + m_number);
        mathForms.at(i)->change_number(i+1);
    }
}

