#include "chminiform.h"
#include "ui_chminiform.h"

ChMiniForm::ChMiniForm(int ch_number, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChMiniForm)
{
    ui->setupUi(this);
    ui->editButton->setText("Канал " + QString::number(ch_number));
    ch_num = ch_number;
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
    QPoint coord;
    coord.setX(this->mapToGlobal(QPoint(0,0)).x() + this->width()/2);
    coord.setY(this->mapToGlobal(QPoint(0,0)).y());
    emit edit_clicked(coord);
}

void ChMiniForm::update_settings(int channel, int range_, bool resist_, int offset_, int filter_)
{
    if(channel == ch_num){
        range = range_;
        resist = resist_;
        offset = offset_;
        filter = filter_;
        QString Str = "";
        switch (range) {
            case 0: Str = "±5.0 В";
            break;
            case 1: Str = "±2.5 В";
            break;
            case 2: Str = "±1.0 В";
            break;
            case 3: Str = "±0.5 В";
            break;
            case 4: Str = "±0.1 В";
            break;
            default: Str = "±5.0 В";
            break;
        }
        ui->label_range->setText(Str);
        if(resist) ui->label_resist->setText("1 МОм");
        else ui->label_resist->setText("50 Ом");
        ui->label_offset->setText(" " + QString::number(int((offset-32768)/12.8)) + " мВ");
        switch (filter) {
            case 0: Str = "Без фильтра";
            break;
            case 1: Str = "750 МГц";
            break;
            case 2: Str = "650 МГц";
            break;
            case 3: Str = "350 МГц";
            break;
            case 4: Str = "200 МГц";
            break;
            case 5: Str = "100 МГц";
            break;
            case 6: Str = "20 МГц";
            break;
            default: Str = "Без фильтра";
            break;
        }
        ui->label_filter->setText(Str);
    }
}

