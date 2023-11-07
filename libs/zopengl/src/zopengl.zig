const std = @import("std");
const assert = std.debug.assert;
const testing = std.testing;

const options = @import("zopengl_options");

const bindings = @import("bindings.zig");

pub usingnamespace switch (options.api) {
    .raw_bindings => bindings,
    .wrapper => @import("wrapper.zig"),
};
//--------------------------------------------------------------------------------------------------
//
// Functions for loading OpenGL function pointers
//
//--------------------------------------------------------------------------------------------------
pub const LoaderFn = *const fn ([:0]const u8) ?*const anyopaque;

pub const Extension = enum {
    KHR_debug,
    NV_bindless_texture,
    NV_shader_buffer_load,
};

pub const EsExtension = enum {
    OES_vertex_array_object,
    KHR_debug,
};

pub fn loadCoreProfile(loader: LoaderFn, major: u32, minor: u32) !void {
    const ver = 10 * major + minor;

    assert(major >= 1 and major <= 4);
    assert(minor >= 0 and minor <= 6);
    assert(ver >= 10 and ver <= 46);

    loaderFunc = loader;

    // OpenGL 1.0
    if (ver >= 10) {
        bindings.cullFace = try getProcAddress(@TypeOf(bindings.cullFace), "glCullFace");
        bindings.frontFace = try getProcAddress(@TypeOf(bindings.frontFace), "glFrontFace");
        bindings.hint = try getProcAddress(@TypeOf(bindings.hint), "glHint");
        bindings.lineWidth = try getProcAddress(@TypeOf(bindings.lineWidth), "glLineWidth");
        bindings.pointSize = try getProcAddress(@TypeOf(bindings.pointSize), "glPointSize");
        bindings.polygonMode = try getProcAddress(@TypeOf(bindings.polygonMode), "glPolygonMode");
        bindings.scissor = try getProcAddress(@TypeOf(bindings.scissor), "glScissor");
        bindings.texParameterf = try getProcAddress(@TypeOf(bindings.texParameterf), "glTexParameterf");
        bindings.texParameterfv = try getProcAddress(@TypeOf(bindings.texParameterfv), "glTexParameterfv");
        bindings.texParameteri = try getProcAddress(@TypeOf(bindings.texParameteri), "glTexParameteri");
        bindings.texParameteriv = try getProcAddress(@TypeOf(bindings.texParameteriv), "glTexParameteriv");
        bindings.texImage1D = try getProcAddress(@TypeOf(bindings.texImage1D), "glTexImage1D");
        bindings.texImage2D = try getProcAddress(@TypeOf(bindings.texImage2D), "glTexImage2D");
        bindings.drawBuffer = try getProcAddress(@TypeOf(bindings.drawBuffer), "glDrawBuffer");
        bindings.clear = try getProcAddress(@TypeOf(bindings.clear), "glClear");
        bindings.clearColor = try getProcAddress(@TypeOf(bindings.clearColor), "glClearColor");
        bindings.clearStencil = try getProcAddress(@TypeOf(bindings.clearStencil), "glClearStencil");
        bindings.clearDepth = try getProcAddress(@TypeOf(bindings.clearDepth), "glClearDepth");
        bindings.stencilMask = try getProcAddress(@TypeOf(bindings.stencilMask), "glStencilMask");
        bindings.colorMask = try getProcAddress(@TypeOf(bindings.colorMask), "glColorMask");
        bindings.depthMask = try getProcAddress(@TypeOf(bindings.depthMask), "glDepthMask");
        bindings.disable = try getProcAddress(@TypeOf(bindings.disable), "glDisable");
        bindings.enable = try getProcAddress(@TypeOf(bindings.enable), "glEnable");
        bindings.finish = try getProcAddress(@TypeOf(bindings.finish), "glFinish");
        bindings.flush = try getProcAddress(@TypeOf(bindings.flush), "glFlush");
        bindings.blendFunc = try getProcAddress(@TypeOf(bindings.blendFunc), "glBlendFunc");
        bindings.logicOp = try getProcAddress(@TypeOf(bindings.logicOp), "glLogicOp");
        bindings.stencilFunc = try getProcAddress(@TypeOf(bindings.stencilFunc), "glStencilFunc");
        bindings.stencilOp = try getProcAddress(@TypeOf(bindings.stencilOp), "glStencilOp");
        bindings.depthFunc = try getProcAddress(@TypeOf(bindings.depthFunc), "glDepthFunc");
        bindings.pixelStoref = try getProcAddress(@TypeOf(bindings.pixelStoref), "glPixelStoref");
        bindings.pixelStorei = try getProcAddress(@TypeOf(bindings.pixelStorei), "glPixelStorei");
        bindings.readBuffer = try getProcAddress(@TypeOf(bindings.readBuffer), "glReadBuffer");
        bindings.readPixels = try getProcAddress(@TypeOf(bindings.readPixels), "glReadPixels");
        bindings.getBooleanv = try getProcAddress(@TypeOf(bindings.getBooleanv), "glGetBooleanv");
        bindings.getDoublev = try getProcAddress(@TypeOf(bindings.getDoublev), "glGetDoublev");
        bindings.getError = try getProcAddress(@TypeOf(bindings.getError), "glGetError");
        bindings.getFloatv = try getProcAddress(@TypeOf(bindings.getFloatv), "glGetFloatv");
        bindings.getIntegerv = try getProcAddress(@TypeOf(bindings.getIntegerv), "glGetIntegerv");
        bindings.getString = try getProcAddress(@TypeOf(bindings.getString), "glGetString");
        bindings.getTexImage = try getProcAddress(@TypeOf(bindings.getTexImage), "glGetTexImage");
        bindings.getTexParameterfv = try getProcAddress(@TypeOf(bindings.getTexParameterfv), "glGetTexParameterfv");
        bindings.getTexParameteriv = try getProcAddress(@TypeOf(bindings.getTexParameteriv), "glGetTexParameteriv");
        bindings.getTexLevelParameterfv = try getProcAddress(
            @TypeOf(bindings.getTexLevelParameterfv),
            "glGetTexLevelParameterfv",
        );
        bindings.getTexLevelParameteriv = try getProcAddress(
            @TypeOf(bindings.getTexLevelParameteriv),
            "glGetTexLevelParameteriv",
        );
        bindings.isEnabled = try getProcAddress(@TypeOf(bindings.isEnabled), "glIsEnabled");
        bindings.depthRange = try getProcAddress(@TypeOf(bindings.depthRange), "glDepthRange");
        bindings.viewport = try getProcAddress(@TypeOf(bindings.viewport), "glViewport");
    }

    // OpenGL 1.1
    if (ver >= 11) {
        bindings.drawArrays = try getProcAddress(@TypeOf(bindings.drawArrays), "glDrawArrays");
        bindings.drawElements = try getProcAddress(@TypeOf(bindings.drawElements), "glDrawElements");
        bindings.polygonOffset = try getProcAddress(@TypeOf(bindings.polygonOffset), "glPolygonOffset");
        bindings.copyTexImage1D = try getProcAddress(@TypeOf(bindings.copyTexImage1D), "glCopyTexImage1D");
        bindings.copyTexImage2D = try getProcAddress(@TypeOf(bindings.copyTexImage2D), "glCopyTexImage2D");
        bindings.copyTexSubImage1D = try getProcAddress(@TypeOf(bindings.copyTexSubImage1D), "glCopyTexSubImage1D");
        bindings.copyTexSubImage2D = try getProcAddress(@TypeOf(bindings.copyTexSubImage2D), "glCopyTexSubImage2D");
        bindings.texSubImage1D = try getProcAddress(@TypeOf(bindings.texSubImage1D), "glTexSubImage1D");
        bindings.texSubImage2D = try getProcAddress(@TypeOf(bindings.texSubImage2D), "glTexSubImage2D");
        bindings.bindTexture = try getProcAddress(@TypeOf(bindings.bindTexture), "glBindTexture");
        bindings.deleteTextures = try getProcAddress(@TypeOf(bindings.deleteTextures), "glDeleteTextures");
        bindings.genTextures = try getProcAddress(@TypeOf(bindings.genTextures), "glGenTextures");
        bindings.isTexture = try getProcAddress(@TypeOf(bindings.isTexture), "glIsTexture");
    }

    // OpenGL 1.2
    if (ver >= 12) {
        bindings.drawRangeElements = try getProcAddress(@TypeOf(bindings.drawRangeElements), "glDrawRangeElements");
        bindings.texImage3D = try getProcAddress(@TypeOf(bindings.texImage3D), "glTexImage3D");
        bindings.texSubImage3D = try getProcAddress(@TypeOf(bindings.texSubImage3D), "glTexSubImage3D");
        bindings.copyTexSubImage3D = try getProcAddress(@TypeOf(bindings.copyTexSubImage3D), "glCopyTexSubImage3D");
    }

    // OpenGL 1.3
    if (ver >= 13) {
        bindings.activeTexture = try getProcAddress(@TypeOf(bindings.activeTexture), "glActiveTexture");
        bindings.sampleCoverage = try getProcAddress(@TypeOf(bindings.sampleCoverage), "glSampleCoverage");
        bindings.compressedTexImage3D = try getProcAddress(
            @TypeOf(bindings.compressedTexImage3D),
            "glCompressedTexImage3D",
        );
        bindings.compressedTexImage2D = try getProcAddress(
            @TypeOf(bindings.compressedTexImage2D),
            "glCompressedTexImage2D",
        );
        bindings.compressedTexImage1D = try getProcAddress(
            @TypeOf(bindings.compressedTexImage1D),
            "glCompressedTexImage1D",
        );
        bindings.compressedTexSubImage3D = try getProcAddress(
            @TypeOf(bindings.compressedTexSubImage3D),
            "glCompressedTexSubImage3D",
        );
        bindings.compressedTexSubImage2D = try getProcAddress(
            @TypeOf(bindings.compressedTexSubImage2D),
            "glCompressedTexSubImage2D",
        );
        bindings.compressedTexSubImage1D = try getProcAddress(
            @TypeOf(bindings.compressedTexSubImage1D),
            "glCompressedTexSubImage1D",
        );
        bindings.getCompressedTexImage = try getProcAddress(
            @TypeOf(bindings.getCompressedTexImage),
            "glGetCompressedTexImage",
        );
    }

    // OpenGL 1.4
    if (ver >= 14) {
        bindings.blendFuncSeparate = try getProcAddress(@TypeOf(bindings.blendFuncSeparate), "glBlendFuncSeparate");
        bindings.multiDrawArrays = try getProcAddress(@TypeOf(bindings.multiDrawArrays), "glMultiDrawArrays");
        bindings.multiDrawElements = try getProcAddress(@TypeOf(bindings.multiDrawElements), "glMultiDrawElements");
        bindings.pointParameterf = try getProcAddress(@TypeOf(bindings.pointParameterf), "glPointParameterf");
        bindings.pointParameterfv = try getProcAddress(@TypeOf(bindings.pointParameterfv), "glPointParameterfv");
        bindings.pointParameteri = try getProcAddress(@TypeOf(bindings.pointParameteri), "glPointParameteri");
        bindings.pointParameteriv = try getProcAddress(@TypeOf(bindings.pointParameteriv), "glPointParameteriv");
        bindings.blendColor = try getProcAddress(@TypeOf(bindings.blendColor), "glBlendColor");
        bindings.blendEquation = try getProcAddress(@TypeOf(bindings.blendEquation), "glBlendEquation");
    }

    // OpenGL 1.5
    if (ver >= 15) {
        bindings.genQueries = try getProcAddress(@TypeOf(bindings.genQueries), "glGenQueries");
        bindings.deleteQueries = try getProcAddress(@TypeOf(bindings.deleteQueries), "glDeleteQueries");
        bindings.isQuery = try getProcAddress(@TypeOf(bindings.isQuery), "glIsQuery");
        bindings.beginQuery = try getProcAddress(@TypeOf(bindings.beginQuery), "glBeginQuery");
        bindings.endQuery = try getProcAddress(@TypeOf(bindings.endQuery), "glEndQuery");
        bindings.getQueryiv = try getProcAddress(@TypeOf(bindings.getQueryiv), "glGetQueryiv");
        bindings.getQueryObjectiv = try getProcAddress(@TypeOf(bindings.getQueryObjectiv), "glGetQueryObjectiv");
        bindings.getQueryObjectuiv = try getProcAddress(@TypeOf(bindings.getQueryObjectuiv), "glGetQueryObjectuiv");
        bindings.bindBuffer = try getProcAddress(@TypeOf(bindings.bindBuffer), "glBindBuffer");
        bindings.deleteBuffers = try getProcAddress(@TypeOf(bindings.deleteBuffers), "glDeleteBuffers");
        bindings.genBuffers = try getProcAddress(@TypeOf(bindings.genBuffers), "glGenBuffers");
        bindings.isBuffer = try getProcAddress(@TypeOf(bindings.isBuffer), "glIsBuffer");
        bindings.bufferData = try getProcAddress(@TypeOf(bindings.bufferData), "glBufferData");
        bindings.bufferSubData = try getProcAddress(@TypeOf(bindings.bufferSubData), "glBufferSubData");
        bindings.getBufferSubData = try getProcAddress(@TypeOf(bindings.getBufferSubData), "glGetBufferSubData");
        bindings.mapBuffer = try getProcAddress(@TypeOf(bindings.mapBuffer), "glMapBuffer");
        bindings.unmapBuffer = try getProcAddress(@TypeOf(bindings.unmapBuffer), "glUnmapBuffer");
        bindings.getBufferParameteriv = try getProcAddress(
            @TypeOf(bindings.getBufferParameteriv),
            "glGetBufferParameteriv",
        );
        bindings.getBufferPointerv = try getProcAddress(@TypeOf(bindings.getBufferPointerv), "glGetBufferPointerv");
    }

    // OpenGL 2.0
    if (ver >= 20) {
        bindings.blendEquationSeparate = try getProcAddress(
            @TypeOf(bindings.blendEquationSeparate),
            "glBlendEquationSeparate",
        );
        bindings.drawBuffers = try getProcAddress(@TypeOf(bindings.drawBuffers), "glDrawBuffers");
        bindings.stencilOpSeparate = try getProcAddress(@TypeOf(bindings.stencilOpSeparate), "glStencilOpSeparate");
        bindings.stencilFuncSeparate = try getProcAddress(
            @TypeOf(bindings.stencilFuncSeparate),
            "glStencilFuncSeparate",
        );
        bindings.stencilMaskSeparate = try getProcAddress(
            @TypeOf(bindings.stencilMaskSeparate),
            "glStencilMaskSeparate",
        );
        bindings.attachShader = try getProcAddress(@TypeOf(bindings.attachShader), "glAttachShader");
        bindings.bindAttribLocation = try getProcAddress(
            @TypeOf(bindings.bindAttribLocation),
            "glBindAttribLocation",
        );
        bindings.compileShader = try getProcAddress(@TypeOf(bindings.compileShader), "glCompileShader");
        bindings.createProgram = try getProcAddress(@TypeOf(bindings.createProgram), "glCreateProgram");
        bindings.createShader = try getProcAddress(@TypeOf(bindings.createShader), "glCreateShader");
        bindings.deleteProgram = try getProcAddress(@TypeOf(bindings.deleteProgram), "glDeleteProgram");
        bindings.deleteShader = try getProcAddress(@TypeOf(bindings.deleteShader), "glDeleteShader");
        bindings.detachShader = try getProcAddress(@TypeOf(bindings.detachShader), "glDetachShader");
        bindings.disableVertexAttribArray = try getProcAddress(
            @TypeOf(bindings.disableVertexAttribArray),
            "glDisableVertexAttribArray",
        );
        bindings.enableVertexAttribArray = try getProcAddress(
            @TypeOf(bindings.enableVertexAttribArray),
            "glEnableVertexAttribArray",
        );
        bindings.getActiveAttrib = try getProcAddress(@TypeOf(bindings.getActiveAttrib), "glGetActiveAttrib");
        bindings.getActiveUniform = try getProcAddress(@TypeOf(bindings.getActiveUniform), "glGetActiveUniform");
        bindings.getAttachedShaders = try getProcAddress(
            @TypeOf(bindings.getAttachedShaders),
            "glGetAttachedShaders",
        );
        bindings.getAttribLocation = try getProcAddress(@TypeOf(bindings.getAttribLocation), "glGetAttribLocation");
        bindings.getProgramiv = try getProcAddress(@TypeOf(bindings.getProgramiv), "glGetProgramiv");
        bindings.getProgramInfoLog = try getProcAddress(@TypeOf(bindings.getProgramInfoLog), "glGetProgramInfoLog");
        bindings.getShaderiv = try getProcAddress(@TypeOf(bindings.getShaderiv), "glGetShaderiv");
        bindings.getShaderInfoLog = try getProcAddress(@TypeOf(bindings.getShaderInfoLog), "glGetShaderInfoLog");
        bindings.getShaderSource = try getProcAddress(@TypeOf(bindings.getShaderSource), "glGetShaderSource");
        bindings.getUniformLocation = try getProcAddress(
            @TypeOf(bindings.getUniformLocation),
            "glGetUniformLocation",
        );
        bindings.getUniformfv = try getProcAddress(@TypeOf(bindings.getUniformfv), "glGetUniformfv");
        bindings.getUniformiv = try getProcAddress(@TypeOf(bindings.getUniformiv), "glGetUniformiv");
        bindings.getVertexAttribdv = try getProcAddress(@TypeOf(bindings.getVertexAttribdv), "glGetVertexAttribdv");
        bindings.getVertexAttribfv = try getProcAddress(@TypeOf(bindings.getVertexAttribfv), "glGetVertexAttribfv");
        bindings.getVertexAttribiv = try getProcAddress(@TypeOf(bindings.getVertexAttribiv), "glGetVertexAttribiv");
        bindings.getVertexAttribPointerv = try getProcAddress(
            @TypeOf(bindings.getVertexAttribPointerv),
            "glGetVertexAttribPointerv",
        );
        bindings.isProgram = try getProcAddress(@TypeOf(bindings.isProgram), "glIsProgram");
        bindings.isShader = try getProcAddress(@TypeOf(bindings.isShader), "glIsShader");
        bindings.linkProgram = try getProcAddress(@TypeOf(bindings.linkProgram), "glLinkProgram");
        bindings.shaderSource = try getProcAddress(@TypeOf(bindings.shaderSource), "glShaderSource");
        bindings.useProgram = try getProcAddress(@TypeOf(bindings.useProgram), "glUseProgram");
        bindings.uniform1f = try getProcAddress(@TypeOf(bindings.uniform1f), "glUniform1f");
        bindings.uniform2f = try getProcAddress(@TypeOf(bindings.uniform2f), "glUniform2f");
        bindings.uniform3f = try getProcAddress(@TypeOf(bindings.uniform3f), "glUniform3f");
        bindings.uniform4f = try getProcAddress(@TypeOf(bindings.uniform4f), "glUniform4f");
        bindings.uniform1i = try getProcAddress(@TypeOf(bindings.uniform1i), "glUniform1i");
        bindings.uniform2i = try getProcAddress(@TypeOf(bindings.uniform2i), "glUniform2i");
        bindings.uniform3i = try getProcAddress(@TypeOf(bindings.uniform3i), "glUniform3i");
        bindings.uniform4i = try getProcAddress(@TypeOf(bindings.uniform4i), "glUniform4i");
        bindings.uniform1fv = try getProcAddress(@TypeOf(bindings.uniform1fv), "glUniform1fv");
        bindings.uniform2fv = try getProcAddress(@TypeOf(bindings.uniform2fv), "glUniform2fv");
        bindings.uniform3fv = try getProcAddress(@TypeOf(bindings.uniform3fv), "glUniform3fv");
        bindings.uniform4fv = try getProcAddress(@TypeOf(bindings.uniform4fv), "glUniform4fv");
        bindings.uniform1iv = try getProcAddress(@TypeOf(bindings.uniform1iv), "glUniform1iv");
        bindings.uniform2iv = try getProcAddress(@TypeOf(bindings.uniform2iv), "glUniform2iv");
        bindings.uniform3iv = try getProcAddress(@TypeOf(bindings.uniform3iv), "glUniform3iv");
        bindings.uniform4iv = try getProcAddress(@TypeOf(bindings.uniform4iv), "glUniform4iv");
        bindings.uniformMatrix2fv = try getProcAddress(@TypeOf(bindings.uniformMatrix2fv), "glUniformMatrix2fv");
        bindings.uniformMatrix3fv = try getProcAddress(@TypeOf(bindings.uniformMatrix3fv), "glUniformMatrix3fv");
        bindings.uniformMatrix4fv = try getProcAddress(@TypeOf(bindings.uniformMatrix4fv), "glUniformMatrix4fv");
        bindings.validateProgram = try getProcAddress(@TypeOf(bindings.validateProgram), "glValidateProgram");
        bindings.vertexAttrib1d = try getProcAddress(@TypeOf(bindings.vertexAttrib1d), "glVertexAttrib1d");
        bindings.vertexAttrib1dv = try getProcAddress(@TypeOf(bindings.vertexAttrib1dv), "glVertexAttrib1dv");
        bindings.vertexAttrib1f = try getProcAddress(@TypeOf(bindings.vertexAttrib1f), "glVertexAttrib1f");
        bindings.vertexAttrib1fv = try getProcAddress(@TypeOf(bindings.vertexAttrib1fv), "glVertexAttrib1fv");
        bindings.vertexAttrib1s = try getProcAddress(@TypeOf(bindings.vertexAttrib1s), "glVertexAttrib1s");
        bindings.vertexAttrib1sv = try getProcAddress(@TypeOf(bindings.vertexAttrib1sv), "glVertexAttrib1sv");
        bindings.vertexAttrib2d = try getProcAddress(@TypeOf(bindings.vertexAttrib2d), "glVertexAttrib2d");
        bindings.vertexAttrib2dv = try getProcAddress(@TypeOf(bindings.vertexAttrib2dv), "glVertexAttrib2dv");
        bindings.vertexAttrib2f = try getProcAddress(@TypeOf(bindings.vertexAttrib2f), "glVertexAttrib2f");
        bindings.vertexAttrib2fv = try getProcAddress(@TypeOf(bindings.vertexAttrib2fv), "glVertexAttrib2fv");
        bindings.vertexAttrib2s = try getProcAddress(@TypeOf(bindings.vertexAttrib2s), "glVertexAttrib2s");
        bindings.vertexAttrib2sv = try getProcAddress(@TypeOf(bindings.vertexAttrib2sv), "glVertexAttrib2sv");
        bindings.vertexAttrib3d = try getProcAddress(@TypeOf(bindings.vertexAttrib3d), "glVertexAttrib3d");
        bindings.vertexAttrib3dv = try getProcAddress(@TypeOf(bindings.vertexAttrib3dv), "glVertexAttrib3dv");
        bindings.vertexAttrib3f = try getProcAddress(@TypeOf(bindings.vertexAttrib3f), "glVertexAttrib3f");
        bindings.vertexAttrib3fv = try getProcAddress(@TypeOf(bindings.vertexAttrib3fv), "glVertexAttrib3fv");
        bindings.vertexAttrib3s = try getProcAddress(@TypeOf(bindings.vertexAttrib3s), "glVertexAttrib3s");
        bindings.vertexAttrib3sv = try getProcAddress(@TypeOf(bindings.vertexAttrib3sv), "glVertexAttrib3sv");
        bindings.vertexAttrib4Nbv = try getProcAddress(@TypeOf(bindings.vertexAttrib4Nbv), "glVertexAttrib4Nbv");
        bindings.vertexAttrib4Niv = try getProcAddress(@TypeOf(bindings.vertexAttrib4Niv), "glVertexAttrib4Niv");
        bindings.vertexAttrib4Nsv = try getProcAddress(@TypeOf(bindings.vertexAttrib4Nsv), "glVertexAttrib4Nsv");
        bindings.vertexAttrib4Nub = try getProcAddress(@TypeOf(bindings.vertexAttrib4Nub), "glVertexAttrib4Nub");
        bindings.vertexAttrib4Nubv = try getProcAddress(@TypeOf(bindings.vertexAttrib4Nubv), "glVertexAttrib4Nubv");
        bindings.vertexAttrib4Nuiv = try getProcAddress(@TypeOf(bindings.vertexAttrib4Nuiv), "glVertexAttrib4Nuiv");
        bindings.vertexAttrib4Nusv = try getProcAddress(@TypeOf(bindings.vertexAttrib4Nusv), "glVertexAttrib4Nusv");
        bindings.vertexAttrib4bv = try getProcAddress(@TypeOf(bindings.vertexAttrib4bv), "glVertexAttrib4bv");
        bindings.vertexAttrib4d = try getProcAddress(@TypeOf(bindings.vertexAttrib4d), "glVertexAttrib4d");
        bindings.vertexAttrib4dv = try getProcAddress(@TypeOf(bindings.vertexAttrib4dv), "glVertexAttrib4dv");
        bindings.vertexAttrib4f = try getProcAddress(@TypeOf(bindings.vertexAttrib4f), "glVertexAttrib4f");
        bindings.vertexAttrib4fv = try getProcAddress(@TypeOf(bindings.vertexAttrib4fv), "glVertexAttrib4fv");
        bindings.vertexAttrib4iv = try getProcAddress(@TypeOf(bindings.vertexAttrib4iv), "glVertexAttrib4iv");
        bindings.vertexAttrib4s = try getProcAddress(@TypeOf(bindings.vertexAttrib4s), "glVertexAttrib4s");
        bindings.vertexAttrib4sv = try getProcAddress(@TypeOf(bindings.vertexAttrib4sv), "glVertexAttrib4sv");
        bindings.vertexAttrib4ubv = try getProcAddress(@TypeOf(bindings.vertexAttrib4ubv), "glVertexAttrib4ubv");
        bindings.vertexAttrib4uiv = try getProcAddress(@TypeOf(bindings.vertexAttrib4uiv), "glVertexAttrib4uiv");
        bindings.vertexAttrib4usv = try getProcAddress(@TypeOf(bindings.vertexAttrib4usv), "glVertexAttrib4usv");
        bindings.vertexAttribPointer = try getProcAddress(
            @TypeOf(bindings.vertexAttribPointer),
            "glVertexAttribPointer",
        );
    }

    // OpenGL 2.1
    if (ver >= 21) {
        bindings.uniformMatrix2x3fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix2x3fv),
            "glUniformMatrix2x3fv",
        );
        bindings.uniformMatrix3x2fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix3x2fv),
            "glUniformMatrix3x2fv",
        );
        bindings.uniformMatrix2x4fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix2x4fv),
            "glUniformMatrix2x4fv",
        );
        bindings.uniformMatrix4x2fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix4x2fv),
            "glUniformMatrix4x2fv",
        );
        bindings.uniformMatrix3x4fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix3x4fv),
            "glUniformMatrix3x4fv",
        );
        bindings.uniformMatrix4x3fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix4x3fv),
            "glUniformMatrix4x3fv",
        );
    }

    // OpenGL 3.0
    if (ver >= 30) {
        bindings.colorMaski = try getProcAddress(@TypeOf(bindings.colorMaski), "glColorMaski");
        bindings.getBooleani_v = try getProcAddress(@TypeOf(bindings.getBooleani_v), "glGetBooleani_v");
        bindings.getIntegeri_v = try getProcAddress(@TypeOf(bindings.getIntegeri_v), "glGetIntegeri_v");
        bindings.enablei = try getProcAddress(@TypeOf(bindings.enablei), "glEnablei");
        bindings.disablei = try getProcAddress(@TypeOf(bindings.disablei), "glDisablei");
        bindings.isEnabledi = try getProcAddress(@TypeOf(bindings.isEnabledi), "glIsEnabledi");
        bindings.beginTransformFeedback = try getProcAddress(
            @TypeOf(bindings.beginTransformFeedback),
            "glBeginTransformFeedback",
        );
        bindings.endTransformFeedback = try getProcAddress(
            @TypeOf(bindings.endTransformFeedback),
            "glEndTransformFeedback",
        );
        bindings.bindBufferRange = try getProcAddress(@TypeOf(bindings.bindBufferRange), "glBindBufferRange");
        bindings.bindBufferBase = try getProcAddress(@TypeOf(bindings.bindBufferBase), "glBindBufferBase");
        bindings.transformFeedbackVaryings = try getProcAddress(
            @TypeOf(bindings.transformFeedbackVaryings),
            "glTransformFeedbackVaryings",
        );
        bindings.getTransformFeedbackVarying = try getProcAddress(
            @TypeOf(bindings.getTransformFeedbackVarying),
            "glGetTransformFeedbackVarying",
        );
        bindings.clampColor = try getProcAddress(@TypeOf(bindings.clampColor), "glClampColor");
        bindings.beginConditionalRender = try getProcAddress(
            @TypeOf(bindings.beginConditionalRender),
            "glBeginConditionalRender",
        );
        bindings.endConditionalRender = try getProcAddress(
            @TypeOf(bindings.endConditionalRender),
            "glEndConditionalRender",
        );
        bindings.vertexAttribIPointer = try getProcAddress(
            @TypeOf(bindings.vertexAttribIPointer),
            "glVertexAttribIPointer",
        );
        bindings.getVertexAttribIiv = try getProcAddress(
            @TypeOf(bindings.getVertexAttribIiv),
            "glGetVertexAttribIiv",
        );
        bindings.getVertexAttribIuiv = try getProcAddress(
            @TypeOf(bindings.getVertexAttribIuiv),
            "glGetVertexAttribIuiv",
        );
        bindings.vertexAttribI1i = try getProcAddress(@TypeOf(bindings.vertexAttribI1i), "glVertexAttribI1i");
        bindings.vertexAttribI2i = try getProcAddress(@TypeOf(bindings.vertexAttribI2i), "glVertexAttribI2i");
        bindings.vertexAttribI3i = try getProcAddress(@TypeOf(bindings.vertexAttribI3i), "glVertexAttribI3i");
        bindings.vertexAttribI4i = try getProcAddress(@TypeOf(bindings.vertexAttribI4i), "glVertexAttribI4i");
        bindings.vertexAttribI1ui = try getProcAddress(@TypeOf(bindings.vertexAttribI1ui), "glVertexAttribI1ui");
        bindings.vertexAttribI2ui = try getProcAddress(@TypeOf(bindings.vertexAttribI2ui), "glVertexAttribI2ui");
        bindings.vertexAttribI3ui = try getProcAddress(@TypeOf(bindings.vertexAttribI3ui), "glVertexAttribI3ui");
        bindings.vertexAttribI4ui = try getProcAddress(@TypeOf(bindings.vertexAttribI4ui), "glVertexAttribI4ui");
        bindings.vertexAttribI1iv = try getProcAddress(@TypeOf(bindings.vertexAttribI1iv), "glVertexAttribI1iv");
        bindings.vertexAttribI2iv = try getProcAddress(@TypeOf(bindings.vertexAttribI2iv), "glVertexAttribI2iv");
        bindings.vertexAttribI3iv = try getProcAddress(@TypeOf(bindings.vertexAttribI3iv), "glVertexAttribI3iv");
        bindings.vertexAttribI4iv = try getProcAddress(@TypeOf(bindings.vertexAttribI4iv), "glVertexAttribI4iv");
        bindings.vertexAttribI1uiv = try getProcAddress(@TypeOf(bindings.vertexAttribI1uiv), "glVertexAttribI1uiv");
        bindings.vertexAttribI2uiv = try getProcAddress(@TypeOf(bindings.vertexAttribI2uiv), "glVertexAttribI2uiv");
        bindings.vertexAttribI3uiv = try getProcAddress(@TypeOf(bindings.vertexAttribI3uiv), "glVertexAttribI3uiv");
        bindings.vertexAttribI4uiv = try getProcAddress(@TypeOf(bindings.vertexAttribI4uiv), "glVertexAttribI4uiv");
        bindings.vertexAttribI4bv = try getProcAddress(@TypeOf(bindings.vertexAttribI4bv), "glVertexAttribI4bv");
        bindings.vertexAttribI4sv = try getProcAddress(@TypeOf(bindings.vertexAttribI4sv), "glVertexAttribI4sv");
        bindings.vertexAttribI4ubv = try getProcAddress(@TypeOf(bindings.vertexAttribI4ubv), "glVertexAttribI4ubv");
        bindings.vertexAttribI4usv = try getProcAddress(@TypeOf(bindings.vertexAttribI4usv), "glVertexAttribI4usv");
        bindings.getUniformuiv = try getProcAddress(@TypeOf(bindings.getUniformuiv), "glGetUniformuiv");
        bindings.bindFragDataLocation = try getProcAddress(
            @TypeOf(bindings.bindFragDataLocation),
            "glBindFragDataLocation",
        );
        bindings.getFragDataLocation = try getProcAddress(
            @TypeOf(bindings.getFragDataLocation),
            "glGetFragDataLocation",
        );
        bindings.uniform1ui = try getProcAddress(@TypeOf(bindings.uniform1ui), "glUniform1ui");
        bindings.uniform2ui = try getProcAddress(@TypeOf(bindings.uniform2ui), "glUniform2ui");
        bindings.uniform3ui = try getProcAddress(@TypeOf(bindings.uniform3ui), "glUniform3ui");
        bindings.uniform4ui = try getProcAddress(@TypeOf(bindings.uniform4ui), "glUniform4ui");
        bindings.uniform1uiv = try getProcAddress(@TypeOf(bindings.uniform1uiv), "glUniform1uiv");
        bindings.uniform2uiv = try getProcAddress(@TypeOf(bindings.uniform2uiv), "glUniform2uiv");
        bindings.uniform3uiv = try getProcAddress(@TypeOf(bindings.uniform3uiv), "glUniform3uiv");
        bindings.uniform4uiv = try getProcAddress(@TypeOf(bindings.uniform4uiv), "glUniform4uiv");
        bindings.texParameterIiv = try getProcAddress(@TypeOf(bindings.texParameterIiv), "glTexParameterIiv");
        bindings.texParameterIuiv = try getProcAddress(@TypeOf(bindings.texParameterIuiv), "glTexParameterIuiv");
        bindings.getTexParameterIiv = try getProcAddress(
            @TypeOf(bindings.getTexParameterIiv),
            "glGetTexParameterIiv",
        );
        bindings.getTexParameterIuiv = try getProcAddress(
            @TypeOf(bindings.getTexParameterIuiv),
            "glGetTexParameterIuiv",
        );
        bindings.clearBufferiv = try getProcAddress(@TypeOf(bindings.clearBufferiv), "glClearBufferiv");
        bindings.clearBufferuiv = try getProcAddress(@TypeOf(bindings.clearBufferuiv), "glClearBufferuiv");
        bindings.clearBufferfv = try getProcAddress(@TypeOf(bindings.clearBufferfv), "glClearBufferfv");
        bindings.clearBufferfi = try getProcAddress(@TypeOf(bindings.clearBufferfi), "glClearBufferfi");
        bindings.getStringi = try getProcAddress(@TypeOf(bindings.getStringi), "glGetStringi");
        bindings.isRenderbuffer = try getProcAddress(@TypeOf(bindings.isRenderbuffer), "glIsRenderbuffer");
        bindings.bindRenderbuffer = try getProcAddress(@TypeOf(bindings.bindRenderbuffer), "glBindRenderbuffer");
        bindings.deleteRenderbuffers = try getProcAddress(
            @TypeOf(bindings.deleteRenderbuffers),
            "glDeleteRenderbuffers",
        );
        bindings.genRenderbuffers = try getProcAddress(@TypeOf(bindings.genRenderbuffers), "glGenRenderbuffers");
        bindings.renderbufferStorage = try getProcAddress(
            @TypeOf(bindings.renderbufferStorage),
            "glRenderbufferStorage",
        );
        bindings.getRenderbufferParameteriv = try getProcAddress(
            @TypeOf(bindings.getRenderbufferParameteriv),
            "glGetRenderbufferParameteriv",
        );
        bindings.isFramebuffer = try getProcAddress(@TypeOf(bindings.isFramebuffer), "glIsFramebuffer");
        bindings.bindFramebuffer = try getProcAddress(@TypeOf(bindings.bindFramebuffer), "glBindFramebuffer");
        bindings.deleteFramebuffers = try getProcAddress(
            @TypeOf(bindings.deleteFramebuffers),
            "glDeleteFramebuffers",
        );
        bindings.genFramebuffers = try getProcAddress(@TypeOf(bindings.genFramebuffers), "glGenFramebuffers");
        bindings.checkFramebufferStatus = try getProcAddress(
            @TypeOf(bindings.checkFramebufferStatus),
            "glCheckFramebufferStatus",
        );
        bindings.framebufferTexture1D = try getProcAddress(
            @TypeOf(bindings.framebufferTexture1D),
            "glFramebufferTexture1D",
        );
        bindings.framebufferTexture2D = try getProcAddress(
            @TypeOf(bindings.framebufferTexture2D),
            "glFramebufferTexture2D",
        );
        bindings.framebufferTexture3D = try getProcAddress(
            @TypeOf(bindings.framebufferTexture3D),
            "glFramebufferTexture3D",
        );
        bindings.framebufferRenderbuffer = try getProcAddress(
            @TypeOf(bindings.framebufferRenderbuffer),
            "glFramebufferRenderbuffer",
        );
        bindings.getFramebufferAttachmentParameteriv = try getProcAddress(
            @TypeOf(bindings.getFramebufferAttachmentParameteriv),
            "glGetFramebufferAttachmentParameteriv",
        );
        bindings.generateMipmap = try getProcAddress(@TypeOf(bindings.generateMipmap), "glGenerateMipmap");
        bindings.blitFramebuffer = try getProcAddress(@TypeOf(bindings.blitFramebuffer), "glBlitFramebuffer");
        bindings.renderbufferStorageMultisample = try getProcAddress(
            @TypeOf(bindings.renderbufferStorageMultisample),
            "glRenderbufferStorageMultisample",
        );
        bindings.framebufferTextureLayer = try getProcAddress(
            @TypeOf(bindings.framebufferTextureLayer),
            "glFramebufferTextureLayer",
        );
        bindings.mapBufferRange = try getProcAddress(@TypeOf(bindings.mapBufferRange), "glMapBufferRange");
        bindings.flushMappedBufferRange = try getProcAddress(
            @TypeOf(bindings.flushMappedBufferRange),
            "glFlushMappedBufferRange",
        );
        bindings.bindVertexArray = try getProcAddress(@TypeOf(bindings.bindVertexArray), "glBindVertexArray");
        bindings.deleteVertexArrays = try getProcAddress(
            @TypeOf(bindings.deleteVertexArrays),
            "glDeleteVertexArrays",
        );
        bindings.genVertexArrays = try getProcAddress(@TypeOf(bindings.genVertexArrays), "glGenVertexArrays");
        bindings.isVertexArray = try getProcAddress(@TypeOf(bindings.isVertexArray), "glIsVertexArray");
    }

    // OpenGL 3.1
    if (ver >= 31) {
        bindings.drawArraysInstanced = try getProcAddress(
            @TypeOf(bindings.drawArraysInstanced),
            "glDrawArraysInstanced",
        );
        bindings.drawElementsInstanced = try getProcAddress(
            @TypeOf(bindings.drawElementsInstanced),
            "glDrawElementsInstanced",
        );
        bindings.texBuffer = try getProcAddress(@TypeOf(bindings.texBuffer), "glTexBuffer");
        bindings.primitiveRestartIndex = try getProcAddress(
            @TypeOf(bindings.primitiveRestartIndex),
            "glPrimitiveRestartIndex",
        );
        bindings.copyBufferSubData = try getProcAddress(@TypeOf(bindings.copyBufferSubData), "glCopyBufferSubData");
        bindings.getUniformIndices = try getProcAddress(@TypeOf(bindings.getUniformIndices), "glGetUniformIndices");
        bindings.getActiveUniformsiv = try getProcAddress(
            @TypeOf(bindings.getActiveUniformsiv),
            "glGetActiveUniformsiv",
        );
        bindings.getActiveUniformName = try getProcAddress(
            @TypeOf(bindings.getActiveUniformName),
            "glGetActiveUniformName",
        );
        bindings.getUniformBlockIndex = try getProcAddress(
            @TypeOf(bindings.getUniformBlockIndex),
            "glGetUniformBlockIndex",
        );
        bindings.getActiveUniformBlockiv = try getProcAddress(
            @TypeOf(bindings.getActiveUniformBlockiv),
            "glGetActiveUniformBlockiv",
        );
        bindings.getActiveUniformBlockName = try getProcAddress(
            @TypeOf(bindings.getActiveUniformBlockName),
            "glGetActiveUniformBlockName",
        );
        bindings.uniformBlockBinding = try getProcAddress(
            @TypeOf(bindings.uniformBlockBinding),
            "glUniformBlockBinding",
        );
    }

    // OpenGL 3.2
    if (ver >= 32) {
        bindings.drawElementsBaseVertex = try getProcAddress(
            @TypeOf(bindings.drawElementsBaseVertex),
            "glDrawElementsBaseVertex",
        );
        bindings.drawRangeElementsBaseVertex = try getProcAddress(
            @TypeOf(bindings.drawRangeElementsBaseVertex),
            "glDrawRangeElementsBaseVertex",
        );
        bindings.drawElementsInstancedBaseVertex = try getProcAddress(
            @TypeOf(bindings.drawElementsInstancedBaseVertex),
            "glDrawElementsInstancedBaseVertex",
        );
        bindings.multiDrawElementsBaseVertex = try getProcAddress(
            @TypeOf(bindings.multiDrawElementsBaseVertex),
            "glMultiDrawElementsBaseVertex",
        );
        bindings.provokingVertex = try getProcAddress(@TypeOf(bindings.provokingVertex), "glProvokingVertex");
        bindings.fenceSync = try getProcAddress(@TypeOf(bindings.fenceSync), "glFenceSync");
        bindings.isSync = try getProcAddress(@TypeOf(bindings.isSync), "glIsSync");
        bindings.deleteSync = try getProcAddress(@TypeOf(bindings.deleteSync), "glDeleteSync");
        bindings.clientWaitSync = try getProcAddress(@TypeOf(bindings.clientWaitSync), "glClientWaitSync");
        bindings.waitSync = try getProcAddress(@TypeOf(bindings.waitSync), "glWaitSync");
        bindings.getInteger64v = try getProcAddress(@TypeOf(bindings.getInteger64v), "glGetInteger64v");
        bindings.getSynciv = try getProcAddress(@TypeOf(bindings.getSynciv), "glGetSynciv");
        bindings.getInteger64i_v = try getProcAddress(@TypeOf(bindings.getInteger64i_v), "glGetInteger64i_v");
        bindings.getBufferParameteri64v = try getProcAddress(
            @TypeOf(bindings.getBufferParameteri64v),
            "glGetBufferParameteri64v",
        );
        bindings.framebufferTexture = try getProcAddress(
            @TypeOf(bindings.framebufferTexture),
            "glFramebufferTexture",
        );
        bindings.texImage2DMultisample = try getProcAddress(
            @TypeOf(bindings.texImage2DMultisample),
            "glTexImage2DMultisample",
        );
        bindings.texImage3DMultisample = try getProcAddress(
            @TypeOf(bindings.texImage3DMultisample),
            "glTexImage3DMultisample",
        );
        bindings.getMultisamplefv = try getProcAddress(@TypeOf(bindings.getMultisamplefv), "glGetMultisamplefv");
        bindings.sampleMaski = try getProcAddress(@TypeOf(bindings.sampleMaski), "glSampleMaski");
    }

    // OpenGL 3.3
    if (ver >= 33) {
        bindings.bindFragDataLocationIndexed = try getProcAddress(
            @TypeOf(bindings.bindFragDataLocationIndexed),
            "glBindFragDataLocationIndexed",
        );
        bindings.getFragDataIndex = try getProcAddress(@TypeOf(bindings.getFragDataIndex), "glGetFragDataIndex");
        bindings.genSamplers = try getProcAddress(@TypeOf(bindings.genSamplers), "glGenSamplers");
        bindings.deleteSamplers = try getProcAddress(@TypeOf(bindings.deleteSamplers), "glDeleteSamplers");
        bindings.isSampler = try getProcAddress(@TypeOf(bindings.isSampler), "glIsSampler");
        bindings.bindSampler = try getProcAddress(@TypeOf(bindings.bindSampler), "glBindSampler");
        bindings.samplerParameteri = try getProcAddress(@TypeOf(bindings.samplerParameteri), "glSamplerParameteri");
        bindings.samplerParameteriv = try getProcAddress(
            @TypeOf(bindings.samplerParameteriv),
            "glSamplerParameteriv",
        );
        bindings.samplerParameterf = try getProcAddress(@TypeOf(bindings.samplerParameterf), "glSamplerParameterf");
        bindings.samplerParameterfv = try getProcAddress(
            @TypeOf(bindings.samplerParameterfv),
            "glSamplerParameterfv",
        );
        bindings.samplerParameterIiv = try getProcAddress(
            @TypeOf(bindings.samplerParameterIiv),
            "glSamplerParameterIiv",
        );
        bindings.samplerParameterIuiv = try getProcAddress(
            @TypeOf(bindings.samplerParameterIuiv),
            "glSamplerParameterIuiv",
        );
        bindings.getSamplerParameteriv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameteriv),
            "glGetSamplerParameteriv",
        );
        bindings.getSamplerParameterIiv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameterIiv),
            "glGetSamplerParameterIiv",
        );
        bindings.getSamplerParameterfv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameterfv),
            "glGetSamplerParameterfv",
        );
        bindings.getSamplerParameterIuiv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameterIuiv),
            "glGetSamplerParameterIuiv",
        );
        bindings.queryCounter = try getProcAddress(@TypeOf(bindings.queryCounter), "glQueryCounter");
        bindings.getQueryObjecti64v = try getProcAddress(
            @TypeOf(bindings.getQueryObjecti64v),
            "glGetQueryObjecti64v",
        );
        bindings.getQueryObjectui64v = try getProcAddress(
            @TypeOf(bindings.getQueryObjectui64v),
            "glGetQueryObjectui64v",
        );
        bindings.vertexAttribDivisor = try getProcAddress(
            @TypeOf(bindings.vertexAttribDivisor),
            "glVertexAttribDivisor",
        );
        bindings.vertexAttribP1ui = try getProcAddress(@TypeOf(bindings.vertexAttribP1ui), "glVertexAttribP1ui");
        bindings.vertexAttribP1uiv = try getProcAddress(@TypeOf(bindings.vertexAttribP1uiv), "glVertexAttribP1uiv");
        bindings.vertexAttribP2ui = try getProcAddress(@TypeOf(bindings.vertexAttribP2ui), "glVertexAttribP2ui");
        bindings.vertexAttribP2uiv = try getProcAddress(@TypeOf(bindings.vertexAttribP2uiv), "glVertexAttribP2uiv");
        bindings.vertexAttribP3ui = try getProcAddress(@TypeOf(bindings.vertexAttribP3ui), "glVertexAttribP3ui");
        bindings.vertexAttribP3uiv = try getProcAddress(@TypeOf(bindings.vertexAttribP3uiv), "glVertexAttribP3uiv");
        bindings.vertexAttribP4ui = try getProcAddress(@TypeOf(bindings.vertexAttribP4ui), "glVertexAttribP4ui");
        bindings.vertexAttribP4uiv = try getProcAddress(@TypeOf(bindings.vertexAttribP4uiv), "glVertexAttribP4uiv");
    }

    // OpenGL 4.0
    if (ver >= 40) {
        bindings.minSampleShading = try getProcAddress(@TypeOf(bindings.minSampleShading), "glMinSampleShading");
        bindings.blendEquationi = try getProcAddress(@TypeOf(bindings.blendEquationi), "glBlendEquationi");
        bindings.blendEquationSeparatei = try getProcAddress(@TypeOf(bindings.blendEquationSeparatei), "glBlendEquationSeparatei");
        bindings.blendFunci = try getProcAddress(@TypeOf(bindings.blendFunci), "glBlendFunci");
        bindings.blendFuncSeparatei = try getProcAddress(@TypeOf(bindings.blendFuncSeparatei), "glBlendFuncSeparatei");
        bindings.drawArraysIndirect = try getProcAddress(@TypeOf(bindings.drawArraysIndirect), "glDrawArraysIndirect");
        bindings.drawElementsIndirect = try getProcAddress(@TypeOf(bindings.drawElementsIndirect), "glDrawElementsIndirect");
        bindings.uniform1d = try getProcAddress(@TypeOf(bindings.uniform1d), "glUniform1d");
        bindings.uniform2d = try getProcAddress(@TypeOf(bindings.uniform2d), "glUniform2d");
        bindings.uniform3d = try getProcAddress(@TypeOf(bindings.uniform3d), "glUniform3d");
        bindings.uniform4d = try getProcAddress(@TypeOf(bindings.uniform4d), "glUniform4d");
        bindings.uniform1dv = try getProcAddress(@TypeOf(bindings.uniform1dv), "glUniform1dv");
        bindings.uniform2dv = try getProcAddress(@TypeOf(bindings.uniform2dv), "glUniform2dv");
        bindings.uniform3dv = try getProcAddress(@TypeOf(bindings.uniform3dv), "glUniform3dv");
        bindings.uniform4dv = try getProcAddress(@TypeOf(bindings.uniform4dv), "glUniform4dv");
        bindings.uniformMatrix2dv = try getProcAddress(@TypeOf(bindings.uniformMatrix2dv), "glUniformMatrix2dv");
        bindings.uniformMatrix3dv = try getProcAddress(@TypeOf(bindings.uniformMatrix3dv), "glUniformMatrix3dv");
        bindings.uniformMatrix4dv = try getProcAddress(@TypeOf(bindings.uniformMatrix4dv), "glUniformMatrix4dv");
        bindings.uniformMatrix2x3dv = try getProcAddress(@TypeOf(bindings.uniformMatrix2x3dv), "glUniformMatrix2x3dv");
        bindings.uniformMatrix2x4dv = try getProcAddress(@TypeOf(bindings.uniformMatrix2x4dv), "glUniformMatrix2x4dv");
        bindings.uniformMatrix3x2dv = try getProcAddress(@TypeOf(bindings.uniformMatrix3x2dv), "glUniformMatrix3x2dv");
        bindings.uniformMatrix3x4dv = try getProcAddress(@TypeOf(bindings.uniformMatrix3x4dv), "glUniformMatrix3x4dv");
        bindings.uniformMatrix4x2dv = try getProcAddress(@TypeOf(bindings.uniformMatrix4x2dv), "glUniformMatrix4x2dv");
        bindings.uniformMatrix4x3dv = try getProcAddress(@TypeOf(bindings.uniformMatrix4x3dv), "glUniformMatrix4x3dv");
        bindings.getUniformdv = try getProcAddress(@TypeOf(bindings.getUniformdv), "glGetUniformdv");
        bindings.getSubroutineUniformLocation = try getProcAddress(@TypeOf(bindings.getSubroutineUniformLocation), "glGetSubroutineUniformLocation");
        bindings.getSubroutineIndex = try getProcAddress(@TypeOf(bindings.getSubroutineIndex), "glGetSubroutineIndex");
        bindings.getActiveSubroutineUniformiv = try getProcAddress(@TypeOf(bindings.getActiveSubroutineUniformiv), "glGetActiveSubroutineUniformiv");
        bindings.getActiveSubroutineUniformName = try getProcAddress(@TypeOf(bindings.getActiveSubroutineUniformName), "glGetActiveSubroutineUniformName");
        bindings.getActiveSubroutineName = try getProcAddress(@TypeOf(bindings.getActiveSubroutineName), "glGetActiveSubroutineName");
        bindings.uniformSubroutinesuiv = try getProcAddress(@TypeOf(bindings.uniformSubroutinesuiv), "glUniformSubroutinesuiv");
        bindings.getUniformSubroutineuiv = try getProcAddress(@TypeOf(bindings.getUniformSubroutineuiv), "glGetUniformSubroutineuiv");
        bindings.getProgramStageiv = try getProcAddress(@TypeOf(bindings.getProgramStageiv), "glGetProgramStageiv");
        bindings.patchParameteri = try getProcAddress(@TypeOf(bindings.patchParameteri), "glPatchParameteri");
        bindings.patchParameterfv = try getProcAddress(@TypeOf(bindings.patchParameterfv), "glPatchParameterfv");
        bindings.bindTransformFeedback = try getProcAddress(@TypeOf(bindings.bindTransformFeedback), "glBindTransformFeedback");
        bindings.deleteTransformFeedbacks = try getProcAddress(@TypeOf(bindings.deleteTransformFeedbacks), "glDeleteTransformFeedbacks");
        bindings.genTransformFeedbacks = try getProcAddress(@TypeOf(bindings.genTransformFeedbacks), "glGenTransformFeedbacks");
        bindings.isTransformFeedback = try getProcAddress(@TypeOf(bindings.isTransformFeedback), "glIsTransformFeedback");
        bindings.pauseTransformFeedback = try getProcAddress(@TypeOf(bindings.pauseTransformFeedback), "glPauseTransformFeedback");
        bindings.resumeTransformFeedback = try getProcAddress(@TypeOf(bindings.resumeTransformFeedback), "glResumeTransformFeedback");
        bindings.drawTransformFeedback = try getProcAddress(@TypeOf(bindings.drawTransformFeedback), "glDrawTransformFeedback");
        bindings.drawTransformFeedbackStream = try getProcAddress(@TypeOf(bindings.drawTransformFeedbackStream), "glDrawTransformFeedbackStream");
        bindings.beginQueryIndexed = try getProcAddress(@TypeOf(bindings.beginQueryIndexed), "glBeginQueryIndexed");
        bindings.endQueryIndexed = try getProcAddress(@TypeOf(bindings.endQueryIndexed), "glEndQueryIndexed");
        bindings.glGetQueryIndexediv = try getProcAddress(@TypeOf(bindings.glGetQueryIndexediv), "glGetQueryIndexediv");
    }

    // OpenGL 4.1
    if (ver >= 41) {
        bindings.createShaderProgramv = try getProcAddress(
            @TypeOf(bindings.createShaderProgramv),
            "glCreateShaderProgramv",
        );
        // TODO
    }

    // OpenGL 4.2
    if (ver >= 42) {
        bindings.bindImageTexture = try getProcAddress(@TypeOf(bindings.bindImageTexture), "glBindImageTexture");
        bindings.memoryBarrier = try getProcAddress(@TypeOf(bindings.memoryBarrier), "glMemoryBarrier");
        // TODO
    }

    // OpenGL 4.3
    if (ver >= 43) {
        bindings.debugMessageControl = try getProcAddress(
            @TypeOf(bindings.debugMessageControl),
            "glDebugMessageControl",
        );
        bindings.debugMessageInsert = try getProcAddress(
            @TypeOf(bindings.debugMessageInsert),
            "glDebugMessageInsert",
        );
        bindings.debugMessageCallback = try getProcAddress(
            @TypeOf(bindings.debugMessageCallback),
            "glDebugMessageCallback",
        );
        bindings.getDebugMessageLog = try getProcAddress(
            @TypeOf(bindings.getDebugMessageLog),
            "glGetDebugMessageLog",
        );
        bindings.getPointerv = try getProcAddress(@TypeOf(bindings.getPointerv), "glGetPointerv");
        bindings.pushDebugGroup = try getProcAddress(@TypeOf(bindings.pushDebugGroup), "glPushDebugGroup");
        bindings.popDebugGroup = try getProcAddress(@TypeOf(bindings.popDebugGroup), "glPopDebugGroup");
        bindings.objectLabel = try getProcAddress(@TypeOf(bindings.objectLabel), "glObjectLabel");
        bindings.getObjectLabel = try getProcAddress(@TypeOf(bindings.getObjectLabel), "glGetObjectLabel");
        bindings.objectPtrLabel = try getProcAddress(@TypeOf(bindings.objectPtrLabel), "glObjectPtrLabel");
        bindings.getObjectPtrLabel = try getProcAddress(@TypeOf(bindings.getObjectPtrLabel), "glGetObjectPtrLabel");
        // TODO
    }

    // OpenGL 4.4
    if (ver >= 44) {
        bindings.clearTexImage = try getProcAddress(@TypeOf(bindings.clearTexImage), "glClearTexImage");
        // TODO
    }

    // OpenGL 4.5
    if (ver >= 45) {
        bindings.textureStorage2D = try getProcAddress(@TypeOf(bindings.textureStorage2D), "glTextureStorage2D");
        bindings.textureStorage2DMultisample = try getProcAddress(
            @TypeOf(bindings.textureStorage2DMultisample),
            "glTextureStorage2DMultisample",
        );
        bindings.createTextures = try getProcAddress(@TypeOf(bindings.createTextures), "glCreateTextures");
        bindings.createFramebuffers = try getProcAddress(
            @TypeOf(bindings.createFramebuffers),
            "glCreateFramebuffers",
        );
        bindings.namedFramebufferTexture = try getProcAddress(
            @TypeOf(bindings.namedFramebufferTexture),
            "glNamedFramebufferTexture",
        );
        bindings.blitNamedFramebuffer = try getProcAddress(
            @TypeOf(bindings.blitNamedFramebuffer),
            "glBlitNamedFramebuffer",
        );
        bindings.createBuffers = try getProcAddress(@TypeOf(bindings.createBuffers), "glCreateBuffers");
        bindings.clearNamedFramebufferfv = try getProcAddress(
            @TypeOf(bindings.clearNamedFramebufferfv),
            "glClearNamedFramebufferfv",
        );
        bindings.namedBufferStorage = try getProcAddress(
            @TypeOf(bindings.namedBufferStorage),
            "glNamedBufferStorage",
        );
        bindings.bindTextureUnit = try getProcAddress(@TypeOf(bindings.bindTextureUnit), "glBindTextureUnit");
        bindings.textureBarrier = try getProcAddress(@TypeOf(bindings.textureBarrier), "glTextureBarrier");
        // TODO
    }

    // OpenGL 4.6
    if (ver >= 46) {
        // TODO
    }
}

