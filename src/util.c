/*
 *	Copyright 2020 Romain Durand
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
*/

#include "util.h"

int8_t sine(uint8_t i)
{
    static const int8_t lut[256] = {
        0, 3, 6, 9, 12, 16, 19, 22, 25, 28, 31, 34, 37, 40, 43, 46, 49, 51, 54,
        57, 60, 63, 65, 68, 71, 73, 76, 78, 81, 83, 85, 88, 90, 92, 94, 96, 98,
        100, 102, 104, 106, 107, 109, 110, 112, 113, 115, 116, 117, 118, 120,
        121, 122, 122, 123, 124, 125, 125, 126, 126, 126, 127, 127, 127, 127,
        127, 127, 127, 126, 126, 126, 125, 125, 124, 123, 122, 122, 121, 120,
        118, 117, 116, 115, 113, 112, 111, 109, 107, 106, 104, 102, 100, 98, 96,
        94, 92, 90, 88, 85, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60, 57, 54, 51,
        49, 46, 43, 40, 37, 34, 31, 28, 25, 22, 19, 16, 12, 9, 6, 3, 0, -3, -6,
        -9, -12, -16, -19, -22, -25, -28, -31, -34, -37, -40, -43, -46, -49,
        -51, -54, -57, -60, -63, -65, -68, -71, -73, -76, -78, -81, -83, -85,
        -88, -90, -92, -94, -96, -98, -100, -102, -104, -106, -107, -109, -110,
        -112, -113, -115, -116, -117, -118, -120, -121, -122, -122, -123, -124,
        -125, -125, -126, -126, -126, -127, -127, -127, -127, -127, -127, -127,
        -126, -126, -126, -125, -125, -124, -123, -122, -122, -121, -120, -118,
        -117, -116, -115, -113, -112, -111, -109, -107, -106, -104, -102, -100,
        -98, -96, -94, -92, -90, -88, -85, -83, -81, -78, -76, -73, -71, -68,
        -65, -63, -60, -57, -54, -51, -49, -46, -43, -40, -37, -34, -31, -28,
        -25, -22, -19, -16, -12, -9, -6, -3,
    };

    return lut[i];
}

int32_t rshift_round(int32_t val, int32_t rsh)
{
    if (val & (1 << rsh-1))
        return (val >> rsh) + 1;
    else
        return (val >> rsh);
}

int32_t constrain(int32_t val, int32_t min, int32_t max)
{
    int32_t r;

    if (val > max)
        r = max;
    else if (val < min)
        r = min;
    else
        r = val;

    return r;
}

int32_t signed_saturate(int32_t val, int32_t sat)
{
    int32_t min;
    int32_t max;

    if ((sat >= 1) && (sat <= 32)) {
        max = (1 << (sat - 1)) - 1;
        min = -max - 1;

        if (val > max)
            return max;
        if (val < min)
            return min;
    }

    return val;
}
