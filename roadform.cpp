#include "roadform.h"
#include "ui_roadform.h"
#include <QDebug>
#include <QStack>
#include <chrono>
#include <algorithm>
#include <QRegularExpression>
#include <QtMath>
#include <QPainter>

static QChar mainBarName('0');

RoadForm::RoadForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoadForm)
{
    ui->setupUi(this);

    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &RoadForm::showStep);
}

RoadForm::~RoadForm()
{
    delete ui;
}

void RoadForm::solve(QString puzzle)
{
    clearData();
    setData(puzzle);
}

void RoadForm::onKeyUpPressed()
{
    int row = ui->listWidget->currentRow();
    row = qBound(0, row-1, ui->listWidget->count()-1);
    ui->listWidget->setCurrentRow(row);
}

void RoadForm::onKeyDownPressed()
{
    int row = ui->listWidget->currentRow();
    row = qBound(0, row+1, ui->listWidget->count()-1);
    ui->listWidget->setCurrentRow(row);
}

void RoadForm::clearData()
{
    road.reset();
    appeared.clear();
    while (!tree.isEmpty()) {
        tree.dequeue();
    }
}

void RoadForm::setData(QString puzzle)
{
    static const QRegularExpression re("^[0-9.a-zA-Z]+$");

    QString input = puzzle;
    input.remove(QRegularExpression(R"([^.\w])"));
    qDebug() << input;
    if (input.contains(re)) {
        road.setData(input, Size(puzzle.size()/4, 4));
        if (!road.soldiers().contains(mainBarName)) {
            qDebug() << "no main bar";
        } else {
            startSolve();
        }
    } else {
        qDebug() << "invalid input";
    }
}

void RoadForm::startSolve()
{
    bool success = false;

    auto t_start = std::chrono::high_resolution_clock::now();
    QSharedPointer<TreeNode> node = QSharedPointer<TreeNode>::create();
    node->road = road;
    node->move = {mainBarName, Qt::NoArrow, 0}; // invalid value, distance is zero
    node->parent.clear();

    qDebug() << "start";
    appeared.insert(node->road.road());
    tree.enqueue(node);

    while (!tree.isEmpty()) {
        QSharedPointer<TreeNode> firstNode = tree.dequeue();

        QVector<Move> moves = firstNode->road.availableMoves();

        Road curBoard = firstNode->road;
        for (Move m: qAsConst(moves)) {
            curBoard.moveSoldier(m);

            auto result = appeared.insert(curBoard.road());

            if (result.second) {
                QSharedPointer<TreeNode> childNode = QSharedPointer<TreeNode>::create();
                childNode->road = curBoard;
                childNode->move = m;
                childNode->parent = firstNode;
                if (childNode->road.checkSuccess()) {
                    qDebug() << "success";
                    success = true;
                    printPath(childNode);
                    break;
                }
                tree.enqueue(childNode);
            }
            m.reverse();
            curBoard.moveSoldier(m);
        }

        if (success) {
            break;
        }
    }

    auto t_end = std::chrono::high_resolution_clock::now();
    qDebug() << "end, time used: "
         << std::chrono::duration<double, std::milli>(t_end-t_start).count()
         << " ms"
         << Qt::endl;
}

void RoadForm::showStep(int index)
{
    if (index < 0 || roadSteps.size() <= index) {
        qDebug() << "index out of range" << roadSteps.size() << index;
        return ;
    }

    const Road &board = roadSteps.at(index);
    const QSize blockSize(ui->label->width() / board.size().colCount,
                          ui->label->height() / board.size().rowCount);

    qDebug() << "label size: " << ui->label->size();

    QImage image(ui->label->size(), QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.fillRect(image.rect(), QColor(85, 20, 4));

    // draw grid
    painter.save();
    for (int row = 0; row < board.size().rowCount; row++) {
        painter.setPen(QColor(66, 14, 2));
        painter.drawLine(QPoint(0, blockSize.height() * row), QPoint(ui->label->width(), blockSize.height() * row));
        painter.setPen(QColor(113, 38, 8));
        painter.drawLine(QPoint(0, blockSize.height() * row + 1), QPoint(ui->label->width(), blockSize.height() * row + 1));
    }
    for (int col = 0; col < board.size().colCount; col++) {
        painter.setPen(QColor(66, 14, 2));
        painter.drawLine(QPoint(blockSize.width() * col, 0), QPoint(blockSize.width() * col, ui->label->height()));
        painter.setPen(QColor(113, 38, 8));
        painter.drawLine(QPoint(blockSize.width() * col + 1, 0), QPoint(blockSize.width() * col + 1, ui->label->height()));
    }
    painter.restore();

    foreach (const Soldier &bar, board.soldiers().values()) {
        QColor color = bar.id() == mainBarName ? QColor(214, 9, 21) : QColor(246, 160, 81);
        paintBar(painter, blockSize, bar, color);
    }

    ui->label->setPixmap(QPixmap::fromImage(image));
}

void RoadForm::paintBar(QPainter &painter, QSize blockSize, const Soldier &bar, QColor color)
{
    const QVector<Pos> &blocks = bar.blocks();
    QPoint topLeft(blocks.first().col * blockSize.width(),
                   blocks.first().row * blockSize.height());
    QPoint bottomRight((1+blocks.last().col) * blockSize.width(),
                       (1+blocks.last().row) * blockSize.height());
    QRect barRect(topLeft, bottomRight);
    barRect.adjust(2, 2, -1, -1);
    painter.fillRect(barRect, color);
    painter.save();
    painter.setPen(color.lighter(120));
    painter.drawLine(barRect.topLeft(), barRect.topRight());
    painter.drawLine(barRect.topLeft() + QPoint(1, 1), barRect.topRight() + QPoint(-1, 1));
    painter.drawLine(barRect.topRight(), barRect.bottomRight());
    painter.drawLine(barRect.topRight() + QPoint(-1, 1), barRect.bottomRight() + QPoint(-1, -1));
    painter.setPen(color.darker(150));
    painter.drawLine(barRect.topLeft(), barRect.bottomLeft());
    painter.drawLine(barRect.topLeft() + QPoint(1, 1), barRect.bottomLeft() + QPoint(1, -1));
    painter.drawLine(barRect.bottomLeft(), barRect.bottomRight());
    painter.drawLine(barRect.bottomLeft() + QPoint(1, -1), barRect.bottomRight() + QPoint(-1, -1));
    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter.setPen(QColor(85, 20, 4));
    painter.drawText(barRect, bar.id(), option);
    painter.restore();
}

void RoadForm::printPath(const QSharedPointer<TreeNode> path)
{
    roadSteps.clear();

    qDebug() << "===== solution =====";
    QStack<Move> solution;
    QSharedPointer<TreeNode> item = path;

    while (!item->parent.isNull()) {
        solution.push(item->move);
        roadSteps.prepend(item->road);
        item = item->parent;
    }
    roadSteps.prepend(item->road);

    QStringList resultItems;
    int cnt = 1;
    int textWidth = int(std::log(solution.size()) / std::log(10)) + 1;
    while (!solution.isEmpty()) {
        Move m = solution.top();
        // qDebug() << cnt << ":" << m.toString();
        resultItems << QString("%1: %2").arg(cnt, textWidth).arg(m.toString());
        if (cnt % 5 == 0) {
            // qDebug() << "";
        }
        cnt++;
        solution.pop();
    }
    resultItems << QString("%1: success!").arg(cnt, textWidth);
    // qDebug() << "";
    ui->listWidget->clear();
    ui->listWidget->addItems(resultItems);
    ui->listWidget->setCurrentRow(0);
    ui->listWidget->setFocus();
}
