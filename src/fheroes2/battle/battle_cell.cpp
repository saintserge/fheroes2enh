/***************************************************************************
 *   Copyright (C) 2010 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "settings.h"
#include "army_troop.h"
#include "battle_board.h"
#include "battle_troop.h"

#define INFL 12

Battle::Position& Battle::Position::operator=(const Position& pos)
{
    first = pos.first;
    second = pos.second;
    return *this;
}

void Battle::Position::Set(s32 head, bool wide, bool reflect)
{
    first = Board::GetCell(head);

    if (first && wide)
        second = Board::GetCell(first->GetIndex(), reflect ? RIGHT : LEFT);
}

void Battle::Position::Swap()
{
    if (first && second)
        std::swap(first, second);
}

Battle::Cell* Battle::Position::GetHead()
{
    return first;
}

Battle::Cell* Battle::Position::GetTail()
{
    return second;
}

const Battle::Cell* Battle::Position::GetHead() const
{
    return first;
}

const Battle::Cell* Battle::Position::GetTail() const
{
    return second;
}

Rect Battle::Position::GetRect() const
{
    if (first)
        return second ? Rect::Get(first->GetPos(), second->GetPos(), false) : first->GetPos();

    return {};
}

Battle::Position Battle::Position::GetCorrect(const Unit& b, s32 head)
{
    Position result;

    result.first = Board::GetCell(head);

    if (!result.first || !b._monster.isWide())
        return result;
    result.second = Board::GetCell(head, b.isReflect() ? RIGHT : LEFT);

    if (!result.second ||
        (result.second != b.GetPosition().GetHead() && !result.second->isPassable1(true)))
    {
        result.second = Board::GetCell(head, b.isReflect() ? LEFT : RIGHT);

        if (!result.second)
            result.second = Board::GetCell(head, b.isReflect() ? RIGHT : LEFT);

        if (result.second)
            std::swap(result.first, result.second);
    }

    return result;
}

bool Battle::Position::isReflect() const
{
    return first && second && first->GetIndex() < second->GetIndex();
}

bool Battle::Position::isValid() const
{
    return first && (!second ||
        (LEFT | RIGHT) & Board::GetDirection(first->GetIndex(), second->GetIndex()));
}


Battle::Cell::Cell() : index(0), object(0), direction(UNKNOWN), quality(0), troop(nullptr)
{
    for (auto& cell : coord)
    {
        cell = Point{};
    }
}

Battle::Cell::Cell(s32 ii) : index(ii), object(0), direction(UNKNOWN), quality(0), troop(nullptr)
{
    SetArea(Rect());
}

void Battle::Cell::SetArea(const Rect& area)
{
    pos.x = area.x + 88 - (index / ARENAW % 2 ? CELLW / 2 : 0) + (CELLW - 1) * (index % ARENAW);
    pos.y = area.y + 85 + (CELLH / 4 * 3 - 1) * (index / ARENAW);
    pos.w = CELLW;
    pos.h = CELLH;

    // center
    coord[0] = Point(INFL * pos.x + INFL * pos.w / 2, INFL * pos.y + INFL * pos.h / 2);
    // coordinates
    coord[1] = Point(INFL * pos.x, INFL * pos.y + INFL * pos.h / 4);
    coord[2] = Point(INFL * pos.x + INFL * pos.w / 2, INFL * pos.y);
    coord[3] = Point(INFL * pos.x + INFL * pos.w, INFL * pos.y + INFL * pos.h / 4);
    coord[4] = Point(INFL * pos.x + INFL * pos.w, INFL * pos.y + INFL * pos.h - INFL * pos.h / 4);
    coord[5] = Point(INFL * pos.x + INFL * pos.w / 2, INFL * pos.y + INFL * pos.h);
    coord[6] = Point(INFL * pos.x, INFL * pos.y + INFL * pos.h - INFL * pos.h / 4);
}

Battle::direction_t Battle::Cell::GetTriangleDirection(const Point& dst) const
{
    const Point pt(INFL * dst.x, INFL * dst.y);

    if (pt == coord[0])
        return CENTER;
    if (pt.inABC(coord[0], coord[1], coord[2]))
        return TOP_LEFT;
    if (pt.inABC(coord[0], coord[2], coord[3]))
        return TOP_RIGHT;
    if (pt.inABC(coord[0], coord[3], coord[4]))
        return RIGHT;
    if (pt.inABC(coord[0], coord[4], coord[5]))
        return BOTTOM_RIGHT;
    if (pt.inABC(coord[0], coord[5], coord[6]))
        return BOTTOM_LEFT;
    if (pt.inABC(coord[0], coord[1], coord[6]))
        return LEFT;

    return UNKNOWN;
}

bool Battle::Cell::isPositionIncludePoint(const Point& pt) const
{
    return UNKNOWN != GetTriangleDirection(pt);
}

s32 Battle::Cell::GetIndex() const
{
    return index;
}

s32 Battle::Cell::GetQuality() const
{
    return quality;
}

void Battle::Cell::SetObject(int val)
{
    object = val;
}

void Battle::Cell::SetDirection(int val)
{
    direction = val;
}

void Battle::Cell::SetQuality(uint32_t val)
{
    quality = val;
}

int Battle::Cell::GetObject() const
{
    return object;
}

int Battle::Cell::GetDirection() const
{
    return direction;
}

const Rect& Battle::Cell::GetPos() const
{
    return pos;
}

const Battle::Unit* Battle::Cell::GetUnit() const
{
    return troop;
}

Battle::Unit* Battle::Cell::GetUnit()
{
    return troop;
}

void Battle::Cell::SetUnit(Unit* val)
{
    troop = val;
}

bool Battle::Cell::isPassable4(const Unit& b, const Cell& from) const
{
    if (!b._monster.isWide())
        return isPassable1(true);
    const int dir = Board::GetDirection(from.index, index);

    switch (dir)
    {
    case BOTTOM_RIGHT:
    case TOP_RIGHT:
    case BOTTOM_LEFT:
    case TOP_LEFT:
        {
            const bool reflect = (BOTTOM_LEFT | TOP_LEFT) & dir;
            const Cell* tail = Board::GetCell(index, reflect ? RIGHT : LEFT);
            return tail && tail->isPassable1(true) && isPassable1(true);
        }

    case LEFT:
    case RIGHT:
        return isPassable1(true) || index == b.GetTailIndex();

    default:
        break;
    }

    return isPassable1(true);
}

bool Battle::Cell::isPassable3(const Unit& b, bool check_reflect) const
{
    if (!b._monster.isWide())
        return isPassable1(true);
    if (index == b.GetTailIndex() || index == b.GetHeadIndex()) return true;

    if (check_reflect)
    {
        const Cell* cell = Board::GetCell(index, b.isReflect() ? RIGHT : LEFT);
        return cell &&
            (cell->isPassable1(true) || cell->index == b.GetTailIndex() || cell->index == b.GetHeadIndex()) &&
            isPassable1(true);
    }

    Cell* left = Board::GetCell(index, LEFT);
    Cell* right = Board::GetCell(index, RIGHT);
    return ((left &&
            (left->isPassable1(true) || left->index == b.GetTailIndex() || left->index == b.GetHeadIndex())) ||
            (right && (right->isPassable1(true) || right->index == b.GetTailIndex() ||
                right->index == b.GetHeadIndex()))) &&
        isPassable1(true);
}

bool Battle::Cell::isPassable1(bool check_troop) const
{
    return 0 == object && (!check_troop || nullptr == troop);
}

void Battle::Cell::ResetQuality()
{
    quality = 0;
}

void Battle::Cell::ResetDirection()
{
    direction = UNKNOWN;
}

ByteVectorWriter& Battle::operator<<(ByteVectorWriter& msg, const Cell& c)
{
    return msg << c.index << c.object << c.direction << c.quality <<
        (c.troop ? c.troop->GetUID() : static_cast<uint32_t>(0));
}

ByteVectorReader& Battle::operator>>(ByteVectorReader& msg, Cell& c)
{
    uint32_t uid = 0;
    msg >> c.index >> c.object >> c.direction >> c.quality >> uid;
    c.troop = GetArena()->GetTroopUID(uid);
    return msg;
}
