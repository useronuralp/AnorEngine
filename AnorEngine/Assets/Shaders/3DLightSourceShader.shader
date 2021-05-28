#type vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv; //These values are grabbed from the glVertexAttribPointer() function in OpenGL. The index (first parameter) value of the attrib function is what the "locations" here become.
layout(location = 2) in float tid;
layout(location = 3) in vec4 color; //The pipeline is CPU --> OpenGL --> Vertex Shader --> Fragment Shader -->
layout(location = 4) in vec3 aNormal;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

uniform vec2 offsets[900];

out DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;
	vec3 Normal;
	vec3 FragPos;
}vs_out;


void main()
{


	vec2 offset = offsets[gl_InstanceID];
	vs_out.Normal = mat3(transpose(inverse(ml_matrix))) * aNormal;
	//vs_out.Normal = aNormal;
	gl_Position = (pr_matrix * vw_matrix * ml_matrix) * (position); //+ vec4(offset, 0.0, 1.0)); 
	vs_out.uv = uv;
	vs_out.tid = tid;
	//vs_out.position = ml_matrix * position;
	vs_out.color = color;
	vs_out.FragPos = vec3(ml_matrix * position);
	//vs_out.v_TexCoord = texCoord;
}

#type fragment
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
	vec3 Normal;
	vec3 FragPos;
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
	texColor = texture(tex, fs_out.uv);
	color = vec4(1, 1, 1, 1); //bright white
}