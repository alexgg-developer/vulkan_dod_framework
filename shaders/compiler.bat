%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\shader_base.vert
move %~dp0\vert.spv %~dp0\compiled/shader_base_vert.spv
%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\shader_base.frag
move %~dp0\frag.spv %~dp0\compiled/shader_base_frag.spv

%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\shader_base_procedural_triangle.vert
move %~dp0\vert.spv %~dp0\compiled/shader_base_procedural_triangle_vert.spv
%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\shader_base_procedural_triangle.frag
move %~dp0\frag.spv %~dp0\compiled/shader_base_procedural_triangle_frag.spv

%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\ubo_shader_base.vert
move %~dp0\vert.spv %~dp0\compiled/ubo_shader_base_vert.spv
%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\ubo_shader_base.frag
move %~dp0\frag.spv %~dp0\compiled/ubo_shader_base_frag.spv

%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\texture_shader_base.vert
move %~dp0\vert.spv %~dp0\compiled/texture_shader_base_vert.spv
%VULKAN_SDK%/Bin/glslangValidator.exe -V %~dp0\texture_shader_base.frag
move %~dp0\frag.spv %~dp0\compiled/texture_shader_base_frag.spv
