#version 330 core

// VBO-ból érkezõ változók
in vec3 vs_in_pos;
in vec3 vs_in_norm;
in vec2 vs_in_tex;

// a pipeline-ban tovább adandó értékek
out vec3 vs_out_pos;
out vec3 vs_out_norm;
out vec2 vs_out_tex;

// shader külsõ paraméterei
uniform mat4 MVP;
uniform mat4 world;
uniform mat4 worldIT;
uniform int time;

uniform sampler2D texImage;

void main()
{
	
	vec3 pos = vs_in_pos;
	float pi = 3.14159265359f;

	float waveCount = 10f;
	float waveSpeed = time / 10000.0f;
	float waveStrength = 0.2f;

	float wave = cos(((pos.x+pos.z)-waveSpeed) * pi * waveCount);
	
	vec4 tex = texture(texImage, vs_in_tex);
	float gray = ((tex.y + tex.x + tex.z) / 3.0f);
	vec3 offset = vec3(0,1,0) * gray * 0.75f - vec3(0, 0.5f, 0);

	pos.y += waveStrength * wave;
	pos += offset;
	
	gl_Position = MVP * vec4( pos, 1 );
	vs_out_pos = (world * vec4(pos, 1)).xyz;
	vs_out_norm = (worldIT * vec4(vs_in_norm, 0)).xyz;
	vs_out_tex = vs_in_tex;
}