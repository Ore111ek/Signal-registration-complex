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
    QColor(255, 36, 226), // Barbie Pink
    QColor(255, 182, 193),  // Light Pink
    QColor(160, 130, 100), // Brown
    QColor(200, 200, 180) // Grey
};
//Набор цветов для светлой темы
QColor graph_palette_l[NUM_OF_COLORS] = {
// 1-4 channels
    QColor(21, 189, 9), //* Islamic Green
    Qt::blue,
    QColor(188, 59, 227),  // Purple
    QColor(250, 100, 50), // Orange
// 5-8 channels
    Qt::red,
    QColor(128, 128, 0), // Olive
    QColor(128, 0, 128),  // Purple
    QColor(128, 0, 0), // Maroon
//for measurements
    QColor(255, 178, 46), // Yellow Orange
    QColor(255, 36, 226), // Barbie Pink
    QColor(0, 128, 128),  // Teal
    QColor(160, 130, 100), // Brown
    QColor(0, 128, 0) // Green
};
QColor cursor_palette[NUM_OF_COLORS] = {
// Dark theme
    QColor(130, 180, 250), // 1 cursor
    QColor(130, 250, 160), // 2 cursor
// Light theme
    QColor(60, 110, 180),
    QColor(50, 180, 50)
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

    label_logo = new QLabel(this);
    QPixmap pixmap(":/img/images/logo.png");
     label_logo->setPixmap(pixmap);
     label_logo->setScaledContents(true);
     label_logo->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
     label_logo->setGeometry(660,0,123,26);
     //label_logo->setVisible(true);

    for(int j = 0; j < NUM_OF_CHANNELS; j++){
        chGrData[j].ch = j;// ВАЖНО в конструкторе MainWindow один раз установить ch
        chGrData[j].resist = false;
        chGrData[j].range = 0;

        QVector <double> xsin, ysin;
        xsin.clear(); ysin.clear();
        for(double i = 0; i <= 100; i += 0.01){
            xsin.append(i);
            ysin.append(1500*sin(i + j) + 800*j+2048);
        }
        chGrData[j].set_data(xsin, ysin);


        /*
        Graph gr;
        gr.data.x.clear();
        gr.data.y.clear();
        for(double i = 0; i <= 100; i += 0.01){
            gr.data.x.append(i);
            gr.data.y.append(1500*sin(i + j) + 800*j+2048);
        }
        chGraphs.append(gr);
        */
        /*
        graph.ch[j].x.clear();
        graph.ch[j].y.clear();
        for(double i = 0; i <= 100; i += 0.01){
            graph.ch[j].x.append(i);
            graph.ch[j].y.append(1500*sin(i + j) + 800*j+2048);
        }*/
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

    //Progress bar
    //progressBar = new QProgressBar();
    //progressDialog = new QProgressDialog("Чтение сигнала","Отмена",0,100);
    //progressDialog->hide();

    // Network
    socket = new QUdpSocket(this);
    getSocket = new QUdpSocket(this);
    getSocket->bind(4000, QUdpSocket::ShareAddress); // Прослушиваем порт 4000
    //destIP = new QHostAddress("132.9.36.64");
     destIP = new QHostAddress("192.168.0.1");
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
    // Для меню
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        ui->comboBox_tr_ch->addItem(QString::number(i+1));
        ui->comboBox_tr2_ch->addItem(QString::number(i+1));
    }

    // Fourier
    ui->customPlot->yAxis2->setLabel("МГц");
    ui->customPlot->yAxis2->setLabelColor(Qt::white);
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
        //ui->spinBox_2->setValue((unsigned char)buffer.at(0)); опрос квитанции
    } else if(datagram->at(1)==0x70){
        QVector <double> recx1, recx2, recy1, recy2;
        recx1.clear(); recx2.clear(); recy1.clear(); recy2.clear();
        for(int w = 0; w < 80; w++){
                //ui->customPlot->graph(2*cur_fpga+1)->addData(cur_x[2*cur_fpga+1], (buffer.at(4*w+1)&0x0F)*256 + (unsigned char)buffer.at(4*w));
                //ui->customPlot->graph(2*cur_fpga)->addData(cur_x[2*cur_fpga], (buffer.at(4*w+3)&0x0F)*256 + (unsigned char)buffer.at(4*w+2));
                recx1.append(cur_x[2*cur_fpga]);
                recx2.append(cur_x[2*cur_fpga+1]);
                recy1.append((buffer.at(4*w+3)&0x0F)*256 + (unsigned char)buffer.at(4*w+2));
                recy2.append((buffer.at(4*w+1)&0x0F)*256 + (unsigned char)buffer.at(4*w));
                cur_x[2*cur_fpga]+=1;
                cur_x[2*cur_fpga+1]+=1;
        }
        chGrData[2*cur_fpga].add_data(recx1,recy1);
        chGrData[2*cur_fpga+1].add_data(recx2,recy2);
        update_ranges_of_Graph();
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
                //ui->spinBox_2->setValue((unsigned char)buffer.at(2)); опрос квитанции
            break;
            case 0x93://Подтверждение сброса ПЛИС

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
    ui->customPlot->xAxis->setLabelColor(Qt::white);
    ui->customPlot->yAxis->setLabelColor(Qt::white);

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
    ui->customPlot->legend->setBrush(QColor(80, 80, 80));
    ui->customPlot->legend->setBorderPen(QPen(Qt::white, 1));
    ui->customPlot->legend->setTextColor(Qt::white);

    for (int j = 0; j < NUM_OF_CHANNELS; j++){
        ui->customPlot->graph(j)->setPen(QPen(graph_palette[j%NUM_OF_CHANNELS], 1.2));
    }
    for (int j = 0; j < MAX_MEASUREMENTS; j++){
        ui->customPlot->graph(NUM_OF_CHANNELS+j)->setPen(QPen(graph_palette[NUM_OF_CHANNELS + j%(NUM_OF_COLORS - NUM_OF_CHANNELS)], 1.2));
        if(j/(NUM_OF_COLORS - NUM_OF_CHANNELS) >= 1) ui->customPlot->graph(NUM_OF_CHANNELS+j)->setPen(QPen(graph_palette[NUM_OF_CHANNELS + j%(NUM_OF_COLORS - NUM_OF_CHANNELS)], 1.5, Qt::DotLine));
    }

    if(tracer1 != nullptr){
        tracer1->setPen(QPen(cursor_palette[0], 1.5, Qt::DashDotDotLine));
    }
    if(tracer2 != nullptr){
        tracer2->setPen(QPen(cursor_palette[1], 1.5, Qt::DashDotDotLine));
    }
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
    ui->customPlot->xAxis->setLabelColor(Qt::black);
    ui->customPlot->yAxis->setLabelColor(Qt::black);

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
    ui->customPlot->legend->setBrush(Qt::white);
    ui->customPlot->legend->setBorderPen(QPen(QColor(80, 80, 80),1));
    ui->customPlot->legend->setTextColor(Qt::black);

    for (int j = 0; j < NUM_OF_CHANNELS; j++){
        ui->customPlot->graph(j)->setPen(QPen(graph_palette_l[j%NUM_OF_CHANNELS], 1.2));
    }
    for (int j = 0; j < MAX_MEASUREMENTS; j++){
        ui->customPlot->graph(NUM_OF_CHANNELS+j)->setPen(QPen(graph_palette_l[NUM_OF_CHANNELS + j%(NUM_OF_COLORS - NUM_OF_CHANNELS)], 1.2));
        if(j/(NUM_OF_COLORS - NUM_OF_CHANNELS) >= 1) ui->customPlot->graph(NUM_OF_CHANNELS+j)->setPen(QPen(graph_palette_l[NUM_OF_CHANNELS + j%(NUM_OF_COLORS - NUM_OF_CHANNELS)], 1.5, Qt::DotLine));
    }

    if(tracer1 != nullptr){
        tracer1->setPen(QPen(cursor_palette[2], 1.5, Qt::DashDotDotLine));
    }
    if(tracer2 != nullptr){
        tracer2->setPen(QPen(cursor_palette[3], 1.5, Qt::DashDotDotLine));
    }
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
    //qDebug() << "Left Mouse button pressed";

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
                if(tracer1 != nullptr){
                    if(event->pos().x() < ui->customPlot->size().rwidth() && event->globalPosition().y() - this->mapToGlobal(this->pos()).y() < ui->customPlot->size().rheight() + ui->customPlot->y()
                            && event->globalPosition().y() - this->mapToGlobal(this->pos()).y() > ui->customPlot->y()){
                        tracer1->setGraphKey((event->pos().x()-64)*ui->customPlot->xAxis->range().size()/(ui->customPlot->size().rwidth()-78) + ui->customPlot->xAxis->range().lower);
                        ui->customPlot->replot();
                    }
                }

                qDebug() << "Right Mouse button pressed in x=" + QString::number(event->globalPosition().x()) + " customY=" + QString::number(ui->customPlot->y());
                break;
            }
            case Qt::MiddleButton:
            {
                if(tracer2 != nullptr){
                    if(event->pos().x() < ui->customPlot->size().rwidth() && event->globalPosition().y() - this->mapToGlobal(this->pos()).y() < ui->customPlot->size().rheight() + ui->customPlot->y()
                            && event->globalPosition().y() - this->mapToGlobal(this->pos()).y() > ui->customPlot->y()){
                        tracer2->setGraphKey((event->pos().x()-64)*ui->customPlot->xAxis->range().size()/(ui->customPlot->size().rwidth()-78) + ui->customPlot->xAxis->range().lower);
                        ui->customPlot->replot();
                    }
                }
                qDebug() << "Middle Mouse button pressed";
                break;
            }
            default:
            {
                qDebug() << "Other button pressed, id = "+QString::number(event->button());
                break;
            }
        }
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

