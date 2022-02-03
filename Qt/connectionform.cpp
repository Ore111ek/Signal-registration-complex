#include "connectionform.h"
#include "ui_connectionform.h"

ConnectionForm::ConnectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectionForm)
{
    ui->setupUi(this);
}

ConnectionForm::~ConnectionForm()
{
    delete ui;
}