/// Loads a subset of OpenGL 4.6 (Compatibility Profile) + some useful, multivendor (NVIDIA, AMD) extensions.
pub fn loadCompatProfileExt(loader: LoaderFn) !void {
    try loadCoreProfile(loader, 4, 6);

    bindings.begin = try getProcAddress(@TypeOf(bindings.begin), "glBegin");
    bindings.end = try getProcAddress(@TypeOf(bindings.end), "glEnd");
    bindings.newList = try getProcAddress(@TypeOf(bindings.newList), "glNewList");
    bindings.callList = try getProcAddress(@TypeOf(bindings.callList), "glCallList");
    bindings.endList = try getProcAddress(@TypeOf(bindings.endList), "glEndList");
    bindings.loadIdentity = try getProcAddress(@TypeOf(bindings.loadIdentity), "glLoadIdentity");
    bindings.vertex2fv = try getProcAddress(@TypeOf(bindings.vertex2fv), "glVertex2fv");
    bindings.vertex3fv = try getProcAddress(@TypeOf(bindings.vertex3fv), "glVertex3fv");
    bindings.vertex4fv = try getProcAddress(@TypeOf(bindings.vertex4fv), "glVertex4fv");
    bindings.color3fv = try getProcAddress(@TypeOf(bindings.color3fv), "glColor3fv");
    bindings.color4fv = try getProcAddress(@TypeOf(bindings.color4fv), "glColor4fv");
    bindings.rectf = try getProcAddress(@TypeOf(bindings.rectf), "glRectf");
    bindings.matrixMode = try getProcAddress(@TypeOf(bindings.matrixMode), "glMatrixMode");
    bindings.vertex2f = try getProcAddress(@TypeOf(bindings.vertex2f), "glVertex2f");
    bindings.vertex2d = try getProcAddress(@TypeOf(bindings.vertex2d), "glVertex2d");
    bindings.vertex2i = try getProcAddress(@TypeOf(bindings.vertex2i), "glVertex2i");
    bindings.color3f = try getProcAddress(@TypeOf(bindings.color3f), "glColor3f");
    bindings.color4f = try getProcAddress(@TypeOf(bindings.color4f), "glColor4f");
    bindings.color4ub = try getProcAddress(@TypeOf(bindings.color4ub), "glColor4ub");
    bindings.pushMatrix = try getProcAddress(@TypeOf(bindings.pushMatrix), "glPushMatrix");
    bindings.popMatrix = try getProcAddress(@TypeOf(bindings.popMatrix), "glPopMatrix");
    bindings.rotatef = try getProcAddress(@TypeOf(bindings.rotatef), "glRotatef");
    bindings.scalef = try getProcAddress(@TypeOf(bindings.scalef), "glScalef");
    bindings.translatef = try getProcAddress(@TypeOf(bindings.translatef), "glTranslatef");
    bindings.matrixLoadIdentityEXT = try getProcAddress(
        @TypeOf(bindings.matrixLoadIdentityEXT),
        "glMatrixLoadIdentityEXT",
    );
    bindings.matrixOrthoEXT = try getProcAddress(@TypeOf(bindings.matrixOrthoEXT), "glMatrixOrthoEXT");
}