void MainWindow::update_ranges_of_Graph()
{

    if(ui->combo_measureX->currentText() == "точки"){
        ui->customPlot->xAxis->setRange(chGrData[0].x().at(0),chGrData->x().at(chGrData[0].x().length()-1));
    }else if(ui->combo_measureX->currentText() == "нс"){
        ui->customPlot->xAxis->setRange(chGrData[0].x().at(0),chGrData->x_ns().at(chGrData[0].x_ns().length()-1));
    }else if(ui->combo_measureX->currentText() == "мкс"){
        ui->customPlot->xAxis->setRange(chGrData[0].x().at(0),chGrData->x_mcs().at(chGrData[0].x_mcs().length()-1));
    }else if(ui->combo_measureX->currentText() == "мс"){
        ui->customPlot->xAxis->setRange(chGrData[0].x().at(0),chGrData->x_ms().at(chGrData[0].x_ms().length()-1));
    }else if(ui->combo_measureX->currentText() == "с"){
        ui->customPlot->xAxis->setRange(chGrData[0].x().at(0),chGrData->x_s().at(chGrData[0].x_s().length()-1));
    }

    if(ui->combo_measureY->currentText() == "отсчёты"){
        ui->customPlot->yAxis->setRange(0,4096);
    }else if(ui->combo_measureY->currentText() == "В"){
        ui->customPlot->yAxis->setRange(-20,20);
    }else if(ui->combo_measureY->currentText() == "мВ"){
        ui->customPlot->yAxis->setRange(-20000,20000);
    }
    ui->customPlot->replot();
}

