#include "chanform.h"
#include "ui_chanform.h"

ChanForm::ChanForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChanForm)
{
    ui->setupUi(this);
}

ChanForm::~ChanForm()
{
    delete ui;
}

void ChanForm::show_settings(QPoint bottom)
{
    this->move(bottom.x() - this->width()/2,bottom.y() - this->height());
    this->hide();
    this->show();
}
