@vertex
#version 330

layout(location = 0) in vec3 pos;

uniform mat4 MVP;

void main () {
    gl_Position = MVP * vec4(pos, 1.0);
};

@fragment

#version 330

void main() {
}