void MainWindow::initialize_graph(){
    //graph_setDark();
   // customPlot->rescaleAxes();
   // customPlot->yAxis->setRange(0, 2);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->setAutoAddPlottableToLegend(false);
    ui->customPlot->xAxis->setRange(0,300);
    ui->customPlot->yAxis->setRange(0,4096);
    for (int j = 0; j < NUM_OF_CHANNELS; j++){
        ui->customPlot->addGraph();

        ui->customPlot->graph(j)->addData(chGrData[j].x(),chGrData[j].y());
        ui->customPlot->graph(j)->setVisible(false); // Hide Graphs
        ui->customPlot->graph(j)->setName("K" + QString::number(j+1));
    }

    for (int j = 0; j < MAX_MEASUREMENTS; j++){
        ui->customPlot->addGraph();
        ui->customPlot->graph(NUM_OF_CHANNELS+j)->setVisible(false); // Hide Graphs
        ui->customPlot->graph(NUM_OF_CHANNELS+j)->setName("M" + QString::number(j+1));
    }
    graph_setDark();
    update_measureXY();

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
    ui->customPlot->graph(n)->addToLegend();
    ui->customPlot->replot();
}

void MainWindow::on_mini_ch_close_clicked()
{
    sender()->setProperty("visible",false);
    int n = sender()->property("objectName").toString()[6].unicode()-49;
    chbutton[n]->show();
    ui->customPlot->graph(n)->setVisible(false);
    ui->customPlot->graph(n)->removeFromLegend();
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
            datagram->clear();
            datagram->append(QByteArray::fromHex("031C00"));
            socket->write(*datagram);
        delayms();
            datagram->clear();
            datagram->append(QByteArray::fromHex("031B01"));
            socket->write(*datagram);
        delayms();
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

        for(int i = 0; i < NUM_OF_CHANNELS; i++){
            cur_x[i] = 0;
        }
        for (int j = 0; j < NUM_OF_COLORS; j++){
            ui->customPlot->graph(j)->data()->clear();
            chGrData[j].clear_data();
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
// , resist true - 1 MOhm,
void MainWindow::handleChSettings(int channel, int range, bool resist, int offset, int filter)
{
    if(ConnectionSet){
        wrongCommand = false;

        datagram->clear();
        badResponse = false;
        goodResponse = false;

        datagram->append(QByteArray::fromHex("03"));
        range > 1 ? datagram->append(QByteArray::fromHex("3C")) : datagram->append(QByteArray::fromHex("3D"));
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
            chGrData[channel-1].range = range;
            chGrData[channel-1].resist = resist;

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

void MainWindow::on_math_update_settings()
{
    for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+MAX_MEASUREMENTS; i++){
        ui->customPlot->graph(i)->setVisible(false);
        ui->customPlot->graph(i)->removeFromLegend();
    }
    mathGrData.clear();
    ui->customPlot->replot();
    emit math_request();
}

void MainWindow::on_math_data_request(int math_num, QVector<int> channels)
{
    QVector <QVector <double>> data;
    for(int i = 0; i < channels.count(); i++){
        QVector <double> graph;
        int n = channels.at(i);
        //for(int j = 0; j < ui->customPlot->graph(n)->data()->size(); j++){
        //    graph.append(ui->customPlot->graph(n)->data()->at(j)->mainValue());
        //}
        //data.append(graph);
        data.append(chGrData[n].y_v());
    }
    //emit math_send_data(data);
    mathForms.at(math_num-1)->on_math_send_data(data);
}

void MainWindow::on_math_result_graph(int math_num, QVector<QVector<double>> graphs)
{
    //Сделать несколько графиков для одного измерения
        //ui->customPlot->graph(7+math_num)->data()->clear();
        while(graphs[0].length()<chGrData->x().length()){
            graphs[0].append(0);
        }
        mathGrData.append(graphs.at(0));
        //for(int j = 0; j < graphs.at(0).count(); j++){
        //    ui->customPlot->graph(7+math_num)->addData(ui->customPlot->graph(0)->data()->at(j)->mainKey(),graphs.at(0).at(j));
        //}
        ui->customPlot->graph(7+math_num)->setVisible(true);
        ui->customPlot->graph(7+math_num)->addToLegend();

    update_measureXY(); // Установит график в соответствии с ед. измерения
}

void MainWindow::on_math_result_number(int math_num, QVector<double> numbers)
{

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
        chGrData[i].range = points[i+1][3].toInt();
        chGrData[i].resist = points[i+1][5].toInt();
        ui->customPlot->graph(i)->data()->clear();
        QVector <double> xcsv, ycsv;
        xcsv.clear(); ycsv.clear();
        for(int j = 0; j < points[1+num_of_chan+i][0].toInt(); j++){
            xcsv.append(points[1+num_of_chan+i][2*j+1].toDouble());
            ycsv.append(points[1+num_of_chan+i][2*(j+1)].toDouble());
            //ui->customPlot->graph(i)->addData(points[1+num_of_chan+i][2*j+1].toDouble(), points[1+num_of_chan+i][2*(j+1)].toDouble());
        }
        chGrData[i].set_data(xcsv,ycsv);
    }
    update_measureXY(); // Установит график в соответствии с ед. измерения
    on_btn_call_math_clicked();
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
    if(last_id < 10){
        mathForms.append(new MathForm(last_id+1));
        ui->scrollAreaWidgetContents->layout()->addWidget(mathForms.at(last_id));
        mathForms.at(last_id)->show();
        connect(mathForms.at(last_id), SIGNAL (close_clicked()), this, SLOT (on_btn_math_close_clicked()));

        QString m_number = last_id>8 ? QString::number(last_id+1):"0" + QString::number(last_id+1);
        mathForms.at(last_id)->setProperty("objectName","Measur" + m_number);

        connect(this, SIGNAL (math_request()), mathForms.at(last_id), SLOT (on_math_request()));
        connect(mathForms.at(last_id), SIGNAL (math_data_request(int, QVector<int>)), this, SLOT (on_math_data_request(int, QVector<int>)));
        //connect(this, SIGNAL (math_send_data(QVector<QVector<double>>)), mathForms.at(last_id), SLOT (on_math_send_data(QVector<QVector<double>>)));
        connect(mathForms.at(last_id), SIGNAL (math_result_graph(int, QVector<QVector<double>>)), this, SLOT (on_math_result_graph(int, QVector<QVector<double>>)));
        connect(mathForms.at(last_id), SIGNAL (math_result_number(int, QVector<double>)), this, SLOT (on_math_result_number(int, QVector<double>)));
        connect(mathForms.at(last_id), SIGNAL (math_update_settings()), this, SLOT (on_btn_call_math_clicked()));
    }
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
    on_btn_call_math_clicked();
}


void MainWindow::on_btn_call_math_clicked()
{
    for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+MAX_MEASUREMENTS; i++){
        ui->customPlot->graph(i)->setVisible(false);
        ui->customPlot->graph(i)->removeFromLegend();
    }
    mathGrData.clear();
    ui->customPlot->replot();
    emit math_request();
}

void MainWindow::update_measureXY()
{
    if (ui->combo_measureY->currentText() == "отсчёты"){
        ui->customPlot->yAxis->setLabel("Код АЦП");
        if (ui->combo_measureX->currentText() == "точки"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x(),chGrData[j].y());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->data().clear();
            }
        }else if(ui->combo_measureX->currentText() == "нс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_ns(),chGrData[j].y());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->data().clear();
            }
        }else if(ui->combo_measureX->currentText() == "мкс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_mcs(),chGrData[j].y());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->data().clear();
            }
        }else if(ui->combo_measureX->currentText() == "мс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_ms(),chGrData[j].y());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->data().clear();
            }
        }else if(ui->combo_measureX->currentText() == "с"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_s(),chGrData[j].y());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->data().clear();
            }
        }
    }else if(ui->combo_measureY->currentText() == "мВ"){
        ui->customPlot->yAxis->setLabel("мВольт");
        QVector<QVector <double>> mv_math;
        for(int j = 0; j < mathGrData.length(); j++){
            QVector <double> mv_math_one;
            for(int i = 0; i < mathGrData.at(j).length();i++){
                mv_math_one.append(mathGrData.at(j).at(i)*1000);
            }
            mv_math.append(mv_math_one);
        }
        if (ui->combo_measureX->currentText() == "точки"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x(),chGrData[j].y_mv());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x(),mv_math.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "нс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_ns(),chGrData[j].y_mv());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_ns(),mv_math.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "мкс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_mcs(),chGrData[j].y_mv());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_mcs(),mv_math.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "мс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_ms(),chGrData[j].y_mv());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_ms(),mv_math.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "с"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_s(),chGrData[j].y_mv());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_s(),mv_math.at(i-NUM_OF_CHANNELS));
            }
        }
    }else if(ui->combo_measureY->currentText() == "В"){
        ui->customPlot->yAxis->setLabel("Вольт");
        if (ui->combo_measureX->currentText() == "точки"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x(),chGrData[j].y_v());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x(),mathGrData.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "нс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_ns(),chGrData[j].y_v());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_ns(),mathGrData.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "мкс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_mcs(),chGrData[j].y_v());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_mcs(),mathGrData.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "мс"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_ms(),chGrData[j].y_v());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_ms(),mathGrData.at(i-NUM_OF_CHANNELS));
            }
        }else if(ui->combo_measureX->currentText() == "с"){
            for (int j = 0; j < NUM_OF_CHANNELS; j++){
                ui->customPlot->graph(j)->setData(chGrData[j].x_s(),chGrData[j].y_v());
            }
            for(int i = NUM_OF_CHANNELS; i < NUM_OF_CHANNELS+mathGrData.length(); i++){
                ui->customPlot->graph(i)->setData(chGrData[0].x_s(),mathGrData.at(i-NUM_OF_CHANNELS));
            }
        }
    }
    ui->customPlot->xAxis->setLabel(ui->combo_measureX->currentText());
    update_ranges_of_Graph();
}


