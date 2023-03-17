const std = @import("std");
const gl = @import("zopengl");

pub var window_fbo: gl.Uint = undefined;
pub var frame_time: f64 = undefined;
pub var frame_delta_time: f32 = undefined;