pub fn loadEsProfile(loader: LoaderFn, major: u32, minor: u32) !void {
    const ver = 10 * major + minor;

    assert(major >= 1 and major <= 3);
    assert(minor >= 0 and minor <= 2);
    assert(ver >= 10 and ver <= 32);

    loaderFunc = loader;

    // OpenGL ES 1.0
    if (ver >= 10) {
        bindings.cullFace = try getProcAddress(@TypeOf(bindings.cullFace), "glCullFace");
        bindings.frontFace = try getProcAddress(@TypeOf(bindings.frontFace), "glFrontFace");
        bindings.hint = try getProcAddress(@TypeOf(bindings.hint), "glHint");
        bindings.lineWidth = try getProcAddress(@TypeOf(bindings.lineWidth), "glLineWidth");
        bindings.scissor = try getProcAddress(@TypeOf(bindings.scissor), "glScissor");
        bindings.texParameterf = try getProcAddress(@TypeOf(bindings.texParameterf), "glTexParameterf");
        bindings.texParameterfv = try getProcAddress(@TypeOf(bindings.texParameterfv), "glTexParameterfv");
        bindings.texParameteri = try getProcAddress(@TypeOf(bindings.texParameteri), "glTexParameteri");
        bindings.texParameteriv = try getProcAddress(@TypeOf(bindings.texParameteriv), "glTexParameteriv");
        bindings.texImage2D = try getProcAddress(@TypeOf(bindings.texImage2D), "glTexImage2D");
        bindings.clear = try getProcAddress(@TypeOf(bindings.clear), "glClear");
        bindings.clearColor = try getProcAddress(@TypeOf(bindings.clearColor), "glClearColor");
        bindings.clearStencil = try getProcAddress(@TypeOf(bindings.clearStencil), "glClearStencil");
        bindings.clearDepthf = try getProcAddress(@TypeOf(bindings.clearDepthf), "glClearDepthf");
        bindings.stencilMask = try getProcAddress(@TypeOf(bindings.stencilMask), "glStencilMask");
        bindings.colorMask = try getProcAddress(@TypeOf(bindings.colorMask), "glColorMask");
        bindings.depthMask = try getProcAddress(@TypeOf(bindings.depthMask), "glDepthMask");
        bindings.disable = try getProcAddress(@TypeOf(bindings.disable), "glDisable");
        bindings.enable = try getProcAddress(@TypeOf(bindings.enable), "glEnable");
        bindings.finish = try getProcAddress(@TypeOf(bindings.finish), "glFinish");
        bindings.flush = try getProcAddress(@TypeOf(bindings.flush), "glFlush");
        bindings.blendFunc = try getProcAddress(@TypeOf(bindings.blendFunc), "glBlendFunc");
        bindings.stencilFunc = try getProcAddress(@TypeOf(bindings.stencilFunc), "glStencilFunc");
        bindings.stencilOp = try getProcAddress(@TypeOf(bindings.stencilOp), "glStencilOp");
        bindings.depthFunc = try getProcAddress(@TypeOf(bindings.depthFunc), "glDepthFunc");
        bindings.pixelStorei = try getProcAddress(@TypeOf(bindings.pixelStorei), "glPixelStorei");
        bindings.readPixels = try getProcAddress(@TypeOf(bindings.readPixels), "glReadPixels");
        bindings.getBooleanv = try getProcAddress(@TypeOf(bindings.getBooleanv), "glGetBooleanv");
        bindings.getError = try getProcAddress(@TypeOf(bindings.getError), "glGetError");
        bindings.getFloatv = try getProcAddress(@TypeOf(bindings.getFloatv), "glGetFloatv");
        bindings.getIntegerv = try getProcAddress(@TypeOf(bindings.getIntegerv), "glGetIntegerv");
        bindings.getString = try getProcAddress(@TypeOf(bindings.getString), "glGetString");
        bindings.isEnabled = try getProcAddress(@TypeOf(bindings.isEnabled), "glIsEnabled");
        bindings.depthRangef = try getProcAddress(@TypeOf(bindings.depthRangef), "glDepthRangef");
        bindings.viewport = try getProcAddress(@TypeOf(bindings.viewport), "glViewport");
        bindings.drawArrays = try getProcAddress(@TypeOf(bindings.drawArrays), "glDrawArrays");
        bindings.drawElements = try getProcAddress(@TypeOf(bindings.drawElements), "glDrawElements");
        bindings.polygonOffset = try getProcAddress(@TypeOf(bindings.polygonOffset), "glPolygonOffset");
        bindings.copyTexImage2D = try getProcAddress(@TypeOf(bindings.copyTexImage2D), "glCopyTexImage2D");
        bindings.copyTexSubImage2D = try getProcAddress(@TypeOf(bindings.copyTexSubImage2D), "glCopyTexSubImage2D");
        bindings.texSubImage2D = try getProcAddress(@TypeOf(bindings.texSubImage2D), "glTexSubImage2D");
        bindings.bindTexture = try getProcAddress(@TypeOf(bindings.bindTexture), "glBindTexture");
        bindings.deleteTextures = try getProcAddress(@TypeOf(bindings.deleteTextures), "glDeleteTextures");
        bindings.genTextures = try getProcAddress(@TypeOf(bindings.genTextures), "glGenTextures");
        bindings.isTexture = try getProcAddress(@TypeOf(bindings.isTexture), "glIsTexture");
        bindings.activeTexture = try getProcAddress(@TypeOf(bindings.activeTexture), "glActiveTexture");
        bindings.sampleCoverage = try getProcAddress(@TypeOf(bindings.sampleCoverage), "glSampleCoverage");
        bindings.compressedTexImage2D = try getProcAddress(
            @TypeOf(bindings.compressedTexImage2D),
            "glCompressedTexImage2D",
        );
        bindings.compressedTexSubImage2D = try getProcAddress(
            @TypeOf(bindings.compressedTexSubImage2D),
            "glCompressedTexSubImage2D",
        );
    }

    // OpenGL ES 1.1
    if (ver >= 11) {
        bindings.blendFuncSeparate = try getProcAddress(@TypeOf(bindings.blendFuncSeparate), "glBlendFuncSeparate");
        bindings.blendColor = try getProcAddress(@TypeOf(bindings.blendColor), "glBlendColor");
        bindings.blendEquation = try getProcAddress(@TypeOf(bindings.blendEquation), "glBlendEquation");
        bindings.bindBuffer = try getProcAddress(@TypeOf(bindings.bindBuffer), "glBindBuffer");
        bindings.deleteBuffers = try getProcAddress(@TypeOf(bindings.deleteBuffers), "glDeleteBuffers");
        bindings.genBuffers = try getProcAddress(@TypeOf(bindings.genBuffers), "glGenBuffers");
        bindings.isBuffer = try getProcAddress(@TypeOf(bindings.isBuffer), "glIsBuffer");
        bindings.bufferData = try getProcAddress(@TypeOf(bindings.bufferData), "glBufferData");
        bindings.bufferSubData = try getProcAddress(@TypeOf(bindings.bufferSubData), "glBufferSubData");
        bindings.getBufferParameteriv = try getProcAddress(
            @TypeOf(bindings.getBufferParameteriv),
            "glGetBufferParameteriv",
        );
    }

    // OpenGL ES 2.0
    if (ver >= 20) {
        bindings.blendEquationSeparate = try getProcAddress(
            @TypeOf(bindings.blendEquationSeparate),
            "glBlendEquationSeparate",
        );
        bindings.stencilOpSeparate = try getProcAddress(@TypeOf(bindings.stencilOpSeparate), "glStencilOpSeparate");
        bindings.stencilFuncSeparate = try getProcAddress(
            @TypeOf(bindings.stencilFuncSeparate),
            "glStencilFuncSeparate",
        );
        bindings.stencilMaskSeparate = try getProcAddress(
            @TypeOf(bindings.stencilMaskSeparate),
            "glStencilMaskSeparate",
        );
        bindings.attachShader = try getProcAddress(@TypeOf(bindings.attachShader), "glAttachShader");
        bindings.bindAttribLocation = try getProcAddress(
            @TypeOf(bindings.bindAttribLocation),
            "glBindAttribLocation",
        );
        bindings.compileShader = try getProcAddress(@TypeOf(bindings.compileShader), "glCompileShader");
        bindings.createProgram = try getProcAddress(@TypeOf(bindings.createProgram), "glCreateProgram");
        bindings.createShader = try getProcAddress(@TypeOf(bindings.createShader), "glCreateShader");
        bindings.deleteProgram = try getProcAddress(@TypeOf(bindings.deleteProgram), "glDeleteProgram");
        bindings.deleteShader = try getProcAddress(@TypeOf(bindings.deleteShader), "glDeleteShader");
        bindings.detachShader = try getProcAddress(@TypeOf(bindings.detachShader), "glDetachShader");
        bindings.disableVertexAttribArray = try getProcAddress(
            @TypeOf(bindings.disableVertexAttribArray),
            "glDisableVertexAttribArray",
        );
        bindings.enableVertexAttribArray = try getProcAddress(
            @TypeOf(bindings.enableVertexAttribArray),
            "glEnableVertexAttribArray",
        );
        bindings.getActiveAttrib = try getProcAddress(@TypeOf(bindings.getActiveAttrib), "glGetActiveAttrib");
        bindings.getActiveUniform = try getProcAddress(@TypeOf(bindings.getActiveUniform), "glGetActiveUniform");
        bindings.getAttachedShaders = try getProcAddress(
            @TypeOf(bindings.getAttachedShaders),
            "glGetAttachedShaders",
        );
        bindings.getAttribLocation = try getProcAddress(@TypeOf(bindings.getAttribLocation), "glGetAttribLocation");
        bindings.getProgramiv = try getProcAddress(@TypeOf(bindings.getProgramiv), "glGetProgramiv");
        bindings.getProgramInfoLog = try getProcAddress(@TypeOf(bindings.getProgramInfoLog), "glGetProgramInfoLog");
        bindings.getShaderiv = try getProcAddress(@TypeOf(bindings.getShaderiv), "glGetShaderiv");
        bindings.getShaderInfoLog = try getProcAddress(@TypeOf(bindings.getShaderInfoLog), "glGetShaderInfoLog");
        bindings.getShaderSource = try getProcAddress(@TypeOf(bindings.getShaderSource), "glGetShaderSource");
        bindings.getUniformLocation = try getProcAddress(
            @TypeOf(bindings.getUniformLocation),
            "glGetUniformLocation",
        );
        bindings.getUniformfv = try getProcAddress(@TypeOf(bindings.getUniformfv), "glGetUniformfv");
        bindings.getUniformiv = try getProcAddress(@TypeOf(bindings.getUniformiv), "glGetUniformiv");
        bindings.getVertexAttribPointerv = try getProcAddress(
            @TypeOf(bindings.getVertexAttribPointerv),
            "glGetVertexAttribPointerv",
        );
        bindings.isProgram = try getProcAddress(@TypeOf(bindings.isProgram), "glIsProgram");
        bindings.isShader = try getProcAddress(@TypeOf(bindings.isShader), "glIsShader");
        bindings.linkProgram = try getProcAddress(@TypeOf(bindings.linkProgram), "glLinkProgram");
        bindings.shaderSource = try getProcAddress(@TypeOf(bindings.shaderSource), "glShaderSource");
        bindings.useProgram = try getProcAddress(@TypeOf(bindings.useProgram), "glUseProgram");
        bindings.uniform1f = try getProcAddress(@TypeOf(bindings.uniform1f), "glUniform1f");
        bindings.uniform2f = try getProcAddress(@TypeOf(bindings.uniform2f), "glUniform2f");
        bindings.uniform3f = try getProcAddress(@TypeOf(bindings.uniform3f), "glUniform3f");
        bindings.uniform4f = try getProcAddress(@TypeOf(bindings.uniform4f), "glUniform4f");
        bindings.uniform1i = try getProcAddress(@TypeOf(bindings.uniform1i), "glUniform1i");
        bindings.uniform2i = try getProcAddress(@TypeOf(bindings.uniform2i), "glUniform2i");
        bindings.uniform3i = try getProcAddress(@TypeOf(bindings.uniform3i), "glUniform3i");
        bindings.uniform4i = try getProcAddress(@TypeOf(bindings.uniform4i), "glUniform4i");
        bindings.uniform1fv = try getProcAddress(@TypeOf(bindings.uniform1fv), "glUniform1fv");
        bindings.uniform2fv = try getProcAddress(@TypeOf(bindings.uniform2fv), "glUniform2fv");
        bindings.uniform3fv = try getProcAddress(@TypeOf(bindings.uniform3fv), "glUniform3fv");
        bindings.uniform4fv = try getProcAddress(@TypeOf(bindings.uniform4fv), "glUniform4fv");
        bindings.uniform1iv = try getProcAddress(@TypeOf(bindings.uniform1iv), "glUniform1iv");
        bindings.uniform2iv = try getProcAddress(@TypeOf(bindings.uniform2iv), "glUniform2iv");
        bindings.uniform3iv = try getProcAddress(@TypeOf(bindings.uniform3iv), "glUniform3iv");
        bindings.uniform4iv = try getProcAddress(@TypeOf(bindings.uniform4iv), "glUniform4iv");
        bindings.uniformMatrix2fv = try getProcAddress(@TypeOf(bindings.uniformMatrix2fv), "glUniformMatrix2fv");
        bindings.uniformMatrix3fv = try getProcAddress(@TypeOf(bindings.uniformMatrix3fv), "glUniformMatrix3fv");
        bindings.uniformMatrix4fv = try getProcAddress(@TypeOf(bindings.uniformMatrix4fv), "glUniformMatrix4fv");
        bindings.validateProgram = try getProcAddress(@TypeOf(bindings.validateProgram), "glValidateProgram");
        bindings.vertexAttribPointer = try getProcAddress(
            @TypeOf(bindings.vertexAttribPointer),
            "glVertexAttribPointer",
        );
        bindings.isRenderbuffer = try getProcAddress(@TypeOf(bindings.isRenderbuffer), "glIsRenderbuffer");
        bindings.bindRenderbuffer = try getProcAddress(@TypeOf(bindings.bindRenderbuffer), "glBindRenderbuffer");
        bindings.deleteRenderbuffers = try getProcAddress(
            @TypeOf(bindings.deleteRenderbuffers),
            "glDeleteRenderbuffers",
        );
        bindings.genRenderbuffers = try getProcAddress(@TypeOf(bindings.genRenderbuffers), "glGenRenderbuffers");
        bindings.renderbufferStorage = try getProcAddress(
            @TypeOf(bindings.renderbufferStorage),
            "glRenderbufferStorage",
        );
        bindings.getRenderbufferParameteriv = try getProcAddress(
            @TypeOf(bindings.getRenderbufferParameteriv),
            "glGetRenderbufferParameteriv",
        );
        bindings.isFramebuffer = try getProcAddress(@TypeOf(bindings.isFramebuffer), "glIsFramebuffer");
        bindings.bindFramebuffer = try getProcAddress(@TypeOf(bindings.bindFramebuffer), "glBindFramebuffer");
        bindings.deleteFramebuffers = try getProcAddress(
            @TypeOf(bindings.deleteFramebuffers),
            "glDeleteFramebuffers",
        );
        bindings.genFramebuffers = try getProcAddress(@TypeOf(bindings.genFramebuffers), "glGenFramebuffers");
        bindings.checkFramebufferStatus = try getProcAddress(
            @TypeOf(bindings.checkFramebufferStatus),
            "glCheckFramebufferStatus",
        );
        bindings.framebufferTexture2D = try getProcAddress(
            @TypeOf(bindings.framebufferTexture2D),
            "glFramebufferTexture2D",
        );
        bindings.framebufferRenderbuffer = try getProcAddress(
            @TypeOf(bindings.framebufferRenderbuffer),
            "glFramebufferRenderbuffer",
        );
        bindings.getFramebufferAttachmentParameteriv = try getProcAddress(
            @TypeOf(bindings.getFramebufferAttachmentParameteriv),
            "glGetFramebufferAttachmentParameteriv",
        );
        bindings.generateMipmap = try getProcAddress(@TypeOf(bindings.generateMipmap), "glGenerateMipmap");
    }

    // OpenGL ES 3.0
    if (ver >= 30) {
        bindings.uniformMatrix2x3fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix2x3fv),
            "glUniformMatrix2x3fv",
        );
        bindings.uniformMatrix3x2fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix3x2fv),
            "glUniformMatrix3x2fv",
        );
        bindings.uniformMatrix2x4fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix2x4fv),
            "glUniformMatrix2x4fv",
        );
        bindings.uniformMatrix4x2fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix4x2fv),
            "glUniformMatrix4x2fv",
        );
        bindings.uniformMatrix3x4fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix3x4fv),
            "glUniformMatrix3x4fv",
        );
        bindings.uniformMatrix4x3fv = try getProcAddress(
            @TypeOf(bindings.uniformMatrix4x3fv),
            "glUniformMatrix4x3fv",
        );
        bindings.getBooleani_v = try getProcAddress(@TypeOf(bindings.getBooleani_v), "glGetBooleani_v");
        bindings.getIntegeri_v = try getProcAddress(@TypeOf(bindings.getIntegeri_v), "glGetIntegeri_v");
        bindings.beginTransformFeedback = try getProcAddress(
            @TypeOf(bindings.beginTransformFeedback),
            "glBeginTransformFeedback",
        );
        bindings.endTransformFeedback = try getProcAddress(
            @TypeOf(bindings.endTransformFeedback),
            "glEndTransformFeedback",
        );
        bindings.bindBufferRange = try getProcAddress(@TypeOf(bindings.bindBufferRange), "glBindBufferRange");
        bindings.bindBufferBase = try getProcAddress(@TypeOf(bindings.bindBufferBase), "glBindBufferBase");
        bindings.transformFeedbackVaryings = try getProcAddress(
            @TypeOf(bindings.transformFeedbackVaryings),
            "glTransformFeedbackVaryings",
        );
        bindings.getTransformFeedbackVarying = try getProcAddress(
            @TypeOf(bindings.getTransformFeedbackVarying),
            "glGetTransformFeedbackVarying",
        );
        bindings.vertexAttribIPointer = try getProcAddress(
            @TypeOf(bindings.vertexAttribIPointer),
            "glVertexAttribIPointer",
        );
        bindings.getVertexAttribIiv = try getProcAddress(
            @TypeOf(bindings.getVertexAttribIiv),
            "glGetVertexAttribIiv",
        );
        bindings.getVertexAttribIuiv = try getProcAddress(
            @TypeOf(bindings.getVertexAttribIuiv),
            "glGetVertexAttribIuiv",
        );
        bindings.getUniformuiv = try getProcAddress(@TypeOf(bindings.getUniformuiv), "glGetUniformuiv");
        bindings.getFragDataLocation = try getProcAddress(
            @TypeOf(bindings.getFragDataLocation),
            "glGetFragDataLocation",
        );
        bindings.uniform1ui = try getProcAddress(@TypeOf(bindings.uniform1ui), "glUniform1ui");
        bindings.uniform2ui = try getProcAddress(@TypeOf(bindings.uniform2ui), "glUniform2ui");
        bindings.uniform3ui = try getProcAddress(@TypeOf(bindings.uniform3ui), "glUniform3ui");
        bindings.uniform4ui = try getProcAddress(@TypeOf(bindings.uniform4ui), "glUniform4ui");
        bindings.uniform1uiv = try getProcAddress(@TypeOf(bindings.uniform1uiv), "glUniform1uiv");
        bindings.uniform2uiv = try getProcAddress(@TypeOf(bindings.uniform2uiv), "glUniform2uiv");
        bindings.uniform3uiv = try getProcAddress(@TypeOf(bindings.uniform3uiv), "glUniform3uiv");
        bindings.uniform4uiv = try getProcAddress(@TypeOf(bindings.uniform4uiv), "glUniform4uiv");
        bindings.clearBufferiv = try getProcAddress(@TypeOf(bindings.clearBufferiv), "glClearBufferiv");
        bindings.clearBufferuiv = try getProcAddress(@TypeOf(bindings.clearBufferuiv), "glClearBufferuiv");
        bindings.clearBufferfv = try getProcAddress(@TypeOf(bindings.clearBufferfv), "glClearBufferfv");
        bindings.clearBufferfi = try getProcAddress(@TypeOf(bindings.clearBufferfi), "glClearBufferfi");
        bindings.getStringi = try getProcAddress(@TypeOf(bindings.getStringi), "glGetStringi");
        bindings.blitFramebuffer = try getProcAddress(@TypeOf(bindings.blitFramebuffer), "glBlitFramebuffer");
        bindings.renderbufferStorageMultisample = try getProcAddress(
            @TypeOf(bindings.renderbufferStorageMultisample),
            "glRenderbufferStorageMultisample",
        );
        bindings.framebufferTextureLayer = try getProcAddress(
            @TypeOf(bindings.framebufferTextureLayer),
            "glFramebufferTextureLayer",
        );
        bindings.mapBufferRange = try getProcAddress(@TypeOf(bindings.mapBufferRange), "glMapBufferRange");
        bindings.flushMappedBufferRange = try getProcAddress(
            @TypeOf(bindings.flushMappedBufferRange),
            "glFlushMappedBufferRange",
        );
        bindings.bindVertexArray = try getProcAddress(@TypeOf(bindings.bindVertexArray), "glBindVertexArray");
        bindings.deleteVertexArrays = try getProcAddress(
            @TypeOf(bindings.deleteVertexArrays),
            "glDeleteVertexArrays",
        );
        bindings.genVertexArrays = try getProcAddress(@TypeOf(bindings.genVertexArrays), "glGenVertexArrays");
        bindings.isVertexArray = try getProcAddress(@TypeOf(bindings.isVertexArray), "glIsVertexArray");
        bindings.drawArraysInstanced = try getProcAddress(
            @TypeOf(bindings.drawArraysInstanced),
            "glDrawArraysInstanced",
        );
        bindings.drawElementsInstanced = try getProcAddress(
            @TypeOf(bindings.drawElementsInstanced),
            "glDrawElementsInstanced",
        );
        bindings.copyBufferSubData = try getProcAddress(@TypeOf(bindings.copyBufferSubData), "glCopyBufferSubData");
        bindings.getUniformIndices = try getProcAddress(@TypeOf(bindings.getUniformIndices), "glGetUniformIndices");
        bindings.getActiveUniformsiv = try getProcAddress(
            @TypeOf(bindings.getActiveUniformsiv),
            "glGetActiveUniformsiv",
        );
        bindings.getUniformBlockIndex = try getProcAddress(
            @TypeOf(bindings.getUniformBlockIndex),
            "glGetUniformBlockIndex",
        );
        bindings.getActiveUniformBlockiv = try getProcAddress(
            @TypeOf(bindings.getActiveUniformBlockiv),
            "glGetActiveUniformBlockiv",
        );
        bindings.getActiveUniformBlockName = try getProcAddress(
            @TypeOf(bindings.getActiveUniformBlockName),
            "glGetActiveUniformBlockName",
        );
        bindings.uniformBlockBinding = try getProcAddress(
            @TypeOf(bindings.uniformBlockBinding),
            "glUniformBlockBinding",
        );
        bindings.fenceSync = try getProcAddress(@TypeOf(bindings.fenceSync), "glFenceSync");
        bindings.isSync = try getProcAddress(@TypeOf(bindings.isSync), "glIsSync");
        bindings.deleteSync = try getProcAddress(@TypeOf(bindings.deleteSync), "glDeleteSync");
        bindings.clientWaitSync = try getProcAddress(@TypeOf(bindings.clientWaitSync), "glClientWaitSync");
        bindings.waitSync = try getProcAddress(@TypeOf(bindings.waitSync), "glWaitSync");
        bindings.getInteger64v = try getProcAddress(@TypeOf(bindings.getInteger64v), "glGetInteger64v");
        bindings.getSynciv = try getProcAddress(@TypeOf(bindings.getSynciv), "glGetSynciv");
        bindings.getInteger64i_v = try getProcAddress(@TypeOf(bindings.getInteger64i_v), "glGetInteger64i_v");
        bindings.getBufferParameteri64v = try getProcAddress(
            @TypeOf(bindings.getBufferParameteri64v),
            "glGetBufferParameteri64v",
        );
        bindings.getMultisamplefv = try getProcAddress(@TypeOf(bindings.getMultisamplefv), "glGetMultisamplefv");
        bindings.sampleMaski = try getProcAddress(@TypeOf(bindings.sampleMaski), "glSampleMaski");
        bindings.genSamplers = try getProcAddress(@TypeOf(bindings.genSamplers), "glGenSamplers");
        bindings.deleteSamplers = try getProcAddress(@TypeOf(bindings.deleteSamplers), "glDeleteSamplers");
        bindings.isSampler = try getProcAddress(@TypeOf(bindings.isSampler), "glIsSampler");
        bindings.bindSampler = try getProcAddress(@TypeOf(bindings.bindSampler), "glBindSampler");
        bindings.samplerParameteri = try getProcAddress(@TypeOf(bindings.samplerParameteri), "glSamplerParameteri");
        bindings.samplerParameteriv = try getProcAddress(
            @TypeOf(bindings.samplerParameteriv),
            "glSamplerParameteriv",
        );
        bindings.samplerParameterf = try getProcAddress(@TypeOf(bindings.samplerParameterf), "glSamplerParameterf");
        bindings.samplerParameterfv = try getProcAddress(
            @TypeOf(bindings.samplerParameterfv),
            "glSamplerParameterfv",
        );
        bindings.samplerParameterIiv = try getProcAddress(
            @TypeOf(bindings.samplerParameterIiv),
            "glSamplerParameterIiv",
        );
        bindings.samplerParameterIuiv = try getProcAddress(
            @TypeOf(bindings.samplerParameterIuiv),
            "glSamplerParameterIuiv",
        );
        bindings.getSamplerParameteriv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameteriv),
            "glGetSamplerParameteriv",
        );
        bindings.getSamplerParameterIiv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameterIiv),
            "glGetSamplerParameterIiv",
        );
        bindings.getSamplerParameterfv = try getProcAddress(
            @TypeOf(bindings.getSamplerParameterfv),
            "glGetSamplerParameterfv",
        );
        bindings.vertexAttribDivisor = try getProcAddress(
            @TypeOf(bindings.vertexAttribDivisor),
            "glVertexAttribDivisor",
        );
        // TODO: from opengl 4.0 to 4.3 *subset*
    }
}

