#include "mathform.h"
#include "ui_mathform.h"

MathForm::MathForm(int m_number, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MathForm)
{
    number = m_number;
    ui->setupUi(this);
    ui->label_m_number->setText("M" + QString::number(m_number));
}

MathForm::~MathForm()
{
    delete ui;
}

void MathForm::change_number(int new_number)
{
    number = new_number;
    ui->label_m_number->setText("M" + QString::number(new_number));
}


void MathForm::on_btn_close_clicked()
{
    emit close_clicked();
}

