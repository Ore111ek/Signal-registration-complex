#include "connectionform.h"
#include "ui_connectionform.h"
#include <QIntValidator>

ConnectionForm::ConnectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionForm)
{
    ui->setupUi(this);
    ui->lineEdit_ip3->setValidator( new QIntValidator(0, 999, this) );
    ui->lineEdit_ip2->setValidator( new QIntValidator(0, 999, this) );
    ui->lineEdit_ip1->setValidator( new QIntValidator(0, 999, this) );
    ui->lineEdit_ip0->setValidator( new QIntValidator(0, 999, this) );

    ui->lineEdit_ip3new->setValidator( new QIntValidator(0, 999, this) );
    ui->lineEdit_ip2new->setValidator( new QIntValidator(0, 999, this) );
    ui->lineEdit_ip1new->setValidator( new QIntValidator(0, 999, this) );
    ui->lineEdit_ip0new->setValidator( new QIntValidator(0, 999, this) );

  //  QPixmap pixm(":/img/images/blue-circle.png");
  //  ui->label_icon_COM->setPixmap(pixm.scaled(48,48,Qt::KeepAspectRatio, Qt::SmoothTransformation));
  //  ui->label_icon_Eth->setPixmap(pixm.scaled(24,24,Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

ConnectionForm::~ConnectionForm()
{
    delete ui;
}

void ConnectionForm::on_pushButton_connectIP_clicked()
{
    emit ping(ui->lineEdit_ip3->text() + "." + ui->lineEdit_ip2->text() + "." + ui->lineEdit_ip1->text() + "." + ui->lineEdit_ip0->text());
}

void ConnectionForm::handle_response(bool state)
{
    if(state){
        //ui->label_icon_Eth->setStyleSheet("background-image: url(:/img/images/green-circle.png)\n");
        ui->label_text_Eth->setText("Связь установлена");
        ui->label_icon_Eth->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.75, y1:1, x2:0.802, y2:0, stop:0 rgba(20, 206, 51, 255), stop:1 rgba(105, 255, 119, 255));border: 1px solid gray;");
    }else{
        //ui->label_icon_Eth->setStyleSheet("background-image: url(:/img/images/red-circle.png)\n");
        ui->label_text_Eth->setText("Связь отсутствует");
        ui->label_icon_Eth->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0.75, y1:1, x2:0.802, y2:0, stop:0 rgba(255, 74, 74, 255), stop:1 rgba(255, 108, 108, 255));border: 1px solid gray;");
    }
}


void ConnectionForm::on_lineEdit_ip3_editingFinished()
{
    if(ui->lineEdit_ip3->text().toInt()>255){
        ui->lineEdit_ip3->setText("255");
    }
    else{
        ui->lineEdit_ip3->setText(QString::number(ui->lineEdit_ip3->text().toInt()));
    }
    ui->lineEdit_ip2->setFocus();
}


void ConnectionForm::on_lineEdit_ip2_editingFinished()
{
    if(ui->lineEdit_ip2->text().toInt()>255){
        ui->lineEdit_ip2->setText("255");
    }
    else{
        ui->lineEdit_ip2->setText(QString::number(ui->lineEdit_ip2->text().toInt()));
    }
    ui->lineEdit_ip1->setFocus();
}


void ConnectionForm::on_lineEdit_ip1_editingFinished()
{
    if(ui->lineEdit_ip1->text().toInt()>255){
        ui->lineEdit_ip1->setText("255");
    }
    else{
        ui->lineEdit_ip1->setText(QString::number(ui->lineEdit_ip1->text().toInt()));
    }
    ui->lineEdit_ip0->setFocus();
}


void ConnectionForm::on_lineEdit_ip0_editingFinished()
{
    if(ui->lineEdit_ip0->text().toInt()>255){
        ui->lineEdit_ip0->setText("255");
    }
    else{
        ui->lineEdit_ip0->setText(QString::number(ui->lineEdit_ip0->text().toInt()));
    }
}


void ConnectionForm::on_lineEdit_ip3new_editingFinished()
{
    if(ui->lineEdit_ip3new->text().toInt()>255){
        ui->lineEdit_ip3new->setText("255");
    }
    else{
        ui->lineEdit_ip3new->setText(QString::number(ui->lineEdit_ip3new->text().toInt()));
    }
    ui->lineEdit_ip2new->setFocus();
}


void ConnectionForm::on_lineEdit_ip2new_editingFinished()
{
    if(ui->lineEdit_ip2new->text().toInt()>255){
        ui->lineEdit_ip2new->setText("255");
    }
    else{
        ui->lineEdit_ip2new->setText(QString::number(ui->lineEdit_ip2new->text().toInt()));
    }
    ui->lineEdit_ip1new->setFocus();
}


void ConnectionForm::on_lineEdit_ip1new_editingFinished()
{
    if(ui->lineEdit_ip1new->text().toInt()>255){
        ui->lineEdit_ip1new->setText("255");
    }
    else{
        ui->lineEdit_ip1new->setText(QString::number(ui->lineEdit_ip1new->text().toInt()));
    }
    ui->lineEdit_ip0new->setFocus();
}


void ConnectionForm::on_lineEdit_ip0new_editingFinished()
{
    if(ui->lineEdit_ip0new->text().toInt()>255){
        ui->lineEdit_ip0new->setText("255");
    }
    else{
        ui->lineEdit_ip0new->setText(QString::number(ui->lineEdit_ip0new->text().toInt()));
    }
}

