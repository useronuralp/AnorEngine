#version 330 core

layout (location = 0) out vec4 color;


uniform vec4 colour;
uniform vec2 light_pos;


in DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;

}fs_out;

//uniform sampler2D u_Textures[32];
uniform sampler2D tex;

void main()
{	
	float intensity = 1.0 / length(fs_out.position.xy - light_pos);
	//color = /*fs_out.color */ colour * intensity; // if you uncomment colour here, you use uniform colour that you can set in the CPU. However, if you uncomment "fs_out.color" you use the attribute color of the vertex.

	vec4 texColor = colour;

	/*if(fs_out.tid > 0.0)
	{	
		int tid = int(fs_out.tid - 0.5);
		texColor = texture(u_Textures[tid], fs_out.uv);
	}*/
	//texColor = texture(tex, fs_out.uv);
	color =  vec4(1.0f); //always constant white
}