pub fn loadExtension(loader: LoaderFn, extension: Extension) !void {
    loaderFunc = loader;

    switch (extension) {
        .KHR_debug => {
            bindings.debugMessageControl = try getProcAddress(
                @TypeOf(bindings.debugMessageControl),
                "glDebugMessageControl",
            );
            bindings.debugMessageInsert = try getProcAddress(
                @TypeOf(bindings.debugMessageInsert),
                "glDebugMessageInsert",
            );
            bindings.debugMessageCallback = try getProcAddress(
                @TypeOf(bindings.debugMessageCallback),
                "glDebugMessageCallback",
            );
            bindings.getDebugMessageLog = try getProcAddress(
                @TypeOf(bindings.getDebugMessageLog),
                "glGetDebugMessageLog",
            );
            bindings.getPointerv = try getProcAddress(
                @TypeOf(bindings.getPointerv),
                "glGetPointerv",
            );
            bindings.pushDebugGroup = try getProcAddress(
                @TypeOf(bindings.pushDebugGroup),
                "glPushDebugGroup",
            );
            bindings.popDebugGroup = try getProcAddress(
                @TypeOf(bindings.popDebugGroup),
                "glPopDebugGroup",
            );
            bindings.objectLabel = try getProcAddress(
                @TypeOf(bindings.objectLabel),
                "glObjectLabel",
            );
            bindings.getObjectLabel = try getProcAddress(
                @TypeOf(bindings.getObjectLabel),
                "glGetObjectLabel",
            );
            bindings.objectPtrLabel = try getProcAddress(
                @TypeOf(bindings.objectPtrLabel),
                "glObjectPtrLabel",
            );
            bindings.getObjectPtrLabel = try getProcAddress(
                @TypeOf(bindings.getObjectPtrLabel),
                "glGetObjectPtrLabel",
            );
        },
        .NV_bindless_texture => {
            bindings.getTextureHandleNV = try getProcAddress(
                @TypeOf(bindings.getTextureHandleNV),
                "glGetTextureHandleNV",
            );
            bindings.makeTextureHandleResidentNV = try getProcAddress(
                @TypeOf(bindings.makeTextureHandleResidentNV),
                "glMakeTextureHandleResidentNV",
            );
            bindings.programUniformHandleui64NV = try getProcAddress(
                @TypeOf(bindings.programUniformHandleui64NV),
                "glProgramUniformHandleui64NV",
            );
        },
        .NV_shader_buffer_load => {
            bindings.makeNamedBufferResidentNV = try getProcAddress(
                @TypeOf(bindings.makeNamedBufferResidentNV),
                "glMakeNamedBufferResidentNV",
            );
            bindings.getNamedBufferParameterui64vNV = try getProcAddress(
                @TypeOf(bindings.getNamedBufferParameterui64vNV),
                "glGetNamedBufferParameterui64vNV",
            );
            bindings.programUniformui64NV = try getProcAddress(
                @TypeOf(bindings.programUniformui64NV),
                "glProgramUniformui64vNV",
            );
        },
    }
}