void MainWindow::on_btn_clear_RAM_clicked()
{
    if(ConnectionSet){
        datagram->clear();
        datagram->append(QByteArray::fromHex("0293"));
        socket->write(*datagram);
    }
}


void MainWindow::on_btn_set_pre_rec_clicked()
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


void MainWindow::on_btn_read_part_clicked()
{
    if(ConnectionSet){
        //progressDialog = new QProgressDialog("Чтение записи сигнала","Отмена",0,100);
        //progressDialog->setValue(75);

        for(cur_fpga = 0; cur_fpga < 4; cur_fpga++){
            datagram->clear();
            datagram->append(QByteArray::fromHex("0370")); // СЛОЖНО
            datagram->append(cur_fpga);
            for(int i = 0; i < ui->spinBox_3->value(); i++){
                socket->write(*datagram);
                delayms();
            }
        }
        ui->customPlot->xAxis->setRange(0,cur_x[0]);
        ui->customPlot->yAxis->setRange(0,4100);
        ui->customPlot->replot();
    }
}


void MainWindow::on_spinBox_3_textChanged(const QString &arg1)
{
    ui->btn_read_part->setText("Чтение\n" + arg1 + "\nпакетов");
}


void MainWindow::on_btn_read_all_clicked()
{
    if(ConnectionSet){
        progressDialog = new QProgressDialog("Чтение записи сигнала","Отмена",0,100);
        for(cur_fpga = 0; cur_fpga < 4; cur_fpga++){
            datagram->clear();
            datagram->append(QByteArray::fromHex("0370")); // СЛОЖНО
            datagram->append(cur_fpga);
            for(int i = 0; i < 500; i++){
                socket->write(*datagram);
                delayms();
            }
            progressDialog->setValue((cur_fpga+1)*25);
        }
        ui->customPlot->xAxis->setRange(0,cur_x[0]);
        ui->customPlot->yAxis->setRange(0,4100);
        ui->customPlot->replot();
        progressDialog->close();
    }
}


