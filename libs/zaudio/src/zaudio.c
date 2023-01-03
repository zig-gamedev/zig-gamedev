#include "miniaudio.h"
#include <stdlib.h>
#include <assert.h>
//--------------------------------------------------------------------------------------------------
void* (*zaudioMallocPtr)(size_t size, void* user_data) = NULL;
void* (*zaudioReallocPtr)(void* ptr, size_t size, void* user_data) = NULL;
void (*zaudioFreePtr)(void* ptr, void* user_data) = NULL;

static ma_allocation_callbacks s_mem;

void zaudioMemInit(void) {
    assert(zaudioMallocPtr && zaudioReallocPtr && zaudioFreePtr);
    s_mem.pUserData = NULL;
    s_mem.onMalloc = zaudioMallocPtr;
    s_mem.onRealloc = zaudioReallocPtr;
    s_mem.onFree = zaudioFreePtr;
}
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
void zaudioNodeConfigInit(ma_node_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_node_config_init();
}
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
void zaudioLoshelfNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    double gain_db,
    double shelf_slope,
    double frequency,
    ma_loshelf_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_loshelf_node_config_init(channels, sample_rate, gain_db, shelf_slope, frequency);
}

ma_result zaudioLoshelfNodeCreate(
    ma_node_graph* node_graph,
    const ma_loshelf_node_config* config,
    ma_loshelf_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_loshelf_node), s_mem.pUserData);
    ma_result res = ma_loshelf_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioLoshelfNodeDestroy(ma_loshelf_node* handle) {
    assert(handle != NULL);
    ma_loshelf_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
void zaudioHishelfNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    double gain_db,
    double shelf_slope,
    double frequency,
    ma_hishelf_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_hishelf_node_config_init(channels, sample_rate, gain_db, shelf_slope, frequency);
}

