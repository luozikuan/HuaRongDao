#include "move.h"

Move::Move(QChar id, Direction d, int distance)
    : m_id(id)
    , m_direction(d)
    , m_distance(distance)
{

}

QChar Move::id() const
{
    return m_id;
}

Direction Move::direction() const
{
    return m_direction;
}

int Move::distance() const
{
    return m_distance;
}

void Move::reverse()
{
    switch (m_direction)
    {
    case Qt::UpArrow   : m_direction = Qt::DownArrow ; break;
    case Qt::DownArrow : m_direction = Qt::UpArrow   ; break;
    case Qt::LeftArrow : m_direction = Qt::RightArrow; break;
    case Qt::RightArrow: m_direction = Qt::LeftArrow ; break;
    default: break;
    }
}

bool Move::operator<(const Move &other) const
{
    return m_id < other.m_id;
}

QString Move::toString() const
{
    static const QString strDirection[5] = {
        "     ",
        " up  ",
        "down ",
        "left ",
        "right"
    };
    return QString("[%1] %2 %3").arg(m_id).arg(strDirection[m_direction]).arg(m_distance);
}
