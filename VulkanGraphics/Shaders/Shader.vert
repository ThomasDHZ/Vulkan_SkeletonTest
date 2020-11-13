

#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
    mat4 BoneTransform[100];
} ubo;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 BoneID;
layout (location = 6) in vec4 BoneWeights;

layout(location = 0) out vec2 fragTexCoord;

void main() 
{
    mat4 BoneTransform = ubo.BoneTransform[BoneID[0]] * BoneWeights[0];
		 BoneTransform += ubo.BoneTransform[BoneID[1]] * BoneWeights[1];
	     BoneTransform += ubo.BoneTransform[BoneID[2]] * BoneWeights[2];
	     BoneTransform += ubo.BoneTransform[BoneID[3]] * BoneWeights[3];

    vec4 BonePosisition = BoneTransform * vec4(inPosition, 1.0);

    gl_Position = ubo.proj * ubo.view * ubo.model * BonePosisition;
    fragTexCoord = inTexCoord;
}

