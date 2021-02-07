#ifndef ROAD_H
#define ROAD_H

#include "move.h"
#include "soldier.h"
#include <QVector>
#include <QString>
#include <QMap>
#include <QSize>

using RoadData = QString;

struct Size
{
    Size(int r, int c) : rowCount(r), colCount(c) {}
    int rowCount = -1;
    int colCount = -1;
};

class Road
{
public:
    Road();
    Road(const Road &other);
    Road &operator=(const Road& other);

    void setData(const RoadData &data, Size size);
    void parseSoldier();
    void moveSoldier(const Move &m);
    void printSoldiers() const;
    QMap<QChar, Soldier> soldiers() const;
    bool checkSuccess() const;
    QVector<Move> availableMoves();
    const RoadData &road() const;
    Size size() const;
    QString toString() const;

    void reset();

private:
    QChar &element(int x, int y);
    QChar element(int x, int y) const;

    RoadData m_road;
    QMap<QChar, Soldier> m_soldiers;

private:
    Size m_size = Size(4, 5);
};

#endif // ROAD_H
