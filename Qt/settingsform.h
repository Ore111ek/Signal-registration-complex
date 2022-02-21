#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QWidget>
#include <QKeyEvent>
#include "DeviceConstants.h"
#include "chanform.h"

namespace Ui {
class SettingsForm;
}

class SettingsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = nullptr, ChanForm *chan[] = nullptr);
    ~SettingsForm();
    void setChannelsGrid();

private slots:

    void on_btn_setAll_clicked();
    void changeAllCh(int channel, int range, bool resist, int offset, int filter);
protected:
     void closeEvent(QCloseEvent *event) override;
private:
    Ui::SettingsForm *ui;
    ChanForm *ch[8];
    QLayout *layout;
    ChanForm * CommonChan;

signals:
    void setAllSettings();
};

#endif // SETTINGSFORM_H
