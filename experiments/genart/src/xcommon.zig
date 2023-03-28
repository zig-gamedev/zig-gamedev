const std = @import("std");
const gl = @import("zopengl");

pub var display_fbo: gl.Uint = undefined;
pub var display_tex: gl.Uint = undefined;
pub var display_texh: gl.Uint64 = undefined;
pub var frame_time: f64 = undefined;
pub var frame_delta_time: f32 = undefined;
