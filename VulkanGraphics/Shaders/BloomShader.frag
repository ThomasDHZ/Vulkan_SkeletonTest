

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 BloomColor;

void main() {
    outColor = texture(texSampler, fragTexCoord);
    BloomColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

