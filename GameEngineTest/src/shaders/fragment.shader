#version 330 core

layout (location = 0) out vec4 color;
//out vec4 FragColor;

uniform vec4 colour;
uniform vec2 light_pos;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 cameraPos;

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
	//float specularStrength = 0.5f;
	float ambientStrength = 0.1f;
	vec3 norm = normalize(fs_out.Normal);
	vec3 lightDir = normalize(vec3(light_pos, 1.0f) - fs_out.FragPos);

	//vec3 viewDir = normalize(cameraPos - fs_out.FragPos);
	//vec3 reflectDir = reflect(-lightDir, fs_out.Normal);		//specular lighting calculations. It doesn't work though. Look into this.
	//float spec = pow(max(dot(cameraPos, reflectDir), 0.0), 32);
	//vec3 specular = specularStrength * spec * lightColor;
    vec3 ambient = ambientStrength * lightColor;
		

    
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	


	float intensity = 1.0 / length(fs_out.position.xy - light_pos);
	//color = /*fs_out.color */ colour * intensity; // if you uncomment colour here, you use uniform colour that you can set in the CPU. However, if you uncomment "fs_out.color" you use the attribute color of the vertex.

	vec4 texColor = colour;

	/*if(fs_out.tid > 0.0)
	{	
		int tid = int(fs_out.tid - 0.5);
		texColor = texture(u_Textures[tid], fs_out.uv);
	}*/
	texColor = texture(tex, fs_out.uv);
	//color =  texColor * intensity ; 

	vec3 result = (ambient + diffuse) * vec3(texColor);
	color = vec4(result, 1.0);
}