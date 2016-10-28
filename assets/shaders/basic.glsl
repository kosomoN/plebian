@vertex
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_uv;

out vec3 vertNormal;
out vec2 uv;
out vec3 vertexPos;

uniform mat4 MVP;
uniform mat4 worldMat;

void main () {
    gl_Position = MVP * vec4(pos, 1.0);
    vertexPos = (worldMat * vec4(pos, 1.0)).xyz;
    vertNormal = (worldMat * vec4(vert_normal, 0)).xyz;
    uv = vert_uv;
};

@fragment

#version 330

uniform sampler2D diffuseTex;

in vec3 vertNormal;
in vec2 uv;
in vec3 vertexPos;

layout (location = 0) out vec3 world_pos_out;
layout (location = 1) out vec3 diffuse_out;
layout (location = 2) out vec3 normal_out;

void main() {
    normal_out = normalize(vertNormal);
    world_pos_out = vertexPos;
    diffuse_out = texture(diffuseTex, uv).rgb;
}
