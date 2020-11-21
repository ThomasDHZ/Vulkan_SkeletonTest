#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 Normal;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout(location = 3) out vec4 BloomColor;

void main() {
    gPosition = vec4(FragPos, 1.0f);
    gNormal = vec4(Normal, 1.0f);
    gAlbedoSpec.rgb = texture(texSampler, fragTexCoord).rgb;
    gAlbedoSpec.a = texture(texSampler, fragTexCoord).r;
    BloomColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

