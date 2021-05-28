#type vertex
#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv; //These values are grabbed from the glVertexAttribPointer() function in OpenGL. The index (first parameter) value of the attrib function is what the "locations" here become.
layout (location = 2) in float tid;
layout (location = 3) in vec4 color; //The pipeline is CPU --> OpenGL --> Vertex Shader --> Fragment Shader -->
layout (location = 4) in vec3 aNormal;

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

layout(location = 0) out vec4 color;
//out vec4 FragColor;

uniform vec4 colour;
uniform vec2 light_pos;
uniform vec3 directionalLight = vec3(-0.2f, -1.0f, -0.3f);
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 cameraPos = vec3(0.3f, 0.3f, 0.3f); //fixed value for now. You should set this in client side if you want to dynamically change it

float constant = 0.3f;
float lin = 0.09f;
float quadratic = 0.032f;


uniform sampler2D tex;
uniform sampler2D textureSampler;

in DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;
	vec3 Normal;
	vec3 FragPos;
}fs_out;



#define DIRECTIONAL_LIGHTING 0 //or POINT_LIGHTING

void main()
{
	float distance = length(vec3(light_pos, 0.0f) - fs_out.FragPos);
	float attenuation = 1.0 / (constant + lin * distance + quadratic * (distance * distance));

	float ambientStrength = 0.1f;
	vec3 normal = normalize(fs_out.Normal);
#if DIRECTIONAL_LIGHTING
	vec3 lightDir = normalize(-directionalLight); // this is for directional light like sun. this function uses a global value directionalLight
#else
	vec3 lightDir = normalize(vec3(light_pos, 1.0f) - fs_out.FragPos); // this is used for point light.
#endif
	vec3 ambient = ambientStrength * lightColor;
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

#if DIRECTIONAL_LIGHTING	
#else
	ambient *= attenuation;
	diffuse *= attenuation;
#endif
	vec4 texColor = texture(textureSampler, fs_out.uv);
	vec3 result = (ambient + diffuse) * vec3(texColor);
	color = vec4(result, 1.0);
}

