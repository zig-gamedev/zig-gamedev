const std = @import("std");
const assert = std.debug.assert;

const options = @import("zopengl_options");

const bindings = @import("bindings.zig");

pub usingnamespace switch (options.api) {
    .raw => bindings,
    .wrapper => @import("wrapper.zig"),
};
//--------------------------------------------------------------------------------------------------
//
// Functions for loading OpenGL function pointers
//
//--------------------------------------------------------------------------------------------------
pub const LoaderFn = *const fn ([:0]const u8) ?*const anyopaque;
pub const Extension = enum {
    OES_vertex_array_object,
};
pub fn loadCoreProfile(loader: LoaderFn, major: u32, minor: u32) !void {
    const ver = 10 * major + minor;

    // Max. supported version is 3.3 for now.
    assert(major >= 1 and major <= 3);
    assert(minor >= 0 and minor <= 5);
    assert(ver >= 10 and ver <= 33);

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
}

pub fn loadEsProfile(loader: LoaderFn, major: u32, minor: u32) !void {
    const ver = 10 * major + minor;

    // Max. supported version is ES 2.0 for now.
    assert(major >= 1 and major <= 2);
    assert(minor >= 0 and minor <= 1);
    assert(ver >= 10 and ver <= 20);

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
    }
}

pub fn loadExtension(loader: LoaderFn, extension: Extension) !void {
    loaderFunc = loader;

    switch (extension) {
        .OES_vertex_array_object => {
            bindings.bindVertexArrayOES = try getProcAddress(
                @TypeOf(bindings.bindVertexArrayOES),
                "glBindVertexArrayOES",
            );
            bindings.deleteVertexArraysOES = try getProcAddress(
                @TypeOf(bindings.deleteVertexArraysOES),
                "glDeleteVertexArraysOES",
            );
            bindings.genVertexArraysOES = try getProcAddress(
                @TypeOf(bindings.genVertexArraysOES),
                "glGenVertexArraysOES",
            );
            bindings.isVertexArrayOES = try getProcAddress(
                @TypeOf(bindings.isVertexArrayOES),
                "glIsVertexArrayOES",
            );
        },
    }
}

//--------------------------------------------------------------------------------------------------
var loaderFunc: LoaderFn = undefined;

