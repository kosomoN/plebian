@vertex
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_uv;

out vec4 normal;
out vec2 uv;

uniform mat4 MVP;
uniform mat4 worldMat;

void main () {
    gl_Position = MVP * vec4(pos, 1.0);
    normal = worldMat * vec4(vert_normal, 0);
    uv = vert_uv;
};

@fragment

#version 330

uniform sampler2D diffuseTex;

in vec4 normal;
in vec2 uv;

out vec4 frag_color;

void main() {
    float diffuse = clamp(dot(normal, vec4(1, -1, 1, 0)), 0.1f, 1);
    frag_color = texture(diffuseTex, uv) * diffuse;
    frag_color.rgb = pow(frag_color.rgb, vec3(1.0/2.2));
}
