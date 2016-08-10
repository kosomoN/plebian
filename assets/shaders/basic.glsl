@vertex
#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vert_normal;
layout(location = 2) in vec2 vert_uv;

out vec3 normal;
out vec2 uv;

uniform mat4 MVP;

void main () {
    gl_Position = MVP * vec4(pos, 1.0);
    normal = vert_normal;
    uv = vert_uv;
};

@fragment

#version 330

uniform sampler2D diffuseTex;

in vec3 normal;
in vec2 uv;

out vec4 frag_color;

void main() {
	float diffuse = clamp(dot(normal, vec3(1, -1, 1)), 0, 1);
	frag_color = texture(diffuseTex, uv) * (diffuse + 0.2f);
}