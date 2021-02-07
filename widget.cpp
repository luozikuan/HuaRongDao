#include "widget.h"
#include "ui_widget.h"
#include "levels.h"

#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QString test = "ahhb"
                   "ciib"
                   "d.ee"
                   "00gf"
                   "00.f";
    QTimer::singleShot(100, [=](){
        int currentLevel = puzzle.size() - 1;
        //int currentLevel = 2;
        ui->widget->solve(puzzle.at(currentLevel));
    });
}

Widget::~Widget()
{
    delete ui;
}

