#include "talkwidget.h"
#include "ui_talkwidget.h"

TalkWidget::TalkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TalkWidget)
{
    ui->setupUi(this);
}

TalkWidget::~TalkWidget()
{
    delete ui;
}
