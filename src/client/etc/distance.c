
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/distance.h>
#include <fightable/sanitizer.h>
#include <math.h>

double _fDistPointToCircle(Vector2 point, Vector2 circle_center, double circle_radius) {
    if (circle_radius <= 0) return 0;

    double dx = point.x - circle_center.x;
    double dy = point.y - circle_center.y;
    double distance  = sqrt(dx * dx + dy * dy);
    double normalized_distance = 1 - (distance / circle_radius);

    return _fSanitizeDouble(normalized_distance, 0, 1);
}

double _fDistPointToPoint(Vector2 point_a, Vector2 point_b) {
    float dx = point_b.x - point_a.x;
    float dy = point_b.y - point_a.y;
    return sqrt(dx * dx + dy * dy);
}
