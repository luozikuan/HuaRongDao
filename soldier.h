#ifndef SOLDIER_H
#define SOLDIER_H

#include "move.h"
#include <QChar>
#include <QVector>

struct Pos
{
    Pos(int row, int col);
    int row = -1;
    int col = -1;
};

class Soldier
{
public:
    Soldier(QChar id = '0');
    QChar id() const;
    void setId(QChar id);
    const QVector<Pos> &blocks() const;
    void addBlock(Pos p);
    bool changePos(const Move &m);
    QString toString() const;

private:
    QChar m_id;
    QVector<Pos> m_blocks;
};

#endif // SOLDIER_H
