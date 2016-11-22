@vertex
#version 330

layout (location = 0) in vec2 position;

out vec2 tex_coords;

void main () {
    gl_Position = vec4(position, 0.0, 1.0);
	tex_coords = ((position + vec2(1.0)) / 2.0);
};

@fragment
#version 330

in vec2 tex_coords;

out vec4 color;

uniform sampler2D diffuse;

void main() {
    color = texture(diffuse, tex_coords);
}
