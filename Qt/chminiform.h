#ifndef CHMINIFORM_H
#define CHMINIFORM_H

#include <QWidget>

namespace Ui {
class ChMiniForm;
}

class ChMiniForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChMiniForm(int ch_number,QWidget *parent = nullptr);
    ~ChMiniForm();

private slots:
    void on_closeButton_clicked();

    void on_editButton_clicked();

private:
    Ui::ChMiniForm *ui;

signals:
    void close_clicked();
    void edit_clicked(QPoint coord);
};

#endif // CHMINIFORM_H
