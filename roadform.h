#ifndef ROADFORM_H
#define ROADFORM_H

#include "road.h"
#include <QWidget>
#include <QSharedPointer>
#include <set>
#include <QQueue>
#include <QList>

using std::set;

namespace Ui {
class RoadForm;
}

class RoadForm : public QWidget
{
    Q_OBJECT
    struct TreeNode
    {
        Road road;
        QSharedPointer<TreeNode> parent;
        Move move;
    };


public:
    explicit RoadForm(QWidget *parent = nullptr);
    ~RoadForm();

public slots:
    void solve(QString puzzle);
    void onKeyUpPressed();
    void onKeyDownPressed();

private slots:
    void clearData();
    void setData(QString puzzle);
    void startSolve();
    void showStep(int index);

private:
    void printPath(const QSharedPointer<TreeNode> path);
    void paintBar(QPainter &painter, QSize blockSize, const Soldier &bar, QColor color);

    Ui::RoadForm *ui;
    Road road;
    QList<Road> roadSteps;
    set<RoadData> appeared;
    QQueue<QSharedPointer<TreeNode> > tree;
};

#endif // ROADFORM_H
