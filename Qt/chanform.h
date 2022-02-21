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
public slots:
    void show_settings(QPoint bottom);
    void on_btn_saveSettings_clicked();
private slots:

    void on_DAC_codeSpin_valueChanged(int arg1);

    void on_DAC_slider_sliderMoved(int position);

    void on_btn_DAC_setZero_clicked();

    void on_DAC_mvSpin_valueChanged(double arg1);



private:
    Ui::ChanForm *ui;
    int numOfChannel;

signals:
    void sendChSettings(int channel,int range,int divider,bool resist,int offset,int filter);
};

#endif // CHANFORM_H
