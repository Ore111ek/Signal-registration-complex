#include "settingsform.h"
#include "ui_settingsform.h"

SettingsForm::SettingsForm(QWidget *parent, ChanForm *chan[]) :
    QWidget(parent),
    ui(new Ui::SettingsForm)
{
    ui->setupUi(this);
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        ch[i] = chan[i];
        connect(this, SIGNAL (setAllSettings()), chan[i], SLOT (on_btn_saveSettings_clicked()));
        connect(ui->CommonChan, SIGNAL (settings_changed(int,int,bool,int,int)), chan[i], SLOT (update_Settings(int,int,bool,int,int)));
        //connect(ui->CommonChan, SIGNAL (sendChSettings(int,int,int,bool,int,int)), chan[i], SLOT (update_Settings(int,int,bool,int,int)));
    }
    ui->CommonChan->setButtonName("Установить\nво все каналы");
    ui->CommonChan->setChannelName("Все каналы");
    connect(ui->CommonChan, SIGNAL (sendChSettings(int,int,int,bool,int,int)), this, SLOT (changeAllCh(int,int,int,bool,int,int)));
}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::setChannelsGrid()
{
    for(int i = 0; i < NUM_OF_CHANNELS; i++){
        ui->gridLayout_Channels->addWidget(ch[i], int(i/4), i%4);
        ch[i]->show();
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

void SettingsForm::changeAllCh(int channel, int range, int divider, bool resist, int offset, int filter)
{
    emit ui->CommonChan->settings_changed(range, divider, resist, offset, filter);
}


