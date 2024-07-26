const builtin = @import("builtin");

const std = @import("std");
const log = std.log.scoped(.zopengl);
const assert = std.debug.assert;

const meta = struct {
    pub fn mergeEnums(comptime Enums: anytype) type {
        const tag_type = @typeInfo(Enums[0]).Enum.tag_type;
        const num_fields = countFields: {
            var count: comptime_int = 0;
            for (Enums) |Subset| {
                for (std.meta.fields(Subset)) |_| count += 1;
            }
            break :countFields count;
        };
        comptime var fields: [num_fields]std.builtin.Type.EnumField = .{undefined} ** num_fields;
        comptime var i = 0;
        for (Enums) |Subset| {
            const subset_info = @typeInfo(Subset).Enum;
            assert(subset_info.tag_type == tag_type);
            for (subset_info.fields) |field| {
                assert(i < fields.len);
                fields[i] = field;
                i += 1;
            }
        }
        return @Type(.{ .Enum = .{
            .tag_type = tag_type,
            .fields = &fields,
            .decls = &.{},
            .is_exhaustive = true,
        } });
    }
};

pub fn Wrap(comptime bindings: anytype) type {
    return struct {
        pub const Framebuffer = extern struct { name: Uint = 0 };
        pub const Renderbuffer = extern struct { name: Uint = 0 };
        pub const Shader = extern struct { name: Uint = 0 };
        pub const Program = extern struct { name: Uint = 0 };
        pub const Texture = extern struct { name: Uint = 0 };
        pub const Buffer = extern struct { name: Uint = 0 };
        pub const VertexArrayObject = extern struct { name: Uint = 0 };

        pub const UniformLocation = extern struct { location: Uint };
        pub const VertexAttribLocation = extern struct { location: Uint };

        pub const Error = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            no_error = NO_ERROR,
            invalid_enum = INVALID_ENUM,
            invalid_value = INVALID_VALUE,
            invalid_operation = INVALID_OPERATION,
            stack_overflow = STACK_OVERFLOW,
            stack_underflow = STACK_UNDERFLOW,
            out_of_memory = OUT_OF_MEMORY,
            invalid_framebuffer_operation = INVALID_FRAMEBUFFER_OPERATION,
        };

        pub const Capability = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            blend = BLEND,
            cull_face = CULL_FACE,
            depth_test = DEPTH_TEST,
            dither = DITHER,
            line_smooth = LINE_SMOOTH,
            polygon_smooth = POLYGON_SMOOTH,
            scissor_test = SCISSOR_TEST,
            stencil_test = STENCIL_TEST,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            color_logic_op = COLOR_LOGIC_OP,
            polygon_offset_fill = POLYGON_OFFSET_FILL,
            polygon_offset_line = POLYGON_OFFSET_LINE,
            polygon_offset_point = POLYGON_OFFSET_POINT,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            multisample = MULTISAMPLE,
            sample_alpha_to_coverage = SAMPLE_ALPHA_TO_COVERAGE,
            sample_alpha_to_one = SAMPLE_ALPHA_TO_ONE,
            sample_coverage = SAMPLE_COVERAGE,
            //--------------------------------------------------------------------------------------
            // OpenGL 2.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            program_point_size = PROGRAM_POINT_SIZE,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            framebuffer_srgb = FRAMEBUFFER_SRGB,
            rasterizer_discard = RASTERIZER_DISCARD,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            primitive_restart = PRIMITIVE_RESTART,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            depth_clamp = DEPTH_CLAMP,
            sample_mask = SAMPLE_MASK,
            texture_cube_map_seamless = TEXTURE_CUBE_MAP_SEAMLESS,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            sample_shading = SAMPLE_SHADING,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            debug_output = DEBUG_OUTPUT,
        };

        pub const StringParamName = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            vendor = VENDOR,
            renderer = RENDERER,
            version = VERSION,
            extensions = EXTENSIONS,
            //--------------------------------------------------------------------------------------
            // OpenGL 2.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            shading_language_version = SHADING_LANGUAGE_VERSION,
        };

        pub const ParamName = meta.mergeEnums(.{
            enum(Enum) {
                //--------------------------------------------------------------------------------------
                // OpenGL 1.0 (Core Profile)
                //--------------------------------------------------------------------------------------
                cull_face = CULL_FACE,
                polygon_smooth = POLYGON_SMOOTH,
                line_smooth = LINE_SMOOTH,
                dither = DITHER,
                blend = BLEND,
                color_writemask = COLOR_WRITEMASK,
                depth_test = DEPTH_TEST,
                depth_writemask = DEPTH_WRITEMASK,
                stencil_test = STENCIL_TEST,
                doublebuffer = DOUBLEBUFFER,
                stereo = STEREO,
                scissor_test = SCISSOR_TEST,
                polygon_mode = POLYGON_MODE,
                polygon_smooth_hint = POLYGON_SMOOTH_HINT,
                line_smooth_hint = LINE_SMOOTH_HINT,
                logic_op_mode = LOGIC_OP_MODE,
                color_clear_value = COLOR_CLEAR_VALUE,
                depth_clear_value = DEPTH_CLEAR_VALUE,
                depth_func = DEPTH_FUNC,
                depth_range = DEPTH_RANGE,
                stencil_clear_value = STENCIL_CLEAR_VALUE,
                stencil_fail = STENCIL_FAIL,
                stencil_func = STENCIL_FUNC,
                stencil_pass_depth_fail = STENCIL_PASS_DEPTH_FAIL,
                stencil_pass_depth_pass = STENCIL_PASS_DEPTH_PASS,
                stencil_ref = STENCIL_REF,
                stencil_value_mask = STENCIL_VALUE_MASK,
                stencil_writemask = STENCIL_WRITEMASK,
                viewport = VIEWPORT,
                subpixel_bits = SUBPIXEL_BITS,
                draw_buffer = DRAW_BUFFER,
                read_buffer = READ_BUFFER,
                scissor_box = SCISSOR_BOX,
                pack_alignment = PACK_ALIGNMENT,
                pack_lsb_first = PACK_LSB_FIRST,
                pack_row_length = PACK_ROW_LENGTH,
                pack_skip_pixels = PACK_SKIP_PIXELS,
                pack_skip_rows = PACK_SKIP_ROWS,
                pack_swap_bytes = PACK_SWAP_BYTES,
                unpack_alignment = UNPACK_ALIGNMENT,
                unpack_lsb_first = UNPACK_LSB_FIRST,
                unpack_row_length = UNPACK_ROW_LENGTH,
                unpack_skip_pixels = UNPACK_SKIP_PIXELS,
                unpack_skip_rows = UNPACK_SKIP_ROWS,
                unpack_swap_bytes = UNPACK_SWAP_BYTES,
                max_texture_size = MAX_TEXTURE_SIZE,
                max_viewport_dims = MAX_VIEWPORT_DIMS,
                point_size = POINT_SIZE,
                point_size_granularity = POINT_SIZE_GRANULARITY,
                point_size_range = POINT_SIZE_RANGE,
                line_width = LINE_WIDTH,
                //--------------------------------------------------------------------------------------
                // OpenGL 1.1 (Core Profile)
                //--------------------------------------------------------------------------------------
                polygon_offset_fill = POLYGON_OFFSET_FILL,
                polygon_offset_line = POLYGON_OFFSET_LINE,
                polygon_offset_point = POLYGON_OFFSET_POINT,
                color_logic_op = COLOR_LOGIC_OP,
                texture_binding_1d = TEXTURE_BINDING_1D,
                texture_binding_2d = TEXTURE_BINDING_2D,
                polygon_offset_factor = POLYGON_OFFSET_FACTOR,
                polygon_offset_units = POLYGON_OFFSET_UNITS,
                //--------------------------------------------------------------------------------------
                // OpenGL 1.2 (Core Profile)
                //--------------------------------------------------------------------------------------
                pack_image_height = PACK_IMAGE_HEIGHT,
                pack_skip_images = PACK_SKIP_IMAGES,
                unpack_image_height = UNPACK_IMAGE_HEIGHT,
                unpack_skip_images = UNPACK_SKIP_IMAGES,
                texture_binding_3d = TEXTURE_BINDING_3D,
                max_3d_texture_size = MAX_3D_TEXTURE_SIZE,
                max_elements_indices = MAX_ELEMENTS_INDICES,
                max_elements_vertices = MAX_ELEMENTS_VERTICES,
                aliased_line_width_range = ALIASED_LINE_WIDTH_RANGE,
                smooth_line_width_granularity = SMOOTH_LINE_WIDTH_GRANULARITY,
                smooth_line_width_range = SMOOTH_LINE_WIDTH_RANGE,
                //--------------------------------------------------------------------------------------
                // OpenGL 1.3 (Core Profile)
                //--------------------------------------------------------------------------------------
                sample_coverage_invert = SAMPLE_COVERAGE_INVERT,
                active_texture = ACTIVE_TEXTURE,
                texture_binding_cube_map = TEXTURE_BINDING_CUBE_MAP,
                texture_compression_hint = TEXTURE_COMPRESSION_HINT,
                compressed_texture_formats = COMPRESSED_TEXTURE_FORMATS,
                samples = SAMPLES,
                sample_buffers = SAMPLE_BUFFERS,
                num_compressed_texture_formats = NUM_COMPRESSED_TEXTURE_FORMATS,
                max_cube_map_texture_size = MAX_CUBE_MAP_TEXTURE_SIZE,
                sample_coverage_value = SAMPLE_COVERAGE_VALUE,
                //--------------------------------------------------------------------------------------
                // OpenGL 1.4 (Core Profile)
                //--------------------------------------------------------------------------------------
                blend_src_rgb = BLEND_SRC_RGB,
                blend_src_alpha = BLEND_SRC_ALPHA,
                blend_dst_rgb = BLEND_DST_RGB,
                blend_dst_alpha = BLEND_DST_ALPHA,
                blend_color = BLEND_COLOR,
                point_fade_threshold_size = POINT_FADE_THRESHOLD_SIZE,
                max_texture_lod_bias = MAX_TEXTURE_LOD_BIAS,
                //--------------------------------------------------------------------------------------
                // OpenGL 1.5 (Core Profile)
                //--------------------------------------------------------------------------------------
                array_buffer_binding = ARRAY_BUFFER_BINDING,
                element_array_buffer_binding = ELEMENT_ARRAY_BUFFER_BINDING,
                //--------------------------------------------------------------------------------------
                // OpenGL 2.0 (Core Profile)
                //--------------------------------------------------------------------------------------
                current_program = CURRENT_PROGRAM,
                blend_equation_rgb = BLEND_EQUATION_RGB,
                blend_equation_alpha = BLEND_EQUATION_ALPHA,
                stencil_back_fail = STENCIL_BACK_FAIL,
                stencil_back_func = STENCIL_BACK_FUNC,
                stencil_back_pass_depth_fail = STENCIL_BACK_PASS_DEPTH_FAIL,
                stencil_back_pass_depth_pass = STENCIL_BACK_PASS_DEPTH_PASS,
                stencil_back_ref = STENCIL_BACK_REF,
                stencil_back_value_mask = STENCIL_BACK_VALUE_MASK,
                stencil_back_writemask = STENCIL_BACK_WRITEMASK,
                draw_buffer0 = DRAW_BUFFER0,
                draw_buffer1 = DRAW_BUFFER1,
                draw_buffer2 = DRAW_BUFFER2,
                draw_buffer3 = DRAW_BUFFER3,
                draw_buffer4 = DRAW_BUFFER4,
                draw_buffer5 = DRAW_BUFFER5,
                draw_buffer6 = DRAW_BUFFER6,
                draw_buffer7 = DRAW_BUFFER7,
                draw_buffer8 = DRAW_BUFFER8,
                draw_buffer9 = DRAW_BUFFER9,
                draw_buffer10 = DRAW_BUFFER10,
                draw_buffer11 = DRAW_BUFFER11,
                draw_buffer12 = DRAW_BUFFER12,
                draw_buffer13 = DRAW_BUFFER13,
                draw_buffer14 = DRAW_BUFFER14,
                draw_buffer15 = DRAW_BUFFER15,
                draw_framebuffer_binding = DRAW_FRAMEBUFFER_BINDING,
                fragment_shader_derivative_hint = FRAGMENT_SHADER_DERIVATIVE_HINT,
                max_combined_texture_image_units = MAX_COMBINED_TEXTURE_IMAGE_UNITS,
                max_draw_buffers = MAX_DRAW_BUFFERS,
                max_fragment_uniform_components = MAX_FRAGMENT_UNIFORM_COMPONENTS,
                max_texture_image_units = MAX_TEXTURE_IMAGE_UNITS,
                //max_varying_floats = MAX_VARYING_FLOATS, // NOTE: MAX_VARYING_FLOATS is equal to MAX_VARYING_COMPONENTS
                max_vertex_attribs = MAX_VERTEX_ATTRIBS,
                max_vertex_texture_image_units = MAX_VERTEX_TEXTURE_IMAGE_UNITS,
                max_vertex_uniform_components = MAX_VERTEX_UNIFORM_COMPONENTS,
                //--------------------------------------------------------------------------------------
                // OpenGL 2.1 (Core Profile)
                //--------------------------------------------------------------------------------------
                pixel_pack_buffer_binding = PIXEL_PACK_BUFFER_BINDING,
                pixel_unpack_buffer_binding = PIXEL_UNPACK_BUFFER_BINDING,
                //--------------------------------------------------------------------------------------
                // OpenGL 3.0 (Core Profile)
                //--------------------------------------------------------------------------------------
                context_flags = CONTEXT_FLAGS,
                major_version = MAJOR_VERSION,
                minor_version = MINOR_VERSION,
                num_extensions = NUM_EXTENSIONS,
                texture_binding_1d_array = TEXTURE_BINDING_1D_ARRAY,
                texture_binding_2d_array = TEXTURE_BINDING_2D_ARRAY,
                transform_feedback_buffer_binding = TRANSFORM_FEEDBACK_BUFFER_BINDING,
                transform_feedback_buffer_size = TRANSFORM_FEEDBACK_BUFFER_SIZE,
                transform_feedback_buffer_start = TRANSFORM_FEEDBACK_BUFFER_START,
                vertex_array_binding = VERTEX_ARRAY_BINDING,
                read_framebuffer_binding = READ_FRAMEBUFFER_BINDING,
                renderbuffer_binding = RENDERBUFFER_BINDING,
                min_program_texel_offset = MIN_PROGRAM_TEXEL_OFFSET,
                max_array_texture_layers = MAX_ARRAY_TEXTURE_LAYERS,
                max_clip_distances = MAX_CLIP_DISTANCES,
                max_program_texel_offset = MAX_PROGRAM_TEXEL_OFFSET,
                max_renderbuffer_size = MAX_RENDERBUFFER_SIZE,
                max_varying_components = MAX_VARYING_COMPONENTS,
                //--------------------------------------------------------------------------------------
                // OpenGL 3.1 (Core Profile)
                //--------------------------------------------------------------------------------------
                primitive_restart_index = PRIMITIVE_RESTART_INDEX,
                texture_binding_buffer = TEXTURE_BINDING_BUFFER,
                texture_binding_rectangle = TEXTURE_BINDING_RECTANGLE,
                uniform_buffer_binding = UNIFORM_BUFFER_BINDING,
                uniform_buffer_offset_alignment = UNIFORM_BUFFER_OFFSET_ALIGNMENT,
                uniform_buffer_size = UNIFORM_BUFFER_SIZE,
                uniform_buffer_start = UNIFORM_BUFFER_START,
                max_combined_vertex_uniform_components = MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,
                max_combined_fragment_uniform_components = MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,
                max_combined_geometry_uniform_components = MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,
                max_combined_uniform_blocks = MAX_COMBINED_UNIFORM_BLOCKS,
                max_fragment_uniform_blocks = MAX_FRAGMENT_UNIFORM_BLOCKS,
                max_geometry_uniform_blocks = MAX_GEOMETRY_UNIFORM_BLOCKS,
                max_rectangle_texture_size = MAX_RECTANGLE_TEXTURE_SIZE,
                max_texture_buffer_size = MAX_TEXTURE_BUFFER_SIZE,
                max_uniform_block_size = MAX_UNIFORM_BLOCK_SIZE,
                max_uniform_buffer_bindings = MAX_UNIFORM_BUFFER_BINDINGS,
                max_vertex_uniform_blocks = MAX_VERTEX_UNIFORM_BLOCKS,
                //--------------------------------------------------------------------------------------
                // OpenGL 3.2 (Core Profile)
                //--------------------------------------------------------------------------------------
                program_point_size = PROGRAM_POINT_SIZE,
                provoking_vertex = PROVOKING_VERTEX,
                texture_binding_2d_multisample = TEXTURE_BINDING_2D_MULTISAMPLE,
                texture_binding_2d_multisample_array = TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,
                max_color_texture_samples = MAX_COLOR_TEXTURE_SAMPLES,
                max_depth_texture_samples = MAX_DEPTH_TEXTURE_SAMPLES,
                max_fragment_input_components = MAX_FRAGMENT_INPUT_COMPONENTS,
                max_geometry_input_components = MAX_GEOMETRY_INPUT_COMPONENTS,
                max_geometry_output_components = MAX_GEOMETRY_OUTPUT_COMPONENTS,
                max_geometry_texture_image_units = MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,
                max_geometry_uniform_components = MAX_GEOMETRY_UNIFORM_COMPONENTS,
                max_integer_samples = MAX_INTEGER_SAMPLES,
                max_sample_mask_words = MAX_SAMPLE_MASK_WORDS,
                max_server_wait_timeout = MAX_SERVER_WAIT_TIMEOUT,
                max_vertex_output_components = MAX_VERTEX_OUTPUT_COMPONENTS,
                //--------------------------------------------------------------------------------------
                // OpenGL 3.3 (Core Profile)
                //--------------------------------------------------------------------------------------
                max_dual_source_draw_buffers = MAX_DUAL_SOURCE_DRAW_BUFFERS,
                sampler_binding = SAMPLER_BINDING,
                timestamp = TIMESTAMP,
                //--------------------------------------------------------------------------------------
                // OpenGL 4.1 (Core Profile)
                //--------------------------------------------------------------------------------------
                shader_compiler = SHADER_COMPILER,
                shader_binary_formats = SHADER_BINARY_FORMATS,
                num_shader_binary_formats = NUM_SHADER_BINARY_FORMATS,
                max_vertex_uniform_vectors = MAX_VERTEX_UNIFORM_VECTORS,
                max_varying_vectors = MAX_VARYING_VECTORS,
                max_fragment_uniform_vectors = MAX_FRAGMENT_UNIFORM_VECTORS,
                implementation_color_read_type = IMPLEMENTATION_COLOR_READ_TYPE,
                implementation_color_read_format = IMPLEMENTATION_COLOR_READ_FORMAT,
                num_program_binary_formats = NUM_PROGRAM_BINARY_FORMATS,
                program_binary_formats = PROGRAM_BINARY_FORMATS,
                program_pipeline_binding = PROGRAM_PIPELINE_BINDING,
                max_viewports = MAX_VIEWPORTS,
                viewport_subpixel_bits = VIEWPORT_SUBPIXEL_BITS,
                viewport_bounds_range = VIEWPORT_BOUNDS_RANGE,
                layer_provoking_vertex = LAYER_PROVOKING_VERTEX,
                viewport_index_provoking_vertex = VIEWPORT_INDEX_PROVOKING_VERTEX,
            },
            CompressedTexturePixelStorage,
        });

        pub const Func = enum(Enum) {
            never = NEVER,
            less = LESS,
            equal = EQUAL,
            lequal = LEQUAL,
            greater = GREATER,
            notequal = NOTEQUAL,
            gequal = GEQUAL,
            always = ALWAYS,
        };

        pub const StencilAction = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            keep = KEEP,
            zero = ZERO,
            replace = REPLACE,
            incr = INCR,
            decr = DECR,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.4 (Core Profile)
            //--------------------------------------------------------------------------------------
            incr_wrap = INCR_WRAP,
            decr_wrap = DECR_WRAP,
        };

        pub const BlendFactor = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            zero = ZERO,
            one = ONE,
            src_color = SRC_COLOR,
            one_minus_src_color = ONE_MINUS_SRC_COLOR,
            dst_color = DST_COLOR,
            one_minus_dst_color = ONE_MINUS_DST_COLOR,
            src_alpha = SRC_ALPHA,
            one_minus_src_alpha = ONE_MINUS_SRC_ALPHA,
            dst_alpha = DST_ALPHA,
            one_minus_dst_alpha = ONE_MINUS_DST_ALPHA,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.4 (Core Profile)
            //--------------------------------------------------------------------------------------
            constant_color = CONSTANT_COLOR,
            one_minus_constant_color = ONE_MINUS_CONSTANT_COLOR,
            constant_alpha = CONSTANT_ALPHA,
            one_minus_constant_alpha = ONE_MINUS_CONSTANT_ALPHA,
        };

        pub const ColorBuffer = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            none = NONE,
            front_left = FRONT_LEFT,
            front_right = FRONT_RIGHT,
            back_left = BACK_LEFT,
            back_right = BACK_RIGHT,
            front = FRONT,
            back = BACK,
            left = LEFT,
            right = RIGHT,
            front_and_back = FRONT_AND_BACK,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            color_attachment0 = COLOR_ATTACHMENT0,
            color_attachment1 = COLOR_ATTACHMENT1,
            color_attachment2 = COLOR_ATTACHMENT2,
            color_attachment3 = COLOR_ATTACHMENT3,
            color_attachment4 = COLOR_ATTACHMENT4,
            color_attachment5 = COLOR_ATTACHMENT5,
            color_attachment6 = COLOR_ATTACHMENT6,
            color_attachment7 = COLOR_ATTACHMENT7,
            color_attachment8 = COLOR_ATTACHMENT8,
            color_attachment9 = COLOR_ATTACHMENT9,
            color_attachment10 = COLOR_ATTACHMENT10,
            color_attachment11 = COLOR_ATTACHMENT11,
            color_attachment12 = COLOR_ATTACHMENT12,
            color_attachment13 = COLOR_ATTACHMENT13,
            color_attachment14 = COLOR_ATTACHMENT14,
            color_attachment15 = COLOR_ATTACHMENT15,
            color_attachment16 = COLOR_ATTACHMENT16,
            color_attachment17 = COLOR_ATTACHMENT17,
            color_attachment18 = COLOR_ATTACHMENT18,
            color_attachment19 = COLOR_ATTACHMENT19,
            color_attachment20 = COLOR_ATTACHMENT20,
            color_attachment21 = COLOR_ATTACHMENT21,
            color_attachment22 = COLOR_ATTACHMENT22,
            color_attachment23 = COLOR_ATTACHMENT23,
            color_attachment24 = COLOR_ATTACHMENT24,
            color_attachment25 = COLOR_ATTACHMENT25,
            color_attachment26 = COLOR_ATTACHMENT26,
            color_attachment27 = COLOR_ATTACHMENT27,
            color_attachment28 = COLOR_ATTACHMENT28,
            color_attachment29 = COLOR_ATTACHMENT29,
            color_attachment30 = COLOR_ATTACHMENT30,
            color_attachment31 = COLOR_ATTACHMENT31,
        };

        pub const FramebufferTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            framebuffer = FRAMEBUFFER,
            draw_framebuffer = DRAW_FRAMEBUFFER,
            read_framebuffer = READ_FRAMEBUFFER,
        };

        pub const FramebufferAttachment = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            front_left = FRONT_LEFT,
            front_right = FRONT_RIGHT,
            back_left = BACK_LEFT,
            back_right = BACK_RIGHT,
            left = LEFT,
            right = RIGHT,
            front = FRONT,
            back = BACK,
            front_and_back = FRONT_AND_BACK,
            depth = DEPTH,
            stencil = STENCIL,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            color_attachment0 = COLOR_ATTACHMENT0,
            color_attachment1 = COLOR_ATTACHMENT1,
            color_attachment2 = COLOR_ATTACHMENT2,
            color_attachment3 = COLOR_ATTACHMENT3,
            color_attachment4 = COLOR_ATTACHMENT4,
            color_attachment5 = COLOR_ATTACHMENT5,
            color_attachment6 = COLOR_ATTACHMENT6,
            color_attachment7 = COLOR_ATTACHMENT7,
            color_attachment8 = COLOR_ATTACHMENT8,
            color_attachment9 = COLOR_ATTACHMENT9,
            color_attachment10 = COLOR_ATTACHMENT10,
            color_attachment11 = COLOR_ATTACHMENT11,
            color_attachment12 = COLOR_ATTACHMENT12,
            color_attachment13 = COLOR_ATTACHMENT13,
            color_attachment14 = COLOR_ATTACHMENT14,
            color_attachment15 = COLOR_ATTACHMENT15,
            color_attachment16 = COLOR_ATTACHMENT16,
            color_attachment17 = COLOR_ATTACHMENT17,
            color_attachment18 = COLOR_ATTACHMENT18,
            color_attachment19 = COLOR_ATTACHMENT19,
            color_attachment20 = COLOR_ATTACHMENT20,
            color_attachment21 = COLOR_ATTACHMENT21,
            color_attachment22 = COLOR_ATTACHMENT22,
            color_attachment23 = COLOR_ATTACHMENT23,
            color_attachment24 = COLOR_ATTACHMENT24,
            color_attachment25 = COLOR_ATTACHMENT25,
            color_attachment26 = COLOR_ATTACHMENT26,
            color_attachment27 = COLOR_ATTACHMENT27,
            color_attachment28 = COLOR_ATTACHMENT28,
            color_attachment29 = COLOR_ATTACHMENT29,
            color_attachment30 = COLOR_ATTACHMENT30,
            color_attachment31 = COLOR_ATTACHMENT31,
            depth_attachment = DEPTH_ATTACHMENT,
            stencil_attachment = STENCIL_ATTACHMENT,
            depth_stencil_attachment = DEPTH_STENCIL_ATTACHMENT,
        };

        pub const FramebufferAttachmentParameter = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            color_encoding = FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING,
            component_type = FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE,
            red_size = FRAMEBUFFER_ATTACHMENT_RED_SIZE,
            green_size = FRAMEBUFFER_ATTACHMENT_GREEN_SIZE,
            blue_size = FRAMEBUFFER_ATTACHMENT_BLUE_SIZE,
            alpha_size = FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE,
            depth_size = FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE,
            stencil_size = FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE,
            object_type = FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
            object_name = FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
            texture_level = FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL,
            texture_cube_map_face = FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
            texture_layer = FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            layered = FRAMEBUFFER_ATTACHMENT_LAYERED,
        };

        pub const RenderbufferTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            renderbuffer = RENDERBUFFER,
        };

        pub const FramebufferStatus = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            complete = FRAMEBUFFER_COMPLETE,
            incomplete_attachment = FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
            incomplete_missing_attachment = FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
            incomplete_draw_buffer = FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
            incomplete_read_buffer = FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
            unsupported = FRAMEBUFFER_UNSUPPORTED,
            incomplete_multisample = FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            incomplete_layer_targets = FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
            //--------------------------------------------------------------------------------------
            // OpenGL ES 2
            //--------------------------------------------------------------------------------------
            incomplete_dimensions = FRAMEBUFFER_INCOMPLETE_DIMENSIONS,
        };

        pub const ShaderType = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 2.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            vertex = VERTEX_SHADER,
            fragment = FRAGMENT_SHADER,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            geometry = GEOMETRY_SHADER,
        };

        pub const ShaderParameter = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 2.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            shader_type = SHADER_TYPE,
            delete_status = DELETE_STATUS,
            compile_status = COMPILE_STATUS,
            info_log_length = INFO_LOG_LENGTH,
            shader_source_length = SHADER_SOURCE_LENGTH,
        };

        pub const VertexAttribType = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            byte = BYTE,
            short = SHORT,
            int = INT,
            float = FLOAT,
            double = DOUBLE,
            unsigned_byte = UNSIGNED_BYTE,
            unsigned_short = UNSIGNED_SHORT,
            unsigned_int = UNSIGNED_INT,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            unsigned_int_2_10_10_10_rev = UNSIGNED_INT_2_10_10_10_REV,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            half_float = HALF_FLOAT,
            unsigned_int_10_f_11_f_11_f_rev = UNSIGNED_INT_10F_11F_11F_REV,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            int_2_10_10_10_rev = INT_2_10_10_10_REV,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            fixed = FIXED,
        };

        pub const TextureTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d = TEXTURE_1D,
            texture_2d = TEXTURE_2D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_3d = TEXTURE_3D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map = TEXTURE_CUBE_MAP,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d_array = TEXTURE_1D_ARRAY,
            texture_2d_array = TEXTURE_2D_ARRAY,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_buffer = TEXTURE_BUFFER,
            texture_rectangle = TEXTURE_RECTANGLE,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_2d_multisample = TEXTURE_2D_MULTISAMPLE,
            texture_2d_multisample_array = TEXTURE_2D_MULTISAMPLE_ARRAY,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map_array = TEXTURE_CUBE_MAP_ARRAY,
        };

        pub const TexImageTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d = TEXTURE_1D,
            texture_2d = TEXTURE_2D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_3d = TEXTURE_3D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map_positive_x = TEXTURE_CUBE_MAP_POSITIVE_X,
            texture_cube_map_positive_y = TEXTURE_CUBE_MAP_POSITIVE_Y,
            texture_cube_map_positive_z = TEXTURE_CUBE_MAP_POSITIVE_Z,
            texture_cube_map_negative_x = TEXTURE_CUBE_MAP_NEGATIVE_X,
            texture_cube_map_negative_y = TEXTURE_CUBE_MAP_NEGATIVE_Y,
            texture_cube_map_negative_z = TEXTURE_CUBE_MAP_NEGATIVE_Z,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d_array = TEXTURE_1D_ARRAY,
            texture_2d_array = TEXTURE_2D_ARRAY,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_rectangle = TEXTURE_RECTANGLE,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map_array = TEXTURE_CUBE_MAP_ARRAY,
        };

        pub const TexLevelTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d = TEXTURE_1D,
            texture_2d = TEXTURE_2D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            proxy_texture_1d = PROXY_TEXTURE_1D,
            proxy_texture_2d = PROXY_TEXTURE_2D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_3d = TEXTURE_3D,
            proxy_texture_3d = PROXY_TEXTURE_3D,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map_positive_x = TEXTURE_CUBE_MAP_POSITIVE_X,
            texture_cube_map_positive_y = TEXTURE_CUBE_MAP_POSITIVE_Y,
            texture_cube_map_positive_z = TEXTURE_CUBE_MAP_POSITIVE_Z,
            texture_cube_map_negative_x = TEXTURE_CUBE_MAP_NEGATIVE_X,
            texture_cube_map_negative_y = TEXTURE_CUBE_MAP_NEGATIVE_Y,
            texture_cube_map_negative_z = TEXTURE_CUBE_MAP_NEGATIVE_Z,
            proxy_texture_cube_map = PROXY_TEXTURE_CUBE_MAP,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d_array = TEXTURE_1D_ARRAY,
            texture_2d_array = TEXTURE_2D_ARRAY,
            proxy_texture_1d_array = PROXY_TEXTURE_1D_ARRAY,
            proxy_texture_2d_array = PROXY_TEXTURE_2D_ARRAY,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_buffer = TEXTURE_BUFFER,
            texture_rectangle = TEXTURE_RECTANGLE,
            proxy_texture_rectangle = PROXY_TEXTURE_RECTANGLE,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_2d_multisample = TEXTURE_2D_MULTISAMPLE,
            texture_2d_mulitsample_array = TEXTURE_2D_MULTISAMPLE_ARRAY,
            proxy_texture_2d_multisample = PROXY_TEXTURE_2D_MULTISAMPLE,
            proxy_texture_2d_mulitsample_array = PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map_array = TEXTURE_CUBE_MAP_ARRAY,
        };

        pub const InternalFormat = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            red = RED,
            rg = RG,
            rgb = RGB,
            rgba = RGBA,
            depth_component = DEPTH_COMPONENT,
            stencil_index = STENCIL_INDEX,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            r3_g3_b2 = R3_G3_B2,
            rgb4 = RGB4,
            rgb5 = RGB5,
            rgb8 = RGB8,
            rgb10 = RGB10,
            rgb12 = RGB12,
            rgb16 = RGB16,
            rgba2 = RGBA2,
            rgba4 = RGBA4,
            rgb5_a1 = RGB5_A1,
            rgba8 = RGBA8,
            rgb10_a2 = RGB10_A2,
            rgba12 = RGBA12,
            rgba16 = RGBA16,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            bgr = BGR,
            bgra = BGRA,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.4 (Core Profile)
            //--------------------------------------------------------------------------------------
            depth_component16 = DEPTH_COMPONENT16,
            depth_component24 = DEPTH_COMPONENT24,
            depth_component32 = DEPTH_COMPONENT32,
            //--------------------------------------------------------------------------------------
            // OpenGL 2.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            srgb8 = SRGB8,
            srgb8_alpha8 = SRGB8_ALPHA8,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            red_integer = RED_INTEGER,
            rg_integer = RG_INTEGER,
            rgb_integer = RGB_INTEGER,
            bgr_integer = BGR_INTEGER,
            rgba_integer = RGBA_INTEGER,
            bgra_integer = BGRA_INTEGER,
            r8 = R8,
            r16 = R16,
            rg8 = RG8,
            rg16 = RG16,
            r16f = R16F,
            rg16f = RG16F,
            rgb16f = RGB16F,
            rgba16f = RGBA16F,
            r32f = R32F,
            rg32f = RG32F,
            rgb32f = RGB32F,
            rgba32f = RGBA32F,
            r11f_g11f_b10f = R11F_G11F_B10F,
            rgb9_e5 = RGB9_E5,
            r8i = R8I,
            r8ui = R8UI,
            r16i = R16I,
            r16ui = R16UI,
            r32i = R32I,
            r32ui = R32UI,
            rg8i = RG8I,
            rg8ui = RG8UI,
            rg16i = RG16I,
            rg16ui = RG16UI,
            rg32i = RG32I,
            rg32ui = RG32UI,
            rgb8i = RGB8I,
            rgb8ui = RGB8UI,
            rgb16i = RGB16I,
            rgb16ui = RGB16UI,
            rgb32i = RGB32I,
            rgb32ui = RGB32UI,
            rgba8i = RGBA8I,
            rgba8ui = RGBA8UI,
            rgba16i = RGBA16I,
            rgba16ui = RGBA16UI,
            rgba32i = RGBA32I,
            rgba32ui = RGBA32UI,
            depth_component32f = DEPTH_COMPONENT32F,
            depth24_stencil8 = DEPTH24_STENCIL8,
            depth32f_stencil8 = DEPTH32F_STENCIL8,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            r8_snorm = R8_SNORM,
            r16_snorm = R16_SNORM,
            rg8_snorm = RG8_SNORM,
            rg16_snorm = RG16_SNORM,
            rgb8_snorm = RGB8_SNORM,
            rgb16_snorm = RGB16_SNORM,
            rgba8_snorm = RGBA8_SNORM,
            rgba16_snorm = RGBA16_SNORM,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            rgb10_a2ui = RGB10_A2UI,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            rgb565 = RGB565,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            compressed_rgba_bptc_unorm = COMPRESSED_RGBA_BPTC_UNORM,
            compressed_srgb_alpha_bptc_unorm = COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
            compressed_rgb_bptc_signed_float = COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
            compressed_rgb_bptc_unsigned_float = COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
        };

        pub const PixelFormat = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            red = RED,
            green = GREEN,
            blue = BLUE,
            rg = RG,
            rgb = RGB,
            rgba = RGBA,
            depth_component = DEPTH_COMPONENT,
            stencil_index = STENCIL_INDEX,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            bgr = BGR,
            bgra = BGRA,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            depth_stencil = DEPTH_STENCIL,
            red_integer = RED_INTEGER,
            green_integer = GREEN_INTEGER,
            blue_integer = BLUE_INTEGER,
            rg_integer = RG_INTEGER,
            rgb_integer = RGB_INTEGER,
            bgr_integer = BGR_INTEGER,
            rgba_integer = RGBA_INTEGER,
            bgra_integer = BGRA_INTEGER,
        };

        pub const CompressedPixelFormat = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            compressed_rgba_bptc_unorm = COMPRESSED_RGBA_BPTC_UNORM,
            compressed_srgb_alpha_bptc_unorm = COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
            compressed_rgb_bptc_signed_float = COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
            compressed_rgb_bptc_unsigned_float = COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
        };

        pub const PixelType = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            unsigned_byte = UNSIGNED_BYTE,
            byte = BYTE,
            unsigned_short = UNSIGNED_SHORT,
            short = SHORT,
            unsigned_int = UNSIGNED_INT,
            int = INT,
            float = FLOAT,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            unsigned_byte_3_3_2 = UNSIGNED_BYTE_3_3_2,
            unsigned_byte_2_3_3_rev = UNSIGNED_BYTE_2_3_3_REV,
            unsigned_short_5_6_5 = UNSIGNED_SHORT_5_6_5,
            unsigned_short_5_6_5_rev = UNSIGNED_SHORT_5_6_5_REV,
            unsigned_short_4_4_4_4 = UNSIGNED_SHORT_4_4_4_4,
            unsigned_short_4_4_4_4_rev = UNSIGNED_SHORT_4_4_4_4_REV,
            unsigned_short_5_5_5_1 = UNSIGNED_SHORT_5_5_5_1,
            unsigned_short_1_5_5_5_rev = UNSIGNED_SHORT_1_5_5_5_REV,
            unsigned_int_8_8_8_8 = UNSIGNED_INT_8_8_8_8,
            unsigned_int_8_8_8_8_rev = UNSIGNED_INT_8_8_8_8_REV,
            unsigned_int_10_10_10_2 = UNSIGNED_INT_10_10_10_2,
            unsigned_int_2_10_10_10_rev = UNSIGNED_INT_2_10_10_10_REV,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            unsigned_int_10f_11f_11f_rev = UNSIGNED_INT_10F_11F_11F_REV,
            unsigned_int_5_9_9_9_rev = UNSIGNED_INT_5_9_9_9_REV,
        };

        pub const TexParameter = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            min_filter = TEXTURE_MIN_FILTER,
            mag_filter = TEXTURE_MAG_FILTER,
            wrap_s = TEXTURE_WRAP_S,
            wrap_t = TEXTURE_WRAP_T,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            base_level = TEXTURE_BASE_LEVEL,
            min_lod = TEXTURE_MIN_LOD,
            max_lod = TEXTURE_MAX_LOD,
            max_level = TEXTURE_MAX_LEVEL,
            wrap_r = TEXTURE_WRAP_R,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.4 (Core Profile)
            //--------------------------------------------------------------------------------------
            compare_func = TEXTURE_COMPARE_FUNC,
            compare_mode = TEXTURE_COMPARE_MODE,
            lod_bias = TEXTURE_LOD_BIAS,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            swizzle_r = TEXTURE_SWIZZLE_R,
            swizzle_g = TEXTURE_SWIZZLE_G,
            swizzle_b = TEXTURE_SWIZZLE_B,
            swizzle_a = TEXTURE_SWIZZLE_A,
        };

        pub const GetTexParameter = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            min_filter = TEXTURE_MIN_FILTER,
            mag_filter = TEXTURE_MAG_FILTER,
            wrap_s = TEXTURE_WRAP_S,
            wrap_t = TEXTURE_WRAP_T,
            border_color = TEXTURE_BORDER_COLOR,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            base_level = TEXTURE_BASE_LEVEL,
            min_lod = TEXTURE_MIN_LOD,
            max_lod = TEXTURE_MAX_LOD,
            max_level = TEXTURE_MAX_LEVEL,
            wrap_r = TEXTURE_WRAP_R,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.4 (Core Profile)
            //--------------------------------------------------------------------------------------
            compare_func = TEXTURE_COMPARE_FUNC,
            compare_mode = TEXTURE_COMPARE_MODE,
            lod_bias = TEXTURE_LOD_BIAS,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            swizzle_r = TEXTURE_SWIZZLE_R,
            swizzle_g = TEXTURE_SWIZZLE_G,
            swizzle_b = TEXTURE_SWIZZLE_B,
            swizzle_a = TEXTURE_SWIZZLE_A,
            swizzle_rgba = TEXTURE_SWIZZLE_RGBA,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_immutable_format = TEXTURE_IMMUTABLE_FORMAT,
        };

        pub const GetTexLevelParameter = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            width = TEXTURE_WIDTH,
            height = TEXTURE_HEIGHT,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            internal_format = TEXTURE_INTERNAL_FORMAT,
            red_size = TEXTURE_RED_SIZE,
            green_size = TEXTURE_GREEN_SIZE,
            blue_size = TEXTURE_BLUE_SIZE,
            alpha_size = TEXTURE_ALPHA_SIZE,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            depth = TEXTURE_DEPTH,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            compressed = TEXTURE_COMPRESSED,
            compressed_image_size = TEXTURE_COMPRESSED_IMAGE_SIZE,
            //--------------------------------------------------------------------------------------
            // OpenGL 1.4 (Core Profile)
            //--------------------------------------------------------------------------------------
            depth_size = TEXTURE_DEPTH_SIZE,
            //--------------------------------------------------------------------------------------
            // TODO
            // buffer_offset = TEXTURE_BUFFER_OFFSET,
        };

        pub const MipmapTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_1d = TEXTURE_1D,
            texture_2d = TEXTURE_2D,
            texture_3d = TEXTURE_3D,
            texture_1d_array = TEXTURE_1D_ARRAY,
            texture_2d_array = TEXTURE_2D_ARRAY,
            texture_cube_map = TEXTURE_CUBE_MAP,
            //--------------------------------------------------------------------------------------
            // OpenGL 4.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            texture_cube_map_array = TEXTURE_CUBE_MAP_ARRAY,
        };

        pub const PixelStoreParameter = meta.mergeEnums(.{
            enum(Enum) {
                //--------------------------------------------------------------------------------------
                // OpenGL 1.0 (Core Profile)
                //--------------------------------------------------------------------------------------
                pack_swap_bytes = PACK_SWAP_BYTES,
                pack_lsb_first = PACK_LSB_FIRST,
                pack_row_length = PACK_ROW_LENGTH,
                pack_skip_pixels = PACK_SKIP_PIXELS,
                pack_skip_rows = PACK_SKIP_ROWS,
                pack_alignment = PACK_ALIGNMENT,
                unpack_swap_bytes = UNPACK_SWAP_BYTES,
                unpack_lsb_first = UNPACK_LSB_FIRST,
                unpack_row_length = UNPACK_ROW_LENGTH,
                unpack_skip_pixels = UNPACK_SKIP_PIXELS,
                unpack_skip_rows = UNPACK_SKIP_ROWS,
                unpack_alignment = UNPACK_ALIGNMENT,
                //--------------------------------------------------------------------------------------
                // OpenGL 1.2 (Core Profile)
                //--------------------------------------------------------------------------------------
                pack_image_height = PACK_IMAGE_HEIGHT,
                pack_skip_images = PACK_SKIP_IMAGES,
                unpack_image_height = UNPACK_IMAGE_HEIGHT,
                unpack_skip_images = UNPACK_SKIP_IMAGES,
            },
            CompressedTexturePixelStorage,
        });

        const CompressedTexturePixelStorage = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            unpack_compressed_block_width = UNPACK_COMPRESSED_BLOCK_WIDTH,
            unpack_compressed_block_height = UNPACK_COMPRESSED_BLOCK_HEIGHT,
            unpack_compressed_block_depth = UNPACK_COMPRESSED_BLOCK_DEPTH,
            unpack_compressed_block_size = UNPACK_COMPRESSED_BLOCK_SIZE,
            pack_compressed_block_width = PACK_COMPRESSED_BLOCK_WIDTH,
            pack_compressed_block_height = PACK_COMPRESSED_BLOCK_HEIGHT,
            pack_compressed_block_depth = PACK_COMPRESSED_BLOCK_DEPTH,
            pack_compressed_block_size = PACK_COMPRESSED_BLOCK_SIZE,
        };

        pub const BufferTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.5 (Core Profile)
            //--------------------------------------------------------------------------------------
            array_buffer = ARRAY_BUFFER,
            element_array_buffer = ELEMENT_ARRAY_BUFFER,
            //--------------------------------------------------------------------------------------
            // OpenGL 2.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            pixel_pack_buffer = PIXEL_PACK_BUFFER,
            pixel_unpack_buffer = PIXEL_UNPACK_BUFFER,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            transform_feedback_buffer = TRANSFORM_FEEDBACK_BUFFER,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            copy_read_buffer = COPY_READ_BUFFER,
            copy_write_buffer = COPY_WRITE_BUFFER,
            texture_buffer = TEXTURE_BUFFER,
            uniform_buffer = UNIFORM_BUFFER,
        };

        pub const IndexedBufferTarget = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            atomic_counter_buffer = ATOMIC_COUNTER_BUFFER,
        };

        pub const BufferUsage = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.5 (Core Profile)
            //--------------------------------------------------------------------------------------
            stream_draw = STREAM_DRAW,
            stream_read = STREAM_READ,
            stream_copy = STREAM_COPY,
            static_draw = STATIC_DRAW,
            static_read = STATIC_READ,
            static_copy = STATIC_COPY,
            dynamic_draw = DYNAMIC_DRAW,
            dynamic_read = DYNAMIC_READ,
            dynamic_copy = DYNAMIC_COPY,
        };

        pub const PrimitiveType = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            points = POINTS,
            line_strip = LINE_STRIP,
            line_loop = LINE_LOOP,
            lines = LINES,
            triangle_strip = TRIANGLE_STRIP,
            triangle_fan = TRIANGLE_FAN,
            triangles = TRIANGLES,
            //--------------------------------------------------------------------------------------
            // OpenGL 3.2 (Core Profile)
            //--------------------------------------------------------------------------------------
            line_strip_adjacency = LINE_STRIP_ADJACENCY,
            lines_adjacency = LINES_ADJACENCY,
            triangle_strip_adjacency = TRIANGLE_STRIP_ADJACENCY,
            triangles_adjacency = TRIANGLES_ADJACENCY,
        };

        pub const Face = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 1.0 (Core Profile)
            //--------------------------------------------------------------------------------------
            front = FRONT,
            back = BACK,
            front_and_back = FRONT_AND_BACK,
        };

        pub const ShaderPrecisionFormat = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.1 (Core Profile)
            //--------------------------------------------------------------------------------------
            low_float = LOW_FLOAT,
            medium_float = MEDIUM_FLOAT,
            high_float = HIGH_FLOAT,
            low_int = LOW_INT,
            medium_int = MEDIUM_INT,
            high_int = HIGH_INT,
        };

        pub const DebugSource = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            api = DEBUG_SOURCE_API,
            window_system = DEBUG_SOURCE_WINDOW_SYSTEM,
            shader_compiler = DEBUG_SOURCE_SHADER_COMPILER,
            third_party = DEBUG_SOURCE_THIRD_PARTY,
            application = DEBUG_SOURCE_APPLICATION,
            other = DEBUG_SOURCE_OTHER,
        };

        pub const DebugType = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            @"error" = DEBUG_TYPE_ERROR,
            deprecated_behavior = DEBUG_TYPE_DEPRECATED_BEHAVIOR,
            undefined_behavior = DEBUG_TYPE_UNDEFINED_BEHAVIOR,
            portability = DEBUG_TYPE_PORTABILITY,
            performance = DEBUG_TYPE_PERFORMANCE,
            marker = DEBUG_TYPE_MARKER,
            push_group = DEBUG_TYPE_PUSH_GROUP,
            pop_group = DEBUG_TYPE_POP_GROUP,
            other = DEBUG_TYPE_OTHER,
            debug_severity_high = DEBUG_SEVERITY_HIGH,
            debug_severity_medium = DEBUG_SEVERITY_MEDIUM,
            debug_severity_low = DEBUG_SEVERITY_LOW,
            debug_severity_notification = DEBUG_SEVERITY_NOTIFICATION,
        };

        pub const DebugSeverity = enum(Enum) {
            //--------------------------------------------------------------------------------------
            // OpenGL 4.3 (Core Profile)
            //--------------------------------------------------------------------------------------
            high = DEBUG_SEVERITY_HIGH,
            medium = DEBUG_SEVERITY_MEDIUM,
            low = DEBUG_SEVERITY_LOW,
            notification = DEBUG_SEVERITY_NOTIFICATION,
        };

        //------------------------------------------------------------------------------------------
        //
        // OpenGL 1.0 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const Enum = bindings.Enum;
        pub const Float = bindings.Float;
        pub const Int = bindings.Int;
        pub const Sizei = bindings.Sizei;
        pub const Bitfield = bindings.Bitfield;
        pub const Double = bindings.Double;
        pub const Uint = bindings.Uint;
        pub const Boolean = bindings.Boolean;
        pub const Ubyte = bindings.Ubyte;

        pub const DEPTH_BUFFER_BIT = bindings.DEPTH_BUFFER_BIT;
        pub const STENCIL_BUFFER_BIT = bindings.STENCIL_BUFFER_BIT;
        pub const COLOR_BUFFER_BIT = bindings.COLOR_BUFFER_BIT;
        pub const FALSE = bindings.FALSE;
        pub const TRUE = bindings.TRUE;
        pub const POINTS = bindings.POINTS;
        pub const LINES = bindings.LINES;
        pub const LINE_LOOP = bindings.LINE_LOOP;
        pub const LINE_STRIP = bindings.LINE_STRIP;
        pub const TRIANGLES = bindings.TRIANGLES;
        pub const TRIANGLE_STRIP = bindings.TRIANGLE_STRIP;
        pub const TRIANGLE_FAN = bindings.TRIANGLE_FAN;
        pub const QUADS = bindings.QUADS;
        pub const NEVER = bindings.NEVER;
        pub const LESS = bindings.LESS;
        pub const EQUAL = bindings.EQUAL;
        pub const LEQUAL = bindings.LEQUAL;
        pub const GREATER = bindings.GREATER;
        pub const NOTEQUAL = bindings.NOTEQUAL;
        pub const GEQUAL = bindings.GEQUAL;
        pub const ALWAYS = bindings.ALWAYS;
        pub const ZERO = bindings.ZERO;
        pub const ONE = bindings.ONE;
        pub const SRC_COLOR = bindings.SRC_COLOR;
        pub const ONE_MINUS_SRC_COLOR = bindings.ONE_MINUS_SRC_COLOR;
        pub const SRC_ALPHA = bindings.SRC_ALPHA;
        pub const ONE_MINUS_SRC_ALPHA = bindings.ONE_MINUS_SRC_ALPHA;
        pub const DST_ALPHA = bindings.DST_ALPHA;
        pub const ONE_MINUS_DST_ALPHA = bindings.ONE_MINUS_DST_ALPHA;
        pub const DST_COLOR = bindings.DST_COLOR;
        pub const ONE_MINUS_DST_COLOR = bindings.ONE_MINUS_DST_COLOR;
        pub const SRC_ALPHA_SATURATE = bindings.SRC_ALPHA_SATURATE;
        pub const NONE = bindings.NONE;
        pub const FRONT_LEFT = bindings.FRONT_LEFT;
        pub const FRONT_RIGHT = bindings.FRONT_RIGHT;
        pub const BACK_LEFT = bindings.BACK_LEFT;
        pub const BACK_RIGHT = bindings.BACK_RIGHT;
        pub const FRONT = bindings.FRONT;
        pub const BACK = bindings.BACK;
        pub const LEFT = bindings.LEFT;
        pub const RIGHT = bindings.RIGHT;
        pub const FRONT_AND_BACK = bindings.FRONT_AND_BACK;
        pub const NO_ERROR = bindings.NO_ERROR;
        pub const INVALID_ENUM = bindings.INVALID_ENUM;
        pub const INVALID_VALUE = bindings.INVALID_VALUE;
        pub const INVALID_OPERATION = bindings.INVALID_OPERATION;
        pub const OUT_OF_MEMORY = bindings.OUT_OF_MEMORY;
        pub const CW = bindings.CW;
        pub const CCW = bindings.CCW;
        pub const POINT_SIZE = bindings.POINT_SIZE;
        pub const POINT_SIZE_RANGE = bindings.POINT_SIZE_RANGE;
        pub const POINT_SIZE_GRANULARITY = bindings.POINT_SIZE_GRANULARITY;
        pub const LINE_SMOOTH = bindings.LINE_SMOOTH;
        pub const LINE_WIDTH = bindings.LINE_WIDTH;
        pub const LINE_WIDTH_RANGE = bindings.LINE_WIDTH_RANGE;
        pub const LINE_WIDTH_GRANULARITY = bindings.LINE_WIDTH_GRANULARITY;
        pub const POLYGON_MODE = bindings.POLYGON_MODE;
        pub const POLYGON_SMOOTH = bindings.POLYGON_SMOOTH;
        pub const CULL_FACE = bindings.CULL_FACE;
        pub const CULL_FACE_MODE = bindings.CULL_FACE_MODE;
        pub const FRONT_FACE = bindings.FRONT_FACE;
        pub const DEPTH_RANGE = bindings.DEPTH_RANGE;
        pub const DEPTH_TEST = bindings.DEPTH_TEST;
        pub const DEPTH_WRITEMASK = bindings.DEPTH_WRITEMASK;
        pub const DEPTH_CLEAR_VALUE = bindings.DEPTH_CLEAR_VALUE;
        pub const DEPTH_FUNC = bindings.DEPTH_FUNC;
        pub const STENCIL_TEST = bindings.STENCIL_TEST;
        pub const STENCIL_CLEAR_VALUE = bindings.STENCIL_CLEAR_VALUE;
        pub const STENCIL_FUNC = bindings.STENCIL_FUNC;
        pub const STENCIL_VALUE_MASK = bindings.STENCIL_VALUE_MASK;
        pub const STENCIL_FAIL = bindings.STENCIL_FAIL;
        pub const STENCIL_PASS_DEPTH_FAIL = bindings.STENCIL_PASS_DEPTH_FAIL;
        pub const STENCIL_PASS_DEPTH_PASS = bindings.STENCIL_PASS_DEPTH_PASS;
        pub const STENCIL_REF = bindings.STENCIL_REF;
        pub const STENCIL_WRITEMASK = bindings.STENCIL_WRITEMASK;
        pub const VIEWPORT = bindings.VIEWPORT;
        pub const DITHER = bindings.DITHER;
        pub const BLEND_DST = bindings.BLEND_DST;
        pub const BLEND_SRC = bindings.BLEND_SRC;
        pub const BLEND = bindings.BLEND;
        pub const LOGIC_OP_MODE = bindings.LOGIC_OP_MODE;
        pub const DRAW_BUFFER = bindings.DRAW_BUFFER;
        pub const READ_BUFFER = bindings.READ_BUFFER;
        pub const SCISSOR_BOX = bindings.SCISSOR_BOX;
        pub const SCISSOR_TEST = bindings.SCISSOR_TEST;
        pub const COLOR_CLEAR_VALUE = bindings.COLOR_CLEAR_VALUE;
        pub const COLOR_WRITEMASK = bindings.COLOR_WRITEMASK;
        pub const DOUBLEBUFFER = bindings.DOUBLEBUFFER;
        pub const STEREO = bindings.STEREO;
        pub const LINE_SMOOTH_HINT = bindings.LINE_SMOOTH_HINT;
        pub const POLYGON_SMOOTH_HINT = bindings.POLYGON_SMOOTH_HINT;
        pub const UNPACK_SWAP_BYTES = bindings.UNPACK_SWAP_BYTES;
        pub const UNPACK_LSB_FIRST = bindings.UNPACK_LSB_FIRST;
        pub const UNPACK_ROW_LENGTH = bindings.UNPACK_ROW_LENGTH;
        pub const UNPACK_SKIP_ROWS = bindings.UNPACK_SKIP_ROWS;
        pub const UNPACK_SKIP_PIXELS = bindings.UNPACK_SKIP_PIXELS;
        pub const UNPACK_ALIGNMENT = bindings.UNPACK_ALIGNMENT;
        pub const PACK_SWAP_BYTES = bindings.PACK_SWAP_BYTES;
        pub const PACK_LSB_FIRST = bindings.PACK_LSB_FIRST;
        pub const PACK_ROW_LENGTH = bindings.PACK_ROW_LENGTH;
        pub const PACK_SKIP_ROWS = bindings.PACK_SKIP_ROWS;
        pub const PACK_SKIP_PIXELS = bindings.PACK_SKIP_PIXELS;
        pub const PACK_ALIGNMENT = bindings.PACK_ALIGNMENT;
        pub const MAX_TEXTURE_SIZE = bindings.MAX_TEXTURE_SIZE;
        pub const MAX_VIEWPORT_DIMS = bindings.MAX_VIEWPORT_DIMS;
        pub const SUBPIXEL_BITS = bindings.SUBPIXEL_BITS;
        pub const TEXTURE_1D = bindings.TEXTURE_1D;
        pub const TEXTURE_2D = bindings.TEXTURE_2D;
        pub const TEXTURE_WIDTH = bindings.TEXTURE_WIDTH;
        pub const TEXTURE_HEIGHT = bindings.TEXTURE_HEIGHT;
        pub const TEXTURE_BORDER_COLOR = bindings.TEXTURE_BORDER_COLOR;
        pub const DONT_CARE = bindings.DONT_CARE;
        pub const FASTEST = bindings.FASTEST;
        pub const NICEST = bindings.NICEST;
        pub const BYTE = bindings.BYTE;
        pub const UNSIGNED_BYTE = bindings.UNSIGNED_BYTE;
        pub const SHORT = bindings.SHORT;
        pub const UNSIGNED_SHORT = bindings.UNSIGNED_SHORT;
        pub const INT = bindings.INT;
        pub const UNSIGNED_INT = bindings.UNSIGNED_INT;
        pub const FLOAT = bindings.FLOAT;
        pub const STACK_OVERFLOW = bindings.STACK_OVERFLOW;
        pub const STACK_UNDERFLOW = bindings.STACK_UNDERFLOW;
        pub const CLEAR = bindings.CLEAR;
        pub const AND = bindings.AND;
        pub const AND_REVERSE = bindings.AND_REVERSE;
        pub const COPY = bindings.COPY;
        pub const AND_INVERTED = bindings.AND_INVERTED;
        pub const NOOP = bindings.NOOP;
        pub const XOR = bindings.XOR;
        pub const OR = bindings.OR;
        pub const NOR = bindings.NOR;
        pub const EQUIV = bindings.EQUIV;
        pub const INVERT = bindings.INVERT;
        pub const OR_REVERSE = bindings.OR_REVERSE;
        pub const COPY_INVERTED = bindings.COPY_INVERTED;
        pub const OR_INVERTED = bindings.OR_INVERTED;
        pub const NAND = bindings.NAND;
        pub const SET = bindings.SET;
        pub const TEXTURE = bindings.TEXTURE;
        pub const COLOR = bindings.COLOR;
        pub const DEPTH = bindings.DEPTH;
        pub const STENCIL = bindings.STENCIL;
        pub const STENCIL_INDEX = bindings.STENCIL_INDEX;
        pub const DEPTH_COMPONENT = bindings.DEPTH_COMPONENT;
        pub const RED = bindings.RED;
        pub const GREEN = bindings.GREEN;
        pub const BLUE = bindings.BLUE;
        pub const ALPHA = bindings.ALPHA;
        pub const RGB = bindings.RGB;
        pub const RGBA = bindings.RGBA;
        pub const POINT = bindings.POINT;
        pub const LINE = bindings.LINE;
        pub const FILL = bindings.FILL;
        pub const KEEP = bindings.KEEP;
        pub const REPLACE = bindings.REPLACE;
        pub const INCR = bindings.INCR;
        pub const DECR = bindings.DECR;
        pub const VENDOR = bindings.VENDOR;
        pub const RENDERER = bindings.RENDERER;
        pub const VERSION = bindings.VERSION;
        pub const EXTENSIONS = bindings.EXTENSIONS;
        pub const NEAREST = bindings.NEAREST;
        pub const LINEAR = bindings.LINEAR;
        pub const NEAREST_MIPMAP_NEAREST = bindings.NEAREST_MIPMAP_NEAREST;
        pub const LINEAR_MIPMAP_NEAREST = bindings.LINEAR_MIPMAP_NEAREST;
        pub const NEAREST_MIPMAP_LINEAR = bindings.NEAREST_MIPMAP_LINEAR;
        pub const LINEAR_MIPMAP_LINEAR = bindings.LINEAR_MIPMAP_LINEAR;
        pub const TEXTURE_MAG_FILTER = bindings.TEXTURE_MAG_FILTER;
        pub const TEXTURE_MIN_FILTER = bindings.TEXTURE_MIN_FILTER;
        pub const TEXTURE_WRAP_S = bindings.TEXTURE_WRAP_S;
        pub const TEXTURE_WRAP_T = bindings.TEXTURE_WRAP_T;
        pub const REPEAT = bindings.REPEAT;

        // pub var cullFace: *const fn (mode: Enum) callconv(.C) void = undefined;
        pub fn cullFace(mode: Face) void {
            bindings.cullFace(@intFromEnum(mode));
        }

        // pub var frontFace: *const fn (mode: Enum) callconv(.C) void = undefined;
        pub fn frontFace(mode: enum(Enum) { cw = CW, ccw = CCW }) void {
            bindings.frontFace(@intFromEnum(mode));
        }

        // pub var hint: *const fn (target: Enum, mode: Enum) callconv(.C) void = undefined;
        pub fn hint(
            target: enum(Enum) {
                //------------------------------------------------------------------------------------------
                // OpenGL 1.0 (Core Profile)
                //------------------------------------------------------------------------------------------
                line_smooth = LINE_SMOOTH_HINT,
                polygon_smooth = POLYGON_SMOOTH_HINT,
                //------------------------------------------------------------------------------------------
                // OpenGL 1.3 (Core Profile)
                //------------------------------------------------------------------------------------------
                texture_compression = TEXTURE_COMPRESSION_HINT,
                //------------------------------------------------------------------------------------------
                // OpenGL 2.0 (Core Profile)
                //------------------------------------------------------------------------------------------
                fragment_shader_derivative = FRAGMENT_SHADER_DERIVATIVE_HINT,
            },
            mode: enum(Enum) {
                fastest = FASTEST,
                nicest = NICEST,
                dont_care = DONT_CARE,
            },
        ) void {
            bindings.hint(@intFromEnum(target), @intFromEnum(mode));
        }

        // pub var lineWidth: *const fn (width: Float) callconv(.C) void = undefined;
        pub fn lineWidth(width: f32) void {
            bindings.lineWidth(width);
        }

        // pub var pointSize: *const fn (size: Float) callconv(.C) void = undefined;
        pub fn pointSize(size: f32) void {
            bindings.pointSize(size);
        }

        // pub var polygonMode: *const fn (face: Enum, mode: Enum) callconv(.C) void = undefined;
        pub fn polygonMode(face: Face, mode: enum(Enum) {
            point = POINT,
            line = LINE,
            fill = FILL,
        }) void {
            bindings.polygonMode(@intFromEnum(face), @intFromEnum(mode));
        }

        // pub var scissor: *const fn (x: Int, y: Int, width: Sizei, height: Sizei) callconv(.C) void = undefined;
        pub fn scissor(x: i32, y: i32, width: i32, height: i32) void {
            bindings.scissor(x, y, width, height);
        }

        // pub var texParameterf: *const fn (target: Enum, pname: Enum, param: Float) callconv(.C) void = undefined;
        pub fn texParameterf(target: TextureTarget, pname: TexParameter, param: f32) void {
            bindings.texParameterf(@intFromEnum(target), @intFromEnum(pname), param);
        }

        // pub var texParameterfv: *const fn (target: Enum, pname: Enum, params: [*c]const Float) callconv(.C) void = undefined;
        pub fn texParameterfv(target: TextureTarget, pname: TexParameter, params: []const f32) void {
            bindings.texParameterfv(@intFromEnum(target), @intFromEnum(pname), params.ptr);
        }

        // pub var texParameteri: *const fn (target: Enum, pname: Enum, param: Int,) callconv(.C) void = undefined;
        pub fn texParameteri(target: TextureTarget, pname: TexParameter, param: i32) void {
            bindings.texParameteri(@intFromEnum(target), @intFromEnum(pname), param);
        }

        // pub var texParameteriv: *const fn (target: Enum, pname: Enum, params: [*c]const Int) callconv(.C) void = undefined;
        pub fn texParameteriv(target: TextureTarget, pname: TexParameter, params: []const i32) void {
            bindings.texParameteriv(@intFromEnum(target), @intFromEnum(pname), params.ptr);
        }

        // pub var texImage1D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     width: Sizei,
        //     border: Int,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn texImage1D(args: struct {
            target: TexImageTarget,
            level: u32,
            internal_format: InternalFormat,
            width: u32,
            format: PixelFormat,
            pixel_type: PixelType,
            data: ?[*]const u8,
        }) void {
            bindings.texImage1D(
                @intFromEnum(args.target),
                @bitCast(args.level),
                @intFromEnum(args.internal_format),
                @bitCast(args.width),
                0,
                @intFromEnum(args.format),
                @intFromEnum(args.pixel_type),
                args.data,
            );
        }

        // pub var texImage2D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     border: Int,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn texImage2D(args: struct {
            target: TexImageTarget,
            level: u32,
            internal_format: InternalFormat,
            width: u32,
            height: u32,
            format: PixelFormat,
            pixel_type: PixelType,
            data: ?[*]const u8,
        }) void {
            bindings.texImage2D(
                @intFromEnum(args.target),
                @bitCast(args.level),
                @intFromEnum(args.internal_format),
                @bitCast(args.width),
                @bitCast(args.height),
                0,
                @intFromEnum(args.format),
                @intFromEnum(args.pixel_type),
                args.data,
            );
        }

        // pub var drawBuffer: *const fn (buf: Enum) callconv(.C) void = undefined;
        pub fn drawBuffer(buf: ColorBuffer) void {
            bindings.drawBuffer(@intFromEnum(buf));
        }

        // pub var clear: *const fn (mask: Bitfield) callconv(.C) void = undefined;
        pub fn clear(mask: packed struct(Bitfield) {
            comptime {
                assert(@clz(@bitReverse(@as(Bitfield, DEPTH_BUFFER_BIT))) == @bitOffsetOf(@This(), "depth"));
                assert(@clz(@bitReverse(@as(Bitfield, STENCIL_BUFFER_BIT))) == @bitOffsetOf(@This(), "stencil"));
                assert(@clz(@bitReverse(@as(Bitfield, COLOR_BUFFER_BIT))) == @bitOffsetOf(@This(), "color"));
            }
            __unused1: u8 = 0,
            depth: bool = false,
            __unused2: u1 = 0,
            stencil: bool = false,
            __unused3: u3 = 0,
            color: bool = false,
            __unused4: u17 = 0,
        }) void {
            bindings.clear(@bitCast(mask));
        }

        // pub var clearColor: *const fn (red: Float, green: Float, blue: Float, alpha: Float) callconv(.C) void = undefined;
        pub fn clearColor(r: f32, g: f32, b: f32, a: f32) void {
            bindings.clearColor(r, g, b, a);
        }

        // pub var clearStencil: *const fn (s: Int) callconv(.C) void = undefined;
        pub fn clearStencil(s: Int) void {
            bindings.clearStencil(s);
        }

        // pub var clearDepth: *const fn (depth: Double) callconv(.C) void = undefined;
        pub fn clearDepth(depth: Double) void {
            bindings.clearDepth(depth);
        }

        // pub var stencilMask: *const fn (mask: Uint) callconv(.C) void = undefined;
        pub fn stencilMask(mask: Uint) void {
            bindings.stencilMask(mask);
        }

        // pub var colorMask: *const fn (
        //     red: Boolean,
        //     green: Boolean,
        //     blue: Boolean,
        //     alpha: Boolean,
        // ) callconv(.C) void = undefined;
        pub fn colorMask(red: bool, green: bool, blue: bool, alpha: bool) void {
            bindings.colorMask(
                @intFromBool(red),
                @intFromBool(green),
                @intFromBool(blue),
                @intFromBool(alpha),
            );
        }

        // pub var depthMask: *const fn (flag: Boolean) callconv(.C) void = undefined;
        pub fn depthMask(flag: bool) void {
            bindings.depthMask(@intFromBool(flag));
        }

        // pub var disable: *const fn (cap: Enum) callconv(.C) void = undefined;
        pub fn disable(capability: Capability) void {
            bindings.disable(@intFromEnum(capability));
        }

        // pub var enable: *const fn (cap: Enum) callconv(.C) void = undefined;
        pub fn enable(capability: Capability) void {
            bindings.enable(@intFromEnum(capability));
        }

        // pub var finish: *const fn () callconv(.C) void = undefined;
        pub fn finish() void {
            bindings.finish();
        }

        // pub var flush: *const fn () callconv(.C) void = undefined;
        pub fn flush() void {
            bindings.flush();
        }

        // pub var blendFunc: *const fn (sfactor: Enum, dfactor: Enum) callconv(.C) void = undefined;
        pub fn blendFunc(sfactor: BlendFactor, dfactor: BlendFactor) void {
            bindings.blendFunc(@intFromEnum(sfactor), @intFromEnum(dfactor));
        }

        // pub var logicOp: *const fn (opcode: Enum) callconv(.C) void = undefined;
        pub fn logicOp(opcode: enum(Enum) {
            clear = CLEAR,
            set = SET,
            copy = COPY,
            copy_inverted = COPY_INVERTED,
            noop = NOOP,
            invert = INVERT,
            @"and" = AND,
            nand = NAND,
            @"or" = OR,
            nor = NOR,
            xor = XOR,
            equiv = EQUIV,
            and_reverse = AND_REVERSE,
            or_reverse = OR_REVERSE,
            or_inverted = OR_INVERTED,
        }) void {
            bindings.logicOp(@intFromEnum(opcode));
        }

        // pub var stencilFunc: *const fn (func: Enum, ref: Int, mask: Uint) callconv(.C) void = undefined;
        pub fn stencilFunc(func: Func, ref: i32, mask: u32) void {
            bindings.stencilFunc(@intFromEnum(func), ref, mask);
        }

        // pub var stencilOp: *const fn (fail: Enum, zfail: Enum, zpass: Enum) callconv(.C) void = undefined;
        pub fn stencilOp(fail: StencilAction, zfail: StencilAction, zpass: StencilAction) void {
            bindings.stencilOp(@intFromEnum(fail), @intFromEnum(zfail), @intFromEnum(zpass));
        }

        // pub var depthFunc: *const fn (func: Enum) callconv(.C) void = undefined;
        pub fn depthFunc(func: Func) void {
            bindings.depthFunc(@intFromEnum(func));
        }

        // pub var pixelStoref: *const fn (pname: Enum, param: Float) callconv(.C) void = undefined;
        pub fn pixelStoref(pname: PixelStoreParameter, param: f32) void {
            bindings.pixelStoref(@intFromEnum(pname), param);
        }

        // pub var pixelStorei: *const fn (pname: Enum, param: Int) callconv(.C) void = undefined;
        pub fn pixelStorei(pname: PixelStoreParameter, param: i32) void {
            bindings.pixelStorei(@intFromEnum(pname), param);
        }

        // pub var readBuffer: *const fn (src: Enum) callconv(.C) void = undefined;
        pub fn readBuffer(src: ColorBuffer) void {
            bindings.readBuffer(@intFromEnum(src));
        }

        // pub var readPixels: *const fn (
        //     x: Int,
        //     y: Int,
        //     width: Sizei,
        //     height: Sizei,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn readPixels(
            x: i32,
            y: i32,
            width: i32,
            height: i32,
            format: PixelFormat,
            pixel_type: PixelType,
            pixels: ?[*]u8,
        ) void {
            bindings.readPixels(
                x,
                y,
                width,
                height,
                @intFromEnum(format),
                @intFromEnum(pixel_type),
                pixels,
            );
        }

        // pub var getBooleanv: *const fn (pname: Enum, data: [*c]Boolean) callconv(.C) void = undefined;
        pub fn getBooleanv(pname: ParamName, ptr: [*]Boolean) void {
            bindings.getBooleanv(@intFromEnum(pname), ptr);
        }

        // pub var getDoublev: *const fn (pname: Enum, data: [*c]Double) callconv(.C) void = undefined;
        pub fn getDoublev(pname: ParamName, ptr: [*]Double) void {
            bindings.getDoublev(@intFromEnum(pname), ptr);
        }

        // pub var getError: *const fn () callconv(.C) Enum = undefined;
        pub fn getError() Error {
            const res = bindings.getError();
            return std.meta.intToEnum(Error, res) catch onInvalid: {
                log.warn("getError returned unexpected value {}", .{res});
                break :onInvalid .no_error;
            };
        }

        // pub var getFloatv: *const fn (pname: Enum, data: [*c]Float) callconv(.C) void = undefined;
        pub fn getFloatv(pname: ParamName, ptr: [*]Float) void {
            bindings.getFloatv(@intFromEnum(pname), ptr);
        }

        // pub var getIntegerv: *const fn (pname: Enum, data: [*c]Int) callconv(.C) void = undefined;
        pub fn getIntegerv(pname: ParamName, ptr: [*]Int) void {
            bindings.getIntegerv(@intFromEnum(pname), ptr);
        }

        // pub var getString: *const fn (name: Enum) callconv(.C) [*c]const Ubyte = undefined;
        pub fn getString(name: StringParamName) [*:0]const u8 {
            return bindings.getString(@intFromEnum(name));
        }

        // pub var getTexImage: *const fn (
        //     target: Enum,
        //     level: Int,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn getTexImage(
            target: TexImageTarget,
            level: u32,
            format: PixelFormat,
            pixel_type: PixelType,
            pixels: ?[*]u8,
        ) void {
            bindings.getTexImage(
                @intFromEnum(target),
                @bitCast(level),
                @intFromEnum(format),
                @intFromEnum(pixel_type),
                pixels,
            );
        }

        // pub var getTexParameterfv: *const fn (target: Enum, pname: Enum, params: [*c]Float) callconv(.C) void = undefined;
        pub fn getTexParameterfv(target: TextureTarget, pname: GetTexParameter, params: []f32) void {
            bindings.getTexParameterfv(@intFromEnum(target), @intFromEnum(pname), params.ptr);
        }

        // pub var getTexParameteriv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        pub fn getTexParameteriv(target: TextureTarget, pname: GetTexParameter, params: []i32) void {
            bindings.getTexParameteriv(@intFromEnum(target), @intFromEnum(pname), params.ptr);
        }

        // pub var getTexLevelParameterfv: *const fn (
        //     target: Enum,
        //     level: Int,
        //     pname: Enum,
        //     params: [*c]Float,
        // ) callconv(.C) void = undefined;
        pub fn getTexLevelParameterfv(
            target: TexLevelTarget,
            level: u32,
            pname: GetTexLevelParameter,
            params: []f32,
        ) void {
            bindings.getTexLevelParameterfv(
                @intFromEnum(target),
                @bitCast(level),
                @intFromEnum(pname),
                params.ptr,
            );
        }

        // pub var getTexLevelParameteriv: *const fn (
        //     target: Enum,
        //     level: Int,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        pub fn getTexLevelParameteriv(
            target: TexLevelTarget,
            level: u32,
            pname: GetTexLevelParameter,
            params: []i32,
        ) void {
            bindings.getTexLevelParameteriv(
                @intFromEnum(target),
                @bitCast(level),
                @intFromEnum(pname),
                params.ptr,
            );
        }

        // pub var isEnabled: *const fn (cap: Enum) callconv(.C) Boolean = undefined;
        pub fn isEnabled(capability: Capability) bool {
            return bindings.isEnabled(@intFromEnum(capability)) == TRUE;
        }

        // pub var depthRange: *const fn (n: Double, f: Double) callconv(.C) void = undefined;
        pub fn depthRange(near: f64, far: f64) void {
            bindings.depthRange(near, far);
        }

        // pub var viewport: *const fn (x: Int, y: Int, width: Sizei, height: Sizei) callconv(.C) void = undefined;
        pub fn viewport(x: Int, y: Int, width: u32, height: u32) void {
            bindings.viewport(x, y, @as(Sizei, @bitCast(width)), @as(Sizei, @bitCast(height)));
        }

        //------------------------------------------------------------------------------------------
        //
        // OpenGL 1.1 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const Clampf = bindings.Clampf;
        pub const Clampd = bindings.Clampd;

        pub const COLOR_LOGIC_OP = bindings.COLOR_LOGIC_OP;
        pub const POLYGON_OFFSET_UNITS = bindings.POLYGON_OFFSET_UNITS;
        pub const POLYGON_OFFSET_POINT = bindings.POLYGON_OFFSET_POINT;
        pub const POLYGON_OFFSET_LINE = bindings.POLYGON_OFFSET_LINE;
        pub const POLYGON_OFFSET_FILL = bindings.POLYGON_OFFSET_FILL;
        pub const POLYGON_OFFSET_FACTOR = bindings.POLYGON_OFFSET_FACTOR;
        pub const TEXTURE_BINDING_1D = bindings.TEXTURE_BINDING_1D;
        pub const TEXTURE_BINDING_2D = bindings.TEXTURE_BINDING_2D;
        pub const TEXTURE_INTERNAL_FORMAT = bindings.TEXTURE_INTERNAL_FORMAT;
        pub const TEXTURE_RED_SIZE = bindings.TEXTURE_RED_SIZE;
        pub const TEXTURE_GREEN_SIZE = bindings.TEXTURE_GREEN_SIZE;
        pub const TEXTURE_BLUE_SIZE = bindings.TEXTURE_BLUE_SIZE;
        pub const TEXTURE_ALPHA_SIZE = bindings.TEXTURE_ALPHA_SIZE;
        pub const DOUBLE = bindings.DOUBLE;
        pub const PROXY_TEXTURE_1D = bindings.PROXY_TEXTURE_1D;
        pub const PROXY_TEXTURE_2D = bindings.PROXY_TEXTURE_2D;
        pub const R3_G3_B2 = bindings.R3_G3_B2;
        pub const RGB4 = bindings.RGB4;
        pub const RGB5 = bindings.RGB5;
        pub const RGB8 = bindings.RGB8;
        pub const RGB10 = bindings.RGB10;
        pub const RGB12 = bindings.RGB12;
        pub const RGB16 = bindings.RGB16;
        pub const RGBA2 = bindings.RGBA2;
        pub const RGBA4 = bindings.RGBA4;
        pub const RGB5_A1 = bindings.RGB5_A1;
        pub const RGBA8 = bindings.RGBA8;
        pub const RGB10_A2 = bindings.RGB10_A2;
        pub const RGBA12 = bindings.RGBA12;
        pub const RGBA16 = bindings.RGBA16;
        pub const VERTEX_ARRAY = bindings.VERTEX_ARRAY;

        // pub var drawArrays: *const fn (mode: Enum, first: Int, count: Sizei) callconv(.C) void = undefined;
        pub fn drawArrays(prim_type: PrimitiveType, first: u32, count: u32) void {
            bindings.drawArrays(@intFromEnum(prim_type), @as(Int, @bitCast(first)), @as(Sizei, @bitCast(count)));
        }

        // pub var drawElements: *const fn (
        //     mode: Enum,
        //     count: Sizei,
        //     type: Enum,
        //     indices: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var polygonOffset: *const fn (factor: Float, units: Float) callconv(.C) void = undefined;
        // pub var copyTexImage1D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     x: Int,
        //     y: Int,
        //     width: Sizei,
        //     border: Int,
        // ) callconv(.C) void = undefined;
        // pub var copyTexImage2D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     x: Int,
        //     y: Int,
        //     width: Sizei,
        //     height: Sizei,
        //     border: Int,
        // ) callconv(.C) void = undefined;
        // pub var copyTexSubImage1D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     x: Int,
        //     y: Int,
        //     width: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var copyTexSubImage2D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     yoffset: Int,
        //     x: Int,
        //     y: Int,
        //     width: Sizei,
        //     height: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var texSubImage1D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     width: Sizei,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var texSubImage2D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     yoffset: Int,
        //     width: Sizei,
        //     height: Sizei,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*const anyopaque,
        // ) callconv(.C) void = undefined;

        // pub var bindTexture: *const fn (target: Enum, texture: Uint) callconv(.C) void = undefined;
        pub fn bindTexture(target: TextureTarget, texture: Texture) void {
            bindings.bindTexture(@intFromEnum(target), @as(Uint, @bitCast(texture)));
        }

        // pub var deleteTextures: *const fn (n: Sizei, textures: [*c]const Uint) callconv(.C) void = undefined;
        pub fn deleteTexture(ptr: *const Texture) void {
            bindings.deleteTextures(1, @as([*c]const Uint, @ptrCast(ptr)));
        }
        pub fn deleteTextures(textures: []const Texture) void {
            bindings.deleteTextures(@intCast(textures.len), @as([*c]const Uint, @ptrCast(textures.ptr)));
        }

        // pub var genTextures: *const fn (n: Sizei, textures: [*c]Uint) callconv(.C) void = undefined;
        pub fn genTexture(ptr: *Texture) void {
            bindings.genTextures(1, @as([*c]Uint, @ptrCast(ptr)));
        }
        pub fn genTextures(textures: []Texture) void {
            bindings.genTextures(@intCast(textures.len), @as([*c]Uint, @ptrCast(textures.ptr)));
        }

        // pub var isTexture: *const fn (texture: Uint) callconv(.C) Boolean = undefined;
        pub fn isTexture(texture: Texture) bool {
            return bindings.isTexture(@as(Uint, @bitCast(texture))) == TRUE;
        }

        //------------------------------------------------------------------------------------------
        //
        // OpenGL 1.2 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const UNSIGNED_BYTE_3_3_2 = bindings.UNSIGNED_BYTE_3_3_2;
        pub const UNSIGNED_SHORT_4_4_4_4 = bindings.UNSIGNED_SHORT_4_4_4_4;
        pub const UNSIGNED_SHORT_5_5_5_1 = bindings.UNSIGNED_SHORT_5_5_5_1;
        pub const UNSIGNED_INT_8_8_8_8 = bindings.UNSIGNED_INT_8_8_8_8;
        pub const UNSIGNED_INT_10_10_10_2 = bindings.UNSIGNED_INT_10_10_10_2;
        pub const TEXTURE_BINDING_3D = bindings.TEXTURE_BINDING_3D;
        pub const PACK_SKIP_IMAGES = bindings.PACK_SKIP_IMAGES;
        pub const PACK_IMAGE_HEIGHT = bindings.PACK_IMAGE_HEIGHT;
        pub const UNPACK_SKIP_IMAGES = bindings.UNPACK_SKIP_IMAGES;
        pub const UNPACK_IMAGE_HEIGHT = bindings.UNPACK_IMAGE_HEIGHT;
        pub const TEXTURE_3D = bindings.TEXTURE_3D;
        pub const PROXY_TEXTURE_3D = bindings.PROXY_TEXTURE_3D;
        pub const TEXTURE_DEPTH = bindings.TEXTURE_DEPTH;
        pub const TEXTURE_WRAP_R = bindings.TEXTURE_WRAP_R;
        pub const MAX_3D_TEXTURE_SIZE = bindings.MAX_3D_TEXTURE_SIZE;
        pub const UNSIGNED_BYTE_2_3_3_REV = bindings.UNSIGNED_BYTE_2_3_3_REV;
        pub const UNSIGNED_SHORT_5_6_5 = bindings.UNSIGNED_SHORT_5_6_5;
        pub const UNSIGNED_SHORT_5_6_5_REV = bindings.UNSIGNED_SHORT_5_6_5_REV;
        pub const UNSIGNED_SHORT_4_4_4_4_REV = bindings.UNSIGNED_SHORT_4_4_4_4_REV;
        pub const UNSIGNED_SHORT_1_5_5_5_REV = bindings.UNSIGNED_SHORT_1_5_5_5_REV;
        pub const UNSIGNED_INT_8_8_8_8_REV = bindings.UNSIGNED_INT_8_8_8_8_REV;
        pub const UNSIGNED_INT_2_10_10_10_REV = bindings.UNSIGNED_INT_2_10_10_10_REV;
        pub const BGR = bindings.BGR;
        pub const BGRA = bindings.BGRA;
        pub const MAX_ELEMENTS_VERTICES = bindings.MAX_ELEMENTS_VERTICES;
        pub const MAX_ELEMENTS_INDICES = bindings.MAX_ELEMENTS_INDICES;
        pub const CLAMP_TO_EDGE = bindings.CLAMP_TO_EDGE;
        pub const TEXTURE_MIN_LOD = bindings.TEXTURE_MIN_LOD;
        pub const TEXTURE_MAX_LOD = bindings.TEXTURE_MAX_LOD;
        pub const TEXTURE_BASE_LEVEL = bindings.TEXTURE_BASE_LEVEL;
        pub const TEXTURE_MAX_LEVEL = bindings.TEXTURE_MAX_LEVEL;
        pub const SMOOTH_POINT_SIZE_RANGE = bindings.SMOOTH_POINT_SIZE_RANGE;
        pub const SMOOTH_POINT_SIZE_GRANULARITY = bindings.SMOOTH_POINT_SIZE_GRANULARITY;
        pub const SMOOTH_LINE_WIDTH_RANGE = bindings.SMOOTH_LINE_WIDTH_RANGE;
        pub const SMOOTH_LINE_WIDTH_GRANULARITY = bindings.SMOOTH_LINE_WIDTH_GRANULARITY;
        pub const ALIASED_LINE_WIDTH_RANGE = bindings.ALIASED_LINE_WIDTH_RANGE;

        // pub var drawRangeElements: *const fn (
        //     mode: Enum,
        //     start: Uint,
        //     end: Uint,
        //     count: Sizei,
        //     type: Enum,
        //     indices: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var texImage3D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     depth: Sizei,
        //     border: Int,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var texSubImage3D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     yoffset: Int,
        //     zoffset: Int,
        //     width: Sizei,
        //     height: Sizei,
        //     depth: Sizei,
        //     format: Enum,
        //     type: Enum,
        //     pixels: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var copyTexSubImage3D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     yoffset: Int,
        //     zoffset: Int,
        //     x: Int,
        //     y: Int,
        //     width: Sizei,
        //     height: Sizei,
        // ) callconv(.C) void = undefined;
        //------------------------------------------------------------------------------------------
        //
        // OpenGL 1.3 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const TEXTURE0 = bindings.TEXTURE0;
        pub const TEXTURE1 = bindings.TEXTURE1;
        pub const TEXTURE2 = bindings.TEXTURE2;
        pub const TEXTURE3 = bindings.TEXTURE3;
        pub const TEXTURE4 = bindings.TEXTURE4;
        pub const TEXTURE5 = bindings.TEXTURE5;
        pub const TEXTURE6 = bindings.TEXTURE6;
        pub const TEXTURE7 = bindings.TEXTURE7;
        pub const TEXTURE8 = bindings.TEXTURE8;
        pub const TEXTURE9 = bindings.TEXTURE9;
        pub const TEXTURE10 = bindings.TEXTURE10;
        pub const TEXTURE11 = bindings.TEXTURE11;
        pub const TEXTURE12 = bindings.TEXTURE12;
        pub const TEXTURE13 = bindings.TEXTURE13;
        pub const TEXTURE14 = bindings.TEXTURE14;
        pub const TEXTURE15 = bindings.TEXTURE15;
        pub const TEXTURE16 = bindings.TEXTURE16;
        pub const TEXTURE17 = bindings.TEXTURE17;
        pub const TEXTURE18 = bindings.TEXTURE18;
        pub const TEXTURE19 = bindings.TEXTURE19;
        pub const TEXTURE20 = bindings.TEXTURE20;
        pub const TEXTURE21 = bindings.TEXTURE21;
        pub const TEXTURE22 = bindings.TEXTURE22;
        pub const TEXTURE23 = bindings.TEXTURE23;
        pub const TEXTURE24 = bindings.TEXTURE24;
        pub const TEXTURE25 = bindings.TEXTURE25;
        pub const TEXTURE26 = bindings.TEXTURE26;
        pub const TEXTURE27 = bindings.TEXTURE27;
        pub const TEXTURE28 = bindings.TEXTURE28;
        pub const TEXTURE29 = bindings.TEXTURE29;
        pub const TEXTURE30 = bindings.TEXTURE30;
        pub const TEXTURE31 = bindings.TEXTURE31;
        pub const ACTIVE_TEXTURE = bindings.ACTIVE_TEXTURE;
        pub const MULTISAMPLE = bindings.MULTISAMPLE;
        pub const SAMPLE_ALPHA_TO_COVERAGE = bindings.SAMPLE_ALPHA_TO_COVERAGE;
        pub const SAMPLE_ALPHA_TO_ONE = bindings.SAMPLE_ALPHA_TO_ONE;
        pub const SAMPLE_COVERAGE = bindings.SAMPLE_COVERAGE;
        pub const SAMPLE_BUFFERS = bindings.SAMPLE_BUFFERS;
        pub const SAMPLES = bindings.SAMPLES;
        pub const SAMPLE_COVERAGE_VALUE = bindings.SAMPLE_COVERAGE_VALUE;
        pub const SAMPLE_COVERAGE_INVERT = bindings.SAMPLE_COVERAGE_INVERT;
        pub const TEXTURE_CUBE_MAP = bindings.TEXTURE_CUBE_MAP;
        pub const TEXTURE_BINDING_CUBE_MAP = bindings.TEXTURE_BINDING_CUBE_MAP;
        pub const TEXTURE_CUBE_MAP_POSITIVE_X = bindings.TEXTURE_CUBE_MAP_POSITIVE_X;
        pub const TEXTURE_CUBE_MAP_NEGATIVE_X = bindings.TEXTURE_CUBE_MAP_NEGATIVE_X;
        pub const TEXTURE_CUBE_MAP_POSITIVE_Y = bindings.TEXTURE_CUBE_MAP_POSITIVE_Y;
        pub const TEXTURE_CUBE_MAP_NEGATIVE_Y = bindings.TEXTURE_CUBE_MAP_NEGATIVE_Y;
        pub const TEXTURE_CUBE_MAP_POSITIVE_Z = bindings.TEXTURE_CUBE_MAP_POSITIVE_Z;
        pub const TEXTURE_CUBE_MAP_NEGATIVE_Z = bindings.TEXTURE_CUBE_MAP_NEGATIVE_Z;
        pub const PROXY_TEXTURE_CUBE_MAP = bindings.PROXY_TEXTURE_CUBE_MAP;
        pub const MAX_CUBE_MAP_TEXTURE_SIZE = bindings.MAX_CUBE_MAP_TEXTURE_SIZE;
        pub const COMPRESSED_RGB = bindings.COMPRESSED_RGB;
        pub const COMPRESSED_RGBA = bindings.COMPRESSED_RGBA;
        pub const TEXTURE_COMPRESSION_HINT = bindings.TEXTURE_COMPRESSION_HINT;
        pub const TEXTURE_COMPRESSED_IMAGE_SIZE = bindings.TEXTURE_COMPRESSED_IMAGE_SIZE;
        pub const TEXTURE_COMPRESSED = bindings.TEXTURE_COMPRESSED;
        pub const NUM_COMPRESSED_TEXTURE_FORMATS = bindings.NUM_COMPRESSED_TEXTURE_FORMATS;
        pub const COMPRESSED_TEXTURE_FORMATS = bindings.COMPRESSED_TEXTURE_FORMATS;
        pub const CLAMP_TO_BORDER = bindings.CLAMP_TO_BORDER;

        // pub var activeTexture: *const fn (texture: Enum) callconv(.C) void = undefined;
        // pub var sampleCoverage: *const fn (value: Float, invert: Boolean) callconv(.C) void = undefined;
        // pub var compressedTexImage3D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     depth: Sizei,
        //     border: Int,
        //     imageSize: Sizei,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var compressedTexImage2D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     border: Int,
        //     imageSize: Sizei,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var compressedTexImage1D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     internalformat: Enum,
        //     width: Sizei,
        //     border: Int,
        //     imageSize: Sizei,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var compressedTexSubImage3D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     yoffset: Int,
        //     zoffset: Int,
        //     width: Sizei,
        //     height: Sizei,
        //     depth: Sizei,
        //     format: Enum,
        //     imageSize: Sizei,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var compressedTexSubImage2D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     yoffset: Int,
        //     width: Sizei,
        //     height: Sizei,
        //     format: Enum,
        //     imageSize: Sizei,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var compressedTexSubImage1D: *const fn (
        //     target: Enum,
        //     level: Int,
        //     xoffset: Int,
        //     width: Sizei,
        //     format: Enum,
        //     imageSize: Sizei,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var getCompressedTexImage: *const fn (target: Enum, level: Int, img: ?*anyopaque) callconv(.C) void = undefined;
        //------------------------------------------------------------------------------------------
        //
        // OpenGL 1.4 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const BLEND_DST_RGB = bindings.BLEND_DST_RGB;
        pub const BLEND_SRC_RGB = bindings.BLEND_SRC_RGB;
        pub const BLEND_DST_ALPHA = bindings.BLEND_DST_ALPHA;
        pub const BLEND_SRC_ALPHA = bindings.BLEND_SRC_ALPHA;
        pub const POINT_FADE_THRESHOLD_SIZE = bindings.POINT_FADE_THRESHOLD_SIZE;
        pub const DEPTH_COMPONENT16 = bindings.DEPTH_COMPONENT16;
        pub const DEPTH_COMPONENT24 = bindings.DEPTH_COMPONENT24;
        pub const DEPTH_COMPONENT32 = bindings.DEPTH_COMPONENT32;
        pub const MIRRORED_REPEAT = bindings.MIRRORED_REPEAT;
        pub const MAX_TEXTURE_LOD_BIAS = bindings.MAX_TEXTURE_LOD_BIAS;
        pub const TEXTURE_LOD_BIAS = bindings.TEXTURE_LOD_BIAS;
        pub const INCR_WRAP = bindings.INCR_WRAP;
        pub const DECR_WRAP = bindings.DECR_WRAP;
        pub const TEXTURE_DEPTH_SIZE = bindings.TEXTURE_DEPTH_SIZE;
        pub const TEXTURE_COMPARE_MODE = bindings.TEXTURE_COMPARE_MODE;
        pub const TEXTURE_COMPARE_FUNC = bindings.TEXTURE_COMPARE_FUNC;
        pub const BLEND_COLOR = bindings.BLEND_COLOR;
        pub const BLEND_EQUATION = bindings.BLEND_EQUATION;
        pub const CONSTANT_COLOR = bindings.CONSTANT_COLOR;
        pub const ONE_MINUS_CONSTANT_COLOR = bindings.ONE_MINUS_CONSTANT_COLOR;
        pub const CONSTANT_ALPHA = bindings.CONSTANT_ALPHA;
        pub const ONE_MINUS_CONSTANT_ALPHA = bindings.ONE_MINUS_CONSTANT_ALPHA;
        pub const FUNC_ADD = bindings.FUNC_ADD;
        pub const FUNC_REVERSE_SUBTRACT = bindings.FUNC_REVERSE_SUBTRACT;
        pub const FUNC_SUBTRACT = bindings.FUNC_SUBTRACT;
        pub const MIN = bindings.MIN;
        pub const MAX = bindings.MAX;

        // pub var blendFuncSeparate: *const fn (
        //     sfactorRGB: Enum,
        //     dfactorRGB: Enum,
        //     sfactorAlpha: Enum,
        //     dfactorAlpha: Enum,
        // ) callconv(.C) void = undefined;
        // pub var multiDrawArrays: *const fn (
        //     mode: Enum,
        //     first: [*c]const Int,
        //     count: [*c]const Sizei,
        //     drawcount: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var multiDrawElements: *const fn (
        //     mode: Enum,
        //     count: [*c]const Sizei,
        //     type: Enum,
        //     indices: [*c]const ?*const anyopaque,
        //     drawcount: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var pointParameterf: *const fn (pname: Enum, param: Float) callconv(.C) void = undefined;
        // pub var pointParameterfv: *const fn (pname: Enum, params: [*c]const Float) callconv(.C) void = undefined;
        // pub var pointParameteri: *const fn (pname: Enum, param: Int) callconv(.C) void = undefined;
        // pub var pointParameteriv: *const fn (pname: Enum, params: [*c]const Int) callconv(.C) void = undefined;
        // pub var blendColor: *const fn (red: Float, green: Float, blue: Float, alpha: Float) callconv(.C) void = undefined;
        // pub var blendEquation: *const fn (mode: Enum) callconv(.C) void = undefined;
        //------------------------------------------------------------------------------------------
        //
        // OpenGL 1.5 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const Sizeiptr = bindings.Sizeiptr;
        pub const Intptr = bindings.Intptr;

        pub const BUFFER_SIZE = bindings.BUFFER_SIZE;
        pub const BUFFER_USAGE = bindings.BUFFER_USAGE;
        pub const QUERY_COUNTER_BITS = bindings.QUERY_COUNTER_BITS;
        pub const CURRENT_QUERY = bindings.CURRENT_QUERY;
        pub const QUERY_RESULT = bindings.QUERY_RESULT;
        pub const QUERY_RESULT_AVAILABLE = bindings.QUERY_RESULT_AVAILABLE;
        pub const ARRAY_BUFFER = bindings.ARRAY_BUFFER;
        pub const ELEMENT_ARRAY_BUFFER = bindings.ELEMENT_ARRAY_BUFFER;
        pub const ARRAY_BUFFER_BINDING = bindings.ARRAY_BUFFER_BINDING;
        pub const ELEMENT_ARRAY_BUFFER_BINDING = bindings.ELEMENT_ARRAY_BUFFER_BINDING;
        pub const VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = bindings.VERTEX_ATTRIB_ARRAY_BUFFER_BINDING;
        pub const READ_ONLY = bindings.READ_ONLY;
        pub const WRITE_ONLY = bindings.WRITE_ONLY;
        pub const READ_WRITE = bindings.READ_WRITE;
        pub const BUFFER_ACCESS = bindings.BUFFER_ACCESS;
        pub const BUFFER_MAPPED = bindings.BUFFER_MAPPED;
        pub const BUFFER_MAP_POINTER = bindings.BUFFER_MAP_POINTER;
        pub const STREAM_DRAW = bindings.STREAM_DRAW;
        pub const STREAM_READ = bindings.STREAM_READ;
        pub const STREAM_COPY = bindings.STREAM_COPY;
        pub const STATIC_DRAW = bindings.STATIC_DRAW;
        pub const STATIC_READ = bindings.STATIC_READ;
        pub const STATIC_COPY = bindings.STATIC_COPY;
        pub const DYNAMIC_DRAW = bindings.DYNAMIC_DRAW;
        pub const DYNAMIC_READ = bindings.DYNAMIC_READ;
        pub const DYNAMIC_COPY = bindings.DYNAMIC_COPY;
        pub const SAMPLES_PASSED = bindings.SAMPLES_PASSED;
        pub const SRC1_ALPHA = bindings.SRC1_ALPHA;

        // pub var genQueries: *const fn (n: Sizei, ids: [*c]Uint) callconv(.C) void = undefined;
        // pub var deleteQueries: *const fn (n: Sizei, ids: [*c]const Uint) callconv(.C) void = undefined;
        // pub var isQuery: *const fn (id: Uint) callconv(.C) Boolean = undefined;
        // pub var beginQuery: *const fn (target: Enum, id: Uint) callconv(.C) void = undefined;
        // pub var endQuery: *const fn (target: Enum) callconv(.C) void = undefined;
        // pub var getQueryiv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getQueryObjectiv: *const fn (id: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getQueryObjectuiv: *const fn (id: Uint, pname: Enum, params: [*c]Uint) callconv(.C) void = undefined;

        // pub var bindBuffer: *const fn (target: Enum, buffer: Uint) callconv(.C) void = undefined;
        pub fn bindBuffer(target: BufferTarget, buffer: Buffer) void {
            bindings.bindBuffer(@intFromEnum(target), @as(Uint, @bitCast(buffer)));
        }

        // pub var deleteBuffers: *const fn (n: Sizei, buffers: [*c]const Uint) callconv(.C) void = undefined;
        pub fn deleteBuffer(ptr: *Buffer) void {
            bindings.deleteBuffers(1, @as([*c]Uint, @ptrCast(ptr)));
        }
        pub fn deleteBuffers(buffers: []Buffer) void {
            bindings.deleteBuffers(@intCast(buffers.len), @as([*c]Uint, @ptrCast(buffers.ptr)));
        }

        // pub var genBuffers: *const fn (n: Sizei, buffers: [*c]Uint) callconv(.C) void = undefined;
        pub fn genBuffer(ptr: *Buffer) void {
            bindings.genBuffers(1, @as([*c]Uint, @ptrCast(ptr)));
        }
        pub fn genBuffers(buffers: []Buffer) void {
            bindings.genBuffers(@intCast(buffers.len), @as([*c]Uint, @ptrCast(buffers.ptr)));
        }

        // pub var isBuffer: *const fn (buffer: Uint) callconv(.C) Boolean = undefined;

        // pub var bufferData: *const fn (
        //     target: Enum,
        //     size: Sizeiptr,
        //     data: ?*const anyopaque,
        //     usage: Enum,
        // ) callconv(.C) void = undefined;
        pub fn bufferData(
            target: BufferTarget,
            size: usize,
            bytes: ?[*]const u8,
            usage: BufferUsage,
        ) void {
            bindings.bufferData(
                @intFromEnum(target),
                @as(Sizeiptr, @bitCast(size)),
                bytes,
                @intFromEnum(usage),
            );
        }

        // pub var bufferSubData: *const fn (
        //     target: Enum,
        //     offset: Intptr,
        //     size: Sizeiptr,
        //     data: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn bufferSubData(target: BufferTarget, offset: usize, bytes: []const u8) void {
            bindings.bufferSubData(
                @intFromEnum(target),
                @as(Intptr, @bitCast(offset)),
                @as(Sizeiptr, @bitCast(bytes.len)),
                bytes.ptr,
            );
        }

        // pub var getBufferSubData: *const fn (
        //     target: Enum,
        //     offset: Intptr,
        //     size: Sizeiptr,
        //     data: ?*anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var mapBuffer: *const fn (target: Enum, access: Enum) callconv(.C) ?*anyopaque = undefined;
        // pub var unmapBuffer: *const fn (target: Enum) callconv(.C) Boolean = undefined;
        // pub var getBufferParameteriv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getBufferPointerv: *const fn (
        //     target: Enum,
        //     pname: Enum,
        //     params: [*c]?*anyopaque,
        // ) callconv(.C) void = undefined;
        //------------------------------------------------------------------------------------------
        //
        // OpenGL 2.0 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const Char = bindings.Char;
        pub const Short = bindings.Short;
        pub const Byte = bindings.Byte;
        pub const Ushort = bindings.Ushort;

        pub const BLEND_EQUATION_RGB = bindings.BLEND_EQUATION_RGB;
        pub const VERTEX_ATTRIB_ARRAY_ENABLED = bindings.VERTEX_ATTRIB_ARRAY_ENABLED;
        pub const VERTEX_ATTRIB_ARRAY_SIZE = bindings.VERTEX_ATTRIB_ARRAY_SIZE;
        pub const VERTEX_ATTRIB_ARRAY_STRIDE = bindings.VERTEX_ATTRIB_ARRAY_STRIDE;
        pub const VERTEX_ATTRIB_ARRAY_TYPE = bindings.VERTEX_ATTRIB_ARRAY_TYPE;
        pub const CURRENT_VERTEX_ATTRIB = bindings.CURRENT_VERTEX_ATTRIB;
        pub const VERTEX_PROGRAM_POINT_SIZE = bindings.VERTEX_PROGRAM_POINT_SIZE;
        pub const VERTEX_ATTRIB_ARRAY_POINTER = bindings.VERTEX_ATTRIB_ARRAY_POINTER;
        pub const STENCIL_BACK_FUNC = bindings.STENCIL_BACK_FUNC;
        pub const STENCIL_BACK_FAIL = bindings.STENCIL_BACK_FAIL;
        pub const STENCIL_BACK_PASS_DEPTH_FAIL = bindings.STENCIL_BACK_PASS_DEPTH_FAIL;
        pub const STENCIL_BACK_PASS_DEPTH_PASS = bindings.STENCIL_BACK_PASS_DEPTH_PASS;
        pub const MAX_DRAW_BUFFERS = bindings.MAX_DRAW_BUFFERS;
        pub const DRAW_BUFFER0 = bindings.DRAW_BUFFER0;
        pub const DRAW_BUFFER1 = bindings.DRAW_BUFFER1;
        pub const DRAW_BUFFER2 = bindings.DRAW_BUFFER2;
        pub const DRAW_BUFFER3 = bindings.DRAW_BUFFER3;
        pub const DRAW_BUFFER4 = bindings.DRAW_BUFFER4;
        pub const DRAW_BUFFER5 = bindings.DRAW_BUFFER5;
        pub const DRAW_BUFFER6 = bindings.DRAW_BUFFER6;
        pub const DRAW_BUFFER7 = bindings.DRAW_BUFFER7;
        pub const DRAW_BUFFER8 = bindings.DRAW_BUFFER8;
        pub const DRAW_BUFFER9 = bindings.DRAW_BUFFER9;
        pub const DRAW_BUFFER10 = bindings.DRAW_BUFFER10;
        pub const DRAW_BUFFER11 = bindings.DRAW_BUFFER11;
        pub const DRAW_BUFFER12 = bindings.DRAW_BUFFER12;
        pub const DRAW_BUFFER13 = bindings.DRAW_BUFFER13;
        pub const DRAW_BUFFER14 = bindings.DRAW_BUFFER14;
        pub const DRAW_BUFFER15 = bindings.DRAW_BUFFER15;
        pub const BLEND_EQUATION_ALPHA = bindings.BLEND_EQUATION_ALPHA;
        pub const MAX_VERTEX_ATTRIBS = bindings.MAX_VERTEX_ATTRIBS;
        pub const VERTEX_ATTRIB_ARRAY_NORMALIZED = bindings.VERTEX_ATTRIB_ARRAY_NORMALIZED;
        pub const MAX_TEXTURE_IMAGE_UNITS = bindings.MAX_TEXTURE_IMAGE_UNITS;
        pub const FRAGMENT_SHADER = bindings.FRAGMENT_SHADER;
        pub const VERTEX_SHADER = bindings.VERTEX_SHADER;
        pub const MAX_FRAGMENT_UNIFORM_COMPONENTS = bindings.MAX_FRAGMENT_UNIFORM_COMPONENTS;
        pub const MAX_VERTEX_UNIFORM_COMPONENTS = bindings.MAX_VERTEX_UNIFORM_COMPONENTS;
        pub const MAX_VARYING_FLOATS = bindings.MAX_VARYING_FLOATS;
        pub const MAX_VERTEX_TEXTURE_IMAGE_UNITS = bindings.MAX_VERTEX_TEXTURE_IMAGE_UNITS;
        pub const MAX_COMBINED_TEXTURE_IMAGE_UNITS = bindings.MAX_COMBINED_TEXTURE_IMAGE_UNITS;
        pub const SHADER_TYPE = bindings.SHADER_TYPE;
        pub const FLOAT_VEC2 = bindings.FLOAT_VEC2;
        pub const FLOAT_VEC3 = bindings.FLOAT_VEC3;
        pub const FLOAT_VEC4 = bindings.FLOAT_VEC4;
        pub const INT_VEC2 = bindings.INT_VEC2;
        pub const INT_VEC3 = bindings.INT_VEC3;
        pub const INT_VEC4 = bindings.INT_VEC4;
        pub const BOOL = bindings.BOOL;
        pub const BOOL_VEC2 = bindings.BOOL_VEC2;
        pub const BOOL_VEC3 = bindings.BOOL_VEC3;
        pub const BOOL_VEC4 = bindings.BOOL_VEC4;
        pub const FLOAT_MAT2 = bindings.FLOAT_MAT2;
        pub const FLOAT_MAT3 = bindings.FLOAT_MAT3;
        pub const FLOAT_MAT4 = bindings.FLOAT_MAT4;
        pub const SAMPLER_1D = bindings.SAMPLER_1D;
        pub const SAMPLER_2D = bindings.SAMPLER_2D;
        pub const SAMPLER_3D = bindings.SAMPLER_3D;
        pub const SAMPLER_CUBE = bindings.SAMPLER_CUBE;
        pub const SAMPLER_1D_SHADOW = bindings.SAMPLER_1D_SHADOW;
        pub const SAMPLER_2D_SHADOW = bindings.SAMPLER_2D_SHADOW;
        pub const DELETE_STATUS = bindings.DELETE_STATUS;
        pub const COMPILE_STATUS = bindings.COMPILE_STATUS;
        pub const LINK_STATUS = bindings.LINK_STATUS;
        pub const VALIDATE_STATUS = bindings.VALIDATE_STATUS;
        pub const INFO_LOG_LENGTH = bindings.INFO_LOG_LENGTH;
        pub const ATTACHED_SHADERS = bindings.ATTACHED_SHADERS;
        pub const ACTIVE_UNIFORMS = bindings.ACTIVE_UNIFORMS;
        pub const ACTIVE_UNIFORM_MAX_LENGTH = bindings.ACTIVE_UNIFORM_MAX_LENGTH;
        pub const SHADER_SOURCE_LENGTH = bindings.SHADER_SOURCE_LENGTH;
        pub const ACTIVE_ATTRIBUTES = bindings.ACTIVE_ATTRIBUTES;
        pub const ACTIVE_ATTRIBUTE_MAX_LENGTH = bindings.ACTIVE_ATTRIBUTE_MAX_LENGTH;
        pub const FRAGMENT_SHADER_DERIVATIVE_HINT = bindings.FRAGMENT_SHADER_DERIVATIVE_HINT;
        pub const SHADING_LANGUAGE_VERSION = bindings.SHADING_LANGUAGE_VERSION;
        pub const CURRENT_PROGRAM = bindings.CURRENT_PROGRAM;
        pub const POINT_SPRITE_COORD_ORIGIN = bindings.POINT_SPRITE_COORD_ORIGIN;
        pub const LOWER_LEFT = bindings.LOWER_LEFT;
        pub const UPPER_LEFT = bindings.UPPER_LEFT;
        pub const STENCIL_BACK_REF = bindings.STENCIL_BACK_REF;
        pub const STENCIL_BACK_VALUE_MASK = bindings.STENCIL_BACK_VALUE_MASK;
        pub const STENCIL_BACK_WRITEMASK = bindings.STENCIL_BACK_WRITEMASK;

        // pub var blendEquationSeparate: *const fn (modeRGB: Enum, modeAlpha: Enum) callconv(.C) void = undefined;
        // pub var drawBuffers: *const fn (n: Sizei, bufs: [*c]const Enum) callconv(.C) void = undefined;
        // pub var stencilOpSeparate: *const fn (
        //     face: Enum,
        //     sfail: Enum,
        //     dpfail: Enum,
        //     dppass: Enum,
        // ) callconv(.C) void = undefined;
        // pub var stencilFuncSeparate: *const fn (face: Enum, func: Enum, ref: Int, mask: Uint) callconv(.C) void = undefined;
        // pub var stencilMaskSeparate: *const fn (face: Enum, mask: Uint) callconv(.C) void = undefined;

        // pub var attachShader: *const fn (program: Uint, shader: Uint) callconv(.C) void = undefined;
        pub fn attachShader(program: Program, shader: Shader) void {
            assert(@as(Uint, @bitCast(program)) > 0);
            assert(@as(Uint, @bitCast(shader)) > 0);
            bindings.attachShader(@as(Uint, @bitCast(program)), @as(Uint, @bitCast(shader)));
        }

        // pub var bindAttribLocation: *const fn (
        //     program: Uint,
        //     index: Uint,
        //     name: [*c]const Char,
        // ) callconv(.C) void = undefined;

        // pub var compileShader: *const fn (shader: Uint) callconv(.C) void = undefined;
        pub fn compileShader(shader: Shader) void {
            assert(@as(Uint, @bitCast(shader)) > 0);
            bindings.compileShader(@as(Uint, @bitCast(shader)));
        }

        // pub var createProgram: *const fn () callconv(.C) Uint = undefined;
        pub fn createProgram() Program {
            return @as(Program, @bitCast(bindings.createProgram()));
        }

        // pub var createShader: *const fn (type: Enum) callconv(.C) Uint = undefined;
        pub fn createShader(@"type": ShaderType) Shader {
            return @as(Shader, @bitCast(bindings.createShader(@intFromEnum(@"type"))));
        }

        // pub var deleteProgram: *const fn (program: Uint) callconv(.C) void = undefined;
        pub fn deleteProgram(program: Program) void {
            assert(@as(Uint, @bitCast(program)) > 0);
            bindings.deleteProgram(@as(Uint, @bitCast(program)));
        }

        // pub var deleteShader: *const fn (shader: Uint) callconv(.C) void = undefined;
        pub fn deleteShader(shader: Shader) void {
            assert(@as(Uint, @bitCast(shader)) > 0);
            bindings.deleteShader(@as(Uint, @bitCast(shader)));
        }

        // pub var detachShader: *const fn (program: Uint, shader: Uint) callconv(.C) void = undefined;
        // pub var disableVertexAttribArray: *const fn (index: Uint) callconv(.C) void = undefined;

        // pub var enableVertexAttribArray: *const fn (index: Uint) callconv(.C) void = undefined;
        pub fn enableVertexAttribArray(location: VertexAttribLocation) void {
            bindings.enableVertexAttribArray(@as(Uint, @bitCast(location)));
        }

        // pub var getActiveAttrib: *const fn (
        //     program: Uint,
        //     index: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     size: [*c]Int,
        //     type: [*c]Enum,
        //     name: [*c]Char,
        // ) callconv(.C) void = undefined;
        // pub var getActiveUniform: *const fn (
        //     program: Uint,
        //     index: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     size: [*c]Int,
        //     type: [*c]Enum,
        //     name: [*c]Char,
        // ) callconv(.C) Int = undefined;
        // pub var getAttachedShaders: *const fn (
        //     program: Uint,
        //     maxCount: Sizei,
        //     count: [*c]Sizei,
        //     shaders: [*c]Uint,
        // ) callconv(.C) void = undefined;

        // pub var getAttribLocation: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
        pub fn getAttribLocation(program: Program, name: [:0]const u8) ?VertexAttribLocation {
            assert(@as(Uint, @bitCast(program)) > 0);
            const location = bindings.getAttribLocation(
                @as(Uint, @bitCast(program)),
                @as([*c]const Char, @ptrCast(name.ptr)),
            );
            return if (location >= 0) @as(VertexAttribLocation, @bitCast(location)) else null;
        }

        // pub var getProgramiv: *const fn (program: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        pub fn getProgramiv(
            program: Program,
            parameter: enum(Enum) {
                //----------------------------------------------------------------------------------
                // OpenGL 2.0 (Core Profile)
                //----------------------------------------------------------------------------------
                delete_status = DELETE_STATUS,
                link_status = LINK_STATUS,
                validate_status = VALIDATE_STATUS,
                info_log_length = INFO_LOG_LENGTH,
                attached_shaders = ATTACHED_SHADERS,
                active_attributes = ACTIVE_ATTRIBUTES,
                active_attribute_max_length = ACTIVE_ATTRIBUTE_MAX_LENGTH,
                active_uniforms = ACTIVE_UNIFORMS,
                active_uniform_blocks = ACTIVE_UNIFORM_BLOCKS,
                active_uniform_block_max_name_length = ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,
                active_uniform_max_length = ACTIVE_UNIFORM_MAX_LENGTH,
                //----------------------------------------------------------------------------------
                // OpenGL 3.0 (Core Profile)
                //----------------------------------------------------------------------------------
                transform_feedback_buffer_mode = TRANSFORM_FEEDBACK_BUFFER_MODE,
                transform_feedback_varyings = TRANSFORM_FEEDBACK_VARYINGS,
                transform_feedback_varying_max_length = TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,
                //----------------------------------------------------------------------------------
                // OpenGL 3.2 (Core Profile)
                //----------------------------------------------------------------------------------
                geometry_vertices_out = GEOMETRY_VERTICES_OUT,
                geometry_input_type = GEOMETRY_INPUT_TYPE,
                geometry_output_type = GEOMETRY_OUTPUT_TYPE,
                //----------------------------------------------------------------------------------
                // OpenGL 4.1 (Core Profile)
                //----------------------------------------------------------------------------------
                program_binary_length = PROGRAM_BINARY_LENGTH,
            },
        ) Int {
            assert(@as(Uint, @bitCast(program)) > 0);
            var value: Int = undefined;
            bindings.getProgramiv(@as(Uint, @bitCast(program)), @intFromEnum(parameter), &value);
            return value;
        }

        // pub var getProgramInfoLog: *const fn (
        //     program: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     infoLog: [*c]Char,
        // ) callconv(.C) void = undefined;
        pub fn getProgramInfoLog(program: Program, buffer: []u8) ?[]const u8 {
            assert(@as(Uint, @bitCast(program)) > 0);
            assert(buffer.len > 0);
            assert(buffer.len <= std.math.maxInt(u32));
            var log_len: Sizei = 0;
            bindings.getProgramInfoLog(
                @as(Uint, @bitCast(program)),
                @as(Sizei, @bitCast(@as(u32, @intCast(buffer.len)))),
                &log_len,
                @as([*c]Char, @ptrCast(buffer.ptr)),
            );
            return if (log_len > 0) buffer[0..@as(usize, @intCast(log_len))] else null;
        }

        // pub var getShaderiv: *const fn (shader: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        pub fn getShaderiv(shader: Shader, parameter: ShaderParameter) Int {
            assert(@as(Uint, @bitCast(shader)) > 0);
            var value: Int = undefined;
            bindings.getShaderiv(@as(Uint, @bitCast(shader)), @intFromEnum(parameter), &value);
            return value;
        }

        // pub var getShaderInfoLog: *const fn (
        //     shader: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     infoLog: [*c]Char,
        // ) callconv(.C) void = undefined;
        pub fn getShaderInfoLog(shader: Shader, buffer: []u8) ?[]const u8 {
            assert(@as(Uint, @bitCast(shader)) > 0);
            assert(buffer.len > 0);
            assert(buffer.len <= std.math.maxInt(u32));
            var log_len: Sizei = 0;
            bindings.getShaderInfoLog(
                @as(Uint, @bitCast(shader)),
                @as(Sizei, @bitCast(@as(u32, @intCast(buffer.len)))),
                &log_len,
                @as([*c]Char, @ptrCast(buffer.ptr)),
            );
            return if (log_len > 0) buffer[0..@as(usize, @intCast(log_len))] else null;
        }

        // pub var getShaderSource: *const fn (
        //     shader: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     source: [*c]Char,
        // ) callconv(.C) void = undefined;

        // pub var getUniformLocation: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
        pub fn getUniformLocation(program: Program, name: [:0]const u8) ?UniformLocation {
            assert(@as(Uint, @bitCast(program)) > 0);
            const location = bindings.getUniformLocation(
                @as(Uint, @bitCast(program)),
                @as([*c]const Char, @ptrCast(name.ptr)),
            );
            return if (location >= 0) @as(UniformLocation, @bitCast(location)) else null;
        }

        // pub var getUniformfv: *const fn (program: Uint, location: Int, params: [*c]Float) callconv(.C) void = undefined;
        // pub var getUniformiv: *const fn (program: Uint, location: Int, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getVertexAttribdv: *const fn (index: Uint, pname: Enum, params: [*c]Double) callconv(.C) void = undefined;
        // pub var getVertexAttribfv: *const fn (index: Uint, pname: Enum, params: [*c]Float) callconv(.C) void = undefined;
        // pub var getVertexAttribiv: *const fn (index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getVertexAttribPointerv: *const fn (
        //     index: Uint,
        //     pname: Enum,
        //     pointer: [*c]?*anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var isProgram: *const fn (program: Uint) callconv(.C) Boolean = undefined;
        // pub var isShader: *const fn (shader: Uint) callconv(.C) Boolean = undefined;

        // pub var linkProgram: *const fn (program: Uint) callconv(.C) void = undefined;
        pub fn linkProgram(program: Program) void {
            assert(@as(Uint, @bitCast(program)) > 0);
            bindings.linkProgram(@as(Uint, @bitCast(program)));
        }

        // pub var shaderSource: *const fn (
        //     shader: Uint,
        //     count: Sizei,
        //     string: [*c]const [*c]const Char,
        //     length: [*c]const Int,
        // ) callconv(.C) void = undefined;
        pub fn shaderSource(shader: Shader, src_ptrs: []const [*:0]const u8, src_lengths: []const u32) void {
            assert(@as(Uint, @bitCast(shader)) > 0);
            assert(src_ptrs.len > 0);
            assert(src_ptrs.len <= std.math.maxInt(u32));
            assert(src_ptrs.len == src_lengths.len);
            bindings.shaderSource(
                @as(Uint, @bitCast(shader)),
                @as(Sizei, @bitCast(@as(u32, @intCast(src_ptrs.len)))),
                @as([*c]const [*c]const Char, @ptrCast(src_ptrs)),
                @as([*c]const Int, @ptrCast(src_lengths.ptr)),
            );
        }

        // pub var useProgram: *const fn (program: Uint) callconv(.C) void = undefined;
        pub fn useProgram(program: Program) void {
            bindings.useProgram(@as(Uint, @bitCast(program)));
        }

        // pub var uniform1f: *const fn (location: Int, v0: Float) callconv(.C) void = undefined;
        pub fn uniform1f(location: UniformLocation, v0: f32) void {
            bindings.uniform1f(@as(Int, @bitCast(location)), v0);
        }

        // pub var uniform2f: *const fn (location: Int, v0: Float, v1: Float) callconv(.C) void = undefined;
        pub fn uniform2f(location: UniformLocation, v0: f32, v1: f32) void {
            bindings.uniform2f(@as(Int, @bitCast(location)), v0, v1);
        }

        // pub var uniform3f: *const fn (location: Int, v0: Float, v1: Float, v2: Float) callconv(.C) void = undefined;
        pub fn uniform3f(location: UniformLocation, v0: f32, v1: f32, v2: f32) void {
            bindings.uniform3f(@as(Int, @bitCast(location)), v0, v1, v2);
        }

        // pub var uniform4f: *const fn (
        //     location: Int,
        //     v0: Float,
        //     v1: Float,
        //     v2: Float,
        //     v3: Float,
        // ) callconv(.C) void = undefined;
        pub fn uniform4f(location: UniformLocation, v0: f32, v1: f32, v2: f32, v3: f32) void {
            bindings.uniform4f(@as(Int, @bitCast(location)), v0, v1, v2, v3);
        }

        // pub var uniform1i: *const fn (location: Int, v0: Int) callconv(.C) void = undefined;
        pub fn uniform1i(location: UniformLocation, value: Int) void {
            bindings.uniform1i(@as(Int, @bitCast(location)), value);
        }

        // pub var uniform2i: *const fn (location: Int, v0: Int, v1: Int) callconv(.C) void = undefined;
        // pub var uniform3i: *const fn (location: Int, v0: Int, v1: Int, v2: Int) callconv(.C) void = undefined;
        // pub var uniform4i: *const fn (
        //     location: Int,
        //     v0: Int,
        //     v1: Int,
        //     v2: Int,
        //     v3: Int,
        // ) callconv(.C) void = undefined;
        // pub var uniform1fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniform2fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniform3fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniform4fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniform1iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
        // pub var uniform2iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
        // pub var uniform3iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
        // pub var uniform4iv: *const fn (location: Int, count: Sizei, value: [*]const Int) callconv(.C) void = undefined;
        // pub var uniformMatrix2fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniformMatrix3fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;

        // pub var uniformMatrix4fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        pub fn uniformMatrix4fv(
            location: UniformLocation,
            count: u32,
            transpose: Boolean,
            value: [*]const f32,
        ) void {
            bindings.uniformMatrix4fv(
                @as(Int, @bitCast(location)),
                @as(Sizei, @bitCast(count)),
                transpose,
                value,
            );
        }

        // pub var validateProgram: *const fn (program: Uint) callconv(.C) void = undefined;
        // pub var vertexAttrib1d: *const fn (index: Uint, x: Double) callconv(.C) void = undefined;
        // pub var vertexAttrib1dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
        // pub var vertexAttrib1f: *const fn (index: Uint, x: Float) callconv(.C) void = undefined;
        // pub var vertexAttrib1fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
        // pub var vertexAttrib1s: *const fn (index: Uint, x: Short) callconv(.C) void = undefined;
        // pub var vertexAttrib1sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
        // pub var vertexAttrib2d: *const fn (index: Uint, x: Double, y: Double) callconv(.C) void = undefined;
        // pub var vertexAttrib2dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
        // pub var vertexAttrib2f: *const fn (index: Uint, x: Float, y: Float) callconv(.C) void = undefined;
        // pub var vertexAttrib2fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
        // pub var vertexAttrib2s: *const fn (index: Uint, x: Short, y: Short) callconv(.C) void = undefined;
        // pub var vertexAttrib2sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
        // pub var vertexAttrib3d: *const fn (index: Uint, x: Double, y: Double, z: Double) callconv(.C) void = undefined;
        // pub var vertexAttrib3dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
        // pub var vertexAttrib3f: *const fn (index: Uint, x: Float, y: Float, z: Float) callconv(.C) void = undefined;
        // pub var vertexAttrib3fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
        // pub var vertexAttrib3s: *const fn (index: Uint, x: Short, y: Short, z: Short) callconv(.C) void = undefined;
        // pub var vertexAttrib3sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
        // pub var vertexAttrib4Nbv: *const fn (index: Uint, v: [*c]const Byte) callconv(.C) void = undefined;
        // pub var vertexAttrib4Niv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
        // pub var vertexAttrib4Nsv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
        // pub var vertexAttrib4Nub: *const fn (
        //     index: Uint,
        //     x: Ubyte,
        //     y: Ubyte,
        //     z: Ubyte,
        //     w: Ubyte,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttrib4Nubv: *const fn (index: Uint, v: [*c]const Ubyte) callconv(.C) void = undefined;
        // pub var vertexAttrib4Nuiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
        // pub var vertexAttrib4Nusv: *const fn (index: Uint, v: [*c]const Ushort) callconv(.C) void = undefined;
        // pub var vertexAttrib4bv: *const fn (index: Uint, v: [*c]const Byte) callconv(.C) void = undefined;
        // pub var vertexAttrib4d: *const fn (
        //     index: Uint,
        //     x: Double,
        //     y: Double,
        //     z: Double,
        //     w: Double,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttrib4dv: *const fn (index: Uint, v: [*c]const Double) callconv(.C) void = undefined;
        // pub var vertexAttrib4f: *const fn (
        //     index: Uint,
        //     x: Float,
        //     y: Float,
        //     z: Float,
        //     w: Float,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttrib4fv: *const fn (index: Uint, v: [*c]const Float) callconv(.C) void = undefined;
        // pub var vertexAttrib4iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
        // pub var vertexAttrib4s: *const fn (
        //     index: Uint,
        //     x: Short,
        //     y: Short,
        //     z: Short,
        //     w: Short,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttrib4sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
        // pub var vertexAttrib4ubv: *const fn (index: Uint, v: [*c]const Ubyte) callconv(.C) void = undefined;
        // pub var vertexAttrib4uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
        // pub var vertexAttrib4usv: *const fn (index: Uint, v: [*c]const Ushort) callconv(.C) void = undefined;

        // pub var vertexAttribPointer: *const fn (
        //     index: Uint,
        //     size: Int,
        //     type: Enum,
        //     normalized: Boolean,
        //     stride: Sizei,
        //     pointer: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn vertexAttribPointer(
            location: VertexAttribLocation,
            size: u32,
            attrib_type: VertexAttribType,
            normalised: Boolean,
            stride: u32,
            offset: usize,
        ) void {
            bindings.vertexAttribPointer(
                @as(Uint, @bitCast(location)),
                @as(Int, @bitCast(size)),
                @intFromEnum(attrib_type),
                normalised,
                @as(Sizei, @bitCast(stride)),
                @as(*allowzero const anyopaque, @ptrFromInt(offset)),
            );
        }
        //------------------------------------------------------------------------------------------
        //
        // OpenGL 2.1 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const PIXEL_PACK_BUFFER = bindings.PIXEL_PACK_BUFFER;
        pub const PIXEL_UNPACK_BUFFER = bindings.PIXEL_UNPACK_BUFFER;
        pub const PIXEL_PACK_BUFFER_BINDING = bindings.PIXEL_PACK_BUFFER_BINDING;
        pub const PIXEL_UNPACK_BUFFER_BINDING = bindings.PIXEL_UNPACK_BUFFER_BINDING;
        pub const FLOAT_MAT2x3 = bindings.FLOAT_MAT2x3;
        pub const FLOAT_MAT2x4 = bindings.FLOAT_MAT2x4;
        pub const FLOAT_MAT3x2 = bindings.FLOAT_MAT3x2;
        pub const FLOAT_MAT3x4 = bindings.FLOAT_MAT3x4;
        pub const FLOAT_MAT4x2 = bindings.FLOAT_MAT4x2;
        pub const FLOAT_MAT4x3 = bindings.FLOAT_MAT4x3;
        pub const SRGB = bindings.SRGB;
        pub const SRGB8 = bindings.SRGB8;
        pub const SRGB_ALPHA = bindings.SRGB_ALPHA;
        pub const SRGB8_ALPHA8 = bindings.SRGB8_ALPHA8;
        pub const COMPRESSED_SRGB = bindings.COMPRESSED_SRGB;
        pub const COMPRESSED_SRGB_ALPHA = bindings.COMPRESSED_SRGB_ALPHA;

        // pub var uniformMatrix2x3fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniformMatrix3x2fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniformMatrix2x4fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniformMatrix4x2fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniformMatrix3x4fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var uniformMatrix4x3fv: *const fn (
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;

        //------------------------------------------------------------------------------------------
        //
        // OpenGL 3.0 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const Half = bindings.Half;

        pub const COMPARE_REF_TO_TEXTURE = bindings.COMPARE_REF_TO_TEXTURE;
        pub const CLIP_DISTANCE0 = bindings.CLIP_DISTANCE0;
        pub const CLIP_DISTANCE1 = bindings.CLIP_DISTANCE1;
        pub const CLIP_DISTANCE2 = bindings.CLIP_DISTANCE2;
        pub const CLIP_DISTANCE3 = bindings.CLIP_DISTANCE3;
        pub const CLIP_DISTANCE4 = bindings.CLIP_DISTANCE4;
        pub const CLIP_DISTANCE5 = bindings.CLIP_DISTANCE5;
        pub const CLIP_DISTANCE6 = bindings.CLIP_DISTANCE6;
        pub const CLIP_DISTANCE7 = bindings.CLIP_DISTANCE7;
        pub const MAX_CLIP_DISTANCES = bindings.MAX_CLIP_DISTANCES;
        pub const MAJOR_VERSION = bindings.MAJOR_VERSION;
        pub const MINOR_VERSION = bindings.MINOR_VERSION;
        pub const NUM_EXTENSIONS = bindings.NUM_EXTENSIONS;
        pub const CONTEXT_FLAGS = bindings.CONTEXT_FLAGS;
        pub const COMPRESSED_RED = bindings.COMPRESSED_RED;
        pub const COMPRESSED_RG = bindings.COMPRESSED_RG;
        pub const CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT = bindings.CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
        pub const RGBA32F = bindings.RGBA32F;
        pub const RGB32F = bindings.RGB32F;
        pub const RGBA16F = bindings.RGBA16F;
        pub const RGB16F = bindings.RGB16F;
        pub const VERTEX_ATTRIB_ARRAY_INTEGER = bindings.VERTEX_ATTRIB_ARRAY_INTEGER;
        pub const MAX_ARRAY_TEXTURE_LAYERS = bindings.MAX_ARRAY_TEXTURE_LAYERS;
        pub const MIN_PROGRAM_TEXEL_OFFSET = bindings.MIN_PROGRAM_TEXEL_OFFSET;
        pub const MAX_PROGRAM_TEXEL_OFFSET = bindings.MAX_PROGRAM_TEXEL_OFFSET;
        pub const CLAMP_READ_COLOR = bindings.CLAMP_READ_COLOR;
        pub const FIXED_ONLY = bindings.FIXED_ONLY;
        pub const MAX_VARYING_COMPONENTS = bindings.MAX_VARYING_COMPONENTS;
        pub const TEXTURE_1D_ARRAY = bindings.TEXTURE_1D_ARRAY;
        pub const PROXY_TEXTURE_1D_ARRAY = bindings.PROXY_TEXTURE_1D_ARRAY;
        pub const TEXTURE_2D_ARRAY = bindings.TEXTURE_2D_ARRAY;
        pub const PROXY_TEXTURE_2D_ARRAY = bindings.PROXY_TEXTURE_2D_ARRAY;
        pub const TEXTURE_BINDING_1D_ARRAY = bindings.TEXTURE_BINDING_1D_ARRAY;
        pub const TEXTURE_BINDING_2D_ARRAY = bindings.TEXTURE_BINDING_2D_ARRAY;
        pub const R11F_G11F_B10F = bindings.R11F_G11F_B10F;
        pub const UNSIGNED_INT_10F_11F_11F_REV = bindings.UNSIGNED_INT_10F_11F_11F_REV;
        pub const RGB9_E5 = bindings.RGB9_E5;
        pub const UNSIGNED_INT_5_9_9_9_REV = bindings.UNSIGNED_INT_5_9_9_9_REV;
        pub const TEXTURE_SHARED_SIZE = bindings.TEXTURE_SHARED_SIZE;
        pub const TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH = bindings.TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH;
        pub const TRANSFORM_FEEDBACK_BUFFER_MODE = bindings.TRANSFORM_FEEDBACK_BUFFER_MODE;
        pub const MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS = bindings.MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS;
        pub const TRANSFORM_FEEDBACK_VARYINGS = bindings.TRANSFORM_FEEDBACK_VARYINGS;
        pub const TRANSFORM_FEEDBACK_BUFFER_START = bindings.TRANSFORM_FEEDBACK_BUFFER_START;
        pub const TRANSFORM_FEEDBACK_BUFFER_SIZE = bindings.TRANSFORM_FEEDBACK_BUFFER_SIZE;
        pub const PRIMITIVES_GENERATED = bindings.PRIMITIVES_GENERATED;
        pub const TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = bindings.TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
        pub const RASTERIZER_DISCARD = bindings.RASTERIZER_DISCARD;
        pub const MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = bindings.MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS;
        pub const MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS = bindings.MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS;
        pub const INTERLEAVED_ATTRIBS = bindings.INTERLEAVED_ATTRIBS;
        pub const SEPARATE_ATTRIBS = bindings.SEPARATE_ATTRIBS;
        pub const TRANSFORM_FEEDBACK_BUFFER = bindings.TRANSFORM_FEEDBACK_BUFFER;
        pub const TRANSFORM_FEEDBACK_BUFFER_BINDING = bindings.TRANSFORM_FEEDBACK_BUFFER_BINDING;
        pub const RGBA32UI = bindings.RGBA32UI;
        pub const RGB32UI = bindings.RGB32UI;
        pub const RGBA16UI = bindings.RGBA16UI;
        pub const RGB16UI = bindings.RGB16UI;
        pub const RGBA8UI = bindings.RGBA8UI;
        pub const RGB8UI = bindings.RGB8UI;
        pub const RGBA32I = bindings.RGBA32I;
        pub const RGB32I = bindings.RGB32I;
        pub const RGBA16I = bindings.RGBA16I;
        pub const RGB16I = bindings.RGB16I;
        pub const RGBA8I = bindings.RGBA8I;
        pub const RGB8I = bindings.RGB8I;
        pub const RED_INTEGER = bindings.RED_INTEGER;
        pub const GREEN_INTEGER = bindings.GREEN_INTEGER;
        pub const BLUE_INTEGER = bindings.BLUE_INTEGER;
        pub const RGB_INTEGER = bindings.RGB_INTEGER;
        pub const RGBA_INTEGER = bindings.RGBA_INTEGER;
        pub const BGR_INTEGER = bindings.BGR_INTEGER;
        pub const BGRA_INTEGER = bindings.BGRA_INTEGER;
        pub const SAMPLER_1D_ARRAY = bindings.SAMPLER_1D_ARRAY;
        pub const SAMPLER_2D_ARRAY = bindings.SAMPLER_2D_ARRAY;
        pub const SAMPLER_1D_ARRAY_SHADOW = bindings.SAMPLER_1D_ARRAY_SHADOW;
        pub const SAMPLER_2D_ARRAY_SHADOW = bindings.SAMPLER_2D_ARRAY_SHADOW;
        pub const SAMPLER_CUBE_SHADOW = bindings.SAMPLER_CUBE_SHADOW;
        pub const UNSIGNED_INT_VEC2 = bindings.UNSIGNED_INT_VEC2;
        pub const UNSIGNED_INT_VEC3 = bindings.UNSIGNED_INT_VEC3;
        pub const UNSIGNED_INT_VEC4 = bindings.UNSIGNED_INT_VEC4;
        pub const INT_SAMPLER_1D = bindings.INT_SAMPLER_1D;
        pub const INT_SAMPLER_2D = bindings.INT_SAMPLER_2D;
        pub const INT_SAMPLER_3D = bindings.INT_SAMPLER_3D;
        pub const INT_SAMPLER_CUBE = bindings.INT_SAMPLER_CUBE;
        pub const INT_SAMPLER_1D_ARRAY = bindings.INT_SAMPLER_1D_ARRAY;
        pub const INT_SAMPLER_2D_ARRAY = bindings.INT_SAMPLER_2D_ARRAY;
        pub const UNSIGNED_INT_SAMPLER_1D = bindings.UNSIGNED_INT_SAMPLER_1D;
        pub const UNSIGNED_INT_SAMPLER_2D = bindings.UNSIGNED_INT_SAMPLER_2D;
        pub const UNSIGNED_INT_SAMPLER_3D = bindings.UNSIGNED_INT_SAMPLER_3D;
        pub const UNSIGNED_INT_SAMPLER_CUBE = bindings.UNSIGNED_INT_SAMPLER_CUBE;
        pub const UNSIGNED_INT_SAMPLER_1D_ARRAY = bindings.UNSIGNED_INT_SAMPLER_1D_ARRAY;
        pub const UNSIGNED_INT_SAMPLER_2D_ARRAY = bindings.UNSIGNED_INT_SAMPLER_2D_ARRAY;
        pub const QUERY_WAIT = bindings.QUERY_WAIT;
        pub const QUERY_NO_WAIT = bindings.QUERY_NO_WAIT;
        pub const QUERY_BY_REGION_WAIT = bindings.QUERY_BY_REGION_WAIT;
        pub const QUERY_BY_REGION_NO_WAIT = bindings.QUERY_BY_REGION_NO_WAIT;
        pub const BUFFER_ACCESS_FLAGS = bindings.BUFFER_ACCESS_FLAGS;
        pub const BUFFER_MAP_LENGTH = bindings.BUFFER_MAP_LENGTH;
        pub const BUFFER_MAP_OFFSET = bindings.BUFFER_MAP_OFFSET;
        pub const DEPTH_COMPONENT32F = bindings.DEPTH_COMPONENT32F;
        pub const DEPTH32F_STENCIL8 = bindings.DEPTH32F_STENCIL8;
        pub const FLOAT_32_UNSIGNED_INT_24_8_REV = bindings.FLOAT_32_UNSIGNED_INT_24_8_REV;
        pub const INVALID_FRAMEBUFFER_OPERATION = bindings.INVALID_FRAMEBUFFER_OPERATION;
        pub const FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING = bindings.FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING;
        pub const FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE = bindings.FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE;
        pub const FRAMEBUFFER_ATTACHMENT_RED_SIZE = bindings.FRAMEBUFFER_ATTACHMENT_RED_SIZE;
        pub const FRAMEBUFFER_ATTACHMENT_GREEN_SIZE = bindings.FRAMEBUFFER_ATTACHMENT_GREEN_SIZE;
        pub const FRAMEBUFFER_ATTACHMENT_BLUE_SIZE = bindings.FRAMEBUFFER_ATTACHMENT_BLUE_SIZE;
        pub const FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE = bindings.FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE;
        pub const FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE = bindings.FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE;
        pub const FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE = bindings.FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE;
        pub const FRAMEBUFFER_DEFAULT = bindings.FRAMEBUFFER_DEFAULT;
        pub const FRAMEBUFFER_UNDEFINED = bindings.FRAMEBUFFER_UNDEFINED;
        pub const DEPTH_STENCIL_ATTACHMENT = bindings.DEPTH_STENCIL_ATTACHMENT;
        pub const MAX_RENDERBUFFER_SIZE = bindings.MAX_RENDERBUFFER_SIZE;
        pub const DEPTH_STENCIL = bindings.DEPTH_STENCIL;
        pub const UNSIGNED_INT_24_8 = bindings.UNSIGNED_INT_24_8;
        pub const DEPTH24_STENCIL8 = bindings.DEPTH24_STENCIL8;
        pub const TEXTURE_STENCIL_SIZE = bindings.TEXTURE_STENCIL_SIZE;
        pub const TEXTURE_RED_TYPE = bindings.TEXTURE_RED_TYPE;
        pub const TEXTURE_GREEN_TYPE = bindings.TEXTURE_GREEN_TYPE;
        pub const TEXTURE_BLUE_TYPE = bindings.TEXTURE_BLUE_TYPE;
        pub const TEXTURE_ALPHA_TYPE = bindings.TEXTURE_ALPHA_TYPE;
        pub const TEXTURE_DEPTH_TYPE = bindings.TEXTURE_DEPTH_TYPE;
        pub const UNSIGNED_NORMALIZED = bindings.UNSIGNED_NORMALIZED;
        pub const FRAMEBUFFER_BINDING = bindings.FRAMEBUFFER_BINDING;
        pub const DRAW_FRAMEBUFFER_BINDING = bindings.DRAW_FRAMEBUFFER_BINDING;
        pub const RENDERBUFFER_BINDING = bindings.RENDERBUFFER_BINDING;
        pub const READ_FRAMEBUFFER = bindings.READ_FRAMEBUFFER;
        pub const DRAW_FRAMEBUFFER = bindings.DRAW_FRAMEBUFFER;
        pub const READ_FRAMEBUFFER_BINDING = bindings.READ_FRAMEBUFFER_BINDING;
        pub const RENDERBUFFER_SAMPLES = bindings.RENDERBUFFER_SAMPLES;
        pub const FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = bindings.FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
        pub const FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = bindings.FRAMEBUFFER_ATTACHMENT_OBJECT_NAME;
        pub const FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = bindings.FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL;
        pub const FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = bindings.FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE;
        pub const FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER = bindings.FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER;
        pub const FRAMEBUFFER_COMPLETE = bindings.FRAMEBUFFER_COMPLETE;
        pub const FRAMEBUFFER_INCOMPLETE_ATTACHMENT = bindings.FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
        pub const FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = bindings.FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT;
        pub const FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER = bindings.FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER;
        pub const FRAMEBUFFER_INCOMPLETE_READ_BUFFER = bindings.FRAMEBUFFER_INCOMPLETE_READ_BUFFER;
        pub const FRAMEBUFFER_UNSUPPORTED = bindings.FRAMEBUFFER_UNSUPPORTED;
        pub const MAX_COLOR_ATTACHMENTS = bindings.MAX_COLOR_ATTACHMENTS;
        pub const COLOR_ATTACHMENT0 = bindings.COLOR_ATTACHMENT0;
        pub const COLOR_ATTACHMENT1 = bindings.COLOR_ATTACHMENT1;
        pub const COLOR_ATTACHMENT2 = bindings.COLOR_ATTACHMENT2;
        pub const COLOR_ATTACHMENT3 = bindings.COLOR_ATTACHMENT3;
        pub const COLOR_ATTACHMENT4 = bindings.COLOR_ATTACHMENT4;
        pub const COLOR_ATTACHMENT5 = bindings.COLOR_ATTACHMENT5;
        pub const COLOR_ATTACHMENT6 = bindings.COLOR_ATTACHMENT6;
        pub const COLOR_ATTACHMENT7 = bindings.COLOR_ATTACHMENT7;
        pub const COLOR_ATTACHMENT8 = bindings.COLOR_ATTACHMENT8;
        pub const COLOR_ATTACHMENT9 = bindings.COLOR_ATTACHMENT9;
        pub const COLOR_ATTACHMENT10 = bindings.COLOR_ATTACHMENT10;
        pub const COLOR_ATTACHMENT11 = bindings.COLOR_ATTACHMENT11;
        pub const COLOR_ATTACHMENT12 = bindings.COLOR_ATTACHMENT12;
        pub const COLOR_ATTACHMENT13 = bindings.COLOR_ATTACHMENT13;
        pub const COLOR_ATTACHMENT14 = bindings.COLOR_ATTACHMENT14;
        pub const COLOR_ATTACHMENT15 = bindings.COLOR_ATTACHMENT15;
        pub const COLOR_ATTACHMENT16 = bindings.COLOR_ATTACHMENT16;
        pub const COLOR_ATTACHMENT17 = bindings.COLOR_ATTACHMENT17;
        pub const COLOR_ATTACHMENT18 = bindings.COLOR_ATTACHMENT18;
        pub const COLOR_ATTACHMENT19 = bindings.COLOR_ATTACHMENT19;
        pub const COLOR_ATTACHMENT20 = bindings.COLOR_ATTACHMENT20;
        pub const COLOR_ATTACHMENT21 = bindings.COLOR_ATTACHMENT21;
        pub const COLOR_ATTACHMENT22 = bindings.COLOR_ATTACHMENT22;
        pub const COLOR_ATTACHMENT23 = bindings.COLOR_ATTACHMENT23;
        pub const COLOR_ATTACHMENT24 = bindings.COLOR_ATTACHMENT24;
        pub const COLOR_ATTACHMENT25 = bindings.COLOR_ATTACHMENT25;
        pub const COLOR_ATTACHMENT26 = bindings.COLOR_ATTACHMENT26;
        pub const COLOR_ATTACHMENT27 = bindings.COLOR_ATTACHMENT27;
        pub const COLOR_ATTACHMENT28 = bindings.COLOR_ATTACHMENT28;
        pub const COLOR_ATTACHMENT29 = bindings.COLOR_ATTACHMENT29;
        pub const COLOR_ATTACHMENT30 = bindings.COLOR_ATTACHMENT30;
        pub const COLOR_ATTACHMENT31 = bindings.COLOR_ATTACHMENT31;
        pub const DEPTH_ATTACHMENT = bindings.DEPTH_ATTACHMENT;
        pub const STENCIL_ATTACHMENT = bindings.STENCIL_ATTACHMENT;
        pub const FRAMEBUFFER = bindings.FRAMEBUFFER;
        pub const RENDERBUFFER = bindings.RENDERBUFFER;
        pub const RENDERBUFFER_WIDTH = bindings.RENDERBUFFER_WIDTH;
        pub const RENDERBUFFER_HEIGHT = bindings.RENDERBUFFER_HEIGHT;
        pub const RENDERBUFFER_INTERNAL_FORMAT = bindings.RENDERBUFFER_INTERNAL_FORMAT;
        pub const STENCIL_INDEX1 = bindings.STENCIL_INDEX1;
        pub const STENCIL_INDEX4 = bindings.STENCIL_INDEX4;
        pub const STENCIL_INDEX8 = bindings.STENCIL_INDEX8;
        pub const STENCIL_INDEX16 = bindings.STENCIL_INDEX16;
        pub const RENDERBUFFER_RED_SIZE = bindings.RENDERBUFFER_RED_SIZE;
        pub const RENDERBUFFER_GREEN_SIZE = bindings.RENDERBUFFER_GREEN_SIZE;
        pub const RENDERBUFFER_BLUE_SIZE = bindings.RENDERBUFFER_BLUE_SIZE;
        pub const RENDERBUFFER_ALPHA_SIZE = bindings.RENDERBUFFER_ALPHA_SIZE;
        pub const RENDERBUFFER_DEPTH_SIZE = bindings.RENDERBUFFER_DEPTH_SIZE;
        pub const RENDERBUFFER_STENCIL_SIZE = bindings.RENDERBUFFER_STENCIL_SIZE;
        pub const FRAMEBUFFER_INCOMPLETE_MULTISAMPLE = bindings.FRAMEBUFFER_INCOMPLETE_MULTISAMPLE;
        pub const MAX_SAMPLES = bindings.MAX_SAMPLES;
        pub const FRAMEBUFFER_SRGB = bindings.FRAMEBUFFER_SRGB;
        pub const HALF_FLOAT = bindings.HALF_FLOAT;
        pub const MAP_READ_BIT = bindings.MAP_READ_BIT;
        pub const MAP_WRITE_BIT = bindings.MAP_WRITE_BIT;
        pub const MAP_INVALIDATE_RANGE_BIT = bindings.MAP_INVALIDATE_RANGE_BIT;
        pub const MAP_INVALIDATE_BUFFER_BIT = bindings.MAP_INVALIDATE_BUFFER_BIT;
        pub const MAP_FLUSH_EXPLICIT_BIT = bindings.MAP_FLUSH_EXPLICIT_BIT;
        pub const MAP_UNSYNCHRONIZED_BIT = bindings.MAP_UNSYNCHRONIZED_BIT;
        pub const COMPRESSED_RED_RGTC1 = bindings.COMPRESSED_RED_RGTC1;
        pub const COMPRESSED_SIGNED_RED_RGTC1 = bindings.COMPRESSED_SIGNED_RED_RGTC1;
        pub const COMPRESSED_RG_RGTC2 = bindings.COMPRESSED_RG_RGTC2;
        pub const COMPRESSED_SIGNED_RG_RGTC2 = bindings.COMPRESSED_SIGNED_RG_RGTC2;
        pub const RG = bindings.RG;
        pub const RG_INTEGER = bindings.RG_INTEGER;
        pub const R8 = bindings.R8;
        pub const R16 = bindings.R16;
        pub const RG8 = bindings.RG8;
        pub const RG16 = bindings.RG16;
        pub const R16F = bindings.R16F;
        pub const R32F = bindings.R32F;
        pub const RG16F = bindings.RG16F;
        pub const RG32F = bindings.RG32F;
        pub const R8I = bindings.R8I;
        pub const R8UI = bindings.R8UI;
        pub const R16I = bindings.R16I;
        pub const R16UI = bindings.R16UI;
        pub const R32I = bindings.R32I;
        pub const R32UI = bindings.R32UI;
        pub const RG8I = bindings.RG8I;
        pub const RG8UI = bindings.RG8UI;
        pub const RG16I = bindings.RG16I;
        pub const RG16UI = bindings.RG16UI;
        pub const RG32I = bindings.RG32I;
        pub const RG32UI = bindings.RG32UI;
        pub const VERTEX_ARRAY_BINDING = bindings.VERTEX_ARRAY_BINDING;

        // pub var colorMaski: *const fn (
        //     index: Uint,
        //     r: Boolean,
        //     g: Boolean,
        //     b: Boolean,
        //     a: Boolean,
        // ) callconv(.C) void = undefined;
        // pub var getBooleani_v: *const fn (target: Enum, index: Uint, data: [*c]Boolean) callconv(.C) void = undefined;
        // pub var getIntegeri_v: *const fn (target: Enum, index: Uint, data: [*c]Int) callconv(.C) void = undefined;
        // pub var enablei: *const fn (target: Enum, index: Uint) callconv(.C) void = undefined;
        // pub var disablei: *const fn (target: Enum, index: Uint) callconv(.C) void = undefined;
        // pub var isEnabledi: *const fn (target: Enum, index: Uint) callconv(.C) Boolean = undefined;
        // pub var beginTransformFeedback: *const fn (primitiveMode: Enum) callconv(.C) void = undefined;
        // pub var endTransformFeedback: *const fn () callconv(.C) void = undefined;

        // pub var bindBufferRange: *const fn (
        //     target: Enum,
        //     index: Uint,
        //     buffer: Uint,
        //     offset: Intptr,
        //     size: Sizeiptr,
        // ) callconv(.C) void = undefined;
        pub fn bindbufferRange(
            target: IndexedBufferTarget,
            index: Uint,
            buffer: Buffer,
            offset: Intptr,
            size: Sizeiptr,
        ) void {
            bindings.bindBufferRange(@intFromEnum(target), index, buffer.name, offset, size);
        }

        // pub var bindBufferBase: *const fn (target: Enum, index: Uint, buffer: Uint) callconv(.C) void = undefined;
        pub fn bindBufferBase(target: IndexedBufferTarget, index: Uint, buffer: Buffer) void {
            bindings.bindBufferBase(@intFromEnum(target), index, buffer.name);
        }

        // pub var transformFeedbackVaryings: *const fn (
        //     program: Uint,
        //     count: Sizei,
        //     varyings: [*c]const [*c]const Char,
        //     bufferMode: Enum,
        // ) callconv(.C) void = undefined;
        // pub var getTransformFeedbackVarying: *const fn (
        //     program: Uint,
        //     index: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     size: [*c]Sizei,
        //     type: [*c]Enum,
        //     name: [*c]Char,
        // ) callconv(.C) void = undefined;
        // pub var clampColor: *const fn (target: Enum, clamp: Enum) callconv(.C) void = undefined;
        // pub var beginConditionalRender: *const fn (id: Uint, mode: Enum) callconv(.C) void = undefined;
        // pub var endConditionalRender: *const fn () callconv(.C) void = undefined;
        // pub var vertexAttribIPointer: *const fn (
        //     index: Uint,
        //     size: Int,
        //     type: Enum,
        //     stride: Sizei,
        //     pointer: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var getVertexAttribIiv: *const fn (index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getVertexAttribIuiv: *const fn (index: Uint, pname: Enum, params: [*c]Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI1i: *const fn (index: Uint, x: Int) callconv(.C) void = undefined;
        // pub var vertexAttribI2i: *const fn (index: Uint, x: Int, y: Int) callconv(.C) void = undefined;
        // pub var vertexAttribI3i: *const fn (index: Uint, x: Int, y: Int, z: Int) callconv(.C) void = undefined;
        // pub var vertexAttribI4i: *const fn (index: Uint, x: Int, y: Int, z: Int, w: Int) callconv(.C) void = undefined;
        // pub var vertexAttribI1ui: *const fn (index: Uint, x: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI2ui: *const fn (index: Uint, x: Uint, y: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI3ui: *const fn (index: Uint, x: Uint, y: Uint, z: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI4ui: *const fn (index: Uint, x: Uint, y: Uint, z: Uint, w: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI1iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
        // pub var vertexAttribI2iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
        // pub var vertexAttribI3iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
        // pub var vertexAttribI4iv: *const fn (index: Uint, v: [*c]const Int) callconv(.C) void = undefined;
        // pub var vertexAttribI1uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI2uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI3uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI4uiv: *const fn (index: Uint, v: [*c]const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribI4bv: *const fn (index: Uint, v: [*c]const Byte) callconv(.C) void = undefined;
        // pub var vertexAttribI4sv: *const fn (index: Uint, v: [*c]const Short) callconv(.C) void = undefined;
        // pub var vertexAttribI4ubv: *const fn (index: Uint, v: [*c]const Ubyte) callconv(.C) void = undefined;
        // pub var vertexAttribI4usv: *const fn (index: Uint, v: [*c]const Ushort) callconv(.C) void = undefined;
        // pub var getUniformuiv: *const fn (program: Uint, location: Int, params: [*c]Uint) callconv(.C) void = undefined;
        // pub var bindFragDataLocation: *const fn (
        //     program: Uint,
        //     color: Uint,
        //     name: [*c]const Char,
        // ) callconv(.C) void = undefined;
        // pub var getFragDataLocation: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
        // pub var uniform1ui: *const fn (location: Int, v0: Uint) callconv(.C) void = undefined;
        // pub var uniform2ui: *const fn (location: Int, v0: Uint, v1: Uint) callconv(.C) void = undefined;
        // pub var uniform3ui: *const fn (location: Int, v0: Uint, v1: Uint, v2: Uint) callconv(.C) void = undefined;
        // pub var uniform4ui: *const fn (location: Int, v0: Uint, v1: Uint, v2: Uint, v3: Uint) callconv(.C) void = undefined;
        // pub var uniform1uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
        // pub var uniform2uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
        // pub var uniform3uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
        // pub var uniform4uiv: *const fn (location: Int, count: Sizei, value: [*c]const Uint) callconv(.C) void = undefined;
        // pub var texParameterIiv: *const fn (target: Enum, pname: Enum, params: [*c]const Int) callconv(.C) void = undefined;
        // pub var texParameterIuiv: *const fn (
        //     target: Enum,
        //     pname: Enum,
        //     params: [*c]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var getTexParameterIiv: *const fn (target: Enum, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;
        // pub var getTexParameterIuiv: *const fn (target: Enum, pname: Enum, params: [*c]Uint) callconv(.C) void = undefined;
        // pub var clearBufferiv: *const fn (buffer: Enum, drawbuffer: Int, value: [*c]const Int) callconv(.C) void = undefined;
        // pub var clearBufferuiv: *const fn (
        //     buffer: Enum,
        //     drawbuffer: Int,
        //     value: [*c]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var clearBufferfv: *const fn (
        //     buffer: Enum,
        //     drawbuffer: Int,
        //     value: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var clearBufferfi: *const fn (
        //     buffer: Enum,
        //     drawbuffer: Int,
        //     depth: Float,
        //     stencil: Int,
        // ) callconv(.C) void = undefined;

        // pub var getStringi: *const fn (name: Enum, index: Uint) callconv(.C) [*c]const Ubyte = undefined;
        pub fn getStringi(name: StringParamName, index: Uint) [*:0]const u8 {
            return bindings.getStringi(@intFromEnum(name), index);
        }

        // pub var isRenderbuffer: *const fn (renderbuffer: Uint) callconv(.C) Boolean = undefined;
        pub fn isRenderbuffer(renderbuffer: Renderbuffer) bool {
            return bindings.isRenderbuffer(@as(Uint, @bitCast(renderbuffer))) == TRUE;
        }

        // pub var bindRenderbuffer: *const fn (target: Enum, renderbuffer: Uint) callconv(.C) void = undefined;
        pub fn bindRenderbuffer(target: RenderbufferTarget, renderbuffer: Renderbuffer) void {
            bindings.bindRenderbuffer(@intFromEnum(target), @as(Uint, @bitCast(renderbuffer)));
        }

        // pub var deleteRenderbuffers: *const fn (n: Sizei, renderbuffers: [*c]const Uint) callconv(.C) void = undefined;
        pub fn deleteRenderbuffer(ptr: *const Renderbuffer) void {
            bindings.deleteRenderbuffers(1, @as([*c]const Uint, @ptrCast(ptr)));
        }
        pub fn deleteRenderbuffers(renderbuffers: []const Renderbuffer) void {
            bindings.deleteRenderbuffers(@intCast(renderbuffers.len), @as([*c]const Uint, @ptrCast(renderbuffers.ptr)));
        }

        // pub var genRenderbuffers: *const fn (n: Sizei, renderbuffers: [*c]Uint) callconv(.C) void = undefined;
        pub fn genRenderbuffer(ptr: *Renderbuffer) void {
            bindings.genRenderbuffers(1, @as([*c]Uint, @ptrCast(ptr)));
        }
        pub fn genRenderbuffers(renderbuffers: []Renderbuffer) void {
            bindings.genRenderbuffers(@intCast(renderbuffers.len), @as([*c]Uint, @ptrCast(renderbuffers.ptr)));
        }

        // pub var renderbufferStorage: *const fn (
        //     target: Enum,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        // ) callconv(.C) void = undefined;
        pub fn renderbufferStorage(
            target: RenderbufferTarget,
            internal_format: InternalFormat,
            width: u32,
            height: u32,
        ) void {
            bindings.renderbufferStorage(
                @intFromEnum(target),
                @intFromEnum(internal_format),
                @as(Sizei, @bitCast(width)),
                @as(Sizei, @bitCast(height)),
            );
        }

        // pub var getRenderbufferParameteriv: *const fn (
        //     target: Enum,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;

        // pub var isFramebuffer: *const fn (framebuffer: Uint) callconv(.C) Boolean = undefined;
        pub fn isFramebuffer(framebuffer: Framebuffer) bool {
            return bindings.isFramebuffer(@as(Uint, @bitCast(framebuffer))) == TRUE;
        }

        // pub var bindFramebuffer: *const fn (target: Enum, framebuffer: Uint) callconv(.C) void = undefined;
        pub fn bindFramebuffer(target: FramebufferTarget, framebuffer: Framebuffer) void {
            bindings.bindFramebuffer(@intFromEnum(target), @as(Uint, @bitCast(framebuffer)));
        }

        // pub var deleteFramebuffers: *const fn (n: Sizei, framebuffers: [*c]const Uint) callconv(.C) void = undefined;
        pub fn deleteFramebuffer(ptr: *const Framebuffer) void {
            bindings.deleteFramebuffers(1, @as([*c]const Uint, @ptrCast(ptr)));
        }
        pub fn deleteFramebuffers(framebuffers: []const Framebuffer) void {
            bindings.deleteFramebuffers(@intCast(framebuffers.len), @as([*c]const Uint, @ptrCast(framebuffers.ptr)));
        }

        // pub var genFramebuffers: *const fn (n: Sizei, framebuffers: [*c]Uint) callconv(.C) void = undefined;
        pub fn genFramebuffer(ptr: *Framebuffer) void {
            bindings.genFramebuffers(1, @as([*c]Uint, @ptrCast(ptr)));
        }
        pub fn genFramebuffers(framebuffers: []Framebuffer) void {
            bindings.genFramebuffers(@intCast(framebuffers.len), @as([*c]Uint, @ptrCast(framebuffers.ptr)));
        }

        // pub var checkFramebufferStatus: *const fn (target: Enum) callconv(.C) Enum = undefined;
        pub fn checkFramebufferStatus(target: FramebufferTarget) FramebufferStatus {
            const res = bindings.checkFramebufferStatus(@intFromEnum(target));
            return std.meta.intToEnum(FramebufferStatus, res) catch onInvalid: {
                log.warn("checkFramebufferStatus returned unexpected value {}", .{res});
                break :onInvalid .complete;
            };
        }

        // pub var framebufferTexture1D: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     textarget: Enum,
        //     texture: Uint,
        //     level: Int,
        // ) callconv(.C) void = undefined;

        // pub var framebufferTexture2D: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     textarget: Enum,
        //     texture: Uint,
        //     level: Int,
        // ) callconv(.C) void = undefined;
        pub fn framebufferTexture2D(
            target: FramebufferTarget,
            attachment: FramebufferAttachment,
            textarget: TextureTarget,
            texture: Texture,
            level: Int,
        ) void {
            bindings.framebufferTexture2D(
                @intFromEnum(target),
                @intFromEnum(attachment),
                @intFromEnum(textarget),
                @as(Uint, @bitCast(texture)),
                level,
            );
        }

        // pub var framebufferTexture3D: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     textarget: Enum,
        //     texture: Uint,
        //     level: Int,
        //     zoffset: Int,
        // ) callconv(.C) void = undefined;

        // pub var framebufferRenderbuffer: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     renderbuffertarget: Enum,
        //     renderbuffer: Uint,
        // ) callconv(.C) void = undefined;
        pub fn framebufferRenderbuffer(
            target: FramebufferTarget,
            attachment: FramebufferAttachment,
            renderbuffertarget: RenderbufferTarget,
            renderbuffer: Renderbuffer,
        ) void {
            bindings.framebufferRenderbuffer(
                @intFromEnum(target),
                @intFromEnum(attachment),
                @intFromEnum(renderbuffertarget),
                @as(Uint, @bitCast(renderbuffer)),
            );
        }

        // pub var getFramebufferAttachmentParameteriv: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        pub fn getFramebufferAttachmentParameteriv(
            target: FramebufferTarget,
            attachment: FramebufferAttachment,
            pname: FramebufferAttachmentParameter,
        ) Int {
            var result: Int = undefined;
            bindings.getFramebufferAttachmentParameteriv(
                @intFromEnum(target),
                @intFromEnum(attachment),
                @intFromEnum(pname),
                &result,
            );
            return result;
        }

        // pub var generateMipmap: *const fn (target: Enum) callconv(.C) void = undefined;
        pub fn generateMipmap(target: MipmapTarget) void {
            bindings.generateMipmap(@intFromEnum(target));
        }

        // pub var blitFramebuffer: *const fn (
        //     srcX0: Int,
        //     srcY0: Int,
        //     srcX1: Int,
        //     srcY1: Int,
        //     dstX0: Int,
        //     dstY0: Int,
        //     dstX1: Int,
        //     dstY1: Int,
        //     mask: Bitfield,
        //     filter: Enum,
        // ) callconv(.C) void = undefined;
        // pub var renderbufferStorageMultisample: *const fn (
        //     target: Enum,
        //     samples: Sizei,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var framebufferTextureLayer: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     texture: Uint,
        //     level: Int,
        //     layer: Int,
        // ) callconv(.C) void = undefined;
        // pub var mapBufferRange: *const fn (
        //     target: Enum,
        //     offset: Intptr,
        //     length: Sizeiptr,
        //     access: Bitfield,
        // ) callconv(.C) ?*anyopaque = undefined;
        // pub var flushMappedBufferRange: *const fn (
        //     target: Enum,
        //     offset: Intptr,
        //     length: Sizeiptr,
        // ) callconv(.C) void = undefined;

        // pub var bindVertexArray: *const fn (array: Uint) callconv(.C) void = undefined;
        pub fn bindVertexArray(array: VertexArrayObject) void {
            bindings.bindVertexArray(@as(Uint, @bitCast(array)));
        }

        // pub var deleteVertexArrays: *const fn (n: Sizei, arrays: [*c]const Uint) callconv(.C) void = undefined;
        pub fn deleteVertexArray(ptr: *const VertexArrayObject) void {
            bindings.deleteVertexArrays(1, @ptrCast(ptr));
        }
        pub fn deleteVertexArrays(arrays: []const VertexArrayObject) void {
            bindings.deleteVertexArrays(@intCast(arrays.len), @ptrCast(arrays.ptr));
        }

        // pub var genVertexArrays: *const fn (n: Sizei, arrays: [*c]Uint) callconv(.C) void = undefined;
        pub fn genVertexArray(ptr: *VertexArrayObject) void {
            bindings.genVertexArrays(1, @as([*c]Uint, @ptrCast(ptr)));
        }
        pub fn genVertexArrays(arrays: []VertexArrayObject) void {
            bindings.genVertexArrays(@intCast(arrays.len), @ptrCast(arrays.ptr));
        }

        // pub var isVertexArray: *const fn (array: Uint) callconv(.C) Boolean = undefined;
        pub fn isVertexArray(array: VertexArrayObject) bool {
            return bindings.isVertexArray(@as(Uint, @bitCast(array))) == TRUE;
        }

        //------------------------------------------------------------------------------------------
        //
        // OpenGL 3.1 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const SAMPLER_2D_RECT = bindings.SAMPLER_2D_RECT;
        pub const SAMPLER_2D_RECT_SHADOW = bindings.SAMPLER_2D_RECT_SHADOW;
        pub const SAMPLER_BUFFER = bindings.SAMPLER_BUFFER;
        pub const INT_SAMPLER_2D_RECT = bindings.INT_SAMPLER_2D_RECT;
        pub const INT_SAMPLER_BUFFER = bindings.INT_SAMPLER_BUFFER;
        pub const UNSIGNED_INT_SAMPLER_2D_RECT = bindings.UNSIGNED_INT_SAMPLER_2D_RECT;
        pub const UNSIGNED_INT_SAMPLER_BUFFER = bindings.UNSIGNED_INT_SAMPLER_BUFFER;
        pub const TEXTURE_BUFFER = bindings.TEXTURE_BUFFER;
        pub const MAX_TEXTURE_BUFFER_SIZE = bindings.MAX_TEXTURE_BUFFER_SIZE;
        pub const TEXTURE_BINDING_BUFFER = bindings.TEXTURE_BINDING_BUFFER;
        pub const TEXTURE_BUFFER_DATA_STORE_BINDING = bindings.TEXTURE_BUFFER_DATA_STORE_BINDING;
        pub const TEXTURE_RECTANGLE = bindings.TEXTURE_RECTANGLE;
        pub const TEXTURE_BINDING_RECTANGLE = bindings.TEXTURE_BINDING_RECTANGLE;
        pub const PROXY_TEXTURE_RECTANGLE = bindings.PROXY_TEXTURE_RECTANGLE;
        pub const MAX_RECTANGLE_TEXTURE_SIZE = bindings.MAX_RECTANGLE_TEXTURE_SIZE;
        pub const R8_SNORM = bindings.R8_SNORM;
        pub const RG8_SNORM = bindings.RG8_SNORM;
        pub const RGB8_SNORM = bindings.RGB8_SNORM;
        pub const RGBA8_SNORM = bindings.RGBA8_SNORM;
        pub const R16_SNORM = bindings.R16_SNORM;
        pub const RG16_SNORM = bindings.RG16_SNORM;
        pub const RGB16_SNORM = bindings.RGB16_SNORM;
        pub const RGBA16_SNORM = bindings.RGBA16_SNORM;
        pub const SIGNED_NORMALIZED = bindings.SIGNED_NORMALIZED;
        pub const PRIMITIVE_RESTART = bindings.PRIMITIVE_RESTART;
        pub const PRIMITIVE_RESTART_INDEX = bindings.PRIMITIVE_RESTART_INDEX;
        pub const COPY_READ_BUFFER = bindings.COPY_READ_BUFFER;
        pub const COPY_WRITE_BUFFER = bindings.COPY_WRITE_BUFFER;
        pub const UNIFORM_BUFFER = bindings.UNIFORM_BUFFER;
        pub const UNIFORM_BUFFER_BINDING = bindings.UNIFORM_BUFFER_BINDING;
        pub const UNIFORM_BUFFER_START = bindings.UNIFORM_BUFFER_START;
        pub const UNIFORM_BUFFER_SIZE = bindings.UNIFORM_BUFFER_SIZE;
        pub const MAX_VERTEX_UNIFORM_BLOCKS = bindings.MAX_VERTEX_UNIFORM_BLOCKS;
        pub const MAX_GEOMETRY_UNIFORM_BLOCKS = bindings.MAX_GEOMETRY_UNIFORM_BLOCKS;
        pub const MAX_FRAGMENT_UNIFORM_BLOCKS = bindings.MAX_FRAGMENT_UNIFORM_BLOCKS;
        pub const MAX_COMBINED_UNIFORM_BLOCKS = bindings.MAX_COMBINED_UNIFORM_BLOCKS;
        pub const MAX_UNIFORM_BUFFER_BINDINGS = bindings.MAX_UNIFORM_BUFFER_BINDINGS;
        pub const MAX_UNIFORM_BLOCK_SIZE = bindings.MAX_UNIFORM_BLOCK_SIZE;
        pub const MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS = bindings.MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS;
        pub const MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS = bindings.MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS;
        pub const MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS = bindings.MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS;
        pub const UNIFORM_BUFFER_OFFSET_ALIGNMENT = bindings.UNIFORM_BUFFER_OFFSET_ALIGNMENT;
        pub const ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH = bindings.ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH;
        pub const ACTIVE_UNIFORM_BLOCKS = bindings.ACTIVE_UNIFORM_BLOCKS;
        pub const UNIFORM_TYPE = bindings.UNIFORM_TYPE;
        pub const UNIFORM_SIZE = bindings.UNIFORM_SIZE;
        pub const UNIFORM_NAME_LENGTH = bindings.UNIFORM_NAME_LENGTH;
        pub const UNIFORM_BLOCK_INDEX = bindings.UNIFORM_BLOCK_INDEX;
        pub const UNIFORM_OFFSET = bindings.UNIFORM_OFFSET;
        pub const UNIFORM_ARRAY_STRIDE = bindings.UNIFORM_ARRAY_STRIDE;
        pub const UNIFORM_MATRIX_STRIDE = bindings.UNIFORM_MATRIX_STRIDE;
        pub const UNIFORM_IS_ROW_MAJOR = bindings.UNIFORM_IS_ROW_MAJOR;
        pub const UNIFORM_BLOCK_BINDING = bindings.UNIFORM_BLOCK_BINDING;
        pub const UNIFORM_BLOCK_DATA_SIZE = bindings.UNIFORM_BLOCK_DATA_SIZE;
        pub const UNIFORM_BLOCK_NAME_LENGTH = bindings.UNIFORM_BLOCK_NAME_LENGTH;
        pub const UNIFORM_BLOCK_ACTIVE_UNIFORMS = bindings.UNIFORM_BLOCK_ACTIVE_UNIFORMS;
        pub const UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES = bindings.UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES;
        pub const UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER = bindings.UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER;
        pub const UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER = bindings.UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER;
        pub const UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER = bindings.UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER;
        pub const INVALID_INDEX = bindings.INVALID_INDEX;

        // pub var drawArraysInstanced: *const fn (
        //     mode: Enum,
        //     first: Int,
        //     count: Sizei,
        //     instancecount: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var drawElementsInstanced: *const fn (
        //     mode: Enum,
        //     count: Sizei,
        //     type: Enum,
        //     indices: ?*const anyopaque,
        //     instancecount: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var texBuffer: *const fn (target: Enum, internalformat: Enum, buffer: Uint) callconv(.C) void = undefined;
        // pub var primitiveRestartIndex: *const fn (index: Uint) callconv(.C) void = undefined;
        // pub var copyBufferSubData: *const fn (
        //     readTarget: Enum,
        //     writeTarget: Enum,
        //     readOffset: Intptr,
        //     writeOffset: Intptr,
        //     size: Sizeiptr,
        // ) callconv(.C) void = undefined;
        // pub var getUniformIndices: *const fn (
        //     program: Uint,
        //     uniformCount: Sizei,
        //     uniformNames: [*c]const [*c]const Char,
        //     uniformIndices: [*c]Uint,
        // ) callconv(.C) void = undefined;
        // pub var getActiveUniformsiv: *const fn (
        //     program: Uint,
        //     uniformCount: Sizei,
        //     uniformIndices: [*c]const Uint,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        // pub var getActiveUniformName: *const fn (
        //     program: Uint,
        //     uniformIndex: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     uniformName: [*c]Char,
        // ) callconv(.C) void = undefined;
        // pub var getUniformBlockIndex: *const fn (
        //     program: Uint,
        //     uniformBlockName: [*c]const Char,
        // ) callconv(.C) Uint = undefined;
        // pub var getActiveUniformBlockiv: *const fn (
        //     program: Uint,
        //     uniformBlockIndex: Uint,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        // pub var getActiveUniformBlockName: *const fn (
        //     program: Uint,
        //     uniformBlockIndex: Uint,
        //     bufSize: Sizei,
        //     length: [*c]Sizei,
        //     uniformBlockName: [*c]Char,
        // ) callconv(.C) void = undefined;
        // pub var uniformBlockBinding: *const fn (
        //     program: Uint,
        //     uniformBlockIndex: Uint,
        //     uniformBlockBinding: Uint,
        // ) callconv(.C) void = undefined;
        //------------------------------------------------------------------------------------------
        //
        // OpenGL 3.2 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const Sync = bindings.Sync;
        pub const Uint64 = bindings.Uint64;
        pub const Int64 = bindings.Int64;

        pub const CONTEXT_CORE_PROFILE_BIT = bindings.CONTEXT_CORE_PROFILE_BIT;
        pub const CONTEXT_COMPATIBILITY_PROFILE_BIT = bindings.CONTEXT_COMPATIBILITY_PROFILE_BIT;
        pub const LINES_ADJACENCY = bindings.LINES_ADJACENCY;
        pub const LINE_STRIP_ADJACENCY = bindings.LINE_STRIP_ADJACENCY;
        pub const TRIANGLES_ADJACENCY = bindings.TRIANGLES_ADJACENCY;
        pub const TRIANGLE_STRIP_ADJACENCY = bindings.TRIANGLE_STRIP_ADJACENCY;
        pub const PROGRAM_POINT_SIZE = bindings.PROGRAM_POINT_SIZE;
        pub const MAX_GEOMETRY_TEXTURE_IMAGE_UNITS = bindings.MAX_GEOMETRY_TEXTURE_IMAGE_UNITS;
        pub const FRAMEBUFFER_ATTACHMENT_LAYERED = bindings.FRAMEBUFFER_ATTACHMENT_LAYERED;
        pub const FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS = bindings.FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS;
        pub const GEOMETRY_SHADER = bindings.GEOMETRY_SHADER;
        pub const GEOMETRY_VERTICES_OUT = bindings.GEOMETRY_VERTICES_OUT;
        pub const GEOMETRY_INPUT_TYPE = bindings.GEOMETRY_INPUT_TYPE;
        pub const GEOMETRY_OUTPUT_TYPE = bindings.GEOMETRY_OUTPUT_TYPE;
        pub const MAX_GEOMETRY_UNIFORM_COMPONENTS = bindings.MAX_GEOMETRY_UNIFORM_COMPONENTS;
        pub const MAX_GEOMETRY_OUTPUT_VERTICES = bindings.MAX_GEOMETRY_OUTPUT_VERTICES;
        pub const MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS = bindings.MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS;
        pub const MAX_VERTEX_OUTPUT_COMPONENTS = bindings.MAX_VERTEX_OUTPUT_COMPONENTS;
        pub const MAX_GEOMETRY_INPUT_COMPONENTS = bindings.MAX_GEOMETRY_INPUT_COMPONENTS;
        pub const MAX_GEOMETRY_OUTPUT_COMPONENTS = bindings.MAX_GEOMETRY_OUTPUT_COMPONENTS;
        pub const MAX_FRAGMENT_INPUT_COMPONENTS = bindings.MAX_FRAGMENT_INPUT_COMPONENTS;
        pub const CONTEXT_PROFILE_MASK = bindings.CONTEXT_PROFILE_MASK;
        pub const DEPTH_CLAMP = bindings.DEPTH_CLAMP;
        pub const QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION = bindings.QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION;
        pub const FIRST_VERTEX_CONVENTION = bindings.FIRST_VERTEX_CONVENTION;
        pub const LAST_VERTEX_CONVENTION = bindings.LAST_VERTEX_CONVENTION;
        pub const PROVOKING_VERTEX = bindings.PROVOKING_VERTEX;
        pub const TEXTURE_CUBE_MAP_SEAMLESS = bindings.TEXTURE_CUBE_MAP_SEAMLESS;
        pub const MAX_SERVER_WAIT_TIMEOUT = bindings.MAX_SERVER_WAIT_TIMEOUT;
        pub const OBJECT_TYPE = bindings.OBJECT_TYPE;
        pub const SYNC_CONDITION = bindings.SYNC_CONDITION;
        pub const SYNC_STATUS = bindings.SYNC_STATUS;
        pub const SYNC_FLAGS = bindings.SYNC_FLAGS;
        pub const SYNC_FENCE = bindings.SYNC_FENCE;
        pub const SYNC_GPU_COMMANDS_COMPLETE = bindings.SYNC_GPU_COMMANDS_COMPLETE;
        pub const UNSIGNALED = bindings.UNSIGNALED;
        pub const SIGNALED = bindings.SIGNALED;
        pub const ALREADY_SIGNALED = bindings.ALREADY_SIGNALED;
        pub const TIMEOUT_EXPIRED = bindings.TIMEOUT_EXPIRED;
        pub const CONDITION_SATISFIED = bindings.CONDITION_SATISFIED;
        pub const WAIT_FAILED = bindings.WAIT_FAILED;
        pub const TIMEOUT_IGNORED = bindings.TIMEOUT_IGNORED;
        pub const SYNC_FLUSH_COMMANDS_BIT = bindings.SYNC_FLUSH_COMMANDS_BIT;
        pub const SAMPLE_POSITION = bindings.SAMPLE_POSITION;
        pub const SAMPLE_MASK = bindings.SAMPLE_MASK;
        pub const SAMPLE_MASK_VALUE = bindings.SAMPLE_MASK_VALUE;
        pub const MAX_SAMPLE_MASK_WORDS = bindings.MAX_SAMPLE_MASK_WORDS;
        pub const TEXTURE_2D_MULTISAMPLE = bindings.TEXTURE_2D_MULTISAMPLE;
        pub const PROXY_TEXTURE_2D_MULTISAMPLE = bindings.PROXY_TEXTURE_2D_MULTISAMPLE;
        pub const TEXTURE_2D_MULTISAMPLE_ARRAY = bindings.TEXTURE_2D_MULTISAMPLE_ARRAY;
        pub const PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY = bindings.PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY;
        pub const TEXTURE_BINDING_2D_MULTISAMPLE = bindings.TEXTURE_BINDING_2D_MULTISAMPLE;
        pub const TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY = bindings.TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY;
        pub const TEXTURE_SAMPLES = bindings.TEXTURE_SAMPLES;
        pub const TEXTURE_FIXED_SAMPLE_LOCATIONS = bindings.TEXTURE_FIXED_SAMPLE_LOCATIONS;
        pub const SAMPLER_2D_MULTISAMPLE = bindings.SAMPLER_2D_MULTISAMPLE;
        pub const INT_SAMPLER_2D_MULTISAMPLE = bindings.INT_SAMPLER_2D_MULTISAMPLE;
        pub const UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE = bindings.UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
        pub const SAMPLER_2D_MULTISAMPLE_ARRAY = bindings.SAMPLER_2D_MULTISAMPLE_ARRAY;
        pub const INT_SAMPLER_2D_MULTISAMPLE_ARRAY = bindings.INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
        pub const UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = bindings.UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
        pub const MAX_COLOR_TEXTURE_SAMPLES = bindings.MAX_COLOR_TEXTURE_SAMPLES;
        pub const MAX_DEPTH_TEXTURE_SAMPLES = bindings.MAX_DEPTH_TEXTURE_SAMPLES;
        pub const MAX_INTEGER_SAMPLES = bindings.MAX_INTEGER_SAMPLES;

        // pub var drawElementsBaseVertex: *const fn (
        //     mode: Enum,
        //     count: Sizei,
        //     type: Enum,
        //     indices: ?*const anyopaque,
        //     basevertex: Int,
        // ) callconv(.C) void = undefined;
        // pub var drawRangeElementsBaseVertex: *const fn (
        //     mode: Enum,
        //     start: Uint,
        //     end: Uint,
        //     count: Sizei,
        //     type: Enum,
        //     indices: ?*const anyopaque,
        //     basevertex: Int,
        // ) callconv(.C) void = undefined;
        // pub var drawElementsInstancedBaseVertex: *const fn (
        //     mode: Enum,
        //     count: Sizei,
        //     type: Enum,
        //     indices: ?*const anyopaque,
        //     instancecount: Sizei,
        //     basevertex: Int,
        // ) callconv(.C) void = undefined;
        // pub var multiDrawElementsBaseVertex: *const fn (
        //     mode: Enum,
        //     count: [*c]const Sizei,
        //     type: Enum,
        //     indices: [*c]const ?*const anyopaque,
        //     drawcount: Sizei,
        //     basevertex: [*c]const Int,
        // ) callconv(.C) void = undefined;
        // pub var provokingVertex: *const fn (mode: Enum) callconv(.C) void = undefined;
        // pub var fenceSync: *const fn (condition: Enum, flags: Bitfield) callconv(.C) Sync = undefined;
        // pub var isSync: *const fn (sync: Sync) callconv(.C) Boolean = undefined;
        // pub var deleteSync: *const fn (sync: Sync) callconv(.C) void = undefined;
        // pub var clientWaitSync: *const fn (sync: Sync, flags: Bitfield, timeout: Uint64) callconv(.C) Enum = undefined;
        // pub var waitSync: *const fn (sync: Sync, flags: Bitfield, timeout: Uint64) callconv(.C) void = undefined;

        // pub var getInteger64v: *const fn (pname: Enum, data: [*c]Int64) callconv(.C) void = undefined;
        pub fn getInteger64v(pname: meta.mergeEnums(.{
            ParamName,
            enum(Enum) {
                ATOMIC_COUNTER_BUFFER_START,
                ATOMIC_COUNTER_BUFFER_SIZE,
            },
        }), ptr: [*]Int64) void {
            bindings.getInteger64v(@intFromEnum(pname), ptr);
        }

        // pub var getSynciv: *const fn (
        //     sync: Sync,
        //     pname: Enum,
        //     count: Sizei,
        //     length: [*c]Sizei,
        //     values: [*c]Int,
        // ) callconv(.C) void = undefined;
        // pub var getInteger64i_v: *const fn (target: Enum, index: Uint, data: [*c]Int64) callconv(.C) void = undefined;
        // pub var getBufferParameteri64v: *const fn (
        //     target: Enum,
        //     pname: Enum,
        //     params: [*c]Int64,
        // ) callconv(.C) void = undefined;
        // pub var framebufferTexture: *const fn (
        //     target: Enum,
        //     attachment: Enum,
        //     texture: Uint,
        //     level: Int,
        // ) callconv(.C) void = undefined;
        // pub var texImage2DMultisample: *const fn (
        //     target: Enum,
        //     samples: Sizei,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     fixedsamplelocations: Boolean,
        // ) callconv(.C) void = undefined;
        // pub var texImage3DMultisample: *const fn (
        //     target: Enum,
        //     samples: Sizei,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     depth: Sizei,
        //     fixedsamplelocations: Boolean,
        // ) callconv(.C) void = undefined;
        // pub var getMultisamplefv: *const fn (pname: Enum, index: Uint, val: [*c]Float) callconv(.C) void = undefined;
        // pub var sampleMaski: *const fn (maskNumber: Uint, mask: Bitfield) callconv(.C) void = undefined;

        //------------------------------------------------------------------------------------------
        //
        // OpenGL 3.3 (Core Profile)
        //
        //------------------------------------------------------------------------------------------
        pub const VERTEX_ATTRIB_ARRAY_DIVISOR = bindings.VERTEX_ATTRIB_ARRAY_DIVISOR;
        pub const SRC1_COLOR = bindings.SRC1_COLOR;
        pub const ONE_MINUS_SRC1_COLOR = bindings.ONE_MINUS_SRC1_COLOR;
        pub const ONE_MINUS_SRC1_ALPHA = bindings.ONE_MINUS_SRC1_ALPHA;
        pub const MAX_DUAL_SOURCE_DRAW_BUFFERS = bindings.MAX_DUAL_SOURCE_DRAW_BUFFERS;
        pub const ANY_SAMPLES_PASSED = bindings.ANY_SAMPLES_PASSED;
        pub const SAMPLER_BINDING = bindings.SAMPLER_BINDING;
        pub const RGB10_A2UI = bindings.RGB10_A2UI;
        pub const TEXTURE_SWIZZLE_R = bindings.TEXTURE_SWIZZLE_R;
        pub const TEXTURE_SWIZZLE_G = bindings.TEXTURE_SWIZZLE_G;
        pub const TEXTURE_SWIZZLE_B = bindings.TEXTURE_SWIZZLE_B;
        pub const TEXTURE_SWIZZLE_A = bindings.TEXTURE_SWIZZLE_A;
        pub const TEXTURE_SWIZZLE_RGBA = bindings.TEXTURE_SWIZZLE_RGBA;
        pub const TIME_ELAPSED = bindings.TIME_ELAPSED;
        pub const TIMESTAMP = bindings.TIMESTAMP;
        pub const INT_2_10_10_10_REV = bindings.INT_2_10_10_10_REV;

        // pub var bindFragDataLocationIndexed: *const fn (
        //     program: Uint,
        //     colorNumber: Uint,
        //     index: Uint,
        //     name: [*:0]const Char,
        // ) callconv(.C) void = undefined;
        // pub var getFragDataIndex: *const fn (program: Uint, name: [*c]const Char) callconv(.C) Int = undefined;
        // pub var genSamplers: *const fn (count: Sizei, samplers: [*c]Uint) callconv(.C) void = undefined;
        // pub var deleteSamplers: *const fn (count: Sizei, samplers: [*c]const Uint) callconv(.C) void = undefined;
        // pub var isSampler: *const fn (sampler: Uint) callconv(.C) Boolean = undefined;
        // pub var bindSampler: *const fn (unit: Uint, sampler: Uint) callconv(.C) void = undefined;
        // pub var samplerParameteri: *const fn (sampler: Uint, pname: Enum, param: Int) callconv(.C) void = undefined;
        // pub var samplerParameteriv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     param: [*c]const Int,
        // ) callconv(.C) void = undefined;
        // pub var samplerParameterf: *const fn (sampler: Uint, pname: Enum, param: Float) callconv(.C) void = undefined;
        // pub var samplerParameterfv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     param: [*c]const Float,
        // ) callconv(.C) void = undefined;
        // pub var samplerParameterIiv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     param: [*c]const Int,
        // ) callconv(.C) void = undefined;
        // pub var samplerParameterIuiv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     param: [*c]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var getSamplerParameteriv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        // pub var getSamplerParameterIiv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        // pub var getSamplerParameterfv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     params: [*c]Float,
        // ) callconv(.C) void = undefined;
        // pub var getSamplerParameterIuiv: *const fn (
        //     sampler: Uint,
        //     pname: Enum,
        //     params: [*c]Uint,
        // ) callconv(.C) void = undefined;
        // pub var queryCounter: *const fn (id: Uint, target: Enum) callconv(.C) void = undefined;
        // pub var getQueryObjecti64v: *const fn (id: Uint, pname: Enum, params: [*c]Int64) callconv(.C) void = undefined;
        // pub var getQueryObjectui64v: *const fn (id: Uint, pname: Enum, params: [*c]Uint64) callconv(.C) void = undefined;
        // pub var vertexAttribDivisor: *const fn (index: Uint, divisor: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP1ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP1uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP2ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP2uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP3ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP3uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP4ui: *const fn (index: Uint, type: Enum, normalized: Boolean, value: Uint) callconv(.C) void = undefined;
        // pub var vertexAttribP4uiv: *const fn (index: Uint, type: Enum, normalized: Boolean, value: *const Uint) callconv(.C) void = undefined;

        // TODO: where do these belong?
        // pub var vertexP2ui: *const fn (type: Enum, value: Uint) callconv(.C) void = undefined;
        // pub var vertexP2uiv: *const fn (type: Enum, value: *const Uint) callconv(.C) void = undefined;
        // pub var vertexP3ui: *const fn (type: Enum, value: Uint) callconv(.C) void = undefined;
        // pub var vertexP3uiv: *const fn (type: Enum, value: *const Uint) callconv(.C) void = undefined;
        // pub var vertexP4ui: *const fn (type: Enum, value: Uint) callconv(.C) void = undefined;
        // pub var vertexP4uiv: *const fn (type: Enum, value: *const Uint) callconv(.C) void = undefined;
        // pub var texCoordP1ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var texCoordP1uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var texCoordP2ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var texCoordP2uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var texCoordP3ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var texCoordP3uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var texCoordP4ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var texCoordP4uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP1ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP1uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP2ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP2uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP3ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP3uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP4ui: *const fn (texture: Enum, type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var multiTexCoordP4uiv: *const fn (texture: Enum, type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var normalP3ui: *const fn (type: Enum, coords: Uint) callconv(.C) void = undefined;
        // pub var normalP3uiv: *const fn (type: Enum, coords: *const Uint) callconv(.C) void = undefined;
        // pub var colorP3ui: *const fn (type: Enum, color: Uint) callconv(.C) void = undefined;
        // pub var colorP3uiv: *const fn (type: Enum, color: *const Uint) callconv(.C) void = undefined;
        // pub var colorP4ui: *const fn (type: Enum, color: Uint) callconv(.C) void = undefined;
        // pub var colorP4uiv: *const fn (type: Enum, color: *const Uint) callconv(.C) void = undefined;
        // pub var secondaryColorP3ui: *const fn (type: Enum, color: Uint) callconv(.C) void = undefined;
        // pub var secondaryColorP3uiv: *const fn (type: Enum, color: *const Uint) callconv(.C) void = undefined;

        //--------------------------------------------------------------------------------------------------
        //
        // OpenGL 4.0 (Core Profile)
        //
        //--------------------------------------------------------------------------------------------------
        pub const SAMPLE_SHADING = bindings.SAMPLE_SHADING;
        pub const MIN_SAMPLE_SHADING_VALUE = bindings.MIN_SAMPLE_SHADING_VALUE;
        pub const MIN_PROGRAM_TEXTURE_GATHER_OFFSET = bindings.MIN_PROGRAM_TEXTURE_GATHER_OFFSET;
        pub const MAX_PROGRAM_TEXTURE_GATHER_OFFSET = bindings.MAX_PROGRAM_TEXTURE_GATHER_OFFSET;
        pub const TEXTURE_CUBE_MAP_ARRAY = bindings.TEXTURE_CUBE_MAP_ARRAY;
        pub const TEXTURE_BINDING_CUBE_MAP_ARRAY = bindings.TEXTURE_BINDING_CUBE_MAP_ARRAY;
        pub const PROXY_TEXTURE_CUBE_MAP_ARRAY = bindings.PROXY_TEXTURE_CUBE_MAP_ARRAY;
        pub const SAMPLER_CUBE_MAP_ARRAY = bindings.SAMPLER_CUBE_MAP_ARRAY;
        pub const SAMPLER_CUBE_MAP_ARRAY_SHADOW = bindings.SAMPLER_CUBE_MAP_ARRAY_SHADOW;
        pub const INT_SAMPLER_CUBE_MAP_ARRAY = bindings.INT_SAMPLER_CUBE_MAP_ARRAY;
        pub const UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY = bindings.UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY;
        pub const DRAW_INDIRECT_BUFFER = bindings.DRAW_INDIRECT_BUFFER;
        pub const DRAW_INDIRECT_BUFFER_BINDING = bindings.DRAW_INDIRECT_BUFFER_BINDING;
        pub const GEOMETRY_SHADER_INVOCATIONS = bindings.GEOMETRY_SHADER_INVOCATIONS;
        pub const MAX_GEOMETRY_SHADER_INVOCATIONS = bindings.MAX_GEOMETRY_SHADER_INVOCATIONS;
        pub const MIN_FRAGMENT_INTERPOLATION_OFFSET = bindings.MIN_FRAGMENT_INTERPOLATION_OFFSET;
        pub const MAX_FRAGMENT_INTERPOLATION_OFFSET = bindings.MAX_FRAGMENT_INTERPOLATION_OFFSET;
        pub const FRAGMENT_INTERPOLATION_OFFSET_BITS = bindings.FRAGMENT_INTERPOLATION_OFFSET_BITS;
        pub const DOUBLE_VEC2 = bindings.DOUBLE_VEC2;
        pub const DOUBLE_VEC3 = bindings.DOUBLE_VEC3;
        pub const DOUBLE_VEC4 = bindings.DOUBLE_VEC4;
        pub const DOUBLE_MAT2 = bindings.DOUBLE_MAT2;
        pub const DOUBLE_MAT3 = bindings.DOUBLE_MAT3;
        pub const DOUBLE_MAT4 = bindings.DOUBLE_MAT4;
        pub const DOUBLE_MAT2x3 = bindings.DOUBLE_MAT2x3;
        pub const DOUBLE_MAT2x4 = bindings.DOUBLE_MAT2x4;
        pub const DOUBLE_MAT3x2 = bindings.DOUBLE_MAT3x2;
        pub const DOUBLE_MAT3x4 = bindings.DOUBLE_MAT3x4;
        pub const DOUBLE_MAT4x2 = bindings.DOUBLE_MAT4x2;
        pub const DOUBLE_MAT4x3 = bindings.DOUBLE_MAT4x3;
        pub const ACTIVE_SUBROUTINES = bindings.ACTIVE_SUBROUTINES;
        pub const ACTIVE_SUBROUTINE_UNIFORMS = bindings.ACTIVE_SUBROUTINE_UNIFORMS;
        pub const ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS = bindings.ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS;
        pub const ACTIVE_SUBROUTINE_MAX_LENGTH = bindings.ACTIVE_SUBROUTINE_MAX_LENGTH;
        pub const ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH = bindings.ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH;
        pub const MAX_SUBROUTINES = bindings.MAX_SUBROUTINES;
        pub const MAX_SUBROUTINE_UNIFORM_LOCATIONS = bindings.MAX_SUBROUTINE_UNIFORM_LOCATIONS;
        pub const NUM_COMPATIBLE_SUBROUTINES = bindings.NUM_COMPATIBLE_SUBROUTINES;
        pub const COMPATIBLE_SUBROUTINES = bindings.COMPATIBLE_SUBROUTINES;
        pub const PATCHES = bindings.PATCHES;
        pub const PATCH_VERTICES = bindings.PATCH_VERTICES;
        pub const PATCH_DEFAULT_INNER_LEVEL = bindings.PATCH_DEFAULT_INNER_LEVEL;
        pub const PATCH_DEFAULT_OUTER_LEVEL = bindings.PATCH_DEFAULT_OUTER_LEVEL;
        pub const TESS_CONTROL_OUTPUT_VERTICES = bindings.TESS_CONTROL_OUTPUT_VERTICES;
        pub const TESS_GEN_MODE = bindings.TESS_GEN_MODE;
        pub const TESS_GEN_SPACING = bindings.TESS_GEN_SPACING;
        pub const TESS_GEN_VERTEX_ORDER = bindings.TESS_GEN_VERTEX_ORDER;
        pub const TESS_GEN_POINT_MODE = bindings.TESS_GEN_POINT_MODE;
        pub const ISOLINES = bindings.ISOLINES;
        pub const FRACTIONAL_ODD = bindings.FRACTIONAL_ODD;
        pub const FRACTIONAL_EVEN = bindings.FRACTIONAL_EVEN;
        pub const MAX_PATCH_VERTICES = bindings.MAX_PATCH_VERTICES;
        pub const MAX_TESS_GEN_LEVEL = bindings.MAX_TESS_GEN_LEVEL;
        pub const MAX_TESS_CONTROL_UNIFORM_COMPONENTS = bindings.MAX_TESS_CONTROL_UNIFORM_COMPONENTS;
        pub const MAX_TESS_EVALUATION_UNIFORM_COMPONENTS = bindings.MAX_TESS_EVALUATION_UNIFORM_COMPONENTS;
        pub const MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS = bindings.MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS;
        pub const MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS = bindings.MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS;
        pub const MAX_TESS_CONTROL_OUTPUT_COMPONENTS = bindings.MAX_TESS_CONTROL_OUTPUT_COMPONENTS;
        pub const MAX_TESS_PATCH_COMPONENTS = bindings.MAX_TESS_PATCH_COMPONENTS;
        pub const MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS = bindings.MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS;
        pub const MAX_TESS_EVALUATION_OUTPUT_COMPONENTS = bindings.MAX_TESS_EVALUATION_OUTPUT_COMPONENTS;
        pub const MAX_TESS_CONTROL_UNIFORM_BLOCKS = bindings.MAX_TESS_CONTROL_UNIFORM_BLOCKS;
        pub const MAX_TESS_EVALUATION_UNIFORM_BLOCKS = bindings.MAX_TESS_EVALUATION_UNIFORM_BLOCKS;
        pub const MAX_TESS_CONTROL_INPUT_COMPONENTS = bindings.MAX_TESS_CONTROL_INPUT_COMPONENTS;
        pub const MAX_TESS_EVALUATION_INPUT_COMPONENTS = bindings.MAX_TESS_EVALUATION_INPUT_COMPONENTS;
        pub const MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS = bindings.MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS;
        pub const MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS = bindings.MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS;
        pub const UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER = bindings.UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER;
        pub const UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER = bindings.UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER;
        pub const TESS_EVALUATION_SHADER = bindings.TESS_EVALUATION_SHADER;
        pub const TESS_CONTROL_SHADER = bindings.TESS_CONTROL_SHADER;
        pub const TRANSFORM_FEEDBACK = bindings.TRANSFORM_FEEDBACK;
        pub const TRANSFORM_FEEDBACK_BUFFER_PAUSED = bindings.TRANSFORM_FEEDBACK_BUFFER_PAUSED;
        pub const TRANSFORM_FEEDBACK_BUFFER_ACTIVE = bindings.TRANSFORM_FEEDBACK_BUFFER_ACTIVE;
        pub const TRANSFORM_FEEDBACK_BINDING = bindings.TRANSFORM_FEEDBACK_BINDING;
        pub const MAX_TRANSFORM_FEEDBACK_BUFFERS = bindings.MAX_TRANSFORM_FEEDBACK_BUFFERS;
        pub const MAX_VERTEX_STREAMS = bindings.MAX_VERTEX_STREAMS;

        pub const DrawArraysIndirectCommand = bindings.DrawArraysIndirectCommand;
        pub const DrawElementsIndirectCommand = bindings.DrawElementsIndirectCommand;

        // pub var minSampleShading: *const fn (value: Float) callconv(.C) void = undefined;
        // pub var blendEquationi: *const fn (buf: Uint, mode: Enum) callconv(.C) void = undefined;
        // pub var blendEquationSeparatei: *const fn (buf: Uint, modeRGB: Enum, modeAlpha: Enum) callconv(.C) void = undefined;
        // pub var blendFunci: *const fn (buf: Uint, src: Enum, dst: Enum) callconv(.C) void = undefined;
        // pub var blendFuncSeparatei: *const fn (buf: Uint, srcRGB: Enum, dstRGB: Enum, srcAlpha: Enum, dstAlpha: Enum) callconv(.C) void = undefined;
        // pub var drawArraysIndirect: *const fn (mode: Enum, indirect: *const DrawArraysIndirectCommand) callconv(.C) void = undefined;
        // pub var drawElementsIndirect: *const fn (mode: Enum, type: Enum, indirect: *const DrawElementsIndirectCommand) callconv(.C) void = undefined;
        // pub var uniform1d: *const fn (location: Int, x: Double) callconv(.C) void = undefined;
        // pub var uniform2d: *const fn (location: Int, x: Double, y: Double) callconv(.C) void = undefined;
        // pub var uniform3d: *const fn (location: Int, x: Double, y: Double, z: Double) callconv(.C) void = undefined;
        // pub var uniform4d: *const fn (location: Int, x: Double, y: Double, z: Double, w: Double) callconv(.C) void = undefined;
        // pub var uniform1dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniform2dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniform3dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniform4dv: *const fn (location: Int, count: Sizei, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix2dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix3dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix4dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix2x3dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix2x4dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix3x2dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix3x4dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix4x2dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var uniformMatrix4x3dv: *const fn (location: Int, count: Sizei, transpose: Boolean, value: [*c]const Double) callconv(.C) void = undefined;
        // pub var getUniformdv: *const fn (program: Uint, location: Int, params: [*c]Double) callconv(.C) void = undefined;
        // pub var getSubroutineUniformLocation: *const fn (program: Uint, shadertype: Enum, name: [*c]const Char) callconv(.C) Int = undefined;
        // pub var getSubroutineIndex: *const fn (program: Uint, shadertype: Enum, name: [*c]const Char) callconv(.C) Uint = undefined;
        // pub var getActiveSubroutineUniformiv: *const fn (program: Uint, shadertype: Enum, index: Uint, pname: Enum, values: [*c]Int) callconv(.C) void = undefined;
        // pub var getActiveSubroutineUniformName: *const fn (program: Uint, shadertype: Enum, index: Uint, bufsize: Sizei, length: [*c]Sizei, name: [*c]Char) callconv(.C) void = undefined;
        // pub var getActiveSubroutineName: *const fn (program: Uint, shadertype: Enum, index: Uint, bufsize: Sizei, length: [*c]Sizei, name: [*c]Char) callconv(.C) void = undefined;
        // pub var uniformSubroutinesuiv: *const fn (shadertype: Enum, count: Sizei, indices: [*c]const Uint) callconv(.C) void = undefined;
        // pub var getUniformSubroutineuiv: *const fn (shadertype: Enum, location: Int, params: [*c]const Uint) callconv(.C) void = undefined;
        // pub var getProgramStageiv: *const fn (program: Uint, shadertype: Enum, pname: Enum, values: [*c]Int) callconv(.C) void = undefined;
        // pub var patchParameteri: *const fn (pname: Enum, value: Int) callconv(.C) void = undefined;
        // pub var patchParameterfv: *const fn (pname: Enum, values: [*c]const Float) callconv(.C) void = undefined;
        // pub var bindTransformFeedback: *const fn (target: Enum, id: Uint) callconv(.C) Boolean = undefined;
        // pub var deleteTransformFeedbacks: *const fn (n: Sizei, ids: [*c]const Uint) callconv(.C) void = undefined;
        // pub var genTransformFeedbacks: *const fn (n: Sizei, ids: [*c]Uint) callconv(.C) void = undefined;
        // pub var isTransformFeedback: *const fn (id: Uint) callconv(.C) void = undefined;
        // pub var pauseTransformFeedback: *const fn () callconv(.C) void = undefined;
        // pub var resumeTransformFeedback: *const fn () callconv(.C) void = undefined;
        // pub var drawTransformFeedback: *const fn (mode: Enum, id: Uint) callconv(.C) void = undefined;
        // pub var drawTransformFeedbackStream: *const fn (mode: Enum, id: Uint, stream: Uint) callconv(.C) void = undefined;
        // pub var beginQueryIndexed: *const fn (target: Enum, index: Uint, id: Uint) callconv(.C) void = undefined;
        // pub var endQueryIndexed: *const fn (target: Enum, index: Uint) callconv(.C) void = undefined;
        // pub var glGetQueryIndexediv: *const fn (target: Enum, index: Uint, pname: Enum, params: [*c]Int) callconv(.C) void = undefined;

        //--------------------------------------------------------------------------------------------------
        //
        // OpenGL 4.1 (Core Profile)
        //
        //--------------------------------------------------------------------------------------------------
        pub const FIXED = bindings.FIXED;
        pub const IMPLEMENTATION_COLOR_READ_TYPE = bindings.IMPLEMENTATION_COLOR_READ_TYPE;
        pub const IMPLEMENTATION_COLOR_READ_FORMAT = bindings.IMPLEMENTATION_COLOR_READ_FORMAT;
        pub const LOW_FLOAT = bindings.LOW_FLOAT;
        pub const MEDIUM_FLOAT = bindings.MEDIUM_FLOAT;
        pub const HIGH_FLOAT = bindings.HIGH_FLOAT;
        pub const LOW_INT = bindings.LOW_INT;
        pub const MEDIUM_INT = bindings.MEDIUM_INT;
        pub const HIGH_INT = bindings.HIGH_INT;
        pub const SHADER_COMPILER = bindings.SHADER_COMPILER;
        pub const SHADER_BINARY_FORMATS = bindings.SHADER_BINARY_FORMATS;
        pub const NUM_SHADER_BINARY_FORMATS = bindings.NUM_SHADER_BINARY_FORMATS;
        pub const MAX_VERTEX_UNIFORM_VECTORS = bindings.MAX_VERTEX_UNIFORM_VECTORS;
        pub const MAX_VARYING_VECTORS = bindings.MAX_VARYING_VECTORS;
        pub const MAX_FRAGMENT_UNIFORM_VECTORS = bindings.MAX_FRAGMENT_UNIFORM_VECTORS;
        pub const RGB565 = bindings.RGB565;
        pub const PROGRAM_BINARY_RETRIEVABLE_HINT = bindings.PROGRAM_BINARY_RETRIEVABLE_HINT;
        pub const PROGRAM_BINARY_LENGTH = bindings.PROGRAM_BINARY_LENGTH;
        pub const NUM_PROGRAM_BINARY_FORMATS = bindings.NUM_PROGRAM_BINARY_FORMATS;
        pub const PROGRAM_BINARY_FORMATS = bindings.PROGRAM_BINARY_FORMATS;
        pub const VERTEX_SHADER_BIT = bindings.VERTEX_SHADER_BIT;
        pub const FRAGMENT_SHADER_BIT = bindings.FRAGMENT_SHADER_BIT;
        pub const GEOMETRY_SHADER_BIT = bindings.GEOMETRY_SHADER_BIT;
        pub const TESS_CONTROL_SHADER_BIT = bindings.TESS_CONTROL_SHADER_BIT;
        pub const TESS_EVALUATION_SHADER_BIT = bindings.TESS_EVALUATION_SHADER_BIT;
        pub const ALL_SHADER_BITS = bindings.ALL_SHADER_BITS;
        pub const PROGRAM_SEPARABLE = bindings.PROGRAM_SEPARABLE;
        pub const ACTIVE_PROGRAM = bindings.ACTIVE_PROGRAM;
        pub const PROGRAM_PIPELINE_BINDING = bindings.PROGRAM_PIPELINE_BINDING;
        pub const MAX_VIEWPORTS = bindings.MAX_VIEWPORTS;
        pub const VIEWPORT_SUBPIXEL_BITS = bindings.VIEWPORT_SUBPIXEL_BITS;
        pub const VIEWPORT_BOUNDS_RANGE = bindings.VIEWPORT_BOUNDS_RANGE;
        pub const LAYER_PROVOKING_VERTEX = bindings.LAYER_PROVOKING_VERTEX;
        pub const VIEWPORT_INDEX_PROVOKING_VERTEX = bindings.VIEWPORT_INDEX_PROVOKING_VERTEX;
        pub const UNDEFINED_VERTEX = bindings.UNDEFINED_VERTEX;

        // pub var releaseShaderCompiler: *const fn () callconv(.C) void = undefined;
        // pub var shaderBinary: *const fn (
        //     count: Sizei,
        //     shaders: [*]const Uint,
        //     binary_format: Enum,
        //     binary: *const anyopaque,
        //     length: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var getShaderPrecisionFormat: *const fn (
        //     shader_type: Enum,
        //     precisionType: Enum,
        //     range: *Int,
        //     precision: *Int,
        // ) callconv(.C) void = undefined;
        // // depthRangef first defined by OpenGL ES 1.0
        // // clearDepthf first defined by OpenGL ES 1.0
        // pub var getProgramBinary: *const fn (
        //     program: Uint,
        //     buf_size: Sizei,
        //     length: *Sizei,
        //     binary_format: *Enum,
        //     binary: *anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var programBinary: *const fn (
        //     program: Uint,
        //     binary_format: Enum,
        //     binary: *const anyopaque,
        //     length: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var programParameteri: *const fn (
        //     program: Uint,
        //     pname: Enum,
        //     value: Int,
        // ) callconv(.C) void = undefined;
        // pub var useProgramStages: *const fn (
        //     pipeline: Uint,
        //     stages: Bitfield,
        //     program: Uint,
        // ) callconv(.C) void = undefined;
        // pub var activeShaderProgram: *const fn (
        //     pipeline: Uint,
        //     program: Uint,
        // ) callconv(.C) void = undefined;
        // pub var createShaderProgramv: *const fn (
        //     type: Enum,
        //     count: Sizei,
        //     strings: [*][*:0]const u8,
        // ) callconv(.C) Uint = undefined;
        // pub var bindProgramPipeline: *const fn (pipeline: Uint) callconv(.C) void = undefined;
        // pub var deleteProgramPipelines: *const fn (
        //     n: Sizei,
        //     pipelines: [*]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var genProgramPipelines: *const fn (n: Sizei, pipelines: [*]Uint) callconv(.C) void = undefined;
        // pub var isProgramPipeline: *const fn (pipeline: Uint) callconv(.C) Boolean = undefined;
        // pub var getProgramPipelineiv: *const fn (
        //     pipeline: Uint,
        //     pname: Enum,
        //     params: [*]Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1i: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2i: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Int,
        //     y: Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3i: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Int,
        //     y: Int,
        //     z: Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4i: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Int,
        //     y: Int,
        //     z: Int,
        //     w: Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1ui: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2ui: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Uint,
        //     y: Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3ui: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Uint,
        //     y: Uint,
        //     z: Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4ui: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Uint,
        //     y: Uint,
        //     z: Uint,
        //     w: Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1f: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2f: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Float,
        //     y: Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3f: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Float,
        //     y: Float,
        //     z: Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4f: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Float,
        //     y: Float,
        //     z: Float,
        //     w: Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1d: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2d: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Double,
        //     y: Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3d: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Double,
        //     y: Double,
        //     z: Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4d: *const fn (
        //     program: Uint,
        //     location: Int,
        //     x: Double,
        //     y: Double,
        //     z: Double,
        //     w: Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1iv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2iv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3iv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4iv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Int,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1uiv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2uiv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3uiv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4uiv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Uint,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniform1dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform2dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform3dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniform4dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix2fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix3fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix4fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix2dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix3dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix4dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix2x3fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix3x2fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix2x4fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix4x2fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix3x4fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix4x3fv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix2x3dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix3x2dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix2x4dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix4x2dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix3x4dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var programUniformMatrix4x3dv: *const fn (
        //     program: Uint,
        //     location: Int,
        //     count: Sizei,
        //     transpose: Boolean,
        //     value: [*]const Double,
        // ) callconv(.C) void = undefined;
        // pub var validateProgramPipeline: *const fn (pipeline: Uint) callconv(.C) void = undefined;
        // pub var getProgramPipelineInfoLog: *const fn (
        //     pipeline: Uint,
        //     bufSize: Sizei,
        //     length: *Sizei,
        //     infoLog: [*]u8,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttribL1d: *const fn (index: Uint, x: Double) callconv(.C) void = undefined;
        // pub var vertexAttribL2d: *const fn (index: Uint, x: Double, y: Double) callconv(.C) void = undefined;
        // pub var vertexAttribL3d: *const fn (
        //     index: Uint,
        //     x: Double,
        //     y: Double,
        //     z: Double,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttribL4d: *const fn (
        //     index: Uint,
        //     x: Double,
        //     y: Double,
        //     z: Double,
        //     w: Double,
        // ) callconv(.C) void = undefined;
        // pub var vertexAttribL1dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
        // pub var vertexAttribL2dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
        // pub var vertexAttribL3dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
        // pub var vertexAttribL4dv: *const fn (index: Uint, v: [*]const Double) callconv(.C) void = undefined;
        // pub var viewportArrayv: *const fn (
        //     first: Uint,
        //     count: Sizei,
        //     v: [*]const Float,
        // ) callconv(.C) void = undefined;
        // pub var viewportIndexedf: *const fn (
        //     index: Uint,
        //     x: Float,
        //     y: Float,
        //     w: Float,
        //     h: Float,
        // ) callconv(.C) void = undefined;
        // pub var viewportIndexedfv: *const fn (index: Uint, v: [*]const Float) callconv(.C) void = undefined;
        // pub var scissorArrayv: *const fn (
        //     first: Uint,
        //     count: Sizei,
        //     v: [*]const Int,
        // ) callconv(.C) void = undefined;
        // pub var scissorIndexed: *const fn (
        //     index: Uint,
        //     left: Int,
        //     bottom: Int,
        //     width: Sizei,
        //     height: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var scissorIndexedv: *const fn (index: Uint, v: [*]const Int) callconv(.C) void = undefined;
        // pub var depthRangeArrayv: *const fn (
        //     first: Uint,
        //     count: Sizei,
        //     v: [*]const Clampd,
        // ) callconv(.C) void = undefined;
        // pub var depthRangeIndexed: *const fn (
        //     index: Uint,
        //     n: Clampd,
        //     f: Clampd,
        // ) callconv(.C) void = undefined;
        // pub var getFloati_v: *const fn (
        //     target: Enum,
        //     index: Uint,
        //     data: [*]Float,
        // ) callconv(.C) void = undefined;
        // pub var getDoublei_v: *const fn (
        //     target: Enum,
        //     index: Uint,
        //     data: [*]Double,
        // ) callconv(.C) void = undefined;

        //--------------------------------------------------------------------------------------------------
        //
        // OpenGL 4.2 (Core Profile)
        //
        //--------------------------------------------------------------------------------------------------
        pub const COPY_READ_BUFFER_BINDING = bindings.COPY_READ_BUFFER_BINDING;
        pub const COPY_WRITE_BUFFER_BINDING = bindings.COPY_WRITE_BUFFER_BINDING;
        pub const TRANSFORM_FEEDBACK_ACTIVE = bindings.TRANSFORM_FEEDBACK_ACTIVE;
        pub const TRANSFORM_FEEDBACK_PAUSED = bindings.TRANSFORM_FEEDBACK_PAUSED;
        pub const UNPACK_COMPRESSED_BLOCK_WIDTH = bindings.UNPACK_COMPRESSED_BLOCK_WIDTH;
        pub const UNPACK_COMPRESSED_BLOCK_HEIGHT = bindings.UNPACK_COMPRESSED_BLOCK_HEIGHT;
        pub const UNPACK_COMPRESSED_BLOCK_DEPTH = bindings.UNPACK_COMPRESSED_BLOCK_DEPTH;
        pub const UNPACK_COMPRESSED_BLOCK_SIZE = bindings.UNPACK_COMPRESSED_BLOCK_SIZE;
        pub const PACK_COMPRESSED_BLOCK_WIDTH = bindings.PACK_COMPRESSED_BLOCK_WIDTH;
        pub const PACK_COMPRESSED_BLOCK_HEIGHT = bindings.PACK_COMPRESSED_BLOCK_HEIGHT;
        pub const PACK_COMPRESSED_BLOCK_DEPTH = bindings.PACK_COMPRESSED_BLOCK_DEPTH;
        pub const PACK_COMPRESSED_BLOCK_SIZE = bindings.PACK_COMPRESSED_BLOCK_SIZE;
        pub const NUM_SAMPLE_COUNTS = bindings.NUM_SAMPLE_COUNTS;
        pub const MIN_MAP_BUFFER_ALIGNMENT = bindings.MIN_MAP_BUFFER_ALIGNMENT;
        pub const ATOMIC_COUNTER_BUFFER = bindings.ATOMIC_COUNTER_BUFFER;
        pub const ATOMIC_COUNTER_BUFFER_BINDING = bindings.ATOMIC_COUNTER_BUFFER_BINDING;
        pub const ATOMIC_COUNTER_BUFFER_START = bindings.ATOMIC_COUNTER_BUFFER_START;
        pub const ATOMIC_COUNTER_BUFFER_SIZE = bindings.ATOMIC_COUNTER_BUFFER_SIZE;
        pub const ATOMIC_COUNTER_BUFFER_DATA_SIZE = bindings.ATOMIC_COUNTER_BUFFER_DATA_SIZE;
        pub const ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS = bindings.ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS;
        pub const ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES = bindings.ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES;
        pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER = bindings.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER;
        pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER = bindings.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER;
        pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER = bindings.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER;
        pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER = bindings.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER;
        pub const ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER = bindings.ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER;
        pub const MAX_VERTEX_ATOMIC_COUNTER_BUFFERS = bindings.MAX_VERTEX_ATOMIC_COUNTER_BUFFERS;
        pub const MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS = bindings.MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS;
        pub const MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS = bindings.MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS;
        pub const MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS = bindings.MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS;
        pub const MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS = bindings.MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS;
        pub const MAX_COMBINED_ATOMIC_COUNTER_BUFFERS = bindings.MAX_COMBINED_ATOMIC_COUNTER_BUFFERS;
        pub const MAX_VERTEX_ATOMIC_COUNTERS = bindings.MAX_VERTEX_ATOMIC_COUNTERS;
        pub const MAX_TESS_CONTROL_ATOMIC_COUNTERS = bindings.MAX_TESS_CONTROL_ATOMIC_COUNTERS;
        pub const MAX_TESS_EVALUATION_ATOMIC_COUNTERS = bindings.MAX_TESS_EVALUATION_ATOMIC_COUNTERS;
        pub const MAX_GEOMETRY_ATOMIC_COUNTERS = bindings.MAX_GEOMETRY_ATOMIC_COUNTERS;
        pub const MAX_FRAGMENT_ATOMIC_COUNTERS = bindings.MAX_FRAGMENT_ATOMIC_COUNTERS;
        pub const MAX_COMBINED_ATOMIC_COUNTERS = bindings.MAX_COMBINED_ATOMIC_COUNTERS;
        pub const MAX_ATOMIC_COUNTER_BUFFER_SIZE = bindings.MAX_ATOMIC_COUNTER_BUFFER_SIZE;
        pub const MAX_ATOMIC_COUNTER_BUFFER_BINDINGS = bindings.MAX_ATOMIC_COUNTER_BUFFER_BINDINGS;
        pub const ACTIVE_ATOMIC_COUNTER_BUFFERS = bindings.ACTIVE_ATOMIC_COUNTER_BUFFERS;
        pub const UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX = bindings.UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX;
        pub const UNSIGNED_INT_ATOMIC_COUNTER = bindings.UNSIGNED_INT_ATOMIC_COUNTER;
        pub const VERTEX_ATTRIB_ARRAY_BARRIER_BIT = bindings.VERTEX_ATTRIB_ARRAY_BARRIER_BIT;
        pub const ELEMENT_ARRAY_BARRIER_BIT = bindings.ELEMENT_ARRAY_BARRIER_BIT;
        pub const UNIFORM_BARRIER_BIT = bindings.UNIFORM_BARRIER_BIT;
        pub const TEXTURE_FETCH_BARRIER_BIT = bindings.TEXTURE_FETCH_BARRIER_BIT;
        pub const SHADER_IMAGE_ACCESS_BARRIER_BIT = bindings.SHADER_IMAGE_ACCESS_BARRIER_BIT;
        pub const COMMAND_BARRIER_BIT = bindings.COMMAND_BARRIER_BIT;
        pub const PIXEL_BUFFER_BARRIER_BIT = bindings.PIXEL_BUFFER_BARRIER_BIT;
        pub const TEXTURE_UPDATE_BARRIER_BIT = bindings.TEXTURE_UPDATE_BARRIER_BIT;
        pub const BUFFER_UPDATE_BARRIER_BIT = bindings.BUFFER_UPDATE_BARRIER_BIT;
        pub const FRAMEBUFFER_BARRIER_BIT = bindings.FRAMEBUFFER_BARRIER_BIT;
        pub const TRANSFORM_FEEDBACK_BARRIER_BIT = bindings.TRANSFORM_FEEDBACK_BARRIER_BIT;
        pub const ATOMIC_COUNTER_BARRIER_BIT = bindings.ATOMIC_COUNTER_BARRIER_BIT;
        pub const MAX_IMAGE_UNITS = bindings.MAX_IMAGE_UNITS;
        pub const MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS = bindings.MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS;
        pub const IMAGE_BINDING_NAME = bindings.IMAGE_BINDING_NAME;
        pub const IMAGE_BINDING_LEVEL = bindings.IMAGE_BINDING_LEVEL;
        pub const IMAGE_BINDING_LAYERED = bindings.IMAGE_BINDING_LAYERED;
        pub const IMAGE_BINDING_LAYER = bindings.IMAGE_BINDING_LAYER;
        pub const IMAGE_BINDING_ACCESS = bindings.IMAGE_BINDING_ACCESS;
        pub const IMAGE_1D = bindings.IMAGE_1D;
        pub const IMAGE_2D = bindings.IMAGE_2D;
        pub const IMAGE_3D = bindings.IMAGE_3D;
        pub const IMAGE_2D_RECT = bindings.IMAGE_2D_RECT;
        pub const IMAGE_CUBE = bindings.IMAGE_CUBE;
        pub const IMAGE_BUFFER = bindings.IMAGE_BUFFER;
        pub const IMAGE_1D_ARRAY = bindings.IMAGE_1D_ARRAY;
        pub const IMAGE_2D_ARRAY = bindings.IMAGE_2D_ARRAY;
        pub const IMAGE_CUBE_MAP_ARRAY = bindings.IMAGE_CUBE_MAP_ARRAY;
        pub const IMAGE_2D_MULTISAMPLE = bindings.IMAGE_2D_MULTISAMPLE;
        pub const IMAGE_2D_MULTISAMPLE_ARRAY = bindings.IMAGE_2D_MULTISAMPLE_ARRAY;
        pub const INT_IMAGE_1D = bindings.INT_IMAGE_1D;
        pub const INT_IMAGE_2D = bindings.INT_IMAGE_2D;
        pub const INT_IMAGE_3D = bindings.INT_IMAGE_3D;
        pub const INT_IMAGE_2D_RECT = bindings.INT_IMAGE_2D_RECT;
        pub const INT_IMAGE_CUBE = bindings.INT_IMAGE_CUBE;
        pub const INT_IMAGE_BUFFER = bindings.INT_IMAGE_BUFFER;
        pub const INT_IMAGE_1D_ARRAY = bindings.INT_IMAGE_1D_ARRAY;
        pub const INT_IMAGE_2D_ARRAY = bindings.INT_IMAGE_2D_ARRAY;
        pub const INT_IMAGE_CUBE_MAP_ARRAY = bindings.INT_IMAGE_CUBE_MAP_ARRAY;
        pub const INT_IMAGE_2D_MULTISAMPLE = bindings.INT_IMAGE_2D_MULTISAMPLE;
        pub const INT_IMAGE_2D_MULTISAMPLE_ARRAY = bindings.INT_IMAGE_2D_MULTISAMPLE_ARRAY;
        pub const UNSIGNED_INT_IMAGE_1D = bindings.UNSIGNED_INT_IMAGE_1D;
        pub const UNSIGNED_INT_IMAGE_2D = bindings.UNSIGNED_INT_IMAGE_2D;
        pub const UNSIGNED_INT_IMAGE_3D = bindings.UNSIGNED_INT_IMAGE_3D;
        pub const UNSIGNED_INT_IMAGE_2D_RECT = bindings.UNSIGNED_INT_IMAGE_2D_RECT;
        pub const UNSIGNED_INT_IMAGE_CUBE = bindings.UNSIGNED_INT_IMAGE_CUBE;
        pub const UNSIGNED_INT_IMAGE_BUFFER = bindings.UNSIGNED_INT_IMAGE_BUFFER;
        pub const UNSIGNED_INT_IMAGE_1D_ARRAY = bindings.UNSIGNED_INT_IMAGE_1D_ARRAY;
        pub const UNSIGNED_INT_IMAGE_2D_ARRAY = bindings.UNSIGNED_INT_IMAGE_2D_ARRAY;
        pub const UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY = bindings.UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY;
        pub const UNSIGNED_INT_IMAGE_2D_MULTISAMPLE = bindings.UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
        pub const UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY = bindings.UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
        pub const MAX_IMAGE_SAMPLES = bindings.MAX_IMAGE_SAMPLES;
        pub const IMAGE_BINDING_FORMAT = bindings.IMAGE_BINDING_FORMAT;
        pub const IMAGE_FORMAT_COMPATIBILITY_TYPE = bindings.IMAGE_FORMAT_COMPATIBILITY_TYPE;
        pub const IMAGE_FORMAT_COMPATIBILITY_BY_SIZE = bindings.IMAGE_FORMAT_COMPATIBILITY_BY_SIZE;
        pub const IMAGE_FORMAT_COMPATIBILITY_BY_CLASS = bindings.IMAGE_FORMAT_COMPATIBILITY_BY_CLASS;
        pub const MAX_VERTEX_IMAGE_UNIFORMS = bindings.MAX_VERTEX_IMAGE_UNIFORMS;
        pub const MAX_TESS_CONTROL_IMAGE_UNIFORMS = bindings.MAX_TESS_CONTROL_IMAGE_UNIFORMS;
        pub const MAX_TESS_EVALUATION_IMAGE_UNIFORMS = bindings.MAX_TESS_EVALUATION_IMAGE_UNIFORMS;
        pub const MAX_GEOMETRY_IMAGE_UNIFORMS = bindings.MAX_GEOMETRY_IMAGE_UNIFORMS;
        pub const MAX_FRAGMENT_IMAGE_UNIFORMS = bindings.MAX_FRAGMENT_IMAGE_UNIFORMS;
        pub const MAX_COMBINED_IMAGE_UNIFORMS = bindings.MAX_COMBINED_IMAGE_UNIFORMS;
        pub const COMPRESSED_RGBA_BPTC_UNORM = bindings.COMPRESSED_RGBA_BPTC_UNORM;
        pub const COMPRESSED_SRGB_ALPHA_BPTC_UNORM = bindings.COMPRESSED_SRGB_ALPHA_BPTC_UNORM;
        pub const COMPRESSED_RGB_BPTC_SIGNED_FLOAT = bindings.COMPRESSED_RGB_BPTC_SIGNED_FLOAT;
        pub const COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT = bindings.COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        pub const TEXTURE_IMMUTABLE_FORMAT = bindings.TEXTURE_IMMUTABLE_FORMAT;

        // pub var drawArraysInstancedBaseInstance: *const fn (
        //     mode: Enum,
        //     first: Int,
        //     count: Sizei,
        //     instancecount: Sizei,
        //     baseinstance: Uint,
        // ) callconv(.C) void = undefined;
        // pub var drawElementsInstancedBaseInstance: *const fn (
        //     mode: Enum,
        //     count: Sizei,
        //     type: Enum,
        //     indices: *const anyopaque,
        //     instancecount: Sizei,
        //     baseinstance: Uint,
        // ) callconv(.C) void = undefined;
        // pub var drawElementsInstancedBaseVertexBaseInstance: *const fn (
        //     mode: Enum,
        //     count: Sizei,
        //     type: Enum,
        //     indices: *const anyopaque,
        //     instancecount: Sizei,
        //     basevertex: Int,
        //     baseinstance: Uint,
        // ) callconv(.C) void = undefined;
        // pub var getInternalformativ: *const fn (
        //     target: Enum,
        //     internalformat: Enum,
        //     pname: Enum,
        //     count: Sizei,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;

        // pub var getActiveAtomicCounterBufferiv: *const fn (
        //     program: Uint,
        //     bufferIndex: Uint,
        //     pname: Enum,
        //     params: [*c]Int,
        // ) callconv(.C) void = undefined;
        pub fn getActiveAtomicCounterBufferiv(
            program: Program,
            bufferIndex: Uint,
            pname: enum(Enum) {
                atomic_counter_buffer_size = ATOMIC_COUNTER_BUFFER_SIZE,
                atomic_counter_buffer_data_size = ATOMIC_COUNTER_BUFFER_DATA_SIZE,
                atomic_counter_buffer_active_atomic_counters = ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS,
                atomic_counter_buffer_active_atomic_counter_indices = ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES,
                atomic_counter_buffer_referenced_by_vertex_shader = ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER,
                atomic_counter_buffer_referenced_by_tess_control_shader = ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER,
                atomic_counter_buffer_referenced_by_tess_evaluation_shader = ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER,
                atomic_counter_buffer_referenced_by_geometry_shader = ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER,
                atomic_counter_buffer_referenced_by_fragment_shader = ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER,
            },
            params: []Int,
        ) void {
            bindings.getActiveAtomicCounterBufferiv(
                program.name,
                bufferIndex,
                @intFromEnum(pname),
                @ptrCast(params.ptr),
            );
        }

        // pub var bindImageTexture: *const fn (
        //     unit: Uint,
        //     texture: Uint,
        //     level: Int,
        //     layered: Boolean,
        //     layer: Int,
        //     access: Enum,
        //     format: Enum,
        // ) callconv(.C) void = undefined;
        // pub var memoryBarrier: *const fn (
        //     barriers: Bitfield,
        // ) callconv(.C) void = undefined;
        // pub var texStorage1D: *const fn (
        //     target: Enum,
        //     levels: Sizei,
        //     internalformat: Enum,
        //     width: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var texStorage2D: *const fn (
        //     target: Enum,
        //     levels: Sizei,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var texStorage3D: *const fn (
        //     target: Enum,
        //     levels: Sizei,
        //     internalformat: Enum,
        //     width: Sizei,
        //     height: Sizei,
        //     depth: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var drawTransformFeedbackInstanced: *const fn (
        //     mode: Enum,
        //     id: Uint,
        //     instancecount: Sizei,
        // ) callconv(.C) void = undefined;
        // pub var drawTransformFeedbackStreamInstanced: *const fn (
        //     mode: Enum,
        //     id: Uint,
        //     stream: Uint,
        //     instancecount: Sizei,
        // ) callconv(.C) void = undefined;

        //--------------------------------------------------------------------------------------------------
        //
        // OpenGL 4.3 (Core Profile)
        //
        //--------------------------------------------------------------------------------------------------
        pub const DEBUGPROC = *const fn (
            source: DebugSource,
            type: DebugType,
            id: Uint,
            severity: DebugSeverity,
            length: u32,
            message: [*]const u8,
            userParam: ?*const anyopaque,
        ) void;
        pub const DEBUG_OUTPUT = bindings.DEBUG_OUTPUT;
        pub const DEBUG_SOURCE_API = bindings.DEBUG_SOURCE_API;
        pub const DEBUG_SOURCE_WINDOW_SYSTEM = bindings.DEBUG_SOURCE_WINDOW_SYSTEM;
        pub const DEBUG_SOURCE_SHADER_COMPILER = bindings.DEBUG_SOURCE_SHADER_COMPILER;
        pub const DEBUG_SOURCE_THIRD_PARTY = bindings.DEBUG_SOURCE_THIRD_PARTY;
        pub const DEBUG_SOURCE_APPLICATION = bindings.DEBUG_SOURCE_APPLICATION;
        pub const DEBUG_SOURCE_OTHER = bindings.DEBUG_SOURCE_OTHER;
        pub const DEBUG_TYPE_ERROR = bindings.DEBUG_TYPE_ERROR;
        pub const DEBUG_TYPE_DEPRECATED_BEHAVIOR = bindings.DEBUG_TYPE_DEPRECATED_BEHAVIOR;
        pub const DEBUG_TYPE_UNDEFINED_BEHAVIOR = bindings.DEBUG_TYPE_UNDEFINED_BEHAVIOR;
        pub const DEBUG_TYPE_PORTABILITY = bindings.DEBUG_TYPE_PORTABILITY;
        pub const DEBUG_TYPE_PERFORMANCE = bindings.DEBUG_TYPE_PERFORMANCE;
        pub const DEBUG_TYPE_MARKER = bindings.DEBUG_TYPE_MARKER;
        pub const DEBUG_TYPE_PUSH_GROUP = bindings.DEBUG_TYPE_PUSH_GROUP;
        pub const DEBUG_TYPE_POP_GROUP = bindings.DEBUG_TYPE_POP_GROUP;
        pub const DEBUG_TYPE_OTHER = bindings.DEBUG_TYPE_OTHER;
        pub const DEBUG_SEVERITY_HIGH = bindings.DEBUG_SEVERITY_HIGH;
        pub const DEBUG_SEVERITY_MEDIUM = bindings.DEBUG_SEVERITY_MEDIUM;
        pub const DEBUG_SEVERITY_LOW = bindings.DEBUG_SEVERITY_LOW;
        pub const DEBUG_SEVERITY_NOTIFICATION = bindings.DEBUG_SEVERITY_NOTIFICATION;

        // pub var debugMessageControl: *const fn (
        //     source: Enum,
        //     type: Enum,
        //     severity: Enum,
        //     count: Sizei,
        //     ids: [*c]const Uint,
        //     enabled: Boolean,
        // ) callconv(.C) void = undefined;
        // pub var debugMessageInsert: *const fn (
        //     source: Enum,
        //     type: Enum,
        //     id: Uint,
        //     severity: Enum,
        //     length: Sizei,
        //     buf: [*c]const u8,
        // ) callconv(.C) void = undefined;

        // pub var debugMessageCallback: *const fn (
        //     callback: DEBUGPROC,
        //     userParam: ?*const anyopaque,
        // ) callconv(.C) void = undefined;
        pub fn debugMessageCallback(
            callback: DEBUGPROC,
            userParam: ?*const anyopaque,
        ) void {
            bindings.debugMessageCallback(@as(bindings.DEBUGPROC, @ptrCast(callback)), userParam);
        }

        // pub var getDebugMessageLog: *const fn (
        //     count: Uint,
        //     bufSize: Sizei,
        //     sources: [*c]Enum,
        //     types: [*c]Enum,
        //     ids: [*c]Uint,
        //     severities: [*c]Enum,
        //     lengths: [*c]Sizei,
        //     messageLog: [*c]Char,
        // ) callconv(.C) Uint = undefined;
        // pub var getPointerv: *const fn (
        //     pname: Enum,
        //     params: [*c][*c]anyopaque,
        // ) callconv(.C) void = undefined;
        // pub var pushDebugGroup: *const fn (
        //     source: Enum,
        //     id: Uint,
        //     length: Sizei,
        //     message: [*c]const Char,
        // ) callconv(.C) void = undefined;
        // pub var popDebugGroup: *const fn () callconv(.C) void = undefined;
        // pub var objectLabel: *const fn (
        //     identifier: Enum,
        //     name: Uint,
        //     length: Sizei,
        //     label: [*c]const Char,
        // ) callconv(.C) void = undefined;
        // pub var getObjectLabel: *const fn (
        //     identifier: Enum,
        //     name: Uint,
        //     bufSize: Sizei,
        //     length: *Sizei,
        //     label: [*c]Char,
        // ) callconv(.C) void = undefined;
        // pub var objectPtrLabel: *const fn (
        //     ptr: *anyopaque,
        //     length: Sizei,
        //     label: [*c]const Char,
        // ) callconv(.C) void = undefined;
        // pub var getObjectPtrLabel: *const fn (
        //     ptr: *anyopaque,
        //     bufSize: Sizei,
        //     length: *Sizei,
        //     label: [*c]Char,
        // ) callconv(.C) void = undefined;

        //------------------------------------------------------------------------------------------
        //
        // OpenGL ES 1.0
        //
        //------------------------------------------------------------------------------------------
        // pub var clearDepthf: *const fn (depth: Float) callconv(.C) void = undefined;
        // pub var depthRangef: *const fn (n: Clampf, f: Clampf) callconv(.C) void = undefined;

        //------------------------------------------------------------------------------------------
        //
        // OpenGL ES 2.0
        //
        //------------------------------------------------------------------------------------------
        pub const FRAMEBUFFER_INCOMPLETE_DIMENSIONS = bindings.FRAMEBUFFER_INCOMPLETE_DIMENSIONS;

        //------------------------------------------------------------------------------------------
        //
        // OES_vertex_array_object
        //
        //------------------------------------------------------------------------------------------
        pub const VERTEX_ARRAY_BINDING_OES = bindings.VERTEX_ARRAY_BINDING_OES;
    };
}
