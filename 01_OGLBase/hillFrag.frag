#version 330 core

// pipeline-b�l bej�v� per-fragment attrib�tumok
in vec3 vs_out_pos;
in vec3 vs_out_norm;
in vec2 vs_out_tex;

out vec4 fs_out_col;

// ir�ny f�nyforr�s: f�ny ir�nya
uniform vec3 light_dir = vec3(0,-1,0);

// f�nytulajdons�gok: ambiens, diff�z, ...
uniform vec3 La = vec3(0.4, 0.4, 0.4);
uniform vec3 Ld = vec3(0.6, 0.6, 0.6);

uniform sampler2D texImage;

void main()
{

	vec3 ambient = La;

	vec3 normal = normalize(vs_out_norm);
	vec3 to_light = normalize(-light_dir);
	
	float cosa = clamp(dot(normal, to_light), 0, 1);

	vec3 diffuse = cosa*Ld;
	float y = vs_out_pos.y / 20.0f;
	if (y > 1)
		y = 1;
	vec4 tex = vec4(y, (y * 0.5f) + 0.5f, y, 1);
	fs_out_col = vec4(ambient + diffuse, 1) * tex;
}