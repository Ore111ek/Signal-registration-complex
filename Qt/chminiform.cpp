#include "chminiform.h"
#include "ui_chminiform.h"

ChMiniForm::ChMiniForm(int ch_number, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChMiniForm)
{
    ui->setupUi(this);
    ui->editButton->setText("Канал " + QString::number(ch_number));
}

ChMiniForm::~ChMiniForm()
{
    delete ui;
}

void ChMiniForm::on_closeButton_clicked()
{
    emit close_clicked();
}


void ChMiniForm::on_editButton_clicked()
{
    emit edit_clicked();
}

