/***************************************************************************
 *   Copyright (C) 2009 by Andrey Afletdinov <fheroes2@gmail.com>          *
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

#include "splitter.h"

/* splitter constructor */
Splitter::Splitter() : step(0), min(0), max(0), cur(0)
{
}

Splitter::Splitter(const Surface& sf, const Rect& rt)
    : SpriteMove(sf), area(rt), step(0), min(0), max(0), cur(0)
{
}

void Splitter::SetSprite(const Surface& sf)
{
    Set(sf, true);
}

void Splitter::SetArea(const Rect& rt)
{
    area = rt;
}

bool Splitter::isVertical() const
{
    return area.w < area.h;
}

/* set range */
void Splitter::SetRange(int smin, int smax)
{
    min = smin;
    max = smax;
    Point move;

    if (min < max)
    {
        step = 100 * (isVertical() ? area.h - h() : area.w - w()) / (max - min);
        cur = min;
        move = GetPositionCursor();
    }
    else
    {
        step = 0;
        move = Point(area.x + (area.w - w()) / 2,
                     area.y + (area.h - h()) / 2);
    }

    background.SetPos(move);
}

Point Splitter::GetPositionCursor() const
{
    Point res;

    if (isVertical())
    {
        res.x = area.x + (area.w - w()) / 2;
        res.y = area.y + cur * step / 100;
    }
    else
    {
        res.x = area.x + cur * step / 100;
        res.y = area.y + (area.h - h()) / 2;
    }

    return res;
}

void Splitter::RedrawCursor()
{
    Redraw();
}

void Splitter::HideCursor()
{
    Hide();
}

void Splitter::ShowCursor()
{
    Show();
}

void Splitter::MoveCenter()
{
    Move(area.x + (area.w - w()) / 2,
         area.y + (area.h - h()) / 2);
}

/* move splitter to pos */
void Splitter::MoveIndex(int num)
{
    if (num <= max && num >= min)
    {
        cur = num;
        Move(GetPositionCursor());
    }
}

/* forward spliter */
void Splitter::Forward()
{
    if (cur != max)
    {
        ++cur;
        Move(GetPositionCursor());
    }
}

/* backward spliter */
void Splitter::Backward()
{
    if (cur)
    {
        --cur;
        Move(GetPositionCursor());
    }
}
