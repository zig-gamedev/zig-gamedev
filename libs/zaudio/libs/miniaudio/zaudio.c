#include "miniaudio.h"
#include <stdlib.h>
#include <assert.h>
//#include <stdio.h>

static void* defaultAlloc(size_t size, void* user_data) {
    return malloc(size);
}
static void* defaulRealloc(void* ptr, size_t size, void* user_data) {
    return realloc(ptr, size);
}
static void defaultFree(void* ptr, void* user_data) {
    return free(ptr);
}

static ma_allocation_callbacks s_mem = {
    .pUserData = NULL,
    .onMalloc = defaultAlloc,
    .onRealloc = defaulRealloc,
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
    *out_handle = s_mem.onMalloc(sizeof(ma_noise), s_mem.pUserData);
    ma_result res = ma_noise_init(config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioNoiseDestroy(ma_noise* handle) {
    assert(handle != NULL);
    ma_noise_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}

void zaudioNodeConfigInit(ma_node_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_node_config_init();
}

void zaudioDataSourceConfigInit(ma_data_source_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_data_source_config_init();
}

ma_result zaudioDataSourceCreate(const ma_data_source_config* config, ma_data_source** out_handle) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_data_source), s_mem.pUserData);
    ma_result res = ma_data_source_init(config, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioDataSourceDestroy(ma_data_source* handle) {
    assert(handle != NULL);
    ma_data_source_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}

void zaudioWaveformConfigInit(
    ma_format format,
    ma_uint32 channels,
    ma_uint32 sampleRate,
    ma_waveform_type type,
    double amplitude,
    double frequency,
    ma_waveform_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_waveform_config_init(format, channels, sampleRate, type, amplitude, frequency);
}

ma_result zaudioWaveformCreate(const ma_waveform_config* config, ma_waveform** out_handle) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_waveform), s_mem.pUserData);
    ma_result res = ma_waveform_init(config, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioWaveformDestroy(ma_waveform* handle) {
    assert(handle != NULL);
    ma_waveform_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}

void zaudioDataSourceNodeConfigInit(ma_data_source* ds, ma_data_source_node_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_data_source_node_config_init(ds);
}

ma_result zaudioDataSourceNodeCreate(
    ma_node_graph* node_graph,
    const ma_data_source_node_config* config,
    ma_data_source** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_data_source_node), s_mem.pUserData);
    ma_result res = ma_data_source_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioDataSourceNodeDestroy(ma_data_source_node* handle) {
    assert(handle != NULL);
    ma_data_source_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}


void zaudioSplitterNodeConfigInit(ma_uint32 channels, ma_splitter_node_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_splitter_node_config_init(channels);
}

ma_result zaudioSplitterNodeCreate(
    ma_node_graph* node_graph,
    const ma_splitter_node_config* config,
    ma_splitter_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_splitter_node), s_mem.pUserData);
    ma_result res = ma_splitter_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioSplitterNodeDestroy(ma_splitter_node* handle) {
    assert(handle != NULL);
    ma_splitter_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}


void zaudioBiquadNodeConfigInit(
    ma_uint32 channels,
    float b0,
    float b1,
    float b2,
    float a0,
    float a1,
    float a2,
    ma_biquad_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_biquad_node_config_init(channels, b0, b1, b2, a0, a1, a2);
}

ma_result zaudioBiquadNodeCreate(
    ma_node_graph* node_graph,
    const ma_biquad_node_config* config,
    ma_biquad_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_biquad_node), s_mem.pUserData);
    ma_result res = ma_biquad_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioBiquadNodeDestroy(ma_biquad_node* handle) {
    assert(handle != NULL);
    ma_biquad_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}


void zaudioLpfNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    double cutoff_frequency,
    ma_uint32 order,
    ma_lpf_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_lpf_node_config_init(channels, sample_rate, cutoff_frequency, order);
}

ma_result zaudioLpfNodeCreate(
    ma_node_graph* node_graph,
    const ma_lpf_node_config* config,
    ma_lpf_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_lpf_node), s_mem.pUserData);
    ma_result res = ma_lpf_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioLpfNodeDestroy(ma_lpf_node* handle) {
    assert(handle != NULL);
    ma_lpf_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}


void zaudioHpfNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    double cutoff_frequency,
    ma_uint32 order,
    ma_hpf_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_hpf_node_config_init(channels, sample_rate, cutoff_frequency, order);
}

ma_result zaudioHpfNodeCreate(
    ma_node_graph* node_graph,
    const ma_hpf_node_config* config,
    ma_hpf_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_hpf_node), s_mem.pUserData);
    ma_result res = ma_hpf_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioHpfNodeDestroy(ma_hpf_node* handle) {
    assert(handle != NULL);
    ma_hpf_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}


void zaudioNotchNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    double q,
    double frequency,
    ma_notch_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_notch_node_config_init(channels, sample_rate, q, frequency);
}

ma_result zaudioNotchNodeCreate(
    ma_node_graph* node_graph,
    const ma_notch_node_config* config,
    ma_notch_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_notch_node), s_mem.pUserData);
    ma_result res = ma_notch_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioNotchNodeDestroy(ma_notch_node* handle) {
    assert(handle != NULL);
    ma_notch_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}


void zaudioPeakNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    double gain_db,
    double q,
    double frequency,
    ma_peak_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_peak_node_config_init(channels, sample_rate, gain_db, q, frequency);
}

ma_result zaudioPeakNodeCreate(
    ma_node_graph* node_graph,
    const ma_peak_node_config* config,
    ma_peak_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_peak_node), s_mem.pUserData);
    ma_result res = ma_peak_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioPeakNodeDestroy(ma_peak_node* handle) {
    assert(handle != NULL);
    ma_peak_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
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
