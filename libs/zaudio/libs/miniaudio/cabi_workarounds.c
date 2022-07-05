#include "miniaudio.h"

// ma_engine
void WA_ma_engine_listener_get_position(const ma_engine* engine, ma_uint32 index, ma_vec3f* vout) {
    *vout = ma_engine_listener_get_position(engine, index);
}

void WA_ma_engine_listener_get_direction(const ma_engine* engine, ma_uint32 index, ma_vec3f* vout) {
    *vout = ma_engine_listener_get_direction(engine, index);
}

void WA_ma_engine_listener_get_velocity(const ma_engine* engine, ma_uint32 index, ma_vec3f* vout) {
    *vout = ma_engine_listener_get_velocity(engine, index);
}

void WA_ma_engine_listener_get_world_up(const ma_engine* engine, ma_uint32 index, ma_vec3f* vout) {
    *vout = ma_engine_listener_get_world_up(engine, index);
}

// ma_sound_group
void WA_ma_sound_group_get_direction_to_listener(const ma_sound_group* sgroup, ma_vec3f* vout) {
    *vout = ma_sound_group_get_direction_to_listener(sgroup);
}

void WA_ma_sound_group_get_position(const ma_sound_group* sgroup, ma_vec3f* vout) {
    *vout = ma_sound_group_get_position(sgroup);
}

void WA_ma_sound_group_get_direction(const ma_sound_group* sgroup, ma_vec3f* vout) {
    *vout = ma_sound_group_get_direction(sgroup);
}

void WA_ma_sound_group_get_velocity(const ma_sound_group* sgroup, ma_vec3f* vout) {
    *vout = ma_sound_group_get_velocity(sgroup);
}

// ma_sound
void WA_ma_sound_get_direction_to_listener(const ma_sound* sound, ma_vec3f* vout) {
    *vout = ma_sound_get_direction_to_listener(sound);
}

void WA_ma_sound_get_position(const ma_sound* sound, ma_vec3f* vout) {
    *vout = ma_sound_get_position(sound);
}

void WA_ma_sound_get_direction(const ma_sound* sound, ma_vec3f* vout) {
    *vout = ma_sound_get_direction(sound);
}

void WA_ma_sound_get_velocity(const ma_sound* sound, ma_vec3f* vout) {
    *vout = ma_sound_get_velocity(sound);
}
