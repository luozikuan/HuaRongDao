#include "road.h"
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QtMath>
#include <QIODevice>

Road::Road()
{
}

Road::Road(const Road &other)
{
    m_road = other.m_road;
    m_soldiers = other.m_soldiers;
    m_size = other.m_size;
}

Road &Road::operator=(const Road &other)
{
    m_road = other.m_road;
    m_soldiers = other.m_soldiers;
    m_size = other.m_size;
    return *this;
}

void Road::reset()
{
    m_road.clear();
    m_soldiers.clear();
}

void Road::setData(const RoadData &data, Size size)
{
    m_road = data;
    m_size = size;
    qDebug() << data << size.rowCount << "row," << size.colCount << "column";
    parseSoldier();
    qDebug() << "checkSuccess" << checkSuccess();
}

void Road::parseSoldier()
{
    for (int i = 0; i < m_size.rowCount; i++) {
        for (int j = 0; j < m_size.colCount; j++) {
            QChar id = element(i, j);
            if (id == '.')
                continue;

            Soldier &soldier = m_soldiers[id];
            soldier.setId(id);
            soldier.addBlock(Pos(i, j));
        }
    }
}

void Road::moveSoldier(const Move &m)
{
    Soldier &soldier = m_soldiers[m.id()];
    for (const Pos &p : soldier.blocks()) {
        element(p.row, p.col) = '.';
    }
    soldier.changePos(m);
    for (const Pos &p : soldier.blocks()) {
        element(p.row, p.col) = m.id();
    }
}

void Road::printSoldiers() const
{
    for (Soldier soldier : qAsConst(m_soldiers)) {
        qDebug() << soldier.toString();
    }
}

QMap<QChar, Soldier> Road::soldiers() const
{
    return m_soldiers;
}

bool Road::checkSuccess() const
{
    for (int row = m_size.rowCount-1; row > 0; row--) {
        // qDebug() << "check row" << row << ":" << element(row, 1) << element(row, 2);
        if (element(row, 1) == '.' && element(row, 2) == '.') {
            continue;
        } else if (element(row, 1) == '0' && element(row, 2) == '0') {
            return true;
        } else {
            return false;
        }
    }

    return false; // code should never goes here.
}

QVector<Move> Road::availableMoves()
{
    auto checkHaveConflict = [=](const Soldier &soldier, int rowDelta, int colDelta)->bool{
        for (Pos block : soldier.blocks()) {
            int newRow = block.row + rowDelta;
            int newCol = block.col + colDelta;
            if (newRow < 0 || newRow >= m_size.rowCount)
                return true;
            if (newCol < 0 || newCol >= m_size.colCount)
                return true;
            QChar e = element(newRow, newCol);
            if (e != soldier.id() && e != '.')
                return true;
        }
        return false;
    };

    QVector<Move> moves;
    for (auto it = m_soldiers.begin(); it != m_soldiers.end(); it++) {
        QChar id = it.key();
        Soldier &soldier = it.value();

        int delta = 0;
        while (true) {
            delta--;
            if (!checkHaveConflict(soldier, 0, delta)) {
                moves.push_back({ id, Qt::LeftArrow, -delta });
            } else {
                break;
            }
        }

        delta = 0;
        while (true) {
            delta++;
            if (!checkHaveConflict(soldier, 0, delta)) {
                moves.push_back({ id, Qt::RightArrow, delta });
            } else {
                break;
            }
        }

        delta = 0;
        while (true) {
            delta--;
            if (!checkHaveConflict(soldier, delta, 0)) {
                moves.push_back({ id, Qt::UpArrow, -delta });
            } else {
                break;
            }
        }

        delta = 0;
        while (true) {
            delta++;
            if (!checkHaveConflict(soldier, delta, 0)) {
                moves.push_back({ id, Qt::DownArrow, delta });
            } else {
                break;
            }
        }
    }
    return moves;
}

const RoadData &Road::road() const
{
    return m_road;
}

Size Road::size() const
{
    return m_size;
}

QString Road::toString() const
{
    QString result;
    QTextStream stream(&result, QIODevice::WriteOnly);
    for (int i = 0, len = m_road.size(); i < len; i++) {
        if (i != 0 && i % m_size.colCount == 0) {
            stream << Qt::endl;
        }
        stream << m_road[i];
    }
    return result;
}

QChar Road::element(int x, int y)
{
    return m_road[x*m_size.colCount + y];
}

QChar Road::element(int x, int y) const
{
    return m_road[x*m_size.colCount + y];
}
