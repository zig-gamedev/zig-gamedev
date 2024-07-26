const std = @import("std");
const assert = std.debug.assert;

const options = @import("zopengl_options");

comptime {
    @setEvalBranchQuota(20_000);
    _ = std.testing.refAllDeclsRecursive(@This());
}

pub const bindings = @import("bindings.zig");
pub const wrapper = @import("wrapper.zig").Wrap(bindings);

pub const LoaderFn = *const fn ([*:0]const u8) ?*const anyopaque;

pub const Extension = enum {
    KHR_debug,
    //
    EXT_copy_texture,
    //
    NV_bindless_texture,
    NV_shader_buffer_load,
};

pub const EsExtension = enum {
    OES_vertex_array_object,
    //
    KHR_debug,
};

//--------------------------------------------------------------------------------------------------
//
// Functions for loading OpenGL function pointers
//
//--------------------------------------------------------------------------------------------------
pub fn loadCoreProfile(loader: LoaderFn, major: u32, minor: u32) !void {
    const ver = 10 * major + minor;

    assert(major >= 1 and major <= 4);
    assert(minor >= 0 and minor <= 6);
    assert(ver >= 10 and ver <= 46);

    loaderFunc = loader;

    // OpenGL 1.0
    if (ver >= 10) {
        try load("glCullFace", .{&bindings.cullFace});
        try load("glFrontFace", .{&bindings.frontFace});
        try load("glHint", .{&bindings.hint});
        try load("glLineWidth", .{&bindings.lineWidth});
        try load("glPointSize", .{&bindings.pointSize});
        try load("glPolygonMode", .{&bindings.polygonMode});
        try load("glScissor", .{&bindings.scissor});
        try load("glTexParameterf", .{&bindings.texParameterf});
        try load("glTexParameterfv", .{&bindings.texParameterfv});
        try load("glTexParameteri", .{&bindings.texParameteri});
        try load("glTexParameteriv", .{&bindings.texParameteriv});
        try load("glTexImage1D", .{&bindings.texImage1D});
        try load("glTexImage2D", .{&bindings.texImage2D});
        try load("glDrawBuffer", .{&bindings.drawBuffer});
        try load("glClear", .{&bindings.clear});
        try load("glClearColor", .{&bindings.clearColor});
        try load("glClearStencil", .{&bindings.clearStencil});
        try load("glClearDepth", .{&bindings.clearDepth});
        try load("glStencilMask", .{&bindings.stencilMask});
        try load("glColorMask", .{&bindings.colorMask});
        try load("glDepthMask", .{&bindings.depthMask});
        try load("glDisable", .{&bindings.disable});
        try load("glEnable", .{&bindings.enable});
        try load("glFinish", .{&bindings.finish});
        try load("glFlush", .{&bindings.flush});
        try load("glBlendFunc", .{&bindings.blendFunc});
        try load("glLogicOp", .{&bindings.logicOp});
        try load("glStencilFunc", .{&bindings.stencilFunc});
        try load("glStencilOp", .{&bindings.stencilOp});
        try load("glDepthFunc", .{&bindings.depthFunc});
        try load("glPixelStoref", .{&bindings.pixelStoref});
        try load("glPixelStorei", .{&bindings.pixelStorei});
        try load("glReadBuffer", .{&bindings.readBuffer});
        try load("glReadPixels", .{&bindings.readPixels});
        try load("glGetBooleanv", .{&bindings.getBooleanv});
        try load("glGetDoublev", .{&bindings.getDoublev});
        try load("glGetError", .{&bindings.getError});
        try load("glGetFloatv", .{&bindings.getFloatv});
        try load("glGetIntegerv", .{&bindings.getIntegerv});
        try load("glGetString", .{&bindings.getString});
        try load("glGetTexImage", .{&bindings.getTexImage});
        try load("glGetTexParameterfv", .{&bindings.getTexParameterfv});
        try load("glGetTexParameteriv", .{&bindings.getTexParameteriv});
        try load("glGetTexLevelParameterfv", .{&bindings.getTexLevelParameterfv});
        try load("glGetTexLevelParameteriv", .{&bindings.getTexLevelParameteriv});
        try load("glIsEnabled", .{&bindings.isEnabled});
        try load("glDepthRange", .{&bindings.depthRange});
        try load("glViewport", .{&bindings.viewport});
    }

    // OpenGL 1.1
    if (ver >= 11) {
        try load("glDrawArrays", .{&bindings.drawArrays});
        try load("glDrawElements", .{&bindings.drawElements});
        try load("glPolygonOffset", .{&bindings.polygonOffset});
        try load("glCopyTexImage1D", .{&bindings.copyTexImage1D});
        try load("glCopyTexImage2D", .{&bindings.copyTexImage2D});
        try load("glCopyTexSubImage1D", .{&bindings.copyTexSubImage1D});
        try load("glCopyTexSubImage2D", .{&bindings.copyTexSubImage2D});
        try load("glTexSubImage1D", .{&bindings.texSubImage1D});
        try load("glTexSubImage2D", .{&bindings.texSubImage2D});
        try load("glBindTexture", .{&bindings.bindTexture});
        try load("glDeleteTextures", .{&bindings.deleteTextures});
        try load("glGenTextures", .{&bindings.genTextures});
        try load("glIsTexture", .{&bindings.isTexture});
    }

    // OpenGL 1.2
    if (ver >= 12) {
        try load("glDrawRangeElements", .{&bindings.drawRangeElements});
        try load("glTexImage3D", .{&bindings.texImage3D});
        try load("glTexSubImage3D", .{&bindings.texSubImage3D});
        try load("glCopyTexSubImage3D", .{&bindings.copyTexSubImage3D});
    }

    // OpenGL 1.3
    if (ver >= 13) {
        try load("glActiveTexture", .{&bindings.activeTexture});
        try load("glSampleCoverage", .{&bindings.sampleCoverage});
        try load("glCompressedTexImage3D", .{&bindings.compressedTexImage3D});
        try load("glCompressedTexImage2D", .{&bindings.compressedTexImage2D});
        try load("glCompressedTexImage1D", .{&bindings.compressedTexImage1D});
        try load("glCompressedTexSubImage3D", .{&bindings.compressedTexSubImage3D});
        try load("glCompressedTexSubImage2D", .{&bindings.compressedTexSubImage2D});
        try load("glCompressedTexSubImage1D", .{&bindings.compressedTexSubImage1D});
        try load("glGetCompressedTexImage", .{&bindings.getCompressedTexImage});
    }

    // OpenGL 1.4
    if (ver >= 14) {
        try load("glBlendFuncSeparate", .{&bindings.blendFuncSeparate});
        try load("glMultiDrawArrays", .{&bindings.multiDrawArrays});
        try load("glMultiDrawElements", .{&bindings.multiDrawElements});
        try load("glPointParameterf", .{&bindings.pointParameterf});
        try load("glPointParameterfv", .{&bindings.pointParameterfv});
        try load("glPointParameteri", .{&bindings.pointParameteri});
        try load("glPointParameteriv", .{&bindings.pointParameteriv});
        try load("glBlendColor", .{&bindings.blendColor});
        try load("glBlendEquation", .{&bindings.blendEquation});
    }

    // OpenGL 1.5
    if (ver >= 15) {
        try load("glGenQueries", .{&bindings.genQueries});
        try load("glDeleteQueries", .{&bindings.deleteQueries});
        try load("glIsQuery", .{&bindings.isQuery});
        try load("glBeginQuery", .{&bindings.beginQuery});
        try load("glEndQuery", .{&bindings.endQuery});
        try load("glGetQueryiv", .{&bindings.getQueryiv});
        try load("glGetQueryObjectiv", .{&bindings.getQueryObjectiv});
        try load("glGetQueryObjectuiv", .{&bindings.getQueryObjectuiv});
        try load("glBindBuffer", .{&bindings.bindBuffer});
        try load("glDeleteBuffers", .{&bindings.deleteBuffers});
        try load("glGenBuffers", .{&bindings.genBuffers});
        try load("glIsBuffer", .{&bindings.isBuffer});
        try load("glBufferData", .{&bindings.bufferData});
        try load("glBufferSubData", .{&bindings.bufferSubData});
        try load("glGetBufferSubData", .{&bindings.getBufferSubData});
        try load("glMapBuffer", .{&bindings.mapBuffer});
        try load("glUnmapBuffer", .{&bindings.unmapBuffer});
        try load("glGetBufferParameteriv", .{&bindings.getBufferParameteriv});
        try load("glGetBufferPointerv", .{&bindings.getBufferPointerv});
    }

    // OpenGL 2.0
    if (ver >= 20) {
        try load("glBlendEquationSeparate", .{&bindings.blendEquationSeparate});
        try load("glDrawBuffers", .{&bindings.drawBuffers});
        try load("glStencilOpSeparate", .{&bindings.stencilOpSeparate});
        try load("glStencilFuncSeparate", .{&bindings.stencilFuncSeparate});
        try load("glStencilMaskSeparate", .{&bindings.stencilMaskSeparate});
        try load("glAttachShader", .{&bindings.attachShader});
        try load("glBindAttribLocation", .{&bindings.bindAttribLocation});
        try load("glCompileShader", .{&bindings.compileShader});
        try load("glCreateProgram", .{&bindings.createProgram});
        try load("glCreateShader", .{&bindings.createShader});
        try load("glDeleteProgram", .{&bindings.deleteProgram});
        try load("glDeleteShader", .{&bindings.deleteShader});
        try load("glDetachShader", .{&bindings.detachShader});
        try load("glDisableVertexAttribArray", .{&bindings.disableVertexAttribArray});
        try load("glEnableVertexAttribArray", .{&bindings.enableVertexAttribArray});
        try load("glGetActiveAttrib", .{&bindings.getActiveAttrib});
        try load("glGetActiveUniform", .{&bindings.getActiveUniform});
        try load("glGetAttachedShaders", .{&bindings.getAttachedShaders});
        try load("glGetAttribLocation", .{&bindings.getAttribLocation});
        try load("glGetProgramiv", .{&bindings.getProgramiv});
        try load("glGetProgramInfoLog", .{&bindings.getProgramInfoLog});
        try load("glGetShaderiv", .{&bindings.getShaderiv});
        try load("glGetShaderInfoLog", .{&bindings.getShaderInfoLog});
        try load("glGetShaderSource", .{&bindings.getShaderSource});
        try load("glGetUniformLocation", .{&bindings.getUniformLocation});
        try load("glGetUniformfv", .{&bindings.getUniformfv});
        try load("glGetUniformiv", .{&bindings.getUniformiv});
        try load("glGetVertexAttribdv", .{&bindings.getVertexAttribdv});
        try load("glGetVertexAttribfv", .{&bindings.getVertexAttribfv});
        try load("glGetVertexAttribiv", .{&bindings.getVertexAttribiv});
        try load("glGetVertexAttribPointerv", .{&bindings.getVertexAttribPointerv});
        try load("glIsProgram", .{&bindings.isProgram});
        try load("glIsShader", .{&bindings.isShader});
        try load("glLinkProgram", .{&bindings.linkProgram});
        try load("glShaderSource", .{&bindings.shaderSource});
        try load("glUseProgram", .{&bindings.useProgram});
        try load("glUniform1f", .{&bindings.uniform1f});
        try load("glUniform2f", .{&bindings.uniform2f});
        try load("glUniform3f", .{&bindings.uniform3f});
        try load("glUniform4f", .{&bindings.uniform4f});
        try load("glUniform1i", .{&bindings.uniform1i});
        try load("glUniform2i", .{&bindings.uniform2i});
        try load("glUniform3i", .{&bindings.uniform3i});
        try load("glUniform4i", .{&bindings.uniform4i});
        try load("glUniform1fv", .{&bindings.uniform1fv});
        try load("glUniform2fv", .{&bindings.uniform2fv});
        try load("glUniform3fv", .{&bindings.uniform3fv});
        try load("glUniform4fv", .{&bindings.uniform4fv});
        try load("glUniform1iv", .{&bindings.uniform1iv});
        try load("glUniform2iv", .{&bindings.uniform2iv});
        try load("glUniform3iv", .{&bindings.uniform3iv});
        try load("glUniform4iv", .{&bindings.uniform4iv});
        try load("glUniformMatrix2fv", .{&bindings.uniformMatrix2fv});
        try load("glUniformMatrix3fv", .{&bindings.uniformMatrix3fv});
        try load("glUniformMatrix4fv", .{&bindings.uniformMatrix4fv});
        try load("glValidateProgram", .{&bindings.validateProgram});
        try load("glVertexAttrib1d", .{&bindings.vertexAttrib1d});
        try load("glVertexAttrib1dv", .{&bindings.vertexAttrib1dv});
        try load("glVertexAttrib1f", .{&bindings.vertexAttrib1f});
        try load("glVertexAttrib1fv", .{&bindings.vertexAttrib1fv});
        try load("glVertexAttrib1s", .{&bindings.vertexAttrib1s});
        try load("glVertexAttrib1sv", .{&bindings.vertexAttrib1sv});
        try load("glVertexAttrib2d", .{&bindings.vertexAttrib2d});
        try load("glVertexAttrib2dv", .{&bindings.vertexAttrib2dv});
        try load("glVertexAttrib2f", .{&bindings.vertexAttrib2f});
        try load("glVertexAttrib2fv", .{&bindings.vertexAttrib2fv});
        try load("glVertexAttrib2s", .{&bindings.vertexAttrib2s});
        try load("glVertexAttrib2sv", .{&bindings.vertexAttrib2sv});
        try load("glVertexAttrib3d", .{&bindings.vertexAttrib3d});
        try load("glVertexAttrib3dv", .{&bindings.vertexAttrib3dv});
        try load("glVertexAttrib3f", .{&bindings.vertexAttrib3f});
        try load("glVertexAttrib3fv", .{&bindings.vertexAttrib3fv});
        try load("glVertexAttrib3s", .{&bindings.vertexAttrib3s});
        try load("glVertexAttrib3sv", .{&bindings.vertexAttrib3sv});
        try load("glVertexAttrib4Nbv", .{&bindings.vertexAttrib4Nbv});
        try load("glVertexAttrib4Niv", .{&bindings.vertexAttrib4Niv});
        try load("glVertexAttrib4Nsv", .{&bindings.vertexAttrib4Nsv});
        try load("glVertexAttrib4Nub", .{&bindings.vertexAttrib4Nub});
        try load("glVertexAttrib4Nubv", .{&bindings.vertexAttrib4Nubv});
        try load("glVertexAttrib4Nuiv", .{&bindings.vertexAttrib4Nuiv});
        try load("glVertexAttrib4Nusv", .{&bindings.vertexAttrib4Nusv});
        try load("glVertexAttrib4bv", .{&bindings.vertexAttrib4bv});
        try load("glVertexAttrib4d", .{&bindings.vertexAttrib4d});
        try load("glVertexAttrib4dv", .{&bindings.vertexAttrib4dv});
        try load("glVertexAttrib4f", .{&bindings.vertexAttrib4f});
        try load("glVertexAttrib4fv", .{&bindings.vertexAttrib4fv});
        try load("glVertexAttrib4iv", .{&bindings.vertexAttrib4iv});
        try load("glVertexAttrib4s", .{&bindings.vertexAttrib4s});
        try load("glVertexAttrib4sv", .{&bindings.vertexAttrib4sv});
        try load("glVertexAttrib4ubv", .{&bindings.vertexAttrib4ubv});
        try load("glVertexAttrib4uiv", .{&bindings.vertexAttrib4uiv});
        try load("glVertexAttrib4usv", .{&bindings.vertexAttrib4usv});
        try load("glVertexAttribPointer", .{&bindings.vertexAttribPointer});
    }

    // OpenGL 2.1
    if (ver >= 21) {
        try load("glUniformMatrix2x3fv", .{&bindings.uniformMatrix2x3fv});
        try load("glUniformMatrix3x2fv", .{&bindings.uniformMatrix3x2fv});
        try load("glUniformMatrix2x4fv", .{&bindings.uniformMatrix2x4fv});
        try load("glUniformMatrix4x2fv", .{&bindings.uniformMatrix4x2fv});
        try load("glUniformMatrix3x4fv", .{&bindings.uniformMatrix3x4fv});
        try load("glUniformMatrix4x3fv", .{&bindings.uniformMatrix4x3fv});
    }

    // OpenGL 3.0
    if (ver >= 30) {
        try load("glColorMaski", .{&bindings.colorMaski});
        try load("glGetBooleani_v", .{&bindings.getBooleani_v});
        try load("glGetIntegeri_v", .{&bindings.getIntegeri_v});
        try load("glEnablei", .{&bindings.enablei});
        try load("glDisablei", .{&bindings.disablei});
        try load("glIsEnabledi", .{&bindings.isEnabledi});
        try load("glBeginTransformFeedback", .{&bindings.beginTransformFeedback});
        try load("glEndTransformFeedback", .{&bindings.endTransformFeedback});
        try load("glBindBufferRange", .{&bindings.bindBufferRange});
        try load("glBindBufferBase", .{&bindings.bindBufferBase});
        try load("glTransformFeedbackVaryings", .{&bindings.transformFeedbackVaryings});
        try load("glGetTransformFeedbackVarying", .{&bindings.getTransformFeedbackVarying});
        try load("glClampColor", .{&bindings.clampColor});
        try load("glBeginConditionalRender", .{&bindings.beginConditionalRender});
        try load("glEndConditionalRender", .{&bindings.endConditionalRender});
        try load("glVertexAttribIPointer", .{&bindings.vertexAttribIPointer});
        try load("glGetVertexAttribIiv", .{&bindings.getVertexAttribIiv});
        try load("glGetVertexAttribIuiv", .{&bindings.getVertexAttribIuiv});
        try load("glVertexAttribI1i", .{&bindings.vertexAttribI1i});
        try load("glVertexAttribI2i", .{&bindings.vertexAttribI2i});
        try load("glVertexAttribI3i", .{&bindings.vertexAttribI3i});
        try load("glVertexAttribI4i", .{&bindings.vertexAttribI4i});
        try load("glVertexAttribI1ui", .{&bindings.vertexAttribI1ui});
        try load("glVertexAttribI2ui", .{&bindings.vertexAttribI2ui});
        try load("glVertexAttribI3ui", .{&bindings.vertexAttribI3ui});
        try load("glVertexAttribI4ui", .{&bindings.vertexAttribI4ui});
        try load("glVertexAttribI1iv", .{&bindings.vertexAttribI1iv});
        try load("glVertexAttribI2iv", .{&bindings.vertexAttribI2iv});
        try load("glVertexAttribI3iv", .{&bindings.vertexAttribI3iv});
        try load("glVertexAttribI4iv", .{&bindings.vertexAttribI4iv});
        try load("glVertexAttribI1uiv", .{&bindings.vertexAttribI1uiv});
        try load("glVertexAttribI2uiv", .{&bindings.vertexAttribI2uiv});
        try load("glVertexAttribI3uiv", .{&bindings.vertexAttribI3uiv});
        try load("glVertexAttribI4uiv", .{&bindings.vertexAttribI4uiv});
        try load("glVertexAttribI4bv", .{&bindings.vertexAttribI4bv});
        try load("glVertexAttribI4sv", .{&bindings.vertexAttribI4sv});
        try load("glVertexAttribI4ubv", .{&bindings.vertexAttribI4ubv});
        try load("glVertexAttribI4usv", .{&bindings.vertexAttribI4usv});
        try load("glGetUniformuiv", .{&bindings.getUniformuiv});
        try load("glBindFragDataLocation", .{&bindings.bindFragDataLocation});
        try load("glGetFragDataLocation", .{&bindings.getFragDataLocation});
        try load("glUniform1ui", .{&bindings.uniform1ui});
        try load("glUniform2ui", .{&bindings.uniform2ui});
        try load("glUniform3ui", .{&bindings.uniform3ui});
        try load("glUniform4ui", .{&bindings.uniform4ui});
        try load("glUniform1uiv", .{&bindings.uniform1uiv});
        try load("glUniform2uiv", .{&bindings.uniform2uiv});
        try load("glUniform3uiv", .{&bindings.uniform3uiv});
        try load("glUniform4uiv", .{&bindings.uniform4uiv});
        try load("glTexParameterIiv", .{&bindings.texParameterIiv});
        try load("glTexParameterIuiv", .{&bindings.texParameterIuiv});
        try load("glGetTexParameterIiv", .{&bindings.getTexParameterIiv});
        try load("glGetTexParameterIuiv", .{&bindings.getTexParameterIuiv});
        try load("glClearBufferiv", .{&bindings.clearBufferiv});
        try load("glClearBufferuiv", .{&bindings.clearBufferuiv});
        try load("glClearBufferfv", .{&bindings.clearBufferfv});
        try load("glClearBufferfi", .{&bindings.clearBufferfi});
        try load("glGetStringi", .{&bindings.getStringi});
        try load("glIsRenderbuffer", .{&bindings.isRenderbuffer});
        try load("glBindRenderbuffer", .{&bindings.bindRenderbuffer});
        try load("glDeleteRenderbuffers", .{&bindings.deleteRenderbuffers});
        try load("glGenRenderbuffers", .{&bindings.genRenderbuffers});
        try load("glRenderbufferStorage", .{&bindings.renderbufferStorage});
        try load("glGetRenderbufferParameteriv", .{&bindings.getRenderbufferParameteriv});
        try load("glIsFramebuffer", .{&bindings.isFramebuffer});
        try load("glBindFramebuffer", .{&bindings.bindFramebuffer});
        try load("glDeleteFramebuffers", .{&bindings.deleteFramebuffers});
        try load("glGenFramebuffers", .{&bindings.genFramebuffers});
        try load("glCheckFramebufferStatus", .{&bindings.checkFramebufferStatus});
        try load("glFramebufferTexture1D", .{&bindings.framebufferTexture1D});
        try load("glFramebufferTexture2D", .{&bindings.framebufferTexture2D});
        try load("glFramebufferTexture3D", .{&bindings.framebufferTexture3D});
        try load("glFramebufferRenderbuffer", .{&bindings.framebufferRenderbuffer});
        try load("glGetFramebufferAttachmentParameteriv", .{&bindings.getFramebufferAttachmentParameteriv});
        try load("glGenerateMipmap", .{&bindings.generateMipmap});
        try load("glBlitFramebuffer", .{&bindings.blitFramebuffer});
        try load("glRenderbufferStorageMultisample", .{&bindings.renderbufferStorageMultisample});
        try load("glFramebufferTextureLayer", .{&bindings.framebufferTextureLayer});
        try load("glMapBufferRange", .{&bindings.mapBufferRange});
        try load("glFlushMappedBufferRange", .{&bindings.flushMappedBufferRange});
        try load("glBindVertexArray", .{&bindings.bindVertexArray});
        try load("glDeleteVertexArrays", .{&bindings.deleteVertexArrays});
        try load("glGenVertexArrays", .{&bindings.genVertexArrays});
        try load("glIsVertexArray", .{&bindings.isVertexArray});
    }

    // OpenGL 3.1
    if (ver >= 31) {
        try load("glDrawArraysInstanced", .{&bindings.drawArraysInstanced});
        try load("glDrawElementsInstanced", .{&bindings.drawElementsInstanced});
        try load("glTexBuffer", .{&bindings.texBuffer});
        try load("glPrimitiveRestartIndex", .{&bindings.primitiveRestartIndex});
        try load("glCopyBufferSubData", .{&bindings.copyBufferSubData});
        try load("glGetUniformIndices", .{&bindings.getUniformIndices});
        try load("glGetActiveUniformsiv", .{&bindings.getActiveUniformsiv});
        try load("glGetActiveUniformName", .{&bindings.getActiveUniformName});
        try load("glGetUniformBlockIndex", .{&bindings.getUniformBlockIndex});
        try load("glGetActiveUniformBlockiv", .{&bindings.getActiveUniformBlockiv});
        try load("glGetActiveUniformBlockName", .{&bindings.getActiveUniformBlockName});
        try load("glUniformBlockBinding", .{&bindings.uniformBlockBinding});
    }

    // OpenGL 3.2
    if (ver >= 32) {
        try load("glDrawElementsBaseVertex", .{&bindings.drawElementsBaseVertex});
        try load("glDrawRangeElementsBaseVertex", .{&bindings.drawRangeElementsBaseVertex});
        try load("glDrawElementsInstancedBaseVertex", .{&bindings.drawElementsInstancedBaseVertex});
        try load("glMultiDrawElementsBaseVertex", .{&bindings.multiDrawElementsBaseVertex});
        try load("glProvokingVertex", .{&bindings.provokingVertex});
        try load("glFenceSync", .{&bindings.fenceSync});
        try load("glIsSync", .{&bindings.isSync});
        try load("glDeleteSync", .{&bindings.deleteSync});
        try load("glClientWaitSync", .{&bindings.clientWaitSync});
        try load("glWaitSync", .{&bindings.waitSync});
        try load("glGetInteger64v", .{&bindings.getInteger64v});
        try load("glGetSynciv", .{&bindings.getSynciv});
        try load("glGetInteger64i_v", .{&bindings.getInteger64i_v});
        try load("glGetBufferParameteri64v", .{&bindings.getBufferParameteri64v});
        try load("glFramebufferTexture", .{&bindings.framebufferTexture});
        try load("glTexImage2DMultisample", .{&bindings.texImage2DMultisample});
        try load("glTexImage3DMultisample", .{&bindings.texImage3DMultisample});
        try load("glGetMultisamplefv", .{&bindings.getMultisamplefv});
        try load("glSampleMaski", .{&bindings.sampleMaski});
    }

    // OpenGL 3.3
    if (ver >= 33) {
        try load("glBindFragDataLocationIndexed", .{&bindings.bindFragDataLocationIndexed});
        try load("glGetFragDataIndex", .{&bindings.getFragDataIndex});
        try load("glGenSamplers", .{&bindings.genSamplers});
        try load("glDeleteSamplers", .{&bindings.deleteSamplers});
        try load("glIsSampler", .{&bindings.isSampler});
        try load("glBindSampler", .{&bindings.bindSampler});
        try load("glSamplerParameteri", .{&bindings.samplerParameteri});
        try load("glSamplerParameteriv", .{&bindings.samplerParameteriv});
        try load("glSamplerParameterf", .{&bindings.samplerParameterf});
        try load("glSamplerParameterfv", .{&bindings.samplerParameterfv});
        try load("glSamplerParameterIiv", .{&bindings.samplerParameterIiv});
        try load("glSamplerParameterIuiv", .{&bindings.samplerParameterIuiv});
        try load("glGetSamplerParameteriv", .{&bindings.getSamplerParameteriv});
        try load("glGetSamplerParameterIiv", .{&bindings.getSamplerParameterIiv});
        try load("glGetSamplerParameterfv", .{&bindings.getSamplerParameterfv});
        try load("glGetSamplerParameterIuiv", .{&bindings.getSamplerParameterIuiv});
        try load("glQueryCounter", .{&bindings.queryCounter});
        try load("glGetQueryObjecti64v", .{&bindings.getQueryObjecti64v});
        try load("glGetQueryObjectui64v", .{&bindings.getQueryObjectui64v});
        try load("glVertexAttribDivisor", .{&bindings.vertexAttribDivisor});
        try load("glVertexAttribP1ui", .{&bindings.vertexAttribP1ui});
        try load("glVertexAttribP1uiv", .{&bindings.vertexAttribP1uiv});
        try load("glVertexAttribP2ui", .{&bindings.vertexAttribP2ui});
        try load("glVertexAttribP2uiv", .{&bindings.vertexAttribP2uiv});
        try load("glVertexAttribP3ui", .{&bindings.vertexAttribP3ui});
        try load("glVertexAttribP3uiv", .{&bindings.vertexAttribP3uiv});
        try load("glVertexAttribP4ui", .{&bindings.vertexAttribP4ui});
        try load("glVertexAttribP4uiv", .{&bindings.vertexAttribP4uiv});

        // TODO: where do these belong?
        // try load("glVertexP2ui", .{&bindings.vertexP2ui});
        // try load("glVertexP2uiv", .{&bindings.vertexP2uiv});
        // try load("glVertexP3ui", .{&bindings.vertexP3ui});
        // try load("glVertexP3uiv", .{&bindings.vertexP3uiv});
        // try load("glVertexP4ui", .{&bindings.vertexP4ui});
        // try load("glVertexP4uiv", .{&bindings.vertexP4uiv});
        // try load("glTexCoordP1ui", .{&bindings.texCoordP1ui});
        // try load("glTexCoordP1uiv", .{&bindings.texCoordP1uiv});
        // try load("glTexCoordP2ui", .{&bindings.texCoordP2ui});
        // try load("glTexCoordP2uiv", .{&bindings.texCoordP2uiv});
        // try load("glTexCoordP3ui", .{&bindings.texCoordP3ui});
        // try load("glTexCoordP3uiv", .{&bindings.texCoordP3uiv});
        // try load("glTexCoordP4ui", .{&bindings.texCoordP4ui});
        // try load("glTexCoordP4uiv", .{&bindings.texCoordP4uiv});
        // try load("glMultiTexCoordP1ui", .{&bindings.multiTexCoordP1ui});
        // try load("glMultiTexCoordP1uiv", .{&bindings.multiTexCoordP1uiv});
        // try load("glMultiTexCoordP2ui", .{&bindings.multiTexCoordP2ui});
        // try load("glMultiTexCoordP2uiv", .{&bindings.multiTexCoordP2uiv});
        // try load("glMultiTexCoordP3ui", .{&bindings.multiTexCoordP3ui});
        // try load("glMultiTexCoordP3uiv", .{&bindings.multiTexCoordP3uiv});
        // try load("glMultiTexCoordP4ui", .{&bindings.multiTexCoordP4ui});
        // try load("glMultiTexCoordP4uiv", .{&bindings.multiTexCoordP4uiv});
        // try load("glNormalP3ui", .{&bindings.normalP3ui});
        // try load("glNormalP3uiv", .{&bindings.normalP3uiv});
        // try load("glColorP3ui", .{&bindings.colorP3ui});
        // try load("glColorP3uiv", .{&bindings.colorP3uiv});
        // try load("glColorP4ui", .{&bindings.colorP4ui});
        // try load("glColorP4uiv", .{&bindings.colorP4uiv});
        // try load("glSecondaryColorP3ui", .{&bindings.secondaryColorP3ui});
        // try load("glSecondaryColorP3uiv", .{&bindings.secondaryColorP3uiv});
    }

    // OpenGL 4.0
    if (ver >= 40) {
        try load("glMinSampleShading", .{&bindings.minSampleShading});
        try load("glBlendEquationi", .{&bindings.blendEquationi});
        try load("glBlendEquationSeparatei", .{&bindings.blendEquationSeparatei});
        try load("glBlendFunci", .{&bindings.blendFunci});
        try load("glBlendFuncSeparatei", .{&bindings.blendFuncSeparatei});
        try load("glDrawArraysIndirect", .{&bindings.drawArraysIndirect});
        try load("glDrawElementsIndirect", .{&bindings.drawElementsIndirect});
        try load("glUniform1d", .{&bindings.uniform1d});
        try load("glUniform2d", .{&bindings.uniform2d});
        try load("glUniform3d", .{&bindings.uniform3d});
        try load("glUniform4d", .{&bindings.uniform4d});
        try load("glUniform1dv", .{&bindings.uniform1dv});
        try load("glUniform2dv", .{&bindings.uniform2dv});
        try load("glUniform3dv", .{&bindings.uniform3dv});
        try load("glUniform4dv", .{&bindings.uniform4dv});
        try load("glUniformMatrix2dv", .{&bindings.uniformMatrix2dv});
        try load("glUniformMatrix3dv", .{&bindings.uniformMatrix3dv});
        try load("glUniformMatrix4dv", .{&bindings.uniformMatrix4dv});
        try load("glUniformMatrix2x3dv", .{&bindings.uniformMatrix2x3dv});
        try load("glUniformMatrix2x4dv", .{&bindings.uniformMatrix2x4dv});
        try load("glUniformMatrix3x2dv", .{&bindings.uniformMatrix3x2dv});
        try load("glUniformMatrix3x4dv", .{&bindings.uniformMatrix3x4dv});
        try load("glUniformMatrix4x2dv", .{&bindings.uniformMatrix4x2dv});
        try load("glUniformMatrix4x3dv", .{&bindings.uniformMatrix4x3dv});
        try load("glGetUniformdv", .{&bindings.getUniformdv});
        try load("glGetSubroutineUniformLocation", .{&bindings.getSubroutineUniformLocation});
        try load("glGetSubroutineIndex", .{&bindings.getSubroutineIndex});
        try load("glGetActiveSubroutineUniformiv", .{&bindings.getActiveSubroutineUniformiv});
        try load("glGetActiveSubroutineUniformName", .{&bindings.getActiveSubroutineUniformName});
        try load("glGetActiveSubroutineName", .{&bindings.getActiveSubroutineName});
        try load("glUniformSubroutinesuiv", .{&bindings.uniformSubroutinesuiv});
        try load("glGetUniformSubroutineuiv", .{&bindings.getUniformSubroutineuiv});
        try load("glGetProgramStageiv", .{&bindings.getProgramStageiv});
        try load("glPatchParameteri", .{&bindings.patchParameteri});
        try load("glPatchParameterfv", .{&bindings.patchParameterfv});
        try load("glBindTransformFeedback", .{&bindings.bindTransformFeedback});
        try load("glDeleteTransformFeedbacks", .{&bindings.deleteTransformFeedbacks});
        try load("glGenTransformFeedbacks", .{&bindings.genTransformFeedbacks});
        try load("glIsTransformFeedback", .{&bindings.isTransformFeedback});
        try load("glPauseTransformFeedback", .{&bindings.pauseTransformFeedback});
        try load("glResumeTransformFeedback", .{&bindings.resumeTransformFeedback});
        try load("glDrawTransformFeedback", .{&bindings.drawTransformFeedback});
        try load("glDrawTransformFeedbackStream", .{&bindings.drawTransformFeedbackStream});
        try load("glBeginQueryIndexed", .{&bindings.beginQueryIndexed});
        try load("glEndQueryIndexed", .{&bindings.endQueryIndexed});
        try load("glGetQueryIndexediv", .{&bindings.glGetQueryIndexediv});
    }

    // OpenGL 4.1
    if (ver >= 41) {
        try load("glReleaseShaderCompiler", .{&bindings.releaseShaderCompiler});
        try load("glShaderBinary", .{&bindings.shaderBinary});
        try load("glGetShaderPrecisionFormat", .{&bindings.getShaderPrecisionFormat});
        try load("glDepthRangef", .{&bindings.depthRangef});
        try load("glClearDepthf", .{&bindings.clearDepthf});
        try load("glGetProgramBinary", .{&bindings.getProgramBinary});
        try load("glProgramBinary", .{&bindings.programBinary});
        try load("glProgramParameteri", .{&bindings.programParameteri});
        try load("glUseProgramStages", .{&bindings.useProgramStages});
        try load("glActiveShaderProgram", .{&bindings.activeShaderProgram});
        try load("glCreateShaderProgramv", .{&bindings.createShaderProgramv});
        try load("glBindProgramPipeline", .{&bindings.bindProgramPipeline});
        try load("glDeleteProgramPipelines", .{&bindings.deleteProgramPipelines});
        try load("glGenProgramPipelines", .{&bindings.genProgramPipelines});
        try load("glIsProgramPipeline", .{&bindings.isProgramPipeline});
        try load("glGetProgramPipelineiv", .{&bindings.getProgramPipelineiv});
        try load("glProgramUniform1i", .{&bindings.programUniform1i});
        try load("glProgramUniform2i", .{&bindings.programUniform2i});
        try load("glProgramUniform3i", .{&bindings.programUniform3i});
        try load("glProgramUniform4i", .{&bindings.programUniform4i});
        try load("glProgramUniform1ui", .{&bindings.programUniform1ui});
        try load("glProgramUniform2ui", .{&bindings.programUniform2ui});
        try load("glProgramUniform3ui", .{&bindings.programUniform3ui});
        try load("glProgramUniform4ui", .{&bindings.programUniform4ui});
        try load("glProgramUniform1f", .{&bindings.programUniform1f});
        try load("glProgramUniform2f", .{&bindings.programUniform2f});
        try load("glProgramUniform3f", .{&bindings.programUniform3f});
        try load("glProgramUniform4f", .{&bindings.programUniform4f});
        try load("glProgramUniform1d", .{&bindings.programUniform1d});
        try load("glProgramUniform2d", .{&bindings.programUniform2d});
        try load("glProgramUniform3d", .{&bindings.programUniform3d});
        try load("glProgramUniform4d", .{&bindings.programUniform4d});
        try load("glProgramUniform1iv", .{&bindings.programUniform1iv});
        try load("glProgramUniform2iv", .{&bindings.programUniform2iv});
        try load("glProgramUniform3iv", .{&bindings.programUniform3iv});
        try load("glProgramUniform4iv", .{&bindings.programUniform4iv});
        try load("glProgramUniform1uiv", .{&bindings.programUniform1uiv});
        try load("glProgramUniform2uiv", .{&bindings.programUniform2uiv});
        try load("glProgramUniform3uiv", .{&bindings.programUniform3uiv});
        try load("glProgramUniform4uiv", .{&bindings.programUniform4uiv});
        try load("glProgramUniform1fv", .{&bindings.programUniform1fv});
        try load("glProgramUniform2fv", .{&bindings.programUniform2fv});
        try load("glProgramUniform3fv", .{&bindings.programUniform3fv});
        try load("glProgramUniform4fv", .{&bindings.programUniform4fv});
        try load("glProgramUniform1dv", .{&bindings.programUniform1dv});
        try load("glProgramUniform2dv", .{&bindings.programUniform2dv});
        try load("glProgramUniform3dv", .{&bindings.programUniform3dv});
        try load("glProgramUniform4dv", .{&bindings.programUniform4dv});
        try load("glProgramUniformMatrix2fv", .{&bindings.programUniformMatrix2fv});
        try load("glProgramUniformMatrix3fv", .{&bindings.programUniformMatrix3fv});
        try load("glProgramUniformMatrix4fv", .{&bindings.programUniformMatrix4fv});
        try load("glProgramUniformMatrix2dv", .{&bindings.programUniformMatrix2dv});
        try load("glProgramUniformMatrix3dv", .{&bindings.programUniformMatrix3dv});
        try load("glProgramUniformMatrix4dv", .{&bindings.programUniformMatrix4dv});
        try load("glProgramUniformMatrix2x3fv", .{&bindings.programUniformMatrix2x3fv});
        try load("glProgramUniformMatrix3x2fv", .{&bindings.programUniformMatrix3x2fv});
        try load("glProgramUniformMatrix2x4fv", .{&bindings.programUniformMatrix2x4fv});
        try load("glProgramUniformMatrix4x2fv", .{&bindings.programUniformMatrix4x2fv});
        try load("glProgramUniformMatrix3x4fv", .{&bindings.programUniformMatrix3x4fv});
        try load("glProgramUniformMatrix4x3fv", .{&bindings.programUniformMatrix4x3fv});
        try load("glProgramUniformMatrix2x3dv", .{&bindings.programUniformMatrix2x3dv});
        try load("glProgramUniformMatrix3x2dv", .{&bindings.programUniformMatrix3x2dv});
        try load("glProgramUniformMatrix2x4dv", .{&bindings.programUniformMatrix2x4dv});
        try load("glProgramUniformMatrix4x2dv", .{&bindings.programUniformMatrix4x2dv});
        try load("glProgramUniformMatrix3x4dv", .{&bindings.programUniformMatrix3x4dv});
        try load("glProgramUniformMatrix4x3dv", .{&bindings.programUniformMatrix4x3dv});
        try load("glValidateProgramPipeline", .{&bindings.validateProgramPipeline});
        try load("glGetProgramPipelineInfoLog", .{&bindings.getProgramPipelineInfoLog});
        try load("glVertexAttribL1d", .{&bindings.vertexAttribL1d});
        try load("glVertexAttribL2d", .{&bindings.vertexAttribL2d});
        try load("glVertexAttribL3d", .{&bindings.vertexAttribL3d});
        try load("glVertexAttribL4d", .{&bindings.vertexAttribL4d});
        try load("glVertexAttribL1dv", .{&bindings.vertexAttribL1dv});
        try load("glVertexAttribL2dv", .{&bindings.vertexAttribL2dv});
        try load("glVertexAttribL3dv", .{&bindings.vertexAttribL3dv});
        try load("glVertexAttribL4dv", .{&bindings.vertexAttribL4dv});
        try load("glViewportArrayv", .{&bindings.viewportArrayv});
        try load("glViewportIndexedf", .{&bindings.viewportIndexedf});
        try load("glViewportIndexedfv", .{&bindings.viewportIndexedfv});
        try load("glScissorArrayv", .{&bindings.scissorArrayv});
        try load("glScissorIndexed", .{&bindings.scissorIndexed});
        try load("glScissorIndexedv", .{&bindings.scissorIndexedv});
        try load("glDepthRangeArrayv", .{&bindings.depthRangeArrayv});
        try load("glDepthRangeIndexed", .{&bindings.depthRangeIndexed});
        try load("glGetFloati_v", .{&bindings.getFloati_v});
        try load("glGetDoublei_v", .{&bindings.getDoublei_v});
    }

    // OpenGL 4.2
    if (ver >= 42) {
        try load("glDrawArraysInstancedBaseInstance", .{&bindings.drawArraysInstancedBaseInstance});
        try load("glDrawElementsInstancedBaseInstance", .{&bindings.drawElementsInstancedBaseInstance});
        try load("glDrawElementsInstancedBaseVertexBaseInstance", .{&bindings.drawElementsInstancedBaseVertexBaseInstance});
        try load("glGetInternalformativ", .{&bindings.getInternalformativ});
        try load("glGetActiveAtomicCounterBufferiv", .{&bindings.getActiveAtomicCounterBufferiv});
        try load("glBindImageTexture", .{&bindings.bindImageTexture});
        try load("glMemoryBarrier", .{&bindings.memoryBarrier});
        try load("glTexStorage1D", .{&bindings.texStorage1D});
        try load("glTexStorage2D", .{&bindings.texStorage2D});
        try load("glTexStorage3D", .{&bindings.texStorage3D});
        try load("glDrawTransformFeedbackInstanced", .{&bindings.drawTransformFeedbackInstanced});
        try load("glDrawTransformFeedbackStreamInstanced", .{&bindings.drawTransformFeedbackStreamInstanced});
    }

    // OpenGL 4.3
    if (ver >= 43) {
        try load("glDebugMessageControl", .{&bindings.debugMessageControl});
        try load("glDebugMessageInsert", .{&bindings.debugMessageInsert});
        try load("glDebugMessageCallback", .{&bindings.debugMessageCallback});
        try load("glGetDebugMessageLog", .{&bindings.getDebugMessageLog});
        try load("glGetPointerv", .{&bindings.getPointerv});
        try load("glPushDebugGroup", .{&bindings.pushDebugGroup});
        try load("glPopDebugGroup", .{&bindings.popDebugGroup});
        try load("glObjectLabel", .{&bindings.objectLabel});
        try load("glGetObjectLabel", .{&bindings.getObjectLabel});
        try load("glObjectPtrLabel", .{&bindings.objectPtrLabel});
        try load("glGetObjectPtrLabel", .{&bindings.getObjectPtrLabel});
        try load("glGetProgramResourceIndex", .{&bindings.getProgramResourceIndex});
        try load("glShaderStorageBlockBinding", .{&bindings.shaderStorageBlockBinding});
        // TODO
    }

    // OpenGL 4.4
    if (ver >= 44) {
        try load("glClearTexImage", .{&bindings.clearTexImage});
        // TODO
    }

    // OpenGL 4.5
    if (ver >= 45) {
        try load("glTextureStorage2D", .{&bindings.textureStorage2D});
        try load("glTextureStorage2DMultisample", .{&bindings.textureStorage2DMultisample});
        try load("glCreateTextures", .{&bindings.createTextures});
        try load("glCreateFramebuffers", .{&bindings.createFramebuffers});
        try load("glNamedFramebufferTexture", .{&bindings.namedFramebufferTexture});
        try load("glBlitNamedFramebuffer", .{&bindings.blitNamedFramebuffer});
        try load("glCreateBuffers", .{&bindings.createBuffers});
        try load("glClearNamedFramebufferfv", .{&bindings.clearNamedFramebufferfv});
        try load("glNamedBufferStorage", .{&bindings.namedBufferStorage});
        try load("glBindTextureUnit", .{&bindings.bindTextureUnit});
        try load("glTextureBarrier", .{&bindings.textureBarrier});
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

    try load("glBegin", .{&bindings.begin});
    try load("glEnd", .{&bindings.end});
    try load("glNewList", .{&bindings.newList});
    try load("glCallList", .{&bindings.callList});
    try load("glEndList", .{&bindings.endList});
    try load("glLoadIdentity", .{&bindings.loadIdentity});
    try load("glVertex2fv", .{&bindings.vertex2fv});
    try load("glVertex3fv", .{&bindings.vertex3fv});
    try load("glVertex4fv", .{&bindings.vertex4fv});
    try load("glColor3fv", .{&bindings.color3fv});
    try load("glColor4fv", .{&bindings.color4fv});
    try load("glRectf", .{&bindings.rectf});
    try load("glMatrixMode", .{&bindings.matrixMode});
    try load("glVertex2f", .{&bindings.vertex2f});
    try load("glVertex2d", .{&bindings.vertex2d});
    try load("glVertex2i", .{&bindings.vertex2i});
    try load("glColor3f", .{&bindings.color3f});
    try load("glColor4f", .{&bindings.color4f});
    try load("glColor4ub", .{&bindings.color4ub});
    try load("glPushMatrix", .{&bindings.pushMatrix});
    try load("glPopMatrix", .{&bindings.popMatrix});
    try load("glRotatef", .{&bindings.rotatef});
    try load("glScalef", .{&bindings.scalef});
    try load("glTranslatef", .{&bindings.translatef});
    try load("glMatrixLoadIdentityEXT", .{&bindings.matrixLoadIdentityEXT});
    try load("glMatrixOrthoEXT", .{&bindings.matrixOrthoEXT});
}

pub fn loadEsProfile(loader: LoaderFn, major: u32, minor: u32) !void {
    const ver = 10 * major + minor;

    assert(major >= 1 and major <= 3);
    assert(minor >= 0 and minor <= 2);
    assert(ver >= 10 and ver <= 32);

    loaderFunc = loader;

    // OpenGL ES 1.0
    if (ver >= 10) {
        try load("glCullFace", .{&bindings.cullFace});
        try load("glFrontFace", .{&bindings.frontFace});
        try load("glHint", .{&bindings.hint});
        try load("glLineWidth", .{&bindings.lineWidth});
        try load("glScissor", .{&bindings.scissor});
        try load("glTexParameterf", .{&bindings.texParameterf});
        try load("glTexParameterfv", .{&bindings.texParameterfv});
        try load("glTexParameteri", .{&bindings.texParameteri});
        try load("glTexParameteriv", .{&bindings.texParameteriv});
        try load("glTexImage2D", .{&bindings.texImage2D});
        try load("glClear", .{&bindings.clear});
        try load("glClearColor", .{&bindings.clearColor});
        try load("glClearStencil", .{&bindings.clearStencil});
        try load("glClearDepthf", .{&bindings.clearDepthf});
        try load("glStencilMask", .{&bindings.stencilMask});
        try load("glColorMask", .{&bindings.colorMask});
        try load("glDepthMask", .{&bindings.depthMask});
        try load("glDisable", .{&bindings.disable});
        try load("glEnable", .{&bindings.enable});
        try load("glFinish", .{&bindings.finish});
        try load("glFlush", .{&bindings.flush});
        try load("glBlendFunc", .{&bindings.blendFunc});
        try load("glStencilFunc", .{&bindings.stencilFunc});
        try load("glStencilOp", .{&bindings.stencilOp});
        try load("glDepthFunc", .{&bindings.depthFunc});
        try load("glPixelStorei", .{&bindings.pixelStorei});
        try load("glReadPixels", .{&bindings.readPixels});
        try load("glGetBooleanv", .{&bindings.getBooleanv});
        try load("glGetError", .{&bindings.getError});
        try load("glGetFloatv", .{&bindings.getFloatv});
        try load("glGetIntegerv", .{&bindings.getIntegerv});
        try load("glGetString", .{&bindings.getString});
        try load("glIsEnabled", .{&bindings.isEnabled});
        try load("glDepthRangef", .{&bindings.depthRangef});
        try load("glViewport", .{&bindings.viewport});
        try load("glDrawArrays", .{&bindings.drawArrays});
        try load("glDrawElements", .{&bindings.drawElements});
        try load("glPolygonOffset", .{&bindings.polygonOffset});
        try load("glCopyTexImage2D", .{&bindings.copyTexImage2D});
        try load("glCopyTexSubImage2D", .{&bindings.copyTexSubImage2D});
        try load("glTexSubImage2D", .{&bindings.texSubImage2D});
        try load("glBindTexture", .{&bindings.bindTexture});
        try load("glDeleteTextures", .{&bindings.deleteTextures});
        try load("glGenTextures", .{&bindings.genTextures});
        try load("glIsTexture", .{&bindings.isTexture});
        try load("glActiveTexture", .{&bindings.activeTexture});
        try load("glSampleCoverage", .{&bindings.sampleCoverage});
        try load("glCompressedTexImage2D", .{&bindings.compressedTexImage2D});
        try load("glCompressedTexSubImage2D", .{&bindings.compressedTexSubImage2D});
    }

    // OpenGL ES 1.1
    if (ver >= 11) {
        try load("glBlendFuncSeparate", .{&bindings.blendFuncSeparate});
        try load("glBlendColor", .{&bindings.blendColor});
        try load("glBlendEquation", .{&bindings.blendEquation});
        try load("glBindBuffer", .{&bindings.bindBuffer});
        try load("glDeleteBuffers", .{&bindings.deleteBuffers});
        try load("glGenBuffers", .{&bindings.genBuffers});
        try load("glIsBuffer", .{&bindings.isBuffer});
        try load("glBufferData", .{&bindings.bufferData});
        try load("glBufferSubData", .{&bindings.bufferSubData});
        try load("glGetBufferParameteriv", .{&bindings.getBufferParameteriv});
    }

    // OpenGL ES 2.0
    if (ver >= 20) {
        try load("glBlendEquationSeparate", .{&bindings.blendEquationSeparate});
        try load("glStencilOpSeparate", .{&bindings.stencilOpSeparate});
        try load("glStencilFuncSeparate", .{&bindings.stencilFuncSeparate});
        try load("glStencilMaskSeparate", .{&bindings.stencilMaskSeparate});
        try load("glAttachShader", .{&bindings.attachShader});
        try load("glBindAttribLocation", .{&bindings.bindAttribLocation});
        try load("glCompileShader", .{&bindings.compileShader});
        try load("glCreateProgram", .{&bindings.createProgram});
        try load("glCreateShader", .{&bindings.createShader});
        try load("glDeleteProgram", .{&bindings.deleteProgram});
        try load("glDeleteShader", .{&bindings.deleteShader});
        try load("glDetachShader", .{&bindings.detachShader});
        try load("glDisableVertexAttribArray", .{&bindings.disableVertexAttribArray});
        try load("glEnableVertexAttribArray", .{&bindings.enableVertexAttribArray});
        try load("glGetActiveAttrib", .{&bindings.getActiveAttrib});
        try load("glGetActiveUniform", .{&bindings.getActiveUniform});
        try load("glGetAttachedShaders", .{&bindings.getAttachedShaders});
        try load("glGetAttribLocation", .{&bindings.getAttribLocation});
        try load("glGetProgramiv", .{&bindings.getProgramiv});
        try load("glGetProgramInfoLog", .{&bindings.getProgramInfoLog});
        try load("glGetShaderiv", .{&bindings.getShaderiv});
        try load("glGetShaderInfoLog", .{&bindings.getShaderInfoLog});
        try load("glGetShaderSource", .{&bindings.getShaderSource});
        try load("glGetUniformLocation", .{&bindings.getUniformLocation});
        try load("glGetUniformfv", .{&bindings.getUniformfv});
        try load("glGetUniformiv", .{&bindings.getUniformiv});
        try load("glGetVertexAttribPointerv", .{&bindings.getVertexAttribPointerv});
        try load("glIsProgram", .{&bindings.isProgram});
        try load("glIsShader", .{&bindings.isShader});
        try load("glLinkProgram", .{&bindings.linkProgram});
        try load("glShaderSource", .{&bindings.shaderSource});
        try load("glUseProgram", .{&bindings.useProgram});
        try load("glUniform1f", .{&bindings.uniform1f});
        try load("glUniform2f", .{&bindings.uniform2f});
        try load("glUniform3f", .{&bindings.uniform3f});
        try load("glUniform4f", .{&bindings.uniform4f});
        try load("glUniform1i", .{&bindings.uniform1i});
        try load("glUniform2i", .{&bindings.uniform2i});
        try load("glUniform3i", .{&bindings.uniform3i});
        try load("glUniform4i", .{&bindings.uniform4i});
        try load("glUniform1fv", .{&bindings.uniform1fv});
        try load("glUniform2fv", .{&bindings.uniform2fv});
        try load("glUniform3fv", .{&bindings.uniform3fv});
        try load("glUniform4fv", .{&bindings.uniform4fv});
        try load("glUniform1iv", .{&bindings.uniform1iv});
        try load("glUniform2iv", .{&bindings.uniform2iv});
        try load("glUniform3iv", .{&bindings.uniform3iv});
        try load("glUniform4iv", .{&bindings.uniform4iv});
        try load("glUniformMatrix2fv", .{&bindings.uniformMatrix2fv});
        try load("glUniformMatrix3fv", .{&bindings.uniformMatrix3fv});
        try load("glUniformMatrix4fv", .{&bindings.uniformMatrix4fv});
        try load("glValidateProgram", .{&bindings.validateProgram});
        try load("glVertexAttribPointer", .{&bindings.vertexAttribPointer});
        try load("glIsRenderbuffer", .{&bindings.isRenderbuffer});
        try load("glBindRenderbuffer", .{&bindings.bindRenderbuffer});
        try load("glDeleteRenderbuffers", .{&bindings.deleteRenderbuffers});
        try load("glGenRenderbuffers", .{&bindings.genRenderbuffers});
        try load("glRenderbufferStorage", .{&bindings.renderbufferStorage});
        try load("glGetRenderbufferParameteriv", .{&bindings.getRenderbufferParameteriv});
        try load("glIsFramebuffer", .{&bindings.isFramebuffer});
        try load("glBindFramebuffer", .{&bindings.bindFramebuffer});
        try load("glDeleteFramebuffers", .{&bindings.deleteFramebuffers});
        try load("glGenFramebuffers", .{&bindings.genFramebuffers});
        try load("glCheckFramebufferStatus", .{&bindings.checkFramebufferStatus});
        try load("glFramebufferTexture2D", .{&bindings.framebufferTexture2D});
        try load("glFramebufferRenderbuffer", .{&bindings.framebufferRenderbuffer});
        try load("glGetFramebufferAttachmentParameteriv", .{&bindings.getFramebufferAttachmentParameteriv});
        try load("glGenerateMipmap", .{&bindings.generateMipmap});
    }

    // OpenGL ES 3.0
    if (ver >= 30) {
        try load("glUniformMatrix2x3fv", .{&bindings.uniformMatrix2x3fv});
        try load("glUniformMatrix3x2fv", .{&bindings.uniformMatrix3x2fv});
        try load("glUniformMatrix2x4fv", .{&bindings.uniformMatrix2x4fv});
        try load("glUniformMatrix4x2fv", .{&bindings.uniformMatrix4x2fv});
        try load("glUniformMatrix3x4fv", .{&bindings.uniformMatrix3x4fv});
        try load("glUniformMatrix4x3fv", .{&bindings.uniformMatrix4x3fv});
        try load("glGetBooleani_v", .{&bindings.getBooleani_v});
        try load("glGetIntegeri_v", .{&bindings.getIntegeri_v});
        try load("glBeginTransformFeedback", .{&bindings.beginTransformFeedback});
        try load("glEndTransformFeedback", .{&bindings.endTransformFeedback});
        try load("glBindBufferRange", .{&bindings.bindBufferRange});
        try load("glBindBufferBase", .{&bindings.bindBufferBase});
        try load("glTransformFeedbackVaryings", .{&bindings.transformFeedbackVaryings});
        try load("glGetTransformFeedbackVarying", .{&bindings.getTransformFeedbackVarying});
        try load("glVertexAttribIPointer", .{&bindings.vertexAttribIPointer});
        try load("glGetVertexAttribIiv", .{&bindings.getVertexAttribIiv});
        try load("glGetVertexAttribIuiv", .{&bindings.getVertexAttribIuiv});
        try load("glGetUniformuiv", .{&bindings.getUniformuiv});
        try load("glGetFragDataLocation", .{&bindings.getFragDataLocation});
        try load("glUniform1ui", .{&bindings.uniform1ui});
        try load("glUniform2ui", .{&bindings.uniform2ui});
        try load("glUniform3ui", .{&bindings.uniform3ui});
        try load("glUniform4ui", .{&bindings.uniform4ui});
        try load("glUniform1uiv", .{&bindings.uniform1uiv});
        try load("glUniform2uiv", .{&bindings.uniform2uiv});
        try load("glUniform3uiv", .{&bindings.uniform3uiv});
        try load("glUniform4uiv", .{&bindings.uniform4uiv});
        try load("glClearBufferiv", .{&bindings.clearBufferiv});
        try load("glClearBufferuiv", .{&bindings.clearBufferuiv});
        try load("glClearBufferfv", .{&bindings.clearBufferfv});
        try load("glClearBufferfi", .{&bindings.clearBufferfi});
        try load("glGetStringi", .{&bindings.getStringi});
        try load("glBlitFramebuffer", .{&bindings.blitFramebuffer});
        try load("glRenderbufferStorageMultisample", .{&bindings.renderbufferStorageMultisample});
        try load("glFramebufferTextureLayer", .{&bindings.framebufferTextureLayer});
        try load("glMapBufferRange", .{&bindings.mapBufferRange});
        try load("glFlushMappedBufferRange", .{&bindings.flushMappedBufferRange});
        try load("glBindVertexArray", .{&bindings.bindVertexArray});
        try load("glDeleteVertexArrays", .{&bindings.deleteVertexArrays});
        try load("glGenVertexArrays", .{&bindings.genVertexArrays});
        try load("glIsVertexArray", .{&bindings.isVertexArray});
        try load("glDrawArraysInstanced", .{&bindings.drawArraysInstanced});
        try load("glDrawElementsInstanced", .{&bindings.drawElementsInstanced});
        try load("glCopyBufferSubData", .{&bindings.copyBufferSubData});
        try load("glGetUniformIndices", .{&bindings.getUniformIndices});
        try load("glGetActiveUniformsiv", .{&bindings.getActiveUniformsiv});
        try load("glGetUniformBlockIndex", .{&bindings.getUniformBlockIndex});
        try load("glGetActiveUniformBlockiv", .{&bindings.getActiveUniformBlockiv});
        try load("glGetActiveUniformBlockName", .{&bindings.getActiveUniformBlockName});
        try load("glUniformBlockBinding", .{&bindings.uniformBlockBinding});
        try load("glFenceSync", .{&bindings.fenceSync});
        try load("glIsSync", .{&bindings.isSync});
        try load("glDeleteSync", .{&bindings.deleteSync});
        try load("glClientWaitSync", .{&bindings.clientWaitSync});
        try load("glWaitSync", .{&bindings.waitSync});
        try load("glGetInteger64v", .{&bindings.getInteger64v});
        try load("glGetSynciv", .{&bindings.getSynciv});
        try load("glGetInteger64i_v", .{&bindings.getInteger64i_v});
        try load("glGetBufferParameteri64v", .{&bindings.getBufferParameteri64v});
        try load("glGetMultisamplefv", .{&bindings.getMultisamplefv});
        try load("glSampleMaski", .{&bindings.sampleMaski});
        try load("glGenSamplers", .{&bindings.genSamplers});
        try load("glDeleteSamplers", .{&bindings.deleteSamplers});
        try load("glIsSampler", .{&bindings.isSampler});
        try load("glBindSampler", .{&bindings.bindSampler});
        try load("glSamplerParameteri", .{&bindings.samplerParameteri});
        try load("glSamplerParameteriv", .{&bindings.samplerParameteriv});
        try load("glSamplerParameterf", .{&bindings.samplerParameterf});
        try load("glSamplerParameterfv", .{&bindings.samplerParameterfv});
        try load("glSamplerParameterIiv", .{&bindings.samplerParameterIiv});
        try load("glSamplerParameterIuiv", .{&bindings.samplerParameterIuiv});
        try load("glGetSamplerParameteriv", .{&bindings.getSamplerParameteriv});
        try load("glGetSamplerParameterIiv", .{&bindings.getSamplerParameterIiv});
        try load("glGetSamplerParameterfv", .{&bindings.getSamplerParameterfv});
        try load("glVertexAttribDivisor", .{&bindings.vertexAttribDivisor});
        // TODO: from opengl 4.0 to 4.3 *subset*
    }
}

pub fn loadExtension(loader: LoaderFn, extension: Extension) !void {
    loaderFunc = loader;

    switch (extension) {
        // KHR extensions ////////////////////////////////////////////////////////////////////////////////////
        .KHR_debug => {
            try load("glDebugMessageControl", .{&bindings.debugMessageControl});
            try load("glDebugMessageInsert", .{&bindings.debugMessageInsert});
            try load("glDebugMessageCallback", .{&bindings.debugMessageCallback});
            try load("glGetDebugMessageLog", .{&bindings.getDebugMessageLog});
            try load("glGetPointerv", .{&bindings.getPointerv});
            try load("glPushDebugGroup", .{&bindings.pushDebugGroup});
            try load("glPopDebugGroup", .{&bindings.popDebugGroup});
            try load("glObjectLabel", .{&bindings.objectLabel});
            try load("glGetObjectLabel", .{&bindings.getObjectLabel});
            try load("glObjectPtrLabel", .{&bindings.objectPtrLabel});
            try load("glGetObjectPtrLabel", .{&bindings.getObjectPtrLabel});
        },
        // EXT extensions ////////////////////////////////////////////////////////////////////////////////////
        .EXT_copy_texture => {
            try load("glCopyTexImage1DEXT", .{ &bindings.copyTexImage1DEXT, &bindings.copyTexImage1D });
            try load("glCopyTexImage2DEXT", .{ &bindings.copyTexImage2DEXT, &bindings.copyTexImage2D });
            try load("glCopyTexSubImage1DEXT", .{ &bindings.copyTexSubImage1DEXT, &bindings.copyTexSubImage1D });
            try load("glCopyTexSubImage2DEXT", .{ &bindings.copyTexSubImage2DEXT, &bindings.copyTexSubImage2D });
            try load("glCopyTexSubImage3DEXT", .{ &bindings.copyTexSubImage3DEXT, &bindings.copyTexSubImage3D });
        },
        // NV extensions /////////////////////////////////////////////////////////////////////////////////////
        .NV_bindless_texture => {
            try load("glGetTextureHandleNV", .{&bindings.getTextureHandleNV});
            try load("glMakeTextureHandleResidentNV", .{&bindings.makeTextureHandleResidentNV});
            try load("glProgramUniformHandleui64NV", .{&bindings.programUniformHandleui64NV});
        },
        .NV_shader_buffer_load => {
            try load("glMakeNamedBufferResidentNV", .{&bindings.makeNamedBufferResidentNV});
            try load("glGetNamedBufferParameterui64vNV", .{&bindings.getNamedBufferParameterui64vNV});
            try load("glProgramUniformui64vNV", .{&bindings.programUniformui64NV});
        },
    }
}

pub fn loadEsExtension(loader: LoaderFn, extension: EsExtension) !void {
    loaderFunc = loader;

    switch (extension) {
        // KHR ES extensions /////////////////////////////////////////////////////////////////////////////////
        .KHR_debug => {
            try load("glDebugMessageControlKHR", .{ &bindings.debugMessageControl, &bindings.debugMessageControlKHR });
            try load("glDebugMessageInsertKHR", .{ &bindings.debugMessageInsert, &bindings.debugMessageInsertKHR });
            try load("glDebugMessageCallbackKHR", .{ &bindings.debugMessageCallback, &bindings.debugMessageCallbackKHR });
            try load("glGetDebugMessageLogKHR", .{ &bindings.getDebugMessageLog, &bindings.getDebugMessageLogKHR });
            try load("glGetPointervKHR", .{ &bindings.getPointerv, &bindings.getPointervKHR });
            try load("glPushDebugGroupKHR", .{ &bindings.pushDebugGroup, &bindings.pushDebugGroupKHR });
            try load("glPopDebugGroupKHR", .{ &bindings.popDebugGroup, &bindings.popDebugGroupKHR });
            try load("glObjectLabelKHR", .{ &bindings.objectLabel, &bindings.objectLabelKHR });
            try load("glGetObjectLabelKHR", .{ &bindings.getObjectLabel, &bindings.getObjectLabelKHR });
            try load("glObjectPtrLabelKHR", .{ &bindings.objectPtrLabel, &bindings.objectPtrLabelKHR });
            try load("glGetObjectPtrLabelKHR", .{ &bindings.getObjectPtrLabel, &bindings.getObjectPtrLabelKHR });
        },
        // OES ES extensions /////////////////////////////////////////////////////////////////////////////////
        .OES_vertex_array_object => {
            try load("glBindVertexArrayOES", .{ &bindings.bindVertexArray, &bindings.bindVertexArrayOES });
            try load("glDeleteVertexArraysOES", .{ &bindings.deleteVertexArrays, &bindings.deleteVertexArraysOES });
            try load("glGenVertexArraysOES", .{ &bindings.genVertexArrays, &bindings.genVertexArraysOES });
            try load("glIsVertexArrayOES", .{ &bindings.isVertexArray, &bindings.isVertexArrayOES });
        },
    }
}

pub fn loadWebProfile(loader: LoaderFn, webgl2: bool) !void {
    loaderFunc = loader;

    // OpenGL ES 1.0
    try load("glCullFace", .{&bindings.cullFace});
    try load("glFrontFace", .{&bindings.frontFace});
    try load("glHint", .{&bindings.hint});
    try load("glLineWidth", .{&bindings.lineWidth});
    try load("glScissor", .{&bindings.scissor});
    try load("glTexParameterf", .{&bindings.texParameterf});
    try load("glTexParameterfv", .{&bindings.texParameterfv});
    try load("glTexParameteri", .{&bindings.texParameteri});
    try load("glTexParameteriv", .{&bindings.texParameteriv});
    try load("glTexImage2D", .{&bindings.texImage2D});
    try load("glClear", .{&bindings.clear});
    try load("glClearColor", .{&bindings.clearColor});
    try load("glClearStencil", .{&bindings.clearStencil});
    try load("glClearDepthf", .{&bindings.clearDepthf});
    try load("glStencilMask", .{&bindings.stencilMask});
    try load("glColorMask", .{&bindings.colorMask});
    try load("glDepthMask", .{&bindings.depthMask});
    try load("glDisable", .{&bindings.disable});
    try load("glEnable", .{&bindings.enable});
    try load("glFinish", .{&bindings.finish});
    try load("glFlush", .{&bindings.flush});
    try load("glBlendFunc", .{&bindings.blendFunc});
    try load("glStencilFunc", .{&bindings.stencilFunc});
    try load("glStencilOp", .{&bindings.stencilOp});
    try load("glDepthFunc", .{&bindings.depthFunc});
    try load("glPixelStorei", .{&bindings.pixelStorei});
    try load("glReadPixels", .{&bindings.readPixels});
    try load("glGetBooleanv", .{&bindings.getBooleanv});
    try load("glGetError", .{&bindings.getError});
    try load("glGetFloatv", .{&bindings.getFloatv});
    try load("glGetIntegerv", .{&bindings.getIntegerv});
    try load("glGetString", .{&bindings.getString});
    try load("glIsEnabled", .{&bindings.isEnabled});
    try load("glDepthRangef", .{&bindings.depthRangef});
    try load("glViewport", .{&bindings.viewport});
    try load("glDrawArrays", .{&bindings.drawArrays});
    try load("glDrawElements", .{&bindings.drawElements});
    try load("glPolygonOffset", .{&bindings.polygonOffset});
    try load("glCopyTexImage2D", .{&bindings.copyTexImage2D});
    try load("glCopyTexSubImage2D", .{&bindings.copyTexSubImage2D});
    try load("glTexSubImage2D", .{&bindings.texSubImage2D});
    try load("glBindTexture", .{&bindings.bindTexture});
    try load("glDeleteTextures", .{&bindings.deleteTextures});
    try load("glGenTextures", .{&bindings.genTextures});
    try load("glIsTexture", .{&bindings.isTexture});
    try load("glActiveTexture", .{&bindings.activeTexture});
    try load("glSampleCoverage", .{&bindings.sampleCoverage});
    try load("glCompressedTexImage2D", .{&bindings.compressedTexImage2D});
    try load("glCompressedTexSubImage2D", .{&bindings.compressedTexSubImage2D});

    // OpenGL ES 1.1
    try load("glBlendFuncSeparate", .{&bindings.blendFuncSeparate});
    try load("glBlendColor", .{&bindings.blendColor});
    try load("glBlendEquation", .{&bindings.blendEquation});
    try load("glBindBuffer", .{&bindings.bindBuffer});
    try load("glDeleteBuffers", .{&bindings.deleteBuffers});
    try load("glGenBuffers", .{&bindings.genBuffers});
    try load("glIsBuffer", .{&bindings.isBuffer});
    try load("glBufferData", .{&bindings.bufferData});
    try load("glBufferSubData", .{&bindings.bufferSubData});
    try load("glGetBufferParameteriv", .{&bindings.getBufferParameteriv});

    // OpenGL ES 2.0
    try load("glBlendEquationSeparate", .{&bindings.blendEquationSeparate});
    try load("glStencilOpSeparate", .{&bindings.stencilOpSeparate});
    try load("glStencilFuncSeparate", .{&bindings.stencilFuncSeparate});
    try load("glStencilMaskSeparate", .{&bindings.stencilMaskSeparate});
    try load("glAttachShader", .{&bindings.attachShader});
    try load("glBindAttribLocation", .{&bindings.bindAttribLocation});
    try load("glCompileShader", .{&bindings.compileShader});
    try load("glCreateProgram", .{&bindings.createProgram});
    try load("glCreateShader", .{&bindings.createShader});
    try load("glDeleteProgram", .{&bindings.deleteProgram});
    try load("glDeleteShader", .{&bindings.deleteShader});
    try load("glDetachShader", .{&bindings.detachShader});
    try load("glDisableVertexAttribArray", .{&bindings.disableVertexAttribArray});
    try load("glEnableVertexAttribArray", .{&bindings.enableVertexAttribArray});
    try load("glGetActiveAttrib", .{&bindings.getActiveAttrib});
    try load("glGetActiveUniform", .{&bindings.getActiveUniform});
    try load("glGetAttachedShaders", .{&bindings.getAttachedShaders});
    try load("glGetAttribLocation", .{&bindings.getAttribLocation});
    try load("glGetProgramiv", .{&bindings.getProgramiv});
    try load("glGetProgramInfoLog", .{&bindings.getProgramInfoLog});
    try load("glGetShaderiv", .{&bindings.getShaderiv});
    try load("glGetShaderInfoLog", .{&bindings.getShaderInfoLog});
    try load("glGetShaderSource", .{&bindings.getShaderSource});
    try load("glGetUniformLocation", .{&bindings.getUniformLocation});
    try load("glGetUniformfv", .{&bindings.getUniformfv});
    try load("glGetUniformiv", .{&bindings.getUniformiv});
    try load("glGetVertexAttribPointerv", .{&bindings.getVertexAttribPointerv});
    try load("glIsProgram", .{&bindings.isProgram});
    try load("glIsShader", .{&bindings.isShader});
    try load("glLinkProgram", .{&bindings.linkProgram});
    try load("glShaderSource", .{&bindings.shaderSource});
    try load("glUseProgram", .{&bindings.useProgram});
    try load("glUniform1f", .{&bindings.uniform1f});
    try load("glUniform2f", .{&bindings.uniform2f});
    try load("glUniform3f", .{&bindings.uniform3f});
    try load("glUniform4f", .{&bindings.uniform4f});
    try load("glUniform1i", .{&bindings.uniform1i});
    try load("glUniform2i", .{&bindings.uniform2i});
    try load("glUniform3i", .{&bindings.uniform3i});
    try load("glUniform4i", .{&bindings.uniform4i});
    try load("glUniform1fv", .{&bindings.uniform1fv});
    try load("glUniform2fv", .{&bindings.uniform2fv});
    try load("glUniform3fv", .{&bindings.uniform3fv});
    try load("glUniform4fv", .{&bindings.uniform4fv});
    try load("glUniform1iv", .{&bindings.uniform1iv});
    try load("glUniform2iv", .{&bindings.uniform2iv});
    try load("glUniform3iv", .{&bindings.uniform3iv});
    try load("glUniform4iv", .{&bindings.uniform4iv});
    try load("glUniformMatrix2fv", .{&bindings.uniformMatrix2fv});
    try load("glUniformMatrix3fv", .{&bindings.uniformMatrix3fv});
    try load("glUniformMatrix4fv", .{&bindings.uniformMatrix4fv});
    try load("glValidateProgram", .{&bindings.validateProgram});
    try load("glVertexAttribPointer", .{&bindings.vertexAttribPointer});
    try load("glIsRenderbuffer", .{&bindings.isRenderbuffer});
    try load("glBindRenderbuffer", .{&bindings.bindRenderbuffer});
    try load("glDeleteRenderbuffers", .{&bindings.deleteRenderbuffers});
    try load("glGenRenderbuffers", .{&bindings.genRenderbuffers});
    try load("glRenderbufferStorage", .{&bindings.renderbufferStorage});
    try load("glGetRenderbufferParameteriv", .{&bindings.getRenderbufferParameteriv});
    try load("glIsFramebuffer", .{&bindings.isFramebuffer});
    try load("glBindFramebuffer", .{&bindings.bindFramebuffer});
    try load("glDeleteFramebuffers", .{&bindings.deleteFramebuffers});
    try load("glGenFramebuffers", .{&bindings.genFramebuffers});
    try load("glCheckFramebufferStatus", .{&bindings.checkFramebufferStatus});
    try load("glFramebufferTexture2D", .{&bindings.framebufferTexture2D});
    try load("glFramebufferRenderbuffer", .{&bindings.framebufferRenderbuffer});
    try load("glGetFramebufferAttachmentParameteriv", .{&bindings.getFramebufferAttachmentParameteriv});
    try load("glGenerateMipmap", .{&bindings.generateMipmap});

    if (webgl2) {
        // OpenGL ES 3.0
        try load("glUniformMatrix2x3fv", .{&bindings.uniformMatrix2x3fv});
        try load("glUniformMatrix3x2fv", .{&bindings.uniformMatrix3x2fv});
        try load("glUniformMatrix2x4fv", .{&bindings.uniformMatrix2x4fv});
        try load("glUniformMatrix4x2fv", .{&bindings.uniformMatrix4x2fv});
        try load("glUniformMatrix3x4fv", .{&bindings.uniformMatrix3x4fv});
        try load("glUniformMatrix4x3fv", .{&bindings.uniformMatrix4x3fv});
        try load("glGetIntegeri_v", .{&bindings.getIntegeri_v});
        try load("glBeginTransformFeedback", .{&bindings.beginTransformFeedback});
        try load("glEndTransformFeedback", .{&bindings.endTransformFeedback});
        try load("glBindBufferRange", .{&bindings.bindBufferRange});
        try load("glBindBufferBase", .{&bindings.bindBufferBase});
        try load("glTransformFeedbackVaryings", .{&bindings.transformFeedbackVaryings});
        try load("glGetTransformFeedbackVarying", .{&bindings.getTransformFeedbackVarying});
        try load("glVertexAttribIPointer", .{&bindings.vertexAttribIPointer});
        try load("glGetVertexAttribIiv", .{&bindings.getVertexAttribIiv});
        try load("glGetVertexAttribIuiv", .{&bindings.getVertexAttribIuiv});
        try load("glGetUniformuiv", .{&bindings.getUniformuiv});
        try load("glGetFragDataLocation", .{&bindings.getFragDataLocation});
        try load("glUniform1ui", .{&bindings.uniform1ui});
        try load("glUniform2ui", .{&bindings.uniform2ui});
        try load("glUniform3ui", .{&bindings.uniform3ui});
        try load("glUniform4ui", .{&bindings.uniform4ui});
        try load("glUniform1uiv", .{&bindings.uniform1uiv});
        try load("glUniform2uiv", .{&bindings.uniform2uiv});
        try load("glUniform3uiv", .{&bindings.uniform3uiv});
        try load("glUniform4uiv", .{&bindings.uniform4uiv});
        try load("glClearBufferiv", .{&bindings.clearBufferiv});
        try load("glClearBufferuiv", .{&bindings.clearBufferuiv});
        try load("glClearBufferfv", .{&bindings.clearBufferfv});
        try load("glClearBufferfi", .{&bindings.clearBufferfi});
        try load("glGetStringi", .{&bindings.getStringi});
        try load("glBlitFramebuffer", .{&bindings.blitFramebuffer});
        try load("glRenderbufferStorageMultisample", .{&bindings.renderbufferStorageMultisample});
        try load("glFramebufferTextureLayer", .{&bindings.framebufferTextureLayer});
        try load("glBindVertexArray", .{&bindings.bindVertexArray});
        try load("glDeleteVertexArrays", .{&bindings.deleteVertexArrays});
        try load("glGenVertexArrays", .{&bindings.genVertexArrays});
        try load("glIsVertexArray", .{&bindings.isVertexArray});
        try load("glDrawArraysInstanced", .{&bindings.drawArraysInstanced});
        try load("glDrawElementsInstanced", .{&bindings.drawElementsInstanced});
        try load("glCopyBufferSubData", .{&bindings.copyBufferSubData});
        try load("glGetUniformIndices", .{&bindings.getUniformIndices});
        try load("glGetActiveUniformsiv", .{&bindings.getActiveUniformsiv});
        try load("glGetUniformBlockIndex", .{&bindings.getUniformBlockIndex});
        try load("glGetActiveUniformBlockiv", .{&bindings.getActiveUniformBlockiv});
        try load("glGetActiveUniformBlockName", .{&bindings.getActiveUniformBlockName});
        try load("glUniformBlockBinding", .{&bindings.uniformBlockBinding});
        try load("glFenceSync", .{&bindings.fenceSync});
        try load("glIsSync", .{&bindings.isSync});
        try load("glDeleteSync", .{&bindings.deleteSync});
        try load("glClientWaitSync", .{&bindings.clientWaitSync});
        try load("glWaitSync", .{&bindings.waitSync});
        try load("glGetInteger64v", .{&bindings.getInteger64v});
        try load("glGetSynciv", .{&bindings.getSynciv});
        try load("glGetInteger64i_v", .{&bindings.getInteger64i_v});
        try load("glGetBufferParameteri64v", .{&bindings.getBufferParameteri64v});
        try load("glGenSamplers", .{&bindings.genSamplers});
        try load("glDeleteSamplers", .{&bindings.deleteSamplers});
        try load("glIsSampler", .{&bindings.isSampler});
        try load("glBindSampler", .{&bindings.bindSampler});
        try load("glSamplerParameteri", .{&bindings.samplerParameteri});
        try load("glSamplerParameteriv", .{&bindings.samplerParameteriv});
        try load("glSamplerParameterf", .{&bindings.samplerParameterf});
        try load("glSamplerParameterfv", .{&bindings.samplerParameterfv});
        try load("glGetSamplerParameteriv", .{&bindings.getSamplerParameteriv});
        try load("glGetSamplerParameterfv", .{&bindings.getSamplerParameterfv});
        try load("glVertexAttribDivisor", .{&bindings.vertexAttribDivisor});
        try load("glDrawBuffers", .{&bindings.drawBuffers});
    }
}

//--------------------------------------------------------------------------------------------------
fn load(proc_name: [:0]const u8, bind_addresses: anytype) !void {
    const ProcType = @typeInfo(@TypeOf(bind_addresses.@"0")).Pointer.child;
    const proc = try getProcAddress(ProcType, proc_name);
    inline for (bind_addresses) |bind_addr| {
        if (@typeInfo(@TypeOf(bind_addr)).Pointer.child != ProcType) {
            @compileError("proc bindings should all be the same type");
        }
        bind_addr.* = proc;
    }
}

var loaderFunc: LoaderFn = undefined;

fn getProcAddress(comptime T: type, proc_name: [:0]const u8) !T {
    if (loaderFunc(proc_name)) |addr| {
        return @as(T, @ptrFromInt(@intFromPtr(addr)));
    }
    std.log.debug("zopengl: {s} not found", .{proc_name});
    return error.OpenGL_FunctionNotFound;
}

//--------------------------------------------------------------------------------------------------
//
// C exports
//
//--------------------------------------------------------------------------------------------------
const linkage: @import("std").builtin.GlobalLinkage = .strong;
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
    // TODO: where do these belong?
    // @export(bindings.vertexP2ui, .{ .name = "glVertexP2ui", .linkage = linkage });
    // @export(bindings.vertexP2uiv, .{ .name = "glVertexP2uiv", .linkage = linkage });
    // @export(bindings.vertexP3ui, .{ .name = "glVertexP3ui", .linkage = linkage });
    // @export(bindings.vertexP3uiv, .{ .name = "glVertexP3uiv", .linkage = linkage });
    // @export(bindings.vertexP4ui, .{ .name = "glVertexP4ui", .linkage = linkage });
    // @export(bindings.vertexP4uiv, .{ .name = "glVertexP4uiv", .linkage = linkage });
    // @export(bindings.texCoordP1ui, .{ .name = "glTexCoordP1ui", .linkage = linkage });
    // @export(bindings.texCoordP1uiv, .{ .name = "glTexCoordP1uiv", .linkage = linkage });
    // @export(bindings.texCoordP2ui, .{ .name = "glTexCoordP2ui", .linkage = linkage });
    // @export(bindings.texCoordP2uiv, .{ .name = "glTexCoordP2uiv", .linkage = linkage });
    // @export(bindings.texCoordP3ui, .{ .name = "glTexCoordP3ui", .linkage = linkage });
    // @export(bindings.texCoordP3uiv, .{ .name = "glTexCoordP3uiv", .linkage = linkage });
    // @export(bindings.texCoordP4ui, .{ .name = "glTexCoordP4ui", .linkage = linkage });
    // @export(bindings.texCoordP4uiv, .{ .name = "glTexCoordP4uiv", .linkage = linkage });
    // @export(bindings.multiTexCoordP1ui, .{ .name = "glMultiTexCoordP1ui", .linkage = linkage });
    // @export(bindings.multiTexCoordP1uiv, .{ .name = "glMultiTexCoordP1uiv", .linkage = linkage });
    // @export(bindings.multiTexCoordP2ui, .{ .name = "glMultiTexCoordP2ui", .linkage = linkage });
    // @export(bindings.multiTexCoordP2uiv, .{ .name = "glMultiTexCoordP2uiv", .linkage = linkage });
    // @export(bindings.multiTexCoordP3ui, .{ .name = "glMultiTexCoordP3ui", .linkage = linkage });
    // @export(bindings.multiTexCoordP3uiv, .{ .name = "glMultiTexCoordP3uiv", .linkage = linkage });
    // @export(bindings.multiTexCoordP4ui, .{ .name = "glMultiTexCoordP4ui", .linkage = linkage });
    // @export(bindings.multiTexCoordP4uiv, .{ .name = "glMultiTexCoordP4uiv", .linkage = linkage });
    // @export(bindings.normalP3ui, .{ .name = "glNormalP3ui", .linkage = linkage });
    // @export(bindings.normalP3uiv, .{ .name = "glNormalP3uiv", .linkage = linkage });
    // @export(bindings.colorP3ui, .{ .name = "glColorP3ui", .linkage = linkage });
    // @export(bindings.colorP3uiv, .{ .name = "glColorP3uiv", .linkage = linkage });
    // @export(bindings.colorP4ui, .{ .name = "glColorP4ui", .linkage = linkage });
    // @export(bindings.colorP4uiv, .{ .name = "glColorP4uiv", .linkage = linkage });
    // @export(bindings.secondaryColorP3ui, .{ .name = "glSecondaryColorP3ui", .linkage = linkage });
    // @export(bindings.secondaryColorP3uiv, .{ .name = "glSecondaryColorP3uiv", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 4.0 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.minSampleShading, .{ .name = "glMinSampleShading", .linkage = linkage });
    @export(bindings.blendEquationi, .{ .name = "glBlendEquationi", .linkage = linkage });
    @export(bindings.blendEquationSeparatei, .{ .name = "glBlendEquationSeparatei", .linkage = linkage });
    @export(bindings.blendFunci, .{ .name = "glBlendFunci", .linkage = linkage });
    @export(bindings.blendFuncSeparatei, .{ .name = "glBlendFuncSeparatei", .linkage = linkage });
    @export(bindings.drawArraysIndirect, .{ .name = "glDrawArraysIndirect", .linkage = linkage });
    @export(bindings.drawElementsIndirect, .{ .name = "glDrawElementsIndirect", .linkage = linkage });
    @export(bindings.uniform1d, .{ .name = "glUniform1d", .linkage = linkage });
    @export(bindings.uniform2d, .{ .name = "glUniform2d", .linkage = linkage });
    @export(bindings.uniform3d, .{ .name = "glUniform3d", .linkage = linkage });
    @export(bindings.uniform4d, .{ .name = "glUniform4d", .linkage = linkage });
    @export(bindings.uniform1dv, .{ .name = "glUniform1dv", .linkage = linkage });
    @export(bindings.uniform2dv, .{ .name = "glUniform2dv", .linkage = linkage });
    @export(bindings.uniform3dv, .{ .name = "glUniform3dv", .linkage = linkage });
    @export(bindings.uniform4dv, .{ .name = "glUniform4dv", .linkage = linkage });
    @export(bindings.uniformMatrix2dv, .{ .name = "glUniformMatrix2dv", .linkage = linkage });
    @export(bindings.uniformMatrix3dv, .{ .name = "glUniformMatrix3dv", .linkage = linkage });
    @export(bindings.uniformMatrix4dv, .{ .name = "glUniformMatrix4dv", .linkage = linkage });
    @export(bindings.uniformMatrix2x3dv, .{ .name = "glUniformMatrix2x3dv", .linkage = linkage });
    @export(bindings.uniformMatrix2x4dv, .{ .name = "glUniformMatrix2x4dv", .linkage = linkage });
    @export(bindings.uniformMatrix3x2dv, .{ .name = "glUniformMatrix3x2dv", .linkage = linkage });
    @export(bindings.uniformMatrix3x4dv, .{ .name = "glUniformMatrix3x4dv", .linkage = linkage });
    @export(bindings.uniformMatrix4x2dv, .{ .name = "glUniformMatrix4x2dv", .linkage = linkage });
    @export(bindings.uniformMatrix4x3dv, .{ .name = "glUniformMatrix4x3dv", .linkage = linkage });
    @export(bindings.getUniformdv, .{ .name = "glGetUniformdv", .linkage = linkage });
    @export(bindings.getSubroutineUniformLocation, .{ .name = "glGetSubroutineUniformLocation", .linkage = linkage });
    @export(bindings.getSubroutineIndex, .{ .name = "glGetSubroutineIndex", .linkage = linkage });
    @export(bindings.getActiveSubroutineUniformiv, .{ .name = "glGetActiveSubroutineUniformiv", .linkage = linkage });
    @export(bindings.getActiveSubroutineUniformName, .{ .name = "glGetActiveSubroutineUniformName", .linkage = linkage });
    @export(bindings.getActiveSubroutineName, .{ .name = "glGetActiveSubroutineName", .linkage = linkage });
    @export(bindings.uniformSubroutinesuiv, .{ .name = "glUniformSubroutinesuiv", .linkage = linkage });
    @export(bindings.getUniformSubroutineuiv, .{ .name = "glGetUniformSubroutineuiv", .linkage = linkage });
    @export(bindings.getProgramStageiv, .{ .name = "glGetProgramStageiv", .linkage = linkage });
    @export(bindings.patchParameteri, .{ .name = "glPatchParameteri", .linkage = linkage });
    @export(bindings.patchParameterfv, .{ .name = "glPatchParameterfv", .linkage = linkage });
    @export(bindings.bindTransformFeedback, .{ .name = "glBindTransformFeedback", .linkage = linkage });
    @export(bindings.deleteTransformFeedbacks, .{ .name = "glDeleteTransformFeedbacks", .linkage = linkage });
    @export(bindings.genTransformFeedbacks, .{ .name = "glGenTransformFeedbacks", .linkage = linkage });
    @export(bindings.isTransformFeedback, .{ .name = "glIsTransformFeedback", .linkage = linkage });
    @export(bindings.pauseTransformFeedback, .{ .name = "glPauseTransformFeedback", .linkage = linkage });
    @export(bindings.resumeTransformFeedback, .{ .name = "glResumeTransformFeedback", .linkage = linkage });
    @export(bindings.drawTransformFeedback, .{ .name = "glDrawTransformFeedback", .linkage = linkage });
    @export(bindings.drawTransformFeedbackStream, .{ .name = "glDrawTransformFeedbackStream", .linkage = linkage });
    @export(bindings.beginQueryIndexed, .{ .name = "glBeginQueryIndexed", .linkage = linkage });
    @export(bindings.endQueryIndexed, .{ .name = "glEndQueryIndexed", .linkage = linkage });
    @export(bindings.glGetQueryIndexediv, .{ .name = "glGetQueryIndexediv", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 4.1 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.releaseShaderCompiler, .{ .name = "glReleaseShaderCompiler", .linkage = linkage });
    @export(bindings.shaderBinary, .{ .name = "glShaderBinary", .linkage = linkage });
    @export(bindings.getShaderPrecisionFormat, .{ .name = "glGetShaderPrecisionFormat", .linkage = linkage });
    @export(bindings.depthRangef, .{ .name = "glDepthRangef", .linkage = linkage });
    @export(bindings.clearDepthf, .{ .name = "glClearDepthf", .linkage = linkage });
    @export(bindings.getProgramBinary, .{ .name = "glGetProgramBinary", .linkage = linkage });
    @export(bindings.programBinary, .{ .name = "glProgramBinary", .linkage = linkage });
    @export(bindings.programParameteri, .{ .name = "glProgramParameteri", .linkage = linkage });
    @export(bindings.useProgramStages, .{ .name = "glUseProgramStages", .linkage = linkage });
    @export(bindings.activeShaderProgram, .{ .name = "glActiveShaderProgram", .linkage = linkage });
    @export(bindings.createShaderProgramv, .{ .name = "glCreateShaderProgramv", .linkage = linkage });
    @export(bindings.bindProgramPipeline, .{ .name = "glBindProgramPipeline", .linkage = linkage });
    @export(bindings.deleteProgramPipelines, .{ .name = "glDeleteProgramPipelines", .linkage = linkage });
    @export(bindings.genProgramPipelines, .{ .name = "glGenProgramPipelines", .linkage = linkage });
    @export(bindings.isProgramPipeline, .{ .name = "glIsProgramPipeline", .linkage = linkage });
    @export(bindings.getProgramPipelineiv, .{ .name = "glGetProgramPipelineiv", .linkage = linkage });
    @export(bindings.programUniform1i, .{ .name = "glProgramUniform1i", .linkage = linkage });
    @export(bindings.programUniform2i, .{ .name = "glProgramUniform2i", .linkage = linkage });
    @export(bindings.programUniform3i, .{ .name = "glProgramUniform3i", .linkage = linkage });
    @export(bindings.programUniform4i, .{ .name = "glProgramUniform4i", .linkage = linkage });
    @export(bindings.programUniform1ui, .{ .name = "glProgramUniform1ui", .linkage = linkage });
    @export(bindings.programUniform2ui, .{ .name = "glProgramUniform2ui", .linkage = linkage });
    @export(bindings.programUniform3ui, .{ .name = "glProgramUniform3ui", .linkage = linkage });
    @export(bindings.programUniform4ui, .{ .name = "glProgramUniform4ui", .linkage = linkage });
    @export(bindings.programUniform1f, .{ .name = "glProgramUniform1f", .linkage = linkage });
    @export(bindings.programUniform2f, .{ .name = "glProgramUniform2f", .linkage = linkage });
    @export(bindings.programUniform3f, .{ .name = "glProgramUniform3f", .linkage = linkage });
    @export(bindings.programUniform4f, .{ .name = "glProgramUniform4f", .linkage = linkage });
    @export(bindings.programUniform1d, .{ .name = "glProgramUniform1d", .linkage = linkage });
    @export(bindings.programUniform2d, .{ .name = "glProgramUniform2d", .linkage = linkage });
    @export(bindings.programUniform3d, .{ .name = "glProgramUniform3d", .linkage = linkage });
    @export(bindings.programUniform4d, .{ .name = "glProgramUniform4d", .linkage = linkage });
    @export(bindings.programUniform1iv, .{ .name = "glProgramUniform1iv", .linkage = linkage });
    @export(bindings.programUniform2iv, .{ .name = "glProgramUniform2iv", .linkage = linkage });
    @export(bindings.programUniform3iv, .{ .name = "glProgramUniform3iv", .linkage = linkage });
    @export(bindings.programUniform4iv, .{ .name = "glProgramUniform4iv", .linkage = linkage });
    @export(bindings.programUniform1uiv, .{ .name = "glProgramUniform1uiv", .linkage = linkage });
    @export(bindings.programUniform2uiv, .{ .name = "glProgramUniform2uiv", .linkage = linkage });
    @export(bindings.programUniform3uiv, .{ .name = "glProgramUniform3uiv", .linkage = linkage });
    @export(bindings.programUniform4uiv, .{ .name = "glProgramUniform4uiv", .linkage = linkage });
    @export(bindings.programUniform1fv, .{ .name = "glProgramUniform1fv", .linkage = linkage });
    @export(bindings.programUniform2fv, .{ .name = "glProgramUniform2fv", .linkage = linkage });
    @export(bindings.programUniform3fv, .{ .name = "glProgramUniform3fv", .linkage = linkage });
    @export(bindings.programUniform4fv, .{ .name = "glProgramUniform4fv", .linkage = linkage });
    @export(bindings.programUniform1dv, .{ .name = "glProgramUniform1dv", .linkage = linkage });
    @export(bindings.programUniform2dv, .{ .name = "glProgramUniform2dv", .linkage = linkage });
    @export(bindings.programUniform3dv, .{ .name = "glProgramUniform3dv", .linkage = linkage });
    @export(bindings.programUniform4dv, .{ .name = "glProgramUniform4dv", .linkage = linkage });
    @export(bindings.programUniformMatrix2fv, .{ .name = "glProgramUniformMatrix2fv", .linkage = linkage });
    @export(bindings.programUniformMatrix3fv, .{ .name = "glProgramUniformMatrix3fv", .linkage = linkage });
    @export(bindings.programUniformMatrix4fv, .{ .name = "glProgramUniformMatrix4fv", .linkage = linkage });
    @export(bindings.programUniformMatrix2dv, .{ .name = "glProgramUniformMatrix2dv", .linkage = linkage });
    @export(bindings.programUniformMatrix3dv, .{ .name = "glProgramUniformMatrix3dv", .linkage = linkage });
    @export(bindings.programUniformMatrix4dv, .{ .name = "glProgramUniformMatrix4dv", .linkage = linkage });
    @export(bindings.programUniformMatrix2x3fv, .{ .name = "glProgramUniformMatrix2x3fv", .linkage = linkage });
    @export(bindings.programUniformMatrix3x2fv, .{ .name = "glProgramUniformMatrix3x2fv", .linkage = linkage });
    @export(bindings.programUniformMatrix2x4fv, .{ .name = "glProgramUniformMatrix2x4fv", .linkage = linkage });
    @export(bindings.programUniformMatrix4x2fv, .{ .name = "glProgramUniformMatrix4x2fv", .linkage = linkage });
    @export(bindings.programUniformMatrix3x4fv, .{ .name = "glProgramUniformMatrix3x4fv", .linkage = linkage });
    @export(bindings.programUniformMatrix4x3fv, .{ .name = "glProgramUniformMatrix4x3fv", .linkage = linkage });
    @export(bindings.programUniformMatrix2x3dv, .{ .name = "glProgramUniformMatrix2x3dv", .linkage = linkage });
    @export(bindings.programUniformMatrix3x2dv, .{ .name = "glProgramUniformMatrix3x2dv", .linkage = linkage });
    @export(bindings.programUniformMatrix2x4dv, .{ .name = "glProgramUniformMatrix2x4dv", .linkage = linkage });
    @export(bindings.programUniformMatrix4x2dv, .{ .name = "glProgramUniformMatrix4x2dv", .linkage = linkage });
    @export(bindings.programUniformMatrix3x4dv, .{ .name = "glProgramUniformMatrix3x4dv", .linkage = linkage });
    @export(bindings.programUniformMatrix4x3dv, .{ .name = "glProgramUniformMatrix4x3dv", .linkage = linkage });
    @export(bindings.validateProgramPipeline, .{ .name = "glValidateProgramPipeline", .linkage = linkage });
    @export(bindings.getProgramPipelineInfoLog, .{ .name = "glGetProgramPipelineInfoLog", .linkage = linkage });
    @export(bindings.vertexAttribL1d, .{ .name = "glVertexAttribL1d", .linkage = linkage });
    @export(bindings.vertexAttribL2d, .{ .name = "glVertexAttribL2d", .linkage = linkage });
    @export(bindings.vertexAttribL3d, .{ .name = "glVertexAttribL3d", .linkage = linkage });
    @export(bindings.vertexAttribL4d, .{ .name = "glVertexAttribL4d", .linkage = linkage });
    @export(bindings.vertexAttribL1dv, .{ .name = "glVertexAttribL1dv", .linkage = linkage });
    @export(bindings.vertexAttribL2dv, .{ .name = "glVertexAttribL2dv", .linkage = linkage });
    @export(bindings.vertexAttribL3dv, .{ .name = "glVertexAttribL3dv", .linkage = linkage });
    @export(bindings.vertexAttribL4dv, .{ .name = "glVertexAttribL4dv", .linkage = linkage });
    @export(bindings.viewportArrayv, .{ .name = "glViewportArrayv", .linkage = linkage });
    @export(bindings.viewportIndexedf, .{ .name = "glViewportIndexedf", .linkage = linkage });
    @export(bindings.viewportIndexedfv, .{ .name = "glViewportIndexedfv", .linkage = linkage });
    @export(bindings.scissorArrayv, .{ .name = "glScissorArrayv", .linkage = linkage });
    @export(bindings.scissorIndexed, .{ .name = "glScissorIndexed", .linkage = linkage });
    @export(bindings.scissorIndexedv, .{ .name = "glScissorIndexedv", .linkage = linkage });
    @export(bindings.depthRangeArrayv, .{ .name = "glDepthRangeArrayv", .linkage = linkage });
    @export(bindings.depthRangeIndexed, .{ .name = "glDepthRangeIndexed", .linkage = linkage });
    @export(bindings.getFloati_v, .{ .name = "glGetFloati_v", .linkage = linkage });
    @export(bindings.getDoublei_v, .{ .name = "glGetDoublei_v", .linkage = linkage });
    //----------------------------------------------------------------------------------------------
    // OpenGL 4.2 (Core Profile)
    //----------------------------------------------------------------------------------------------
    @export(bindings.drawArraysInstancedBaseInstance, .{ .name = "glDrawArraysInstancedBaseInstance", .linkage = linkage });
    @export(bindings.drawElementsInstancedBaseInstance, .{ .name = "glDrawElementsInstancedBaseInstance", .linkage = linkage });
    @export(bindings.drawElementsInstancedBaseVertexBaseInstance, .{ .name = "glDrawElementsInstancedBaseVertexBaseInstance", .linkage = linkage });
    @export(bindings.getInternalformativ, .{ .name = "glGetInternalformativ", .linkage = linkage });
    @export(bindings.getActiveAtomicCounterBufferiv, .{ .name = "glGetActiveAtomicCounterBufferiv", .linkage = linkage });
    @export(bindings.bindImageTexture, .{ .name = "glBindImageTexture", .linkage = linkage });
    @export(bindings.memoryBarrier, .{ .name = "glMemoryBarrier", .linkage = linkage });
    @export(bindings.texStorage1D, .{ .name = "glTexStorage1D", .linkage = linkage });
    @export(bindings.texStorage2D, .{ .name = "glTexStorage2D", .linkage = linkage });
    @export(bindings.texStorage3D, .{ .name = "glTexStorage3D", .linkage = linkage });
    @export(bindings.drawTransformFeedbackInstanced, .{ .name = "glDrawTransformFeedbackInstanced", .linkage = linkage });
    @export(bindings.drawTransformFeedbackStreamInstanced, .{ .name = "glDrawTransformFeedbackStreamInstanced", .linkage = linkage });
}
