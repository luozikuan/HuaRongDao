#include "widget.h"
#include "ui_widget.h"

#include <QTimer>

QString puzzle {
    "a00b"
    ".00c"
    "deef"
    "ghif"
    "ghi."
};

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString test = "ahhb"
                   "ciib"
                   "dgee"
                   "00.f"
                   "00.f";
    QTimer::singleShot(100, [=](){
        ui->widget->solve(test);
    });
}

Widget::~Widget()
{
    delete ui;
}

