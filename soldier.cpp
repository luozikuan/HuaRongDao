#include "soldier.h"
#include <QDebug>
#include <QIODevice>

Pos::Pos(int row, int col)
    : row(row)
    , col(col)
{}

// =====

Soldier::Soldier(QChar id)
    : m_id(id)
{

}

QChar Soldier::id() const
{
    return m_id;
}

void Soldier::setId(QChar id)
{
    m_id = id;
}

const QVector<Pos> &Soldier::blocks() const
{
    return m_blocks;
}

void Soldier::addBlock(Pos p)
{
    m_blocks.push_back(p);
}

bool Soldier::changePos(const Move &m)
{
    if (m.id() != m_id) {
        qWarning() << "bar.id " << m_id << " and move.id " << m.id() << " not match";
        return false;
    }
    int deltaX = 0, deltaY = 0;
    switch (m.direction())
    {
    case Qt::UpArrow:
        deltaX = -m.distance();
        break;
    case Qt::DownArrow:
        deltaX = m.distance();
        break;
    case Qt::LeftArrow:
        deltaY = -m.distance();
        break;
    case Qt::RightArrow:
        deltaY = m.distance();
        break;
    default:
        return false;
    }
    for (Pos &p : m_blocks) {
        p.row += deltaX;
        p.col += deltaY;
        if (p.row < 0 || p.col < 0)
            return false;
    }
    return true;
}

QString Soldier::toString() const
{
    QString result;
    QTextStream stream(&result, QIODevice::WriteOnly);
    stream << "[" << m_id << "] " << ' ';
    for (Pos p : m_blocks)
        stream << '(' << p.row << ',' << p.col << ") ";

    return result;
}
