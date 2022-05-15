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
    explicit ChMiniForm(int ch_number, QWidget *parent = nullptr);
    ~ChMiniForm();
    int get_range(){return range;};
    int get_resist(){return resist;};
    int get_offset(){return offset;};
    int get_filter(){return filter;};

private slots:
    void on_closeButton_clicked();

    void on_editButton_clicked();

    void update_settings(int channel, int range, bool resist, int offset, int filter);

private:
    Ui::ChMiniForm *ui;
    int ch_num;
    int range;
    bool resist;
    int offset;
    int filter;

signals:
    void close_clicked();
    void edit_clicked(QPoint coord);
};

#endif // CHMINIFORM_H
