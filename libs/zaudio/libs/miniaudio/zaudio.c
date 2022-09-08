#include "miniaudio.h"
#include <stdlib.h>
#include <assert.h>
//#include <stdio.h>

static void* defaultAlloc(size_t size, void* user_data) {
    return malloc(size);
}
static void* defaulrRealloc(void* ptr, size_t size, void* user_data) {
    return realloc(ptr, size);
}
static void defaultFree(void* ptr, void* user_data) {
    return free(ptr);
}

static ma_allocation_callbacks s_mem = {
    .pUserData = NULL,
    .onMalloc = defaultAlloc,
    .onRealloc = defaulrRealloc,
    .onFree = defaultFree,
};

void zaudioNoiseConfigInit(
    ma_format format,
    ma_uint32 channels,
    ma_noise_type type,
    ma_int32 seed,
    double amplitude,
    ma_noise_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_noise_config_init(format, channels, type, seed, amplitude);
}

ma_result zaudioNoiseCreate(const ma_noise_config* config, ma_noise** out_handle) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_noise), NULL);
    ma_result res = ma_noise_init(config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, NULL);
        *out_handle = NULL;
    }
    return res;
}

void zaudioNoiseDestroy(ma_noise* handle) {
    assert(handle != NULL);
    ma_noise_uninit(handle, &s_mem);
    s_mem.onFree(handle, NULL);
}

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

void WA_ma_waveform_config_init(
    ma_format format,
    ma_uint32 channels,
    ma_uint32 sampleRate,
    ma_waveform_type type,
    double amplitude,
    double frequency,
    ma_waveform_config* config
) {
    *config = ma_waveform_config_init(format, channels, sampleRate, type, amplitude, frequency);
}