ma_result zaudioHishelfNodeCreate(
    ma_node_graph* node_graph,
    const ma_hishelf_node_config* config,
    ma_hishelf_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_hishelf_node), s_mem.pUserData);
    ma_result res = ma_hishelf_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioHishelfNodeDestroy(ma_hishelf_node* handle) {
    assert(handle != NULL);
    ma_hishelf_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
void zaudioDelayNodeConfigInit(
    ma_uint32 channels,
    ma_uint32 sample_rate,
    ma_uint32 delay_in_frames,
    float decay,
    ma_delay_node_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_delay_node_config_init(channels, sample_rate, delay_in_frames, decay);
}

ma_result zaudioDelayNodeCreate(
    ma_node_graph* node_graph,
    const ma_delay_node_config* config,
    ma_delay_node** out_handle
) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_delay_node), s_mem.pUserData);
    ma_result res = ma_delay_node_init(node_graph, config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioDelayNodeDestroy(ma_delay_node* handle) {
    assert(handle != NULL);
    ma_delay_node_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
void zaudioNodeGraphConfigInit(ma_uint32 channels, ma_node_graph_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_node_graph_config_init(channels);
}

ma_result zaudioNodeGraphCreate(const ma_node_graph_config* config, ma_node_graph** out_handle) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_node_graph), s_mem.pUserData);
    ma_result res = ma_node_graph_init(config, &s_mem, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioNodeGraphDestroy(ma_node_graph* handle) {
    assert(handle != NULL);
    ma_node_graph_uninit(handle, &s_mem);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
void zaudioDeviceConfigInit(ma_device_type device_type, ma_device_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_device_config_init(device_type);
}

ma_result zaudioDeviceCreate(ma_context* context, const ma_device_config* config, ma_device** out_handle) {
    assert(config != NULL && out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_device), s_mem.pUserData);
    ma_result res = ma_device_init(context, config, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioDeviceDestroy(ma_device* handle) {
    assert(handle != NULL);
    ma_device_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}

void* zaudioDeviceGetUserData(ma_device* handle) {
    assert(handle != NULL);
    return handle->pUserData;
}
//--------------------------------------------------------------------------------------------------
void zaudioEngineConfigInit(ma_engine_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_engine_config_init();
    out_config->allocationCallbacks = s_mem;
}

ma_result zaudioEngineCreate(const ma_engine_config* config, ma_engine** out_handle) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_engine), s_mem.pUserData);
    ma_result res = ma_engine_init(config, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioEngineDestroy(ma_engine* handle) {
    assert(handle != NULL);
    ma_engine_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
void zaudioSoundConfigInit(ma_sound_config* out_config) {
    assert(out_config != NULL);
    *out_config = ma_sound_config_init();
}

ma_result zaudioSoundCreate(ma_engine* engine, const ma_sound_config* config, ma_sound** out_handle) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_sound), s_mem.pUserData);
    ma_result res = ma_sound_init_ex(engine, config, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

ma_result zaudioSoundCreateFromFile(
    ma_engine* engine,
    const char* file_path,
    ma_uint32 flags,
    ma_sound_group* sgroup,
    ma_fence* done_fence,
    ma_sound** out_handle
) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_sound), s_mem.pUserData);
    ma_result res = ma_sound_init_from_file(engine, file_path, flags, sgroup, done_fence, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

ma_result zaudioSoundCreateFromDataSource(
    ma_engine* engine,
    ma_data_source* data_source,
    ma_uint32 flags,
    ma_sound_group* sgroup,
    ma_sound** out_handle
) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_sound), s_mem.pUserData);
    ma_result res = ma_sound_init_from_data_source(engine, data_source, flags, sgroup, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

ma_result zaudioSoundCreateCopy(
    ma_engine* engine,
    ma_sound* existing_sound,
    ma_uint32 flags,
    ma_sound_group* sgroup,
    ma_sound** out_handle
) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_sound), s_mem.pUserData);
    ma_result res = ma_sound_init_copy(engine, existing_sound, flags, sgroup, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioSoundDestroy(ma_sound* handle) {
    assert(handle != NULL);
    ma_sound_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
ma_result zaudioSoundGroupCreate(
    ma_engine* engine,
    ma_uint32 flags,
    ma_sound_group* parent,
    ma_sound_group** out_handle
) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_sound_group), s_mem.pUserData);
    ma_result res = ma_sound_group_init(engine, flags, parent, *out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioSoundGroupDestroy(ma_sound_group* handle) {
    assert(handle != NULL);
    ma_sound_group_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
ma_result zaudioFenceCreate(ma_fence** out_handle) {
    assert(out_handle != NULL);
    *out_handle = s_mem.onMalloc(sizeof(ma_fence), s_mem.pUserData);
    ma_result res = ma_fence_init(*out_handle);
    if (res != MA_SUCCESS) {
        s_mem.onFree(*out_handle, s_mem.pUserData);
        *out_handle = NULL;
    }
    return res;
}

void zaudioFenceDestroy(ma_fence* handle) {
    assert(handle != NULL);
    ma_fence_uninit(handle);
    s_mem.onFree(handle, s_mem.pUserData);
}
//--------------------------------------------------------------------------------------------------
void zaudioAudioBufferConfigInit(
    ma_format format,
    ma_uint32 channels,
    ma_int64 size_in_frames,
    const void* data,
    ma_audio_buffer_config* out_config
) {
    assert(out_config != NULL);
    *out_config = ma_audio_buffer_config_init(format, channels, size_in_frames, data, &s_mem);
}

ma_result zaudioAudioBufferCreate(const ma_audio_buffer_config* config, ma_audio_buffer** out_handle) {
    assert(config && out_handle != NULL);
    ma_result res = ma_audio_buffer_alloc_and_init(config, out_handle);
    if (res != MA_SUCCESS) {
        *out_handle = NULL;
    }
    return res;
}

void zaudioAudioBufferDestroy(ma_audio_buffer* handle) {
    assert(handle != NULL);
    ma_audio_buffer_uninit_and_free(handle);
}
//--------------------------------------------------------------------------------------------------
//
// C ABI workarounds
//
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