pub fn loadEsExtension(loader: LoaderFn, extension: EsExtension) !void {
    loaderFunc = loader;

    switch (extension) {
        .KHR_debug => {
            try bind("glDebugMessageControlKHR", .{
                &bindings.debugMessageControl,
                &bindings.debugMessageControlKHR,
            });
            try bind("glDebugMessageInsertKHR", .{
                &bindings.debugMessageInsert,
                &bindings.debugMessageInsertKHR,
            });
            try bind("glDebugMessageCallbackKHR", .{
                &bindings.debugMessageCallback,
                &bindings.debugMessageCallbackKHR,
            });
            try bind("glGetDebugMessageLogKHR", .{
                &bindings.getDebugMessageLog,
                &bindings.getDebugMessageLogKHR,
            });
            try bind("glGetPointervKHR", .{
                &bindings.getPointerv,
                &bindings.getPointervKHR,
            });
            try bind("glPushDebugGroupKHR", .{
                &bindings.pushDebugGroup,
                &bindings.pushDebugGroupKHR,
            });
            try bind("glPopDebugGroupKHR", .{
                &bindings.popDebugGroup,
                &bindings.popDebugGroupKHR,
            });
            try bind("glObjectLabelKHR", .{
                &bindings.objectLabel,
                &bindings.objectLabelKHR,
            });
            try bind("glGetObjectLabelKHR", .{
                &bindings.getObjectLabel,
                &bindings.getObjectLabelKHR,
            });
            try bind("glObjectPtrLabelKHR", .{
                &bindings.objectPtrLabel,
                &bindings.objectPtrLabelKHR,
            });
            try bind("glGetObjectPtrLabelKHR", .{
                &bindings.getObjectPtrLabel,
                &bindings.getObjectPtrLabelKHR,
            });
        },
        .OES_vertex_array_object => {
            try bind("glBindVertexArrayOES", .{
                &bindings.bindVertexArray,
                &bindings.bindVertexArrayOES,
            });
            try bind("glDeleteVertexArraysOES", .{
                &bindings.deleteVertexArrays,
                &bindings.deleteVertexArraysOES,
            });
            try bind("glGenVertexArraysOES", .{
                &bindings.genVertexArrays,
                &bindings.genVertexArraysOES,
            });
            try bind("glIsVertexArrayOES", .{
                &bindings.isVertexArray,
                &bindings.isVertexArrayOES,
            });
        },
    }
}
//--------------------------------------------------------------------------------------------------
fn bind(gl_proc_name: [:0]const u8, bind_addresses: anytype) !void {
    const ProcType = @typeInfo(@TypeOf(bind_addresses.@"0")).Pointer.child;
    const proc = try getProcAddress(ProcType, gl_proc_name);
    inline for (bind_addresses) |bind_addr| {
        if (@typeInfo(@TypeOf(bind_addr)).Pointer.child != ProcType) {
            @compileError("proc bindings should all be the same type");
        }
        bind_addr.* = proc;
    }
}

