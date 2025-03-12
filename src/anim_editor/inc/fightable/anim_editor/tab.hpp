
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>
#include <raylib.h>

namespace fightable {
    class tab {
    private:
        struct Logic {
            Vector2 position = {}; // Position

            Vector2 _internal[4] = {};

            bool movable = true; // Can move map around with mouse

            bool reverse_accel_x = false;
            bool reverse_accel_y = false;
        } cam_logic;

        float _MapEaseInterpolation(float x);
        bool _CheckFloatAroundValue(float x, float r);

    protected:
        RLRectangle _area = {};
        Color _col = WHITE;
        std::string _title = "";
        Camera2D _cam = {};

        bool _verticalMovement = false;
        bool _horizontalMovement = false;

        bool _limitScroll = false;
        Vector2 _limitScrollSzMin = {};
        Vector2 _limitScrollSzHigh = {};

        virtual void drawTabTitle();
        void setCamPos(const Vector2 &v);

    public:
        tab(RLRectangle area, Color col, const std::string &title);
        void visit();
        virtual void draw() = 0;
    };
}