void MainWindow::on_btn_tracer_clicked()
{
    if(tracer1 == nullptr){
        tracer1 = new QCPItemTracer(ui->customPlot);
        if(ui->btn_graph_theme->text() == "Включить\nсветлую\nтему"){
            tracer1->setPen(QPen(cursor_palette[0], 1.5, Qt::DashDotDotLine)); // Dark theme
        }
        else {
            tracer1->setPen(QPen(cursor_palette[2], 1.5, Qt::DashDotDotLine));
        }
        tracer1->setGraph(ui->customPlot->graph(ui->comboBox_tr_ch->currentIndex()));
        ui->btn_tracer->setText("Спрятать\nкурсор 1");
        ui->btn_tracer->setStyleSheet("QPushButton {border: 1px solid #bc3be3;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #99aacc, stop: 1 #aabbff);color: black; }");
    } else {
        delete tracer1;
        tracer1 = nullptr;
        ui->btn_tracer->setText("Показать\nкурсор 1");
        ui->btn_tracer->setStyleSheet("QPushButton:hover {border: 1px solid #bc3be3;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #99aacc, stop: 1 #aabbff);color: black; }");
    }
    ui->customPlot->replot();
}


void MainWindow::on_btn_tracer2_clicked()
{
    if(tracer2 == nullptr){
        tracer2 = new QCPItemTracer(ui->customPlot);
        if(ui->btn_graph_theme->text() == "Включить\nсветлую\nтему"){
            tracer2->setPen(QPen(cursor_palette[1], 1.5, Qt::DashDotDotLine));
        }
        else {
            tracer2->setPen(QPen(cursor_palette[3], 1.5, Qt::DashDotDotLine));
        }
        tracer2->setGraph(ui->customPlot->graph(ui->comboBox_tr2_ch->currentIndex()));
        ui->btn_tracer2->setText("Спрятать\nкурсор 2");
        ui->btn_tracer2->setStyleSheet("QPushButton {border: 1px solid #bc3be3;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #99ccaa, stop: 1 #aaffbb);color: black; }");
    } else {
        delete tracer2;
        tracer2 = nullptr;
        ui->btn_tracer2->setText("Показать\nкурсор 2");
        ui->btn_tracer2->setStyleSheet("");ui->btn_tracer2->setStyleSheet("QPushButton:hover {border: 1px solid #bc3be3;background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #99ccaa, stop: 1 #aaffbb);color: black; }");
    }
    ui->customPlot->replot();
}