fn getProcAddress(comptime T: type, name: [:0]const u8) !T {
    if (loaderFunc(name)) |addr| {
        return @intToPtr(T, @ptrToInt(addr));
    }
    std.log.debug("zopengl: {s} not found", .{name});
    return error.OpenGL_FunctionNotFound;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//
// C exports
//
//--------------------------------------------------------------------------------------------------
comptime {
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.cullFace, .{ .name = "glCullFace", .linkage = .Strong });
    @export(bindings.frontFace, .{ .name = "glFrontFace", .linkage = .Strong });
    @export(bindings.hint, .{ .name = "glHint", .linkage = .Strong });
    @export(bindings.lineWidth, .{ .name = "glLineWidth", .linkage = .Strong });
    @export(bindings.pointSize, .{ .name = "glPointSize", .linkage = .Strong });
    @export(bindings.polygonMode, .{ .name = "glPolygonMode", .linkage = .Strong });
    @export(bindings.scissor, .{ .name = "glScissor", .linkage = .Strong });
    @export(bindings.texParameterf, .{ .name = "glTexParameterf", .linkage = .Strong });
    @export(bindings.texParameterfv, .{ .name = "glTexParameterfv", .linkage = .Strong });
    @export(bindings.texParameteri, .{ .name = "glTexParameteri", .linkage = .Strong });
    @export(bindings.texParameteriv, .{ .name = "glTexParameteriv", .linkage = .Strong });
    @export(bindings.texImage1D, .{ .name = "glTexImage1D", .linkage = .Strong });
    @export(bindings.texImage2D, .{ .name = "glTexImage2D", .linkage = .Strong });
    @export(bindings.drawBuffer, .{ .name = "glDrawBuffer", .linkage = .Strong });
    @export(bindings.clear, .{ .name = "glClear", .linkage = .Strong });
    @export(bindings.clearColor, .{ .name = "glClearColor", .linkage = .Strong });
    @export(bindings.clearStencil, .{ .name = "glClearStencil", .linkage = .Strong });
    @export(bindings.stencilMask, .{ .name = "glStencilMask", .linkage = .Strong });
    @export(bindings.colorMask, .{ .name = "glColorMask", .linkage = .Strong });
    @export(bindings.depthMask, .{ .name = "glDepthMask", .linkage = .Strong });
    @export(bindings.disable, .{ .name = "glDisable", .linkage = .Strong });
    @export(bindings.enable, .{ .name = "glEnable", .linkage = .Strong });
    @export(bindings.finish, .{ .name = "glFinish", .linkage = .Strong });
    @export(bindings.flush, .{ .name = "glFlush", .linkage = .Strong });
    @export(bindings.blendFunc, .{ .name = "glBlendFunc", .linkage = .Strong });
    @export(bindings.logicOp, .{ .name = "glLogicOp", .linkage = .Strong });
    @export(bindings.stencilFunc, .{ .name = "glStencilFunc", .linkage = .Strong });
    @export(bindings.stencilOp, .{ .name = "glStencilOp", .linkage = .Strong });
    @export(bindings.depthFunc, .{ .name = "glDepthFunc", .linkage = .Strong });
    @export(bindings.pixelStoref, .{ .name = "glPixelStoref", .linkage = .Strong });
    @export(bindings.pixelStorei, .{ .name = "glPixelStorei", .linkage = .Strong });
    @export(bindings.readBuffer, .{ .name = "glReadBuffer", .linkage = .Strong });
    @export(bindings.readPixels, .{ .name = "glReadPixels", .linkage = .Strong });
    @export(bindings.getBooleanv, .{ .name = "glGetBooleanv", .linkage = .Strong });
    @export(bindings.getDoublev, .{ .name = "glGetDoublev", .linkage = .Strong });
    @export(bindings.getError, .{ .name = "glGetError", .linkage = .Strong });
    @export(bindings.getFloatv, .{ .name = "glGetFloatv", .linkage = .Strong });
    @export(bindings.getIntegerv, .{ .name = "glGetIntegerv", .linkage = .Strong });
    @export(bindings.getString, .{ .name = "glGetString", .linkage = .Strong });
    @export(bindings.getTexImage, .{ .name = "glGetTexImage", .linkage = .Strong });
    @export(bindings.getTexParameterfv, .{ .name = "glGetTexParameterfv", .linkage = .Strong });
    @export(bindings.getTexParameteriv, .{ .name = "glGetTexParameteriv", .linkage = .Strong });
    @export(bindings.getTexLevelParameterfv, .{ .name = "glGetTexLevelParameterfv", .linkage = .Strong });
    @export(bindings.getTexLevelParameteriv, .{ .name = "glGetTexLevelParameteriv", .linkage = .Strong });
    @export(bindings.isEnabled, .{ .name = "glIsEnabled", .linkage = .Strong });
    @export(bindings.depthRange, .{ .name = "glDepthRange", .linkage = .Strong });
    @export(bindings.viewport, .{ .name = "glViewport", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawArrays, .{ .name = "glDrawArrays", .linkage = .Strong });
    @export(bindings.drawElements, .{ .name = "glDrawElements", .linkage = .Strong });
    @export(bindings.polygonOffset, .{ .name = "glPolygonOffset", .linkage = .Strong });
    @export(bindings.copyTexImage1D, .{ .name = "glCopyTexImage1D", .linkage = .Strong });
    @export(bindings.copyTexImage2D, .{ .name = "glCopyTexImage2D", .linkage = .Strong });
    @export(bindings.copyTexSubImage1D, .{ .name = "glCopyTexSubImage1D", .linkage = .Strong });
    @export(bindings.copyTexSubImage2D, .{ .name = "glCopyTexSubImage2D", .linkage = .Strong });
    @export(bindings.texSubImage1D, .{ .name = "glTexSubImage1D", .linkage = .Strong });
    @export(bindings.texSubImage2D, .{ .name = "glTexSubImage2D", .linkage = .Strong });
    @export(bindings.bindTexture, .{ .name = "glBindTexture", .linkage = .Strong });
    @export(bindings.deleteTextures, .{ .name = "glDeleteTextures", .linkage = .Strong });
    @export(bindings.genTextures, .{ .name = "glGenTextures", .linkage = .Strong });
    @export(bindings.isTexture, .{ .name = "glIsTexture", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.2 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawRangeElements, .{ .name = "glDrawRangeElements", .linkage = .Strong });
    @export(bindings.texImage3D, .{ .name = "glTexImage3D", .linkage = .Strong });
    @export(bindings.texSubImage3D, .{ .name = "glTexSubImage3D", .linkage = .Strong });
    @export(bindings.copyTexSubImage3D, .{ .name = "glCopyTexSubImage3D", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.3 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.activeTexture, .{ .name = "glActiveTexture", .linkage = .Strong });
    @export(bindings.sampleCoverage, .{ .name = "glSampleCoverage", .linkage = .Strong });
    @export(bindings.compressedTexImage3D, .{ .name = "glCompressedTexImage3D", .linkage = .Strong });
    @export(bindings.compressedTexImage2D, .{ .name = "glCompressedTexImage2D", .linkage = .Strong });
    @export(bindings.compressedTexImage1D, .{ .name = "glCompressedTexImage1D", .linkage = .Strong });
    @export(bindings.compressedTexSubImage3D, .{ .name = "glCompressedTexSubImage3D", .linkage = .Strong });
    @export(bindings.compressedTexSubImage2D, .{ .name = "glCompressedTexSubImage2D", .linkage = .Strong });
    @export(bindings.compressedTexSubImage1D, .{ .name = "glCompressedTexSubImage1D", .linkage = .Strong });
    @export(bindings.getCompressedTexImage, .{ .name = "glGetCompressedTexImage", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.4 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.pointParameterf, .{ .name = "glPointParameterf", .linkage = .Strong });
    @export(bindings.pointParameterfv, .{ .name = "glPointParameterfv", .linkage = .Strong });
    @export(bindings.pointParameteri, .{ .name = "glPointParameteri", .linkage = .Strong });
    @export(bindings.pointParameteriv, .{ .name = "glPointParameteriv", .linkage = .Strong });
    @export(bindings.blendColor, .{ .name = "glBlendColor", .linkage = .Strong });
    @export(bindings.blendEquation, .{ .name = "glBlendEquation", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 1.5 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.genQueries, .{ .name = "glGenQueries", .linkage = .Strong });
    @export(bindings.deleteQueries, .{ .name = "glDeleteQueries", .linkage = .Strong });
    @export(bindings.isQuery, .{ .name = "glIsQuery", .linkage = .Strong });
    @export(bindings.beginQuery, .{ .name = "glBeginQuery", .linkage = .Strong });
    @export(bindings.endQuery, .{ .name = "glEndQuery", .linkage = .Strong });
    @export(bindings.getQueryiv, .{ .name = "glGetQueryiv", .linkage = .Strong });
    @export(bindings.getQueryObjectiv, .{ .name = "glGetQueryObjectiv", .linkage = .Strong });
    @export(bindings.getQueryObjectuiv, .{ .name = "glGetQueryObjectuiv", .linkage = .Strong });
    @export(bindings.bindBuffer, .{ .name = "glBindBuffer", .linkage = .Strong });
    @export(bindings.deleteBuffers, .{ .name = "glDeleteBuffers", .linkage = .Strong });
    @export(bindings.genBuffers, .{ .name = "glGenBuffers", .linkage = .Strong });
    @export(bindings.isBuffer, .{ .name = "glIsBuffer", .linkage = .Strong });
    @export(bindings.bufferData, .{ .name = "glBufferData", .linkage = .Strong });
    @export(bindings.bufferSubData, .{ .name = "glBufferSubData", .linkage = .Strong });
    @export(bindings.getBufferSubData, .{ .name = "glGetBufferSubData", .linkage = .Strong });
    @export(bindings.mapBuffer, .{ .name = "glMapBuffer", .linkage = .Strong });
    @export(bindings.unmapBuffer, .{ .name = "glUnmapBuffer", .linkage = .Strong });
    @export(bindings.getBufferParameteriv, .{ .name = "glGetBufferParameteriv", .linkage = .Strong });
    @export(bindings.getBufferPointerv, .{ .name = "glGetBufferPointerv", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 2.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.blendEquationSeparate, .{ .name = "glBlendEquationSeparate", .linkage = .Strong });
    @export(bindings.drawBuffers, .{ .name = "glDrawBuffers", .linkage = .Strong });
    @export(bindings.stencilOpSeparate, .{ .name = "glStencilOpSeparate", .linkage = .Strong });
    @export(bindings.stencilFuncSeparate, .{ .name = "glStencilFuncSeparate", .linkage = .Strong });
    @export(bindings.stencilMaskSeparate, .{ .name = "glStencilMaskSeparate", .linkage = .Strong });
    @export(bindings.attachShader, .{ .name = "glAttachShader", .linkage = .Strong });
    @export(bindings.bindAttribLocation, .{ .name = "glBindAttribLocation", .linkage = .Strong });
    @export(bindings.compileShader, .{ .name = "glCompileShader", .linkage = .Strong });
    @export(bindings.createProgram, .{ .name = "glCreateProgram", .linkage = .Strong });
    @export(bindings.createShader, .{ .name = "glCreateShader", .linkage = .Strong });
    @export(bindings.deleteProgram, .{ .name = "glDeleteProgram", .linkage = .Strong });
    @export(bindings.deleteShader, .{ .name = "glDeleteShader", .linkage = .Strong });
    @export(bindings.detachShader, .{ .name = "glDetachShader", .linkage = .Strong });
    @export(bindings.disableVertexAttribArray, .{ .name = "glDisableVertexAttribArray", .linkage = .Strong });
    @export(bindings.enableVertexAttribArray, .{ .name = "glEnableVertexAttribArray", .linkage = .Strong });
    @export(bindings.getActiveAttrib, .{ .name = "glGetActiveAttrib", .linkage = .Strong });
    @export(bindings.getActiveUniform, .{ .name = "glGetActiveUniform", .linkage = .Strong });
    @export(bindings.getAttachedShaders, .{ .name = "glGetAttachedShaders", .linkage = .Strong });
    @export(bindings.getAttribLocation, .{ .name = "glGetAttribLocation", .linkage = .Strong });
    @export(bindings.getProgramiv, .{ .name = "glGetProgramiv", .linkage = .Strong });
    @export(bindings.getProgramInfoLog, .{ .name = "glGetProgramInfoLog", .linkage = .Strong });
    @export(bindings.getShaderiv, .{ .name = "glGetShaderiv", .linkage = .Strong });
    @export(bindings.getShaderInfoLog, .{ .name = "glGetShaderInfoLog", .linkage = .Strong });
    @export(bindings.getShaderSource, .{ .name = "glGetShaderSource", .linkage = .Strong });
    @export(bindings.getUniformLocation, .{ .name = "glGetUniformLocation", .linkage = .Strong });
    @export(bindings.getUniformfv, .{ .name = "glGetUniformfv", .linkage = .Strong });
    @export(bindings.getUniformiv, .{ .name = "glGetUniformiv", .linkage = .Strong });
    @export(bindings.getVertexAttribdv, .{ .name = "glGetVertexAttribdv", .linkage = .Strong });
    @export(bindings.getVertexAttribfv, .{ .name = "glGetVertexAttribfv", .linkage = .Strong });
    @export(bindings.getVertexAttribiv, .{ .name = "glGetVertexAttribiv", .linkage = .Strong });
    @export(bindings.getVertexAttribPointerv, .{ .name = "glGetVertexAttribPointerv", .linkage = .Strong });
    @export(bindings.isProgram, .{ .name = "glIsProgram", .linkage = .Strong });
    @export(bindings.isShader, .{ .name = "glIsShader", .linkage = .Strong });
    @export(bindings.linkProgram, .{ .name = "glLinkProgram", .linkage = .Strong });
    @export(bindings.shaderSource, .{ .name = "glShaderSource", .linkage = .Strong });
    @export(bindings.useProgram, .{ .name = "glUseProgram", .linkage = .Strong });
    @export(bindings.uniform1f, .{ .name = "glUniform1f", .linkage = .Strong });
    @export(bindings.uniform2f, .{ .name = "glUniform2f", .linkage = .Strong });
    @export(bindings.uniform3f, .{ .name = "glUniform3f", .linkage = .Strong });
    @export(bindings.uniform4f, .{ .name = "glUniform4f", .linkage = .Strong });
    @export(bindings.uniform1i, .{ .name = "glUniform1i", .linkage = .Strong });
    @export(bindings.uniform2i, .{ .name = "glUniform2i", .linkage = .Strong });
    @export(bindings.uniform3i, .{ .name = "glUniform3i", .linkage = .Strong });
    @export(bindings.uniform4i, .{ .name = "glUniform4i", .linkage = .Strong });
    @export(bindings.uniform1fv, .{ .name = "glUniform1fv", .linkage = .Strong });
    @export(bindings.uniform2fv, .{ .name = "glUniform2fv", .linkage = .Strong });
    @export(bindings.uniform3fv, .{ .name = "glUniform3fv", .linkage = .Strong });
    @export(bindings.uniform4fv, .{ .name = "glUniform4fv", .linkage = .Strong });
    @export(bindings.uniform1iv, .{ .name = "glUniform1iv", .linkage = .Strong });
    @export(bindings.uniform2iv, .{ .name = "glUniform2iv", .linkage = .Strong });
    @export(bindings.uniform3iv, .{ .name = "glUniform3iv", .linkage = .Strong });
    @export(bindings.uniform4iv, .{ .name = "glUniform4iv", .linkage = .Strong });
    @export(bindings.uniformMatrix2fv, .{ .name = "glUniformMatrix2fv", .linkage = .Strong });
    @export(bindings.uniformMatrix3fv, .{ .name = "glUniformMatrix3fv", .linkage = .Strong });
    @export(bindings.uniformMatrix4fv, .{ .name = "glUniformMatrix4fv", .linkage = .Strong });
    @export(bindings.validateProgram, .{ .name = "glValidateProgram", .linkage = .Strong });
    @export(bindings.vertexAttrib1d, .{ .name = "glVertexAttrib1d", .linkage = .Strong });
    @export(bindings.vertexAttrib1dv, .{ .name = "glVertexAttrib1dv", .linkage = .Strong });
    @export(bindings.vertexAttrib1f, .{ .name = "glVertexAttrib1f", .linkage = .Strong });
    @export(bindings.vertexAttrib1fv, .{ .name = "glVertexAttrib1fv", .linkage = .Strong });
    @export(bindings.vertexAttrib1s, .{ .name = "glVertexAttrib1s", .linkage = .Strong });
    @export(bindings.vertexAttrib1sv, .{ .name = "glVertexAttrib1sv", .linkage = .Strong });
    @export(bindings.vertexAttrib2d, .{ .name = "glVertexAttrib2d", .linkage = .Strong });
    @export(bindings.vertexAttrib2dv, .{ .name = "glVertexAttrib2dv", .linkage = .Strong });
    @export(bindings.vertexAttrib2f, .{ .name = "glVertexAttrib2f", .linkage = .Strong });
    @export(bindings.vertexAttrib2fv, .{ .name = "glVertexAttrib2fv", .linkage = .Strong });
    @export(bindings.vertexAttrib2s, .{ .name = "glVertexAttrib2s", .linkage = .Strong });
    @export(bindings.vertexAttrib2sv, .{ .name = "glVertexAttrib2sv", .linkage = .Strong });
    @export(bindings.vertexAttrib3d, .{ .name = "glVertexAttrib3d", .linkage = .Strong });
    @export(bindings.vertexAttrib3dv, .{ .name = "glVertexAttrib3dv", .linkage = .Strong });
    @export(bindings.vertexAttrib3f, .{ .name = "glVertexAttrib3f", .linkage = .Strong });
    @export(bindings.vertexAttrib3fv, .{ .name = "glVertexAttrib3fv", .linkage = .Strong });
    @export(bindings.vertexAttrib3s, .{ .name = "glVertexAttrib3s", .linkage = .Strong });
    @export(bindings.vertexAttrib3sv, .{ .name = "glVertexAttrib3sv", .linkage = .Strong });
    @export(bindings.vertexAttrib4Nbv, .{ .name = "glVertexAttrib4Nbv", .linkage = .Strong });
    @export(bindings.vertexAttrib4Niv, .{ .name = "glVertexAttrib4Niv", .linkage = .Strong });
    @export(bindings.vertexAttrib4Nsv, .{ .name = "glVertexAttrib4Nsv", .linkage = .Strong });
    @export(bindings.vertexAttrib4Nub, .{ .name = "glVertexAttrib4Nub", .linkage = .Strong });
    @export(bindings.vertexAttrib4Nubv, .{ .name = "glVertexAttrib4Nubv", .linkage = .Strong });
    @export(bindings.vertexAttrib4Nuiv, .{ .name = "glVertexAttrib4Nuiv", .linkage = .Strong });
    @export(bindings.vertexAttrib4Nusv, .{ .name = "glVertexAttrib4Nusv", .linkage = .Strong });
    @export(bindings.vertexAttrib4bv, .{ .name = "glVertexAttrib4bv", .linkage = .Strong });
    @export(bindings.vertexAttrib4d, .{ .name = "glVertexAttrib4d", .linkage = .Strong });
    @export(bindings.vertexAttrib4dv, .{ .name = "glVertexAttrib4dv", .linkage = .Strong });
    @export(bindings.vertexAttrib4f, .{ .name = "glVertexAttrib4f", .linkage = .Strong });
    @export(bindings.vertexAttrib4fv, .{ .name = "glVertexAttrib4fv", .linkage = .Strong });
    @export(bindings.vertexAttrib4iv, .{ .name = "glVertexAttrib4iv", .linkage = .Strong });
    @export(bindings.vertexAttrib4s, .{ .name = "glVertexAttrib4s", .linkage = .Strong });
    @export(bindings.vertexAttrib4sv, .{ .name = "glVertexAttrib4sv", .linkage = .Strong });
    @export(bindings.vertexAttrib4ubv, .{ .name = "glVertexAttrib4ubv", .linkage = .Strong });
    @export(bindings.vertexAttrib4uiv, .{ .name = "glVertexAttrib4uiv", .linkage = .Strong });
    @export(bindings.vertexAttrib4usv, .{ .name = "glVertexAttrib4usv", .linkage = .Strong });
    @export(bindings.vertexAttribPointer, .{ .name = "glVertexAttribPointer", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 2.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.uniformMatrix2x3fv, .{ .name = "glUniformMatrix2x3fv", .linkage = .Strong });
    @export(bindings.uniformMatrix3x2fv, .{ .name = "glUniformMatrix3x2fv", .linkage = .Strong });
    @export(bindings.uniformMatrix2x4fv, .{ .name = "glUniformMatrix2x4fv", .linkage = .Strong });
    @export(bindings.uniformMatrix4x2fv, .{ .name = "glUniformMatrix4x2fv", .linkage = .Strong });
    @export(bindings.uniformMatrix3x4fv, .{ .name = "glUniformMatrix3x4fv", .linkage = .Strong });
    @export(bindings.uniformMatrix4x3fv, .{ .name = "glUniformMatrix4x3fv", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.colorMaski, .{ .name = "glColorMaski", .linkage = .Strong });
    @export(bindings.getBooleani_v, .{ .name = "glGetBooleani_v", .linkage = .Strong });
    @export(bindings.getIntegeri_v, .{ .name = "glGetIntegeri_v", .linkage = .Strong });
    @export(bindings.enablei, .{ .name = "glEnablei", .linkage = .Strong });
    @export(bindings.disablei, .{ .name = "glDisablei", .linkage = .Strong });
    @export(bindings.isEnabledi, .{ .name = "glIsEnabledi", .linkage = .Strong });
    @export(bindings.beginTransformFeedback, .{ .name = "glBeginTransformFeedback", .linkage = .Strong });
    @export(bindings.endTransformFeedback, .{ .name = "glEndTransformFeedback", .linkage = .Strong });
    @export(bindings.bindBufferRange, .{ .name = "glBindBufferRange", .linkage = .Strong });
    @export(bindings.bindBufferBase, .{ .name = "glBindBufferBase", .linkage = .Strong });
    @export(bindings.transformFeedbackVaryings, .{ .name = "glTransformFeedbackVaryings", .linkage = .Strong });
    @export(bindings.getTransformFeedbackVarying, .{ .name = "glGetTransformFeedbackVarying", .linkage = .Strong });
    @export(bindings.beginConditionalRender, .{ .name = "glBeginConditionalRender", .linkage = .Strong });
    @export(bindings.endConditionalRender, .{ .name = "glEndConditionalRender", .linkage = .Strong });
    @export(bindings.vertexAttribIPointer, .{ .name = "glVertexAttribIPointer", .linkage = .Strong });
    @export(bindings.getVertexAttribIiv, .{ .name = "glGetVertexAttribIiv", .linkage = .Strong });
    @export(bindings.getVertexAttribIuiv, .{ .name = "glGetVertexAttribIuiv", .linkage = .Strong });
    @export(bindings.vertexAttribI1i, .{ .name = "glVertexAttribI1i", .linkage = .Strong });
    @export(bindings.vertexAttribI2i, .{ .name = "glVertexAttribI2i", .linkage = .Strong });
    @export(bindings.vertexAttribI3i, .{ .name = "glVertexAttribI3i", .linkage = .Strong });
    @export(bindings.vertexAttribI4i, .{ .name = "glVertexAttribI4i", .linkage = .Strong });
    @export(bindings.vertexAttribI1ui, .{ .name = "glVertexAttribI1ui", .linkage = .Strong });
    @export(bindings.vertexAttribI2ui, .{ .name = "glVertexAttribI2ui", .linkage = .Strong });
    @export(bindings.vertexAttribI3ui, .{ .name = "glVertexAttribI3ui", .linkage = .Strong });
    @export(bindings.vertexAttribI4ui, .{ .name = "glVertexAttribI4ui", .linkage = .Strong });
    @export(bindings.vertexAttribI1iv, .{ .name = "glVertexAttribI1iv", .linkage = .Strong });
    @export(bindings.vertexAttribI2iv, .{ .name = "glVertexAttribI2iv", .linkage = .Strong });
    @export(bindings.vertexAttribI3iv, .{ .name = "glVertexAttribI3iv", .linkage = .Strong });
    @export(bindings.vertexAttribI4iv, .{ .name = "glVertexAttribI4iv", .linkage = .Strong });
    @export(bindings.vertexAttribI1uiv, .{ .name = "glVertexAttribI1uiv", .linkage = .Strong });
    @export(bindings.vertexAttribI2uiv, .{ .name = "glVertexAttribI2uiv", .linkage = .Strong });
    @export(bindings.vertexAttribI3uiv, .{ .name = "glVertexAttribI3uiv", .linkage = .Strong });
    @export(bindings.vertexAttribI4uiv, .{ .name = "glVertexAttribI4uiv", .linkage = .Strong });
    @export(bindings.vertexAttribI4bv, .{ .name = "glVertexAttribI4bv", .linkage = .Strong });
    @export(bindings.vertexAttribI4sv, .{ .name = "glVertexAttribI4sv", .linkage = .Strong });
    @export(bindings.vertexAttribI4ubv, .{ .name = "glVertexAttribI4ubv", .linkage = .Strong });
    @export(bindings.vertexAttribI4usv, .{ .name = "glVertexAttribI4usv", .linkage = .Strong });
    @export(bindings.getUniformuiv, .{ .name = "glGetUniformuiv", .linkage = .Strong });
    @export(bindings.bindFragDataLocation, .{ .name = "glBindFragDataLocation", .linkage = .Strong });
    @export(bindings.getFragDataLocation, .{ .name = "glGetFragDataLocation", .linkage = .Strong });
    @export(bindings.uniform1ui, .{ .name = "glUniform1ui", .linkage = .Strong });
    @export(bindings.uniform2ui, .{ .name = "glUniform2ui", .linkage = .Strong });
    @export(bindings.uniform3ui, .{ .name = "glUniform3ui", .linkage = .Strong });
    @export(bindings.uniform4ui, .{ .name = "glUniform4ui", .linkage = .Strong });
    @export(bindings.uniform1uiv, .{ .name = "glUniform1uiv", .linkage = .Strong });
    @export(bindings.uniform2uiv, .{ .name = "glUniform2uiv", .linkage = .Strong });
    @export(bindings.uniform3uiv, .{ .name = "glUniform3uiv", .linkage = .Strong });
    @export(bindings.uniform4uiv, .{ .name = "glUniform4uiv", .linkage = .Strong });
    @export(bindings.texParameterIiv, .{ .name = "glTexParameterIiv", .linkage = .Strong });
    @export(bindings.texParameterIuiv, .{ .name = "glTexParameterIuiv", .linkage = .Strong });
    @export(bindings.getTexParameterIiv, .{ .name = "glGetTexParameterIiv", .linkage = .Strong });
    @export(bindings.getTexParameterIuiv, .{ .name = "glGetTexParameterIuiv", .linkage = .Strong });
    @export(bindings.clearBufferiv, .{ .name = "glClearBufferiv", .linkage = .Strong });
    @export(bindings.clearBufferuiv, .{ .name = "glClearBufferuiv", .linkage = .Strong });
    @export(bindings.clearBufferfv, .{ .name = "glClearBufferfv", .linkage = .Strong });
    @export(bindings.clearBufferfi, .{ .name = "glClearBufferfi", .linkage = .Strong });
    @export(bindings.getStringi, .{ .name = "glGetStringi", .linkage = .Strong });
    @export(bindings.isRenderbuffer, .{ .name = "glIsRenderbuffer", .linkage = .Strong });
    @export(bindings.bindRenderbuffer, .{ .name = "glBindRenderbuffer", .linkage = .Strong });
    @export(bindings.deleteRenderbuffers, .{ .name = "glDeleteRenderbuffers", .linkage = .Strong });
    @export(bindings.genRenderbuffers, .{ .name = "glGenRenderbuffers", .linkage = .Strong });
    @export(bindings.renderbufferStorage, .{ .name = "glRenderbufferStorage", .linkage = .Strong });
    @export(bindings.getRenderbufferParameteriv, .{ .name = "glGetRenderbufferParameteriv", .linkage = .Strong });
    @export(bindings.isFramebuffer, .{ .name = "glIsFramebuffer", .linkage = .Strong });
    @export(bindings.bindFramebuffer, .{ .name = "glBindFramebuffer", .linkage = .Strong });
    @export(bindings.deleteFramebuffers, .{ .name = "glDeleteFramebuffers", .linkage = .Strong });
    @export(bindings.genFramebuffers, .{ .name = "glGenFramebuffers", .linkage = .Strong });
    @export(bindings.checkFramebufferStatus, .{ .name = "glCheckFramebufferStatus", .linkage = .Strong });
    @export(bindings.framebufferTexture1D, .{ .name = "glFramebufferTexture1D", .linkage = .Strong });
    @export(bindings.framebufferTexture2D, .{ .name = "glFramebufferTexture2D", .linkage = .Strong });
    @export(bindings.framebufferTexture3D, .{ .name = "glFramebufferTexture3D", .linkage = .Strong });
    @export(bindings.framebufferRenderbuffer, .{ .name = "glFramebufferRenderbuffer", .linkage = .Strong });
    @export(bindings.getFramebufferAttachmentParameteriv, .{ .name = "glGetFramebufferAttachmentParameteriv", .linkage = .Strong });
    @export(bindings.generateMipmap, .{ .name = "glGenerateMipmap", .linkage = .Strong });
    @export(bindings.blitFramebuffer, .{ .name = "glBlitFramebuffer", .linkage = .Strong });
    @export(bindings.renderbufferStorageMultisample, .{ .name = "glRenderbufferStorageMultisample", .linkage = .Strong });
    @export(bindings.framebufferTextureLayer, .{ .name = "glFramebufferTextureLayer", .linkage = .Strong });
    @export(bindings.mapBufferRange, .{ .name = "glMapBufferRange", .linkage = .Strong });
    @export(bindings.flushMappedBufferRange, .{ .name = "glFlushMappedBufferRange", .linkage = .Strong });
    @export(bindings.bindVertexArray, .{ .name = "glBindVertexArray", .linkage = .Strong });
    @export(bindings.deleteVertexArrays, .{ .name = "glDeleteVertexArrays", .linkage = .Strong });
    @export(bindings.genVertexArrays, .{ .name = "glGenVertexArrays", .linkage = .Strong });
    @export(bindings.isVertexArray, .{ .name = "glIsVertexArray", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawArraysInstanced, .{ .name = "glDrawArraysInstanced", .linkage = .Strong });
    @export(bindings.drawElementsInstanced, .{ .name = "glDrawElementsInstanced", .linkage = .Strong });
    @export(bindings.texBuffer, .{ .name = "glTexBuffer", .linkage = .Strong });
    @export(bindings.primitiveRestartIndex, .{ .name = "glPrimitiveRestartIndex", .linkage = .Strong });
    @export(bindings.copyBufferSubData, .{ .name = "glCopyBufferSubData", .linkage = .Strong });
    @export(bindings.getUniformIndices, .{ .name = "glGetUniformIndices", .linkage = .Strong });
    @export(bindings.getActiveUniformsiv, .{ .name = "glGetActiveUniformsiv", .linkage = .Strong });
    @export(bindings.getActiveUniformName, .{ .name = "glGetActiveUniformName", .linkage = .Strong });
    @export(bindings.getUniformBlockIndex, .{ .name = "glGetUniformBlockIndex", .linkage = .Strong });
    @export(bindings.getActiveUniformBlockiv, .{ .name = "glGetActiveUniformBlockiv", .linkage = .Strong });
    @export(bindings.getActiveUniformBlockName, .{ .name = "glGetActiveUniformBlockName", .linkage = .Strong });
    @export(bindings.uniformBlockBinding, .{ .name = "glUniformBlockBinding", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.2 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawElementsBaseVertex, .{ .name = "glDrawElementsBaseVertex", .linkage = .Strong });
    @export(bindings.drawRangeElementsBaseVertex, .{ .name = "glDrawRangeElementsBaseVertex", .linkage = .Strong });
    @export(bindings.drawElementsInstancedBaseVertex, .{ .name = "glDrawElementsInstancedBaseVertex", .linkage = .Strong });
    @export(bindings.multiDrawElementsBaseVertex, .{ .name = "glMultiDrawElementsBaseVertex", .linkage = .Strong });
    @export(bindings.provokingVertex, .{ .name = "glProvokingVertex", .linkage = .Strong });
    @export(bindings.fenceSync, .{ .name = "glFenceSync", .linkage = .Strong });
    @export(bindings.isSync, .{ .name = "glIsSync", .linkage = .Strong });
    @export(bindings.deleteSync, .{ .name = "glDeleteSync", .linkage = .Strong });
    @export(bindings.clientWaitSync, .{ .name = "glClientWaitSync", .linkage = .Strong });
    @export(bindings.waitSync, .{ .name = "glWaitSync", .linkage = .Strong });
    @export(bindings.getInteger64v, .{ .name = "glGetInteger64v", .linkage = .Strong });
    @export(bindings.getSynciv, .{ .name = "glGetSynciv", .linkage = .Strong });
    @export(bindings.getInteger64i_v, .{ .name = "glGetInteger64i_v", .linkage = .Strong });
    @export(bindings.getBufferParameteri64v, .{ .name = "glGetBufferParameteri64v", .linkage = .Strong });
    @export(bindings.framebufferTexture, .{ .name = "glFramebufferTexture", .linkage = .Strong });
    @export(bindings.texImage2DMultisample, .{ .name = "glTexImage2DMultisample", .linkage = .Strong });
    @export(bindings.texImage3DMultisample, .{ .name = "glTexImage3DMultisample", .linkage = .Strong });
    @export(bindings.getMultisamplefv, .{ .name = "glGetMultisamplefv", .linkage = .Strong });
    @export(bindings.sampleMaski, .{ .name = "glSampleMaski", .linkage = .Strong });
    //----------------------------------------------------------------------------------------------
    // OpenGL 3.3 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.bindFragDataLocationIndexed, .{ .name = "glBindFragDataLocationIndexed", .linkage = .Strong });
    @export(bindings.getFragDataIndex, .{ .name = "glGetFragDataIndex", .linkage = .Strong });
    @export(bindings.genSamplers, .{ .name = "glGenSamplers", .linkage = .Strong });
    @export(bindings.deleteSamplers, .{ .name = "glDeleteSamplers", .linkage = .Strong });
    @export(bindings.isSampler, .{ .name = "glIsSampler", .linkage = .Strong });
    @export(bindings.bindSampler, .{ .name = "glBindSampler", .linkage = .Strong });
    @export(bindings.samplerParameteri, .{ .name = "glSamplerParameteri", .linkage = .Strong });
    @export(bindings.samplerParameteriv, .{ .name = "glSamplerParameteriv", .linkage = .Strong });
    @export(bindings.samplerParameterf, .{ .name = "glSamplerParameterf", .linkage = .Strong });
    @export(bindings.samplerParameterfv, .{ .name = "glSamplerParameterfv", .linkage = .Strong });
    @export(bindings.samplerParameterIiv, .{ .name = "glSamplerParameterIiv", .linkage = .Strong });
    @export(bindings.samplerParameterIuiv, .{ .name = "glSamplerParameterIuiv", .linkage = .Strong });
    @export(bindings.getSamplerParameteriv, .{ .name = "glGetSamplerParameteriv", .linkage = .Strong });
    @export(bindings.getSamplerParameterIiv, .{ .name = "glGetSamplerParameterIiv", .linkage = .Strong });
    @export(bindings.getSamplerParameterfv, .{ .name = "glGetSamplerParameterfv", .linkage = .Strong });
    @export(bindings.getSamplerParameterIuiv, .{ .name = "glGetSamplerParameterIuiv", .linkage = .Strong });
    @export(bindings.queryCounter, .{ .name = "glQueryCounter", .linkage = .Strong });
    @export(bindings.getQueryObjecti64v, .{ .name = "glGetQueryObjecti64v", .linkage = .Strong });
    @export(bindings.getQueryObjectui64v, .{ .name = "glGetQueryObjectui64v", .linkage = .Strong });
    @export(bindings.vertexAttribDivisor, .{ .name = "glVertexAttribDivisor", .linkage = .Strong });
    @export(bindings.vertexAttribP1ui, .{ .name = "glVertexAttribP1ui", .linkage = .Strong });
    @export(bindings.vertexAttribP1uiv, .{ .name = "glVertexAttribP1uiv", .linkage = .Strong });
    @export(bindings.vertexAttribP2ui, .{ .name = "glVertexAttribP2ui", .linkage = .Strong });
    @export(bindings.vertexAttribP2uiv, .{ .name = "glVertexAttribP2uiv", .linkage = .Strong });
    @export(bindings.vertexAttribP3ui, .{ .name = "glVertexAttribP3ui", .linkage = .Strong });
    @export(bindings.vertexAttribP3uiv, .{ .name = "glVertexAttribP3uiv", .linkage = .Strong });
    @export(bindings.vertexAttribP4ui, .{ .name = "glVertexAttribP4ui", .linkage = .Strong });
    @export(bindings.vertexAttribP4uiv, .{ .name = "glVertexAttribP4uiv", .linkage = .Strong });
}
