#include "miniaudio.h"

#define ZAUDIO_API extern "C"

ZAUDIO_API void zma_sound_group_get_direction(const ma_sound_group* sgroup, ma_vec3f* vout) {
    *vout = ma_sound_group_get_direction(sgroup);
}
