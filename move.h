#ifndef MOVE_H
#define MOVE_H

#include <QString>
#include <Qt>

using Direction = Qt::ArrowType;

class Move
{
public:
    Move(QChar id = '0', Direction d = Qt::NoArrow, int distance = 1);
    QChar id() const;
    Direction direction() const;
    int distance() const;

    void reverse();
    Q_REQUIRED_RESULT Move reversed() const;
    bool operator<(const Move &other) const;
    bool operator==(const Move &other) const;
    QString toString() const;

private:
    QChar m_id;
    Direction m_direction;
    int m_distance;
};

#endif // MOVE_H
