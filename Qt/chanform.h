#ifndef CHANFORM_H
#define CHANFORM_H

#include <QWidget>
#include <QScreen>

namespace Ui {
class ChanForm;
}

class ChanForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChanForm(int numOfChannel, QWidget *parent = nullptr);
    ~ChanForm();
    void setButtonName(QString btn_name);
    void setChannelName(QString ch_name);
public slots:
    void show_settings(QPoint bottom);
    void on_btn_saveSettings_clicked();
private slots:

    void on_DAC_codeSpin_valueChanged(int arg1);

    void on_DAC_slider_sliderMoved(int position);

    void on_btn_DAC_setZero_clicked();

    void on_DAC_mvSpin_valueChanged(double arg1);

    void update_Settings(int range,int divider,bool resist,int offset,int filter);

    void on_radio_1MOm_clicked();

    void on_radio_50Om_clicked();

    void on_comboBox_divider_currentIndexChanged(int index);

    void on_comboBox_range_currentIndexChanged(int index);

    void on_comboBox_filter_currentIndexChanged(int index);

private:
    Ui::ChanForm *ui;
    int numOfChannel;

signals:
    void sendChSettings(int channel,int range,int divider,bool resist,int offset,int filter);
    void settings_changed(int range,int divider,bool resist,int offset,int filter);
};

#endif // CHANFORM_H