void MainWindow::on_btn_tr_time_clicked()
{
    if (tracer1 != nullptr && tracer2 != nullptr){
        double time = 0;
        time = tracer2->position->key() - tracer1->position->key();
        ui->textEdit_tr_result->append("Время между курсорами: " +  QString::number(time) + " " + ui->combo_measureX->currentText());
    }
}


void MainWindow::on_btn_tr_freq_clicked()
{
    if (tracer1 != nullptr && tracer2 != nullptr){
        double time = 1/(tracer2->position->key() - tracer1->position->key());
        QString freqMeasure = "";
        if(ui->combo_measureX->currentText() == "нс"){
            time *= 1000;
            freqMeasure = "МГц";
        }else if(ui->combo_measureX->currentText() == "мкс"){
            freqMeasure = "МГц";
        }else if(ui->combo_measureX->currentText() == "мс"){
            freqMeasure = "кГц";
        }else if(ui->combo_measureX->currentText() == "с"){
            freqMeasure = "Гц";
        }
        ui->textEdit_tr_result->append("Частота: " +  QString::number(time) + " " + freqMeasure);
    }
}


void MainWindow::on_btn_tr_max_min_clicked()
{
    if (tracer1 != nullptr && tracer2 != nullptr){
    QSharedPointer <QCPGraphDataContainer> data = tracer1->graph()->data();
    double minx, miny, maxx, maxy;
    bool flag = true;
    for(int i = 0; i < data->size(); i++){
        if(data->at(i)->key > tracer1->graphKey() && data->at(i)->key < tracer2->graphKey()){
            if(flag){
                maxx = minx = data->at(i)->key;
                maxy = miny = data->at(i)->value;
                flag = false;
            }
            if(data->at(i)->value > maxy){
                maxx = data->at(i)->key;
                maxy = data->at(i)->value;
            }else if(data->at(i)->value < miny){
                minx = data->at(i)->key;
                miny = data->at(i)->value;
            }
        }
    }
    ui->textEdit_tr_result->append("Максимум: x = " + QString::number(maxx) + " " + ui->combo_measureX->currentText() + "; y = " + QString::number(maxy) + ui->combo_measureY->currentText());
    ui->textEdit_tr_result->append("Минимум: x = " + QString::number(minx) + " " + ui->combo_measureX->currentText() + "; y = " + QString::number(miny) + ui->combo_measureY->currentText());
    }
}

