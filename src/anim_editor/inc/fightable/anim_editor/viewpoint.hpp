
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/anim_editor/tab.hpp>
#include <vector>
#include <map>
#include <thread>

namespace fightable {
    class viewpoint_tab : public fightable::tab {
    private:
        std::map<int, std::vector<Vector2>> _points;
        std::map<int, std::vector<Vector2>> _drawablePoints;
        std::map<int, Color> _pointColors;
        std::map<int, std::vector<Vector2>> _keyframes;

        std::thread _calcThread;
        bool _stopCalcThread = false;

        bool _inDecart = false;
        bool _inMacro = false;

        float _maxHeight = 0.f;

        int _drawnPoints = 0;

        void DrawGrid2D(int sizeWidth, int sizeHeight, int spacing);
        RLRectangle getVisibleCameraArea();

        void postDraw() override;

        int getScaling();

        void updatePointRendering();
    public:
        viewpoint_tab();
        ~viewpoint_tab();

        void draw() override;
    };
}
