  #version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 BoneID;
layout (location = 6) in vec4 BoneWeights;

layout(location = 0) out vec3 TexCoords;

void main() {
	TexCoords = aPos;
	vec4 pos = ubo.projection * ubo.view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