void MainWindow::on_btn_tr_ampl_clicked()
{
    if (tracer1 != nullptr && tracer2 != nullptr){
    QSharedPointer <QCPGraphDataContainer> data = tracer1->graph()->data();
    double miny, maxy;
    bool flag = true;
    for(int i = 0; i < data->size(); i++){
        if(data->at(i)->key > tracer1->graphKey() && data->at(i)->key < tracer2->graphKey()){
            if(flag){
                maxy = miny = data->at(i)->value;
                flag = false;
            }
            if(data->at(i)->value > maxy){
                maxy = data->at(i)->value;
            }else if(data->at(i)->value < miny){
                miny = data->at(i)->value;
            }
        }
    }
    ui->textEdit_tr_result->append("Размах: " + QString::number(maxy - miny));
    }
}

void MainWindow::on_btn_tr_clear_clicked()
{
    ui->textEdit_tr_result->clear();
}

void MainWindow::on_comboBox_tr_ch_currentIndexChanged(int index)
{
    if(tracer1 != nullptr)
        tracer1->setGraph(ui->customPlot->graph(index));
}

void MainWindow::on_comboBox_tr2_ch_currentIndexChanged(int index)
{
    if(tracer2 != nullptr)
        tracer2->setGraph(ui->customPlot->graph(index));
}

