#ifndef CONNECTIONFORM_H
#define CONNECTIONFORM_H

#include <QWidget>

namespace Ui {
class ConnectionForm;
}

class ConnectionForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionForm(QWidget *parent = nullptr);
    ~ConnectionForm();

private:
    Ui::ConnectionForm *ui;
signals:
    void ping(QString ip_str);
private slots:
    void on_pushButton_connectIP_clicked();
    void on_lineEdit_ip3_editingFinished();

    void on_lineEdit_ip2_editingFinished();

    void on_lineEdit_ip1_editingFinished();

    void on_lineEdit_ip0_editingFinished();

    void on_lineEdit_ip3new_editingFinished();

    void on_lineEdit_ip2new_editingFinished();

    void on_lineEdit_ip1new_editingFinished();

    void on_lineEdit_ip0new_editingFinished();

public slots:
    void handle_response(bool state);
};

#endif // CONNECTIONFORM_H
