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
        connect(chan[i], SIGNAL (sendChSettings(int,int,int,bool,int,int)), this, SLOT (handleChSettings(int,int,int,bool,int,int)));
    }

    SettingForm = new SettingsForm();

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
    wrongCommand = false;

    connect(getSocket,SIGNAL(readyRead()),this,SLOT(readyReadUDP()));// Функция обработки входящих пакетов от МК

    ConnectForm = new ConnectionForm;
    connect(ui->btn_conForm, SIGNAL (clicked()), this, SLOT (on_btn_conForm_clicked()));
    connect(ConnectForm, SIGNAL (ping(QString)), this, SLOT (ping_device(QString)));
    connect(this, SIGNAL (ping_response(bool)), ConnectForm, SLOT (handle_response(bool)));


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
        switch(buffer.at(1)){
            case 0x01://Опрос квитанции FIFO??
            break;
            case 0x70://Чтение пакета FIFO?
            break;
            default: // Ответ - это подтверждение выполнения команды
                if(buffer.at(2)==-1){
                    //Всё хорошо, обработка не требуется
                    qDebug() << "Message correct: " << buffer;
                    badResponse = false;
                }else if(buffer.at(2)==-18){
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

    ui->customPlot->xAxis->setRange(0,100);
    ui->customPlot->yAxis->setRange(-1,4);
    for (int j = 0; j < NUM_OF_COLORS; j++){
        ui->customPlot->addGraph();
        ui->customPlot->graph(j)->addData(graph.ch[j].x, graph.ch[j].y);
        ui->customPlot->graph(j)->setPen(QPen(graph_palette[j], 1.2));
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

void MainWindow::on_btn_addMath_clicked()
{

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
    ui->customPlot->savePdf(QFileDialog::getSaveFileName(this,"Сохранить график", "C:/Users", "PDF files (*.pdf);"));
}


void MainWindow::on_btn_save_graph_png_clicked()
{
    ui->customPlot->savePng(QFileDialog::getSaveFileName(this,"Сохранить график", "C:/Users", "PNG files (*.png);"), 0, 0, 1, 100);
}


void MainWindow::on_btn_save_graph_jpg_clicked()
{
    ui->customPlot->saveJpg(QFileDialog::getSaveFileName(this,"Сохранить график", "C:/Users", "JPEG files (*.jpg);"), 0, 0, 1, 100);
}


void MainWindow::on_btn_save_graph_bmp_clicked()
{
    ui->customPlot->saveBmp(QFileDialog::getSaveFileName(this,"Сохранить график", "C:/Users", "BMP files (*.bmp);"), 0, 0, 1);
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
    // Изменить на отправку команды регистрации
    datagram->clear();
    datagram->append("Hello from DIMA");
    socket->write(*datagram);
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

void MainWindow::handleChSettings(int channel, int range, int divider, bool resist, int offset, int filter)
{
    if(ConnectionSet){
        wrongCommand = false;

        datagram->clear();
        badResponse = false;
        datagram->append(QByteArray::fromHex("03"));
        divider == 0 ? datagram->append(QByteArray::fromHex("3C")) : datagram->append(QByteArray::fromHex("3D"));
        char ch = channel & 0xFF;
        datagram->append(ch);
        socket->write(*datagram); // set divider - Устанавливать Делитель и Усиление в зависимости от диапазона!!!

        delayms();

        datagram->clear();
        badResponse = false;
        datagram->append(QByteArray::fromHex("03"));
        resist == 0 ? datagram->append(QByteArray::fromHex("3B")) : datagram->append(QByteArray::fromHex("3A"));
        datagram->append(ch);
        socket->write(*datagram); // set resistance

        delayms();

        datagram->clear();
        badResponse = false;
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
        datagram->clear();

        if(wrongCommand){
            QMessageBox::warning(this, "Ошибка выполнения","Одна из команд не была выполнена.    \n Проверьте вводимые данные и повторите операцию  \n");
            wrongCommand = false;
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
    SettingForm->setChannelsGrid(chan);
    SettingForm->show();

/*
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        chan[i]->show();
    }
*/


}

