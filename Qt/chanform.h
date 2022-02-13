#ifndef CHANFORM_H
#define CHANFORM_H

#include <QWidget>

namespace Ui {
class ChanForm;
}

class ChanForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChanForm(QWidget *parent = nullptr);
    ~ChanForm();
public slots:
    void show_settings(QPoint bottom);
private:
    Ui::ChanForm *ui;
};

#endif // CHANFORM_H
