
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fraylib.h>

#ifdef TARGET_SUPPORTS_RLGL_3D
extern void rlColor3f(float x, float y, float z);
extern void rlVertex3f(float x, float y, float z);
#endif
#ifdef TARGET_SUPPORTS_RLGL_2D
extern void rlBegin(int mode);
extern void rlEnd(void);
#endif

// Draw a grid centered at (0, y, 0)
void DrawGridEx(int slices, float spacing, float y)
{
#ifdef TARGET_SUPPORTS_3D
    int halfSlices = slices / 2;

    rlBegin(0x0001);
    for (int i = -halfSlices; i <= halfSlices; i++)
    {
        if (i == 0)
        {
            rlColor3f(0.5f, 0.5f, 0.5f);
        }
        else
        {
            rlColor3f(0.75f, 0.75f, 0.75f);
        }

        rlVertex3f((float)i * spacing, y, (float)-halfSlices * spacing);
        rlVertex3f((float)i * spacing, y, (float)halfSlices * spacing);

        rlVertex3f((float)-halfSlices * spacing, y, (float)i * spacing);
        rlVertex3f((float)halfSlices * spacing, y, (float)i * spacing);
    }
    rlEnd();
#endif
}