void MainWindow::on_combo_measureY_currentTextChanged(const QString &arg1)
{
    update_measureXY();
}


void MainWindow::on_combo_measureX_currentTextChanged(const QString &arg1)
{
    update_measureXY();
}

void MainWindow::on_btn_tr_fourier_clicked()
{
    if(tracer1 != nullptr){
        double freq;
        if (ui->combo_measureX->currentText() == "точки"){
            freq = (tracer1->graphKey()-chGrData[0].x().at(0))/(chGrData[0].x().at(chGrData[0].x().length()-1)-chGrData[0].x().at(0))*sample_rate/2;
        }else if(ui->combo_measureX->currentText() == "нс"){
            freq = (tracer1->graphKey()-chGrData[0].x_ns().at(0))/(chGrData[0].x_ns().at(chGrData[0].x_ns().length()-1)-chGrData[0].x_ns().at(0))*sample_rate/2;
        }else if(ui->combo_measureX->currentText() == "мкс"){
            freq = (tracer1->graphKey()-chGrData[0].x_mcs().at(0))/(chGrData[0].x_mcs().at(chGrData[0].x_mcs().length()-1)-chGrData[0].x_mcs().at(0))*sample_rate/2;
        }else if(ui->combo_measureX->currentText() == "мс"){
            freq = (tracer1->graphKey()-chGrData[0].x_ms().at(0))/(chGrData[0].x_ms().at(chGrData[0].x_ms().length()-1)-chGrData[0].x_ms().at(0))*sample_rate/2;
        }else if(ui->combo_measureX->currentText() == "с"){
            freq = (tracer1->graphKey()-chGrData[0].x_s().at(0))/(chGrData[0].x_s().at(chGrData[0].x_s().length()-1)-chGrData[0].x_s().at(0))*sample_rate/2;
        }else{freq = 0;}
        //freq = (tracer1->graphKey()/)*(sample_rate/2)/chGrData[0].x().length();
        ui->textEdit_tr_result->append("Частота гармоники: " + QString::number(freq/1000000) + "МГц");
    }
}

