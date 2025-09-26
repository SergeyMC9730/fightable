#version 320 es

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

vec2 wave(vec2 p) {
	float pi = 3.14159;

	float A = 0.15;
	float w = 10.0 * pi;
	float t = 30.0 * pi / 180.0;
	float y = sin(w * p.x + t) * A;

	return vec2(p.x, p.y + y);
}

void main() {
	vec2 p = fragTexCoord;
	vec2 uv = wave(p);
	vec4 tcolor = texture2D(texture0, uv);
	
	gl_FragColor = tcolor;
}