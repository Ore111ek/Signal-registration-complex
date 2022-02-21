#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::setChannelsGrid(ChanForm *chan[])
{
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        ui->gridLayout_Channels->addWidget(chan[i], int(i/4), i%4);
        chan[i]->show();
        ch[i] = chan[i];
        connect(this, SIGNAL (setAllSettings()), chan[i], SLOT (on_btn_saveSettings_clicked()));
    }
}

void SettingsForm::closeEvent(QCloseEvent *event)
{
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        ui->gridLayout_Channels->removeWidget(ch[i]);
        //qDebug() << "Parent: " <<ch[i]->parent();
        ch[i]->setParent(nullptr);
        //qDebug() << "new Parent: " <<ch[i]->parent();
        //ch[i]->hide();
    }
    // do event->ignore();
    //qDebug() << "Count: " << ui->gridLayout_Channels->count();
    QWidget::closeEvent(event);

}


void SettingsForm::on_btn_setAll_clicked()
{
    emit setAllSettings();
}

