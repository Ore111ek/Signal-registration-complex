#include "chanform.h"
#include "ui_chanform.h"


ChanForm::ChanForm(int num, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChanForm)
{
    numOfChannel = num;
    ui->setupUi(this);
    ui->label_chNumber->setText("Канал " + QString::number(num));
}

ChanForm::~ChanForm()
{
    delete ui;
}

void ChanForm::setButtonName(QString btn_name)
{
    ui->btn_saveSettings->setText(btn_name);
}

void ChanForm::setChannelName(QString ch_name)
{
    ui->label_chNumber->setText(ch_name);
}

void ChanForm::show_settings(QPoint bottom)
{
    //this->move(bottom.x() - this->width()/2, bottom.y() - this->height()); // bottom - точка снизу посередине ChanForm
    int x = bottom.x() - this->width()/2;
    int screenWidth = QGuiApplication::primaryScreen()->geometry().width();
    if(x + this->width() > screenWidth){
        x = screenWidth - this->width();
    }else if(x < 0){
        x = 0;
    }
    this->move(x, bottom.y() - this->height());
    this->hide();
    this->show();
}



void ChanForm::on_DAC_codeSpin_valueChanged(int arg1)
{
    ui->DAC_slider->setSliderPosition(arg1);
    ui->DAC_mvSpin->setValue(double(arg1-32768)/12.8);
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}


void ChanForm::on_DAC_slider_sliderMoved(int position)
{
    ui->DAC_codeSpin->setValue(position);
    ui->DAC_mvSpin->setValue((position-32768)/12.8);
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}


void ChanForm::on_btn_DAC_setZero_clicked()
{
    ui->DAC_slider->setSliderPosition(32768);
    ui->DAC_codeSpin->setValue(32768);
    ui->DAC_mvSpin->setValue(0);
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}


void ChanForm::on_DAC_mvSpin_valueChanged(double arg1)
{
    int code = arg1*12.8+32768;
    ui->DAC_slider->setSliderPosition(code);
    ui->DAC_codeSpin->setValue(code);
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}

void ChanForm::update_Settings(int range, bool resist, int offset, int filter)
{

    ui->comboBox_range->setCurrentIndex(range);
    if(resist) ui->radio_1MOm->setChecked(true);
    else ui->radio_50Om->setChecked(true);
    //ui->DAC_slider->setValue(offset);
    ui->DAC_codeSpin->setValue(offset);
    //ui->DAC_mvSpin->setValue((offset-32768)/12.8);
    ui->comboBox_filter->setCurrentIndex(filter);
}


void ChanForm::on_btn_saveSettings_clicked()
{
    emit sendChSettings(numOfChannel,ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}

void ChanForm::on_radio_1MOm_clicked()
{
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}

void ChanForm::on_radio_50Om_clicked()
{
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}

void ChanForm::on_comboBox_range_currentIndexChanged(int index)
{
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}

void ChanForm::on_comboBox_filter_currentIndexChanged(int index)
{
    emit settings_changed(ui->comboBox_range->currentIndex(),ui->radio_1MOm->isChecked(),ui->DAC_slider->value(),ui->comboBox_filter->currentIndex());
}

