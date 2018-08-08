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

#pragma once

#include "thread.h"
#include "interface_border.h"

class Surface;

class Castle;

class Heroes;

enum
{
    STATUS_UNKNOWN, STATUS_DAY, STATUS_FUNDS, STATUS_ARMY, STATUS_RESOURCE, STATUS_AITURN
};

namespace Interface
{
    class Basic;

    class StatusWindow : public BorderWindow
    {
    public:
        StatusWindow(Basic &);

        void SetPos(s32, s32);

        void SavePosition();

        void SetRedraw() const;

        void Reset();

        void Redraw();

        void NextState();

        int GetState() const;

        void SetState(int);

        void SetResource(int, uint32_t);

        void RedrawTurnProgress(uint32_t);

        void QueueEventProcessing();

        static void ResetTimer();

    private:
        void DrawKingdomInfo(int oh = 0) const;

        void DrawDayInfo(int oh = 0) const;

        void DrawArmyInfo(int oh = 0) const;

        void DrawResourceInfo(int oh = 0) const;

        void DrawBackground() const;

        void DrawAITurns() const;

        static uint32_t ResetResourceStatus(uint32_t, void *);

        static uint32_t RedrawAIStatus(uint32_t, void *);

        Basic &interface;

        int state;
        int oldState;
        int lastResource;
        uint32_t countLastResource{};
        SDL::Timer timerShowLastResource;
        SDL::Timer timerRedrawAIStatus;
        uint32_t turn_progress{};
    };
}