//--------------------------------------------------------------------------------------------------
var loaderFunc: LoaderFn = undefined;

fn getProcAddress(comptime T: type, name: [:0]const u8) !T {
    if (loaderFunc(name)) |addr| {
        return @as(T, @ptrFromInt(@intFromPtr(addr)));
    }
    std.log.debug("zopengl: {s} not found", .{name});
    return error.OpenGL_FunctionNotFound;
}
//--------------------------------------------------------------------------------------------------
//
// C exports
//
//--------------------------------------------------------------------------------------------------
const linkage: @import("std").builtin.GlobalLinkage = .Strong;
comptime {
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.cullFace, .{ .name = "glCullFace", .linkage = linkage });
    @export(bindings.frontFace, .{ .name = "glFrontFace", .linkage = linkage });
    @export(bindings.hint, .{ .name = "glHint", .linkage = linkage });
    @export(bindings.lineWidth, .{ .name = "glLineWidth", .linkage = linkage });
    @export(bindings.pointSize, .{ .name = "glPointSize", .linkage = linkage });
    @export(bindings.polygonMode, .{ .name = "glPolygonMode", .linkage = linkage });
    @export(bindings.scissor, .{ .name = "glScissor", .linkage = linkage });
    @export(bindings.texParameterf, .{ .name = "glTexParameterf", .linkage = linkage });
    @export(bindings.texParameterfv, .{ .name = "glTexParameterfv", .linkage = linkage });
    @export(bindings.texParameteri, .{ .name = "glTexParameteri", .linkage = linkage });
    @export(bindings.texParameteriv, .{ .name = "glTexParameteriv", .linkage = linkage });
    @export(bindings.texImage1D, .{ .name = "glTexImage1D", .linkage = linkage });
    @export(bindings.texImage2D, .{ .name = "glTexImage2D", .linkage = linkage });
    @export(bindings.drawBuffer, .{ .name = "glDrawBuffer", .linkage = linkage });
    @export(bindings.clear, .{ .name = "glClear", .linkage = linkage });
    @export(bindings.clearColor, .{ .name = "glClearColor", .linkage = linkage });
    @export(bindings.clearStencil, .{ .name = "glClearStencil", .linkage = linkage });
    @export(bindings.stencilMask, .{ .name = "glStencilMask", .linkage = linkage });
    @export(bindings.colorMask, .{ .name = "glColorMask", .linkage = linkage });
    @export(bindings.depthMask, .{ .name = "glDepthMask", .linkage = linkage });
    @export(bindings.disable, .{ .name = "glDisable", .linkage = linkage });
    @export(bindings.enable, .{ .name = "glEnable", .linkage = linkage });
    @export(bindings.finish, .{ .name = "glFinish", .linkage = linkage });
    @export(bindings.flush, .{ .name = "glFlush", .linkage = linkage });
    @export(bindings.blendFunc, .{ .name = "glBlendFunc", .linkage = linkage });
    @export(bindings.logicOp, .{ .name = "glLogicOp", .linkage = linkage });
    @export(bindings.stencilFunc, .{ .name = "glStencilFunc", .linkage = linkage });
    @export(bindings.stencilOp, .{ .name = "glStencilOp", .linkage = linkage });
    @export(bindings.depthFunc, .{ .name = "glDepthFunc", .linkage = linkage });
    @export(bindings.pixelStoref, .{ .name = "glPixelStoref", .linkage = linkage });
    @export(bindings.pixelStorei, .{ .name = "glPixelStorei", .linkage = linkage });
    @export(bindings.readBuffer, .{ .name = "glReadBuffer", .linkage = linkage });
    @export(bindings.readPixels, .{ .name = "glReadPixels", .linkage = linkage });
    @export(bindings.getBooleanv, .{ .name = "glGetBooleanv", .linkage = linkage });
    @export(bindings.getDoublev, .{ .name = "glGetDoublev", .linkage = linkage });
    @export(bindings.getError, .{ .name = "glGetError", .linkage = linkage });
    @export(bindings.getFloatv, .{ .name = "glGetFloatv", .linkage = linkage });
    @export(bindings.getIntegerv, .{ .name = "glGetIntegerv", .linkage = linkage });
    @export(bindings.getString, .{ .name = "glGetString", .linkage = linkage });
    @export(bindings.getTexImage, .{ .name = "glGetTexImage", .linkage = linkage });
    @export(bindings.getTexParameterfv, .{ .name = "glGetTexParameterfv", .linkage = linkage });
    @export(bindings.getTexParameteriv, .{ .name = "glGetTexParameteriv", .linkage = linkage });
    @export(bindings.getTexLevelParameterfv, .{ .name = "glGetTexLevelParameterfv", .linkage = linkage });
    @export(bindings.getTexLevelParameteriv, .{ .name = "glGetTexLevelParameteriv", .linkage = linkage });
    @export(bindings.isEnabled, .{ .name = "glIsEnabled", .linkage = linkage });
    @export(bindings.depthRange, .{ .name = "glDepthRange", .linkage = linkage });
    @export(bindings.viewport, .{ .name = "glViewport", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawArrays, .{ .name = "glDrawArrays", .linkage = linkage });
    @export(bindings.drawElements, .{ .name = "glDrawElements", .linkage = linkage });
    @export(bindings.polygonOffset, .{ .name = "glPolygonOffset", .linkage = linkage });
    @export(bindings.copyTexImage1D, .{ .name = "glCopyTexImage1D", .linkage = linkage });
    @export(bindings.copyTexImage2D, .{ .name = "glCopyTexImage2D", .linkage = linkage });
    @export(bindings.copyTexSubImage1D, .{ .name = "glCopyTexSubImage1D", .linkage = linkage });
    @export(bindings.copyTexSubImage2D, .{ .name = "glCopyTexSubImage2D", .linkage = linkage });
    @export(bindings.texSubImage1D, .{ .name = "glTexSubImage1D", .linkage = linkage });
    @export(bindings.texSubImage2D, .{ .name = "glTexSubImage2D", .linkage = linkage });
    @export(bindings.bindTexture, .{ .name = "glBindTexture", .linkage = linkage });
    @export(bindings.deleteTextures, .{ .name = "glDeleteTextures", .linkage = linkage });
    @export(bindings.genTextures, .{ .name = "glGenTextures", .linkage = linkage });
    @export(bindings.isTexture, .{ .name = "glIsTexture", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.2 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawRangeElements, .{ .name = "glDrawRangeElements", .linkage = linkage });
    @export(bindings.texImage3D, .{ .name = "glTexImage3D", .linkage = linkage });
    @export(bindings.texSubImage3D, .{ .name = "glTexSubImage3D", .linkage = linkage });
    @export(bindings.copyTexSubImage3D, .{ .name = "glCopyTexSubImage3D", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.3 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.activeTexture, .{ .name = "glActiveTexture", .linkage = linkage });
    @export(bindings.sampleCoverage, .{ .name = "glSampleCoverage", .linkage = linkage });
    @export(bindings.compressedTexImage3D, .{ .name = "glCompressedTexImage3D", .linkage = linkage });
    @export(bindings.compressedTexImage2D, .{ .name = "glCompressedTexImage2D", .linkage = linkage });
    @export(bindings.compressedTexImage1D, .{ .name = "glCompressedTexImage1D", .linkage = linkage });
    @export(bindings.compressedTexSubImage3D, .{ .name = "glCompressedTexSubImage3D", .linkage = linkage });
    @export(bindings.compressedTexSubImage2D, .{ .name = "glCompressedTexSubImage2D", .linkage = linkage });
    @export(bindings.compressedTexSubImage1D, .{ .name = "glCompressedTexSubImage1D", .linkage = linkage });
    @export(bindings.getCompressedTexImage, .{ .name = "glGetCompressedTexImage", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.4 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.blendFuncSeparate, .{ .name = "glBlendFuncSeparate", .linkage = linkage });
    @export(bindings.multiDrawArrays, .{ .name = "glMultiDrawArrays", .linkage = linkage });
    @export(bindings.multiDrawElements, .{ .name = "glMultiDrawElements", .linkage = linkage });
    @export(bindings.pointParameterf, .{ .name = "glPointParameterf", .linkage = linkage });
    @export(bindings.pointParameterfv, .{ .name = "glPointParameterfv", .linkage = linkage });
    @export(bindings.pointParameteri, .{ .name = "glPointParameteri", .linkage = linkage });
    @export(bindings.pointParameteriv, .{ .name = "glPointParameteriv", .linkage = linkage });
    @export(bindings.blendColor, .{ .name = "glBlendColor", .linkage = linkage });
    @export(bindings.blendEquation, .{ .name = "glBlendEquation", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.5 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.genQueries, .{ .name = "glGenQueries", .linkage = linkage });
    @export(bindings.deleteQueries, .{ .name = "glDeleteQueries", .linkage = linkage });
    @export(bindings.isQuery, .{ .name = "glIsQuery", .linkage = linkage });
    @export(bindings.beginQuery, .{ .name = "glBeginQuery", .linkage = linkage });
    @export(bindings.endQuery, .{ .name = "glEndQuery", .linkage = linkage });
    @export(bindings.getQueryiv, .{ .name = "glGetQueryiv", .linkage = linkage });
    @export(bindings.getQueryObjectiv, .{ .name = "glGetQueryObjectiv", .linkage = linkage });
    @export(bindings.getQueryObjectuiv, .{ .name = "glGetQueryObjectuiv", .linkage = linkage });
    @export(bindings.bindBuffer, .{ .name = "glBindBuffer", .linkage = linkage });
    @export(bindings.deleteBuffers, .{ .name = "glDeleteBuffers", .linkage = linkage });
    @export(bindings.genBuffers, .{ .name = "glGenBuffers", .linkage = linkage });
    @export(bindings.isBuffer, .{ .name = "glIsBuffer", .linkage = linkage });
    @export(bindings.bufferData, .{ .name = "glBufferData", .linkage = linkage });
    @export(bindings.bufferSubData, .{ .name = "glBufferSubData", .linkage = linkage });
    @export(bindings.getBufferSubData, .{ .name = "glGetBufferSubData", .linkage = linkage });
    @export(bindings.mapBuffer, .{ .name = "glMapBuffer", .linkage = linkage });
    @export(bindings.unmapBuffer, .{ .name = "glUnmapBuffer", .linkage = linkage });
    @export(bindings.getBufferParameteriv, .{ .name = "glGetBufferParameteriv", .linkage = linkage });
    @export(bindings.getBufferPointerv, .{ .name = "glGetBufferPointerv", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 2.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.blendEquationSeparate, .{ .name = "glBlendEquationSeparate", .linkage = linkage });
    @export(bindings.drawBuffers, .{ .name = "glDrawBuffers", .linkage = linkage });
    @export(bindings.stencilOpSeparate, .{ .name = "glStencilOpSeparate", .linkage = linkage });
    @export(bindings.stencilFuncSeparate, .{ .name = "glStencilFuncSeparate", .linkage = linkage });
    @export(bindings.stencilMaskSeparate, .{ .name = "glStencilMaskSeparate", .linkage = linkage });
    @export(bindings.attachShader, .{ .name = "glAttachShader", .linkage = linkage });
    @export(bindings.bindAttribLocation, .{ .name = "glBindAttribLocation", .linkage = linkage });
    @export(bindings.compileShader, .{ .name = "glCompileShader", .linkage = linkage });
    @export(bindings.createProgram, .{ .name = "glCreateProgram", .linkage = linkage });
    @export(bindings.createShader, .{ .name = "glCreateShader", .linkage = linkage });
    @export(bindings.deleteProgram, .{ .name = "glDeleteProgram", .linkage = linkage });
    @export(bindings.deleteShader, .{ .name = "glDeleteShader", .linkage = linkage });
    @export(bindings.detachShader, .{ .name = "glDetachShader", .linkage = linkage });
    @export(bindings.disableVertexAttribArray, .{ .name = "glDisableVertexAttribArray", .linkage = linkage });
    @export(bindings.enableVertexAttribArray, .{ .name = "glEnableVertexAttribArray", .linkage = linkage });
    @export(bindings.getActiveAttrib, .{ .name = "glGetActiveAttrib", .linkage = linkage });
    @export(bindings.getActiveUniform, .{ .name = "glGetActiveUniform", .linkage = linkage });
    @export(bindings.getAttachedShaders, .{ .name = "glGetAttachedShaders", .linkage = linkage });
    @export(bindings.getAttribLocation, .{ .name = "glGetAttribLocation", .linkage = linkage });
    @export(bindings.getProgramiv, .{ .name = "glGetProgramiv", .linkage = linkage });
    @export(bindings.getProgramInfoLog, .{ .name = "glGetProgramInfoLog", .linkage = linkage });
    @export(bindings.getShaderiv, .{ .name = "glGetShaderiv", .linkage = linkage });
    @export(bindings.getShaderInfoLog, .{ .name = "glGetShaderInfoLog", .linkage = linkage });
    @export(bindings.getShaderSource, .{ .name = "glGetShaderSource", .linkage = linkage });
    @export(bindings.getUniformLocation, .{ .name = "glGetUniformLocation", .linkage = linkage });
    @export(bindings.getUniformfv, .{ .name = "glGetUniformfv", .linkage = linkage });
    @export(bindings.getUniformiv, .{ .name = "glGetUniformiv", .linkage = linkage });
    @export(bindings.getVertexAttribdv, .{ .name = "glGetVertexAttribdv", .linkage = linkage });
    @export(bindings.getVertexAttribfv, .{ .name = "glGetVertexAttribfv", .linkage = linkage });
    @export(bindings.getVertexAttribiv, .{ .name = "glGetVertexAttribiv", .linkage = linkage });
    @export(bindings.getVertexAttribPointerv, .{ .name = "glGetVertexAttribPointerv", .linkage = linkage });
    @export(bindings.isProgram, .{ .name = "glIsProgram", .linkage = linkage });
    @export(bindings.isShader, .{ .name = "glIsShader", .linkage = linkage });
    @export(bindings.linkProgram, .{ .name = "glLinkProgram", .linkage = linkage });
    @export(bindings.shaderSource, .{ .name = "glShaderSource", .linkage = linkage });
    @export(bindings.useProgram, .{ .name = "glUseProgram", .linkage = linkage });
    @export(bindings.uniform1f, .{ .name = "glUniform1f", .linkage = linkage });
    @export(bindings.uniform2f, .{ .name = "glUniform2f", .linkage = linkage });
    @export(bindings.uniform3f, .{ .name = "glUniform3f", .linkage = linkage });
    @export(bindings.uniform4f, .{ .name = "glUniform4f", .linkage = linkage });
    @export(bindings.uniform1i, .{ .name = "glUniform1i", .linkage = linkage });
    @export(bindings.uniform2i, .{ .name = "glUniform2i", .linkage = linkage });
    @export(bindings.uniform3i, .{ .name = "glUniform3i", .linkage = linkage });
    @export(bindings.uniform4i, .{ .name = "glUniform4i", .linkage = linkage });
    @export(bindings.uniform1fv, .{ .name = "glUniform1fv", .linkage = linkage });
    @export(bindings.uniform2fv, .{ .name = "glUniform2fv", .linkage = linkage });
    @export(bindings.uniform3fv, .{ .name = "glUniform3fv", .linkage = linkage });
    @export(bindings.uniform4fv, .{ .name = "glUniform4fv", .linkage = linkage });
    @export(bindings.uniform1iv, .{ .name = "glUniform1iv", .linkage = linkage });
    @export(bindings.uniform2iv, .{ .name = "glUniform2iv", .linkage = linkage });
    @export(bindings.uniform3iv, .{ .name = "glUniform3iv", .linkage = linkage });
    @export(bindings.uniform4iv, .{ .name = "glUniform4iv", .linkage = linkage });
    @export(bindings.uniformMatrix2fv, .{ .name = "glUniformMatrix2fv", .linkage = linkage });
    @export(bindings.uniformMatrix3fv, .{ .name = "glUniformMatrix3fv", .linkage = linkage });
    @export(bindings.uniformMatrix4fv, .{ .name = "glUniformMatrix4fv", .linkage = linkage });
    @export(bindings.validateProgram, .{ .name = "glValidateProgram", .linkage = linkage });
    @export(bindings.vertexAttrib1d, .{ .name = "glVertexAttrib1d", .linkage = linkage });
    @export(bindings.vertexAttrib1dv, .{ .name = "glVertexAttrib1dv", .linkage = linkage });
    @export(bindings.vertexAttrib1f, .{ .name = "glVertexAttrib1f", .linkage = linkage });
    @export(bindings.vertexAttrib1fv, .{ .name = "glVertexAttrib1fv", .linkage = linkage });
    @export(bindings.vertexAttrib1s, .{ .name = "glVertexAttrib1s", .linkage = linkage });
    @export(bindings.vertexAttrib1sv, .{ .name = "glVertexAttrib1sv", .linkage = linkage });
    @export(bindings.vertexAttrib2d, .{ .name = "glVertexAttrib2d", .linkage = linkage });
    @export(bindings.vertexAttrib2dv, .{ .name = "glVertexAttrib2dv", .linkage = linkage });
    @export(bindings.vertexAttrib2f, .{ .name = "glVertexAttrib2f", .linkage = linkage });
    @export(bindings.vertexAttrib2fv, .{ .name = "glVertexAttrib2fv", .linkage = linkage });
    @export(bindings.vertexAttrib2s, .{ .name = "glVertexAttrib2s", .linkage = linkage });
    @export(bindings.vertexAttrib2sv, .{ .name = "glVertexAttrib2sv", .linkage = linkage });
    @export(bindings.vertexAttrib3d, .{ .name = "glVertexAttrib3d", .linkage = linkage });
    @export(bindings.vertexAttrib3dv, .{ .name = "glVertexAttrib3dv", .linkage = linkage });
    @export(bindings.vertexAttrib3f, .{ .name = "glVertexAttrib3f", .linkage = linkage });
    @export(bindings.vertexAttrib3fv, .{ .name = "glVertexAttrib3fv", .linkage = linkage });
    @export(bindings.vertexAttrib3s, .{ .name = "glVertexAttrib3s", .linkage = linkage });
    @export(bindings.vertexAttrib3sv, .{ .name = "glVertexAttrib3sv", .linkage = linkage });
    @export(bindings.vertexAttrib4Nbv, .{ .name = "glVertexAttrib4Nbv", .linkage = linkage });
    @export(bindings.vertexAttrib4Niv, .{ .name = "glVertexAttrib4Niv", .linkage = linkage });
    @export(bindings.vertexAttrib4Nsv, .{ .name = "glVertexAttrib4Nsv", .linkage = linkage });
    @export(bindings.vertexAttrib4Nub, .{ .name = "glVertexAttrib4Nub", .linkage = linkage });
    @export(bindings.vertexAttrib4Nubv, .{ .name = "glVertexAttrib4Nubv", .linkage = linkage });
    @export(bindings.vertexAttrib4Nuiv, .{ .name = "glVertexAttrib4Nuiv", .linkage = linkage });
    @export(bindings.vertexAttrib4Nusv, .{ .name = "glVertexAttrib4Nusv", .linkage = linkage });
    @export(bindings.vertexAttrib4bv, .{ .name = "glVertexAttrib4bv", .linkage = linkage });
    @export(bindings.vertexAttrib4d, .{ .name = "glVertexAttrib4d", .linkage = linkage });
    @export(bindings.vertexAttrib4dv, .{ .name = "glVertexAttrib4dv", .linkage = linkage });
    @export(bindings.vertexAttrib4f, .{ .name = "glVertexAttrib4f", .linkage = linkage });
    @export(bindings.vertexAttrib4fv, .{ .name = "glVertexAttrib4fv", .linkage = linkage });
    @export(bindings.vertexAttrib4iv, .{ .name = "glVertexAttrib4iv", .linkage = linkage });
    @export(bindings.vertexAttrib4s, .{ .name = "glVertexAttrib4s", .linkage = linkage });
    @export(bindings.vertexAttrib4sv, .{ .name = "glVertexAttrib4sv", .linkage = linkage });
    @export(bindings.vertexAttrib4ubv, .{ .name = "glVertexAttrib4ubv", .linkage = linkage });
    @export(bindings.vertexAttrib4uiv, .{ .name = "glVertexAttrib4uiv", .linkage = linkage });
    @export(bindings.vertexAttrib4usv, .{ .name = "glVertexAttrib4usv", .linkage = linkage });
    @export(bindings.vertexAttribPointer, .{ .name = "glVertexAttribPointer", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 2.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.uniformMatrix2x3fv, .{ .name = "glUniformMatrix2x3fv", .linkage = linkage });
    @export(bindings.uniformMatrix3x2fv, .{ .name = "glUniformMatrix3x2fv", .linkage = linkage });
    @export(bindings.uniformMatrix2x4fv, .{ .name = "glUniformMatrix2x4fv", .linkage = linkage });
    @export(bindings.uniformMatrix4x2fv, .{ .name = "glUniformMatrix4x2fv", .linkage = linkage });
    @export(bindings.uniformMatrix3x4fv, .{ .name = "glUniformMatrix3x4fv", .linkage = linkage });
    @export(bindings.uniformMatrix4x3fv, .{ .name = "glUniformMatrix4x3fv", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.colorMaski, .{ .name = "glColorMaski", .linkage = linkage });
    @export(bindings.getBooleani_v, .{ .name = "glGetBooleani_v", .linkage = linkage });
    @export(bindings.getIntegeri_v, .{ .name = "glGetIntegeri_v", .linkage = linkage });
    @export(bindings.enablei, .{ .name = "glEnablei", .linkage = linkage });
    @export(bindings.disablei, .{ .name = "glDisablei", .linkage = linkage });
    @export(bindings.isEnabledi, .{ .name = "glIsEnabledi", .linkage = linkage });
    @export(bindings.beginTransformFeedback, .{ .name = "glBeginTransformFeedback", .linkage = linkage });
    @export(bindings.endTransformFeedback, .{ .name = "glEndTransformFeedback", .linkage = linkage });
    @export(bindings.bindBufferRange, .{ .name = "glBindBufferRange", .linkage = linkage });
    @export(bindings.bindBufferBase, .{ .name = "glBindBufferBase", .linkage = linkage });
    @export(bindings.transformFeedbackVaryings, .{ .name = "glTransformFeedbackVaryings", .linkage = linkage });
    @export(bindings.getTransformFeedbackVarying, .{ .name = "glGetTransformFeedbackVarying", .linkage = linkage });
    @export(bindings.beginConditionalRender, .{ .name = "glBeginConditionalRender", .linkage = linkage });
    @export(bindings.endConditionalRender, .{ .name = "glEndConditionalRender", .linkage = linkage });
    @export(bindings.vertexAttribIPointer, .{ .name = "glVertexAttribIPointer", .linkage = linkage });
    @export(bindings.getVertexAttribIiv, .{ .name = "glGetVertexAttribIiv", .linkage = linkage });
    @export(bindings.getVertexAttribIuiv, .{ .name = "glGetVertexAttribIuiv", .linkage = linkage });
    @export(bindings.vertexAttribI1i, .{ .name = "glVertexAttribI1i", .linkage = linkage });
    @export(bindings.vertexAttribI2i, .{ .name = "glVertexAttribI2i", .linkage = linkage });
    @export(bindings.vertexAttribI3i, .{ .name = "glVertexAttribI3i", .linkage = linkage });
    @export(bindings.vertexAttribI4i, .{ .name = "glVertexAttribI4i", .linkage = linkage });
    @export(bindings.vertexAttribI1ui, .{ .name = "glVertexAttribI1ui", .linkage = linkage });
    @export(bindings.vertexAttribI2ui, .{ .name = "glVertexAttribI2ui", .linkage = linkage });
    @export(bindings.vertexAttribI3ui, .{ .name = "glVertexAttribI3ui", .linkage = linkage });
    @export(bindings.vertexAttribI4ui, .{ .name = "glVertexAttribI4ui", .linkage = linkage });
    @export(bindings.vertexAttribI1iv, .{ .name = "glVertexAttribI1iv", .linkage = linkage });
    @export(bindings.vertexAttribI2iv, .{ .name = "glVertexAttribI2iv", .linkage = linkage });
    @export(bindings.vertexAttribI3iv, .{ .name = "glVertexAttribI3iv", .linkage = linkage });
    @export(bindings.vertexAttribI4iv, .{ .name = "glVertexAttribI4iv", .linkage = linkage });
    @export(bindings.vertexAttribI1uiv, .{ .name = "glVertexAttribI1uiv", .linkage = linkage });
    @export(bindings.vertexAttribI2uiv, .{ .name = "glVertexAttribI2uiv", .linkage = linkage });
    @export(bindings.vertexAttribI3uiv, .{ .name = "glVertexAttribI3uiv", .linkage = linkage });
    @export(bindings.vertexAttribI4uiv, .{ .name = "glVertexAttribI4uiv", .linkage = linkage });
    @export(bindings.vertexAttribI4bv, .{ .name = "glVertexAttribI4bv", .linkage = linkage });
    @export(bindings.vertexAttribI4sv, .{ .name = "glVertexAttribI4sv", .linkage = linkage });
    @export(bindings.vertexAttribI4ubv, .{ .name = "glVertexAttribI4ubv", .linkage = linkage });
    @export(bindings.vertexAttribI4usv, .{ .name = "glVertexAttribI4usv", .linkage = linkage });
    @export(bindings.getUniformuiv, .{ .name = "glGetUniformuiv", .linkage = linkage });
    @export(bindings.bindFragDataLocation, .{ .name = "glBindFragDataLocation", .linkage = linkage });
    @export(bindings.getFragDataLocation, .{ .name = "glGetFragDataLocation", .linkage = linkage });
    @export(bindings.uniform1ui, .{ .name = "glUniform1ui", .linkage = linkage });
    @export(bindings.uniform2ui, .{ .name = "glUniform2ui", .linkage = linkage });
    @export(bindings.uniform3ui, .{ .name = "glUniform3ui", .linkage = linkage });
    @export(bindings.uniform4ui, .{ .name = "glUniform4ui", .linkage = linkage });
    @export(bindings.uniform1uiv, .{ .name = "glUniform1uiv", .linkage = linkage });
    @export(bindings.uniform2uiv, .{ .name = "glUniform2uiv", .linkage = linkage });
    @export(bindings.uniform3uiv, .{ .name = "glUniform3uiv", .linkage = linkage });
    @export(bindings.uniform4uiv, .{ .name = "glUniform4uiv", .linkage = linkage });
    @export(bindings.texParameterIiv, .{ .name = "glTexParameterIiv", .linkage = linkage });
    @export(bindings.texParameterIuiv, .{ .name = "glTexParameterIuiv", .linkage = linkage });
    @export(bindings.getTexParameterIiv, .{ .name = "glGetTexParameterIiv", .linkage = linkage });
    @export(bindings.getTexParameterIuiv, .{ .name = "glGetTexParameterIuiv", .linkage = linkage });
    @export(bindings.clearBufferiv, .{ .name = "glClearBufferiv", .linkage = linkage });
    @export(bindings.clearBufferuiv, .{ .name = "glClearBufferuiv", .linkage = linkage });
    @export(bindings.clearBufferfv, .{ .name = "glClearBufferfv", .linkage = linkage });
    @export(bindings.clearBufferfi, .{ .name = "glClearBufferfi", .linkage = linkage });
    @export(bindings.getStringi, .{ .name = "glGetStringi", .linkage = linkage });
    @export(bindings.isRenderbuffer, .{ .name = "glIsRenderbuffer", .linkage = linkage });
    @export(bindings.bindRenderbuffer, .{ .name = "glBindRenderbuffer", .linkage = linkage });
    @export(bindings.deleteRenderbuffers, .{ .name = "glDeleteRenderbuffers", .linkage = linkage });
    @export(bindings.genRenderbuffers, .{ .name = "glGenRenderbuffers", .linkage = linkage });
    @export(bindings.renderbufferStorage, .{ .name = "glRenderbufferStorage", .linkage = linkage });
    @export(bindings.getRenderbufferParameteriv, .{ .name = "glGetRenderbufferParameteriv", .linkage = linkage });
    @export(bindings.isFramebuffer, .{ .name = "glIsFramebuffer", .linkage = linkage });
    @export(bindings.bindFramebuffer, .{ .name = "glBindFramebuffer", .linkage = linkage });
    @export(bindings.deleteFramebuffers, .{ .name = "glDeleteFramebuffers", .linkage = linkage });
    @export(bindings.genFramebuffers, .{ .name = "glGenFramebuffers", .linkage = linkage });
    @export(bindings.checkFramebufferStatus, .{ .name = "glCheckFramebufferStatus", .linkage = linkage });
    @export(bindings.framebufferTexture1D, .{ .name = "glFramebufferTexture1D", .linkage = linkage });
    @export(bindings.framebufferTexture2D, .{ .name = "glFramebufferTexture2D", .linkage = linkage });
    @export(bindings.framebufferTexture3D, .{ .name = "glFramebufferTexture3D", .linkage = linkage });
    @export(bindings.framebufferRenderbuffer, .{ .name = "glFramebufferRenderbuffer", .linkage = linkage });
    @export(
        bindings.getFramebufferAttachmentParameteriv,
        .{ .name = "glGetFramebufferAttachmentParameteriv", .linkage = linkage },
    );
    @export(bindings.generateMipmap, .{ .name = "glGenerateMipmap", .linkage = linkage });
    @export(bindings.blitFramebuffer, .{ .name = "glBlitFramebuffer", .linkage = linkage });
    @export(
        bindings.renderbufferStorageMultisample,
        .{ .name = "glRenderbufferStorageMultisample", .linkage = linkage },
    );
    @export(bindings.framebufferTextureLayer, .{ .name = "glFramebufferTextureLayer", .linkage = linkage });
    @export(bindings.mapBufferRange, .{ .name = "glMapBufferRange", .linkage = linkage });
    @export(bindings.flushMappedBufferRange, .{ .name = "glFlushMappedBufferRange", .linkage = linkage });
    @export(bindings.bindVertexArray, .{ .name = "glBindVertexArray", .linkage = linkage });
    @export(bindings.deleteVertexArrays, .{ .name = "glDeleteVertexArrays", .linkage = linkage });
    @export(bindings.genVertexArrays, .{ .name = "glGenVertexArrays", .linkage = linkage });
    @export(bindings.isVertexArray, .{ .name = "glIsVertexArray", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawArraysInstanced, .{ .name = "glDrawArraysInstanced", .linkage = linkage });
    @export(bindings.drawElementsInstanced, .{ .name = "glDrawElementsInstanced", .linkage = linkage });
    @export(bindings.texBuffer, .{ .name = "glTexBuffer", .linkage = linkage });
    @export(bindings.primitiveRestartIndex, .{ .name = "glPrimitiveRestartIndex", .linkage = linkage });
    @export(bindings.copyBufferSubData, .{ .name = "glCopyBufferSubData", .linkage = linkage });
    @export(bindings.getUniformIndices, .{ .name = "glGetUniformIndices", .linkage = linkage });
    @export(bindings.getActiveUniformsiv, .{ .name = "glGetActiveUniformsiv", .linkage = linkage });
    @export(bindings.getActiveUniformName, .{ .name = "glGetActiveUniformName", .linkage = linkage });
    @export(bindings.getUniformBlockIndex, .{ .name = "glGetUniformBlockIndex", .linkage = linkage });
    @export(bindings.getActiveUniformBlockiv, .{ .name = "glGetActiveUniformBlockiv", .linkage = linkage });
    @export(bindings.getActiveUniformBlockName, .{ .name = "glGetActiveUniformBlockName", .linkage = linkage });
    @export(bindings.uniformBlockBinding, .{ .name = "glUniformBlockBinding", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.2 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawElementsBaseVertex, .{ .name = "glDrawElementsBaseVertex", .linkage = linkage });
    @export(bindings.drawRangeElementsBaseVertex, .{ .name = "glDrawRangeElementsBaseVertex", .linkage = linkage });
    @export(
        bindings.drawElementsInstancedBaseVertex,
        .{ .name = "glDrawElementsInstancedBaseVertex", .linkage = linkage },
    );
    @export(bindings.multiDrawElementsBaseVertex, .{ .name = "glMultiDrawElementsBaseVertex", .linkage = linkage });
    @export(bindings.provokingVertex, .{ .name = "glProvokingVertex", .linkage = linkage });
    @export(bindings.fenceSync, .{ .name = "glFenceSync", .linkage = linkage });
    @export(bindings.isSync, .{ .name = "glIsSync", .linkage = linkage });
    @export(bindings.deleteSync, .{ .name = "glDeleteSync", .linkage = linkage });
    @export(bindings.clientWaitSync, .{ .name = "glClientWaitSync", .linkage = linkage });
    @export(bindings.waitSync, .{ .name = "glWaitSync", .linkage = linkage });
    @export(bindings.getInteger64v, .{ .name = "glGetInteger64v", .linkage = linkage });
    @export(bindings.getSynciv, .{ .name = "glGetSynciv", .linkage = linkage });
    @export(bindings.getInteger64i_v, .{ .name = "glGetInteger64i_v", .linkage = linkage });
    @export(bindings.getBufferParameteri64v, .{ .name = "glGetBufferParameteri64v", .linkage = linkage });
    @export(bindings.framebufferTexture, .{ .name = "glFramebufferTexture", .linkage = linkage });
    @export(bindings.texImage2DMultisample, .{ .name = "glTexImage2DMultisample", .linkage = linkage });
    @export(bindings.texImage3DMultisample, .{ .name = "glTexImage3DMultisample", .linkage = linkage });
    @export(bindings.getMultisamplefv, .{ .name = "glGetMultisamplefv", .linkage = linkage });
    @export(bindings.sampleMaski, .{ .name = "glSampleMaski", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.3 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.bindFragDataLocationIndexed, .{ .name = "glBindFragDataLocationIndexed", .linkage = linkage });
    @export(bindings.getFragDataIndex, .{ .name = "glGetFragDataIndex", .linkage = linkage });
    @export(bindings.genSamplers, .{ .name = "glGenSamplers", .linkage = linkage });
    @export(bindings.deleteSamplers, .{ .name = "glDeleteSamplers", .linkage = linkage });
    @export(bindings.isSampler, .{ .name = "glIsSampler", .linkage = linkage });
    @export(bindings.bindSampler, .{ .name = "glBindSampler", .linkage = linkage });
    @export(bindings.samplerParameteri, .{ .name = "glSamplerParameteri", .linkage = linkage });
    @export(bindings.samplerParameteriv, .{ .name = "glSamplerParameteriv", .linkage = linkage });
    @export(bindings.samplerParameterf, .{ .name = "glSamplerParameterf", .linkage = linkage });
    @export(bindings.samplerParameterfv, .{ .name = "glSamplerParameterfv", .linkage = linkage });
    @export(bindings.samplerParameterIiv, .{ .name = "glSamplerParameterIiv", .linkage = linkage });
    @export(bindings.samplerParameterIuiv, .{ .name = "glSamplerParameterIuiv", .linkage = linkage });
    @export(bindings.getSamplerParameteriv, .{ .name = "glGetSamplerParameteriv", .linkage = linkage });
    @export(bindings.getSamplerParameterIiv, .{ .name = "glGetSamplerParameterIiv", .linkage = linkage });
    @export(bindings.getSamplerParameterfv, .{ .name = "glGetSamplerParameterfv", .linkage = linkage });
    @export(bindings.getSamplerParameterIuiv, .{ .name = "glGetSamplerParameterIuiv", .linkage = linkage });
    @export(bindings.queryCounter, .{ .name = "glQueryCounter", .linkage = linkage });
    @export(bindings.getQueryObjecti64v, .{ .name = "glGetQueryObjecti64v", .linkage = linkage });
    @export(bindings.getQueryObjectui64v, .{ .name = "glGetQueryObjectui64v", .linkage = linkage });
    @export(bindings.vertexAttribDivisor, .{ .name = "glVertexAttribDivisor", .linkage = linkage });
    @export(bindings.vertexAttribP1ui, .{ .name = "glVertexAttribP1ui", .linkage = linkage });
    @export(bindings.vertexAttribP1uiv, .{ .name = "glVertexAttribP1uiv", .linkage = linkage });
    @export(bindings.vertexAttribP2ui, .{ .name = "glVertexAttribP2ui", .linkage = linkage });
    @export(bindings.vertexAttribP2uiv, .{ .name = "glVertexAttribP2uiv", .linkage = linkage });
    @export(bindings.vertexAttribP3ui, .{ .name = "glVertexAttribP3ui", .linkage = linkage });
    @export(bindings.vertexAttribP3uiv, .{ .name = "glVertexAttribP3uiv", .linkage = linkage });
    @export(bindings.vertexAttribP4ui, .{ .name = "glVertexAttribP4ui", .linkage = linkage });
    @export(bindings.vertexAttribP4uiv, .{ .name = "glVertexAttribP4uiv", .linkage = linkage });
}

test {
    @setEvalBranchQuota(100_000);
    _ = testing.refAllDecls(@This());
}
