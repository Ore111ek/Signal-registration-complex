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
    explicit SettingsForm(QWidget *parent = nullptr);
    ~SettingsForm();
    void setChannelsGrid(ChanForm *chan[]);

private slots:

    void on_btn_setAll_clicked();
protected:
     void closeEvent(QCloseEvent *event) override;
private:
    Ui::SettingsForm *ui;
    ChanForm *ch[8];
    QLayout *layout;

signals:
    void setAllSettings();
};

#endif // SETTINGSFORM_H
