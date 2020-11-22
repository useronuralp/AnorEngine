#include "src/Graphics/window.h"
#include "src/Math/maths.h"
#include "src/Graphics/shader.h"
#include "src/Graphics/Buffers/buffer.h"
#include "src/Graphics/Buffers/indexbuffer.h"
#include "src/Graphics/Buffers/vertexarray.h"
#include "src/Graphics/renderer2D.h"
#include "src/Graphics/renderable2D.h"
#include "src/Graphics/simpleRenderer2D.h"
#include "src/Graphics/static_sprite.h"
#include "src/Graphics/batchrenderer2D.h"
#include "src/Graphics/sprite.h"
#include <time.h>
#include "src/Graphics/Layers/tilelayer.h"
#include "src/Graphics/texture.h"
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <algorithm> 
#include "src/Graphics/renderable3D.h"
#include "src/Graphics/simplerenderer3D.h"
#include "src/vendor/Models/cube.h"

using namespace GameEngineTest;
using namespace Math;
using namespace Graphics;


int main()
{		
	
	std::vector<StaticSprite*> sprites;
	srand(time(NULL));

	Window window("Onuralp", 1920, 1080);
	

	GLint texIDs[] =
	{
		0,1,2,3,4,5,6,7,8,9  // You can't set these without enabling the shader.
	};


	Shader* shader = new Shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	Shader* shader2 = new Shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");



	Shader shader3("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	Shader shader5("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	Shader shader4("src/shaders/vertex.shader", "src/shaders/lightsourceFrag.shader");

	shader->enable();
	shader->setUniform2f("light_pos", vec2(4.0f, 1.5f));
	shader->setUniform1iv("u_Textures", texIDs, 10);
	shader->disable();


	shader2->enable();
	shader2->setUniform1iv("u_Textures", texIDs, 10);
	shader2->disable();

	shader3.enable();
	shader3.setUniform1i("tex", 0);
	shader3.setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
	shader3.setUniform3f("objectColor", vec3(1.0f, 0.5f, 0.31f));		//this is object
	shader3.setUniform4f("colour", vec4(0.8f, 0.2f, 1,1));
	shader3.disable();

	shader4.enable();
	shader4.setUniform1i("tex", 0);
	shader4.setUniform2f("light_pos", vec2(1, 1));		//this is light
	shader4.setUniform4f("colour", vec4(1, 1, 1, 1));
	shader4.disable();

	shader5.enable();
	shader5.setUniform1i("tex", 0);
	shader5.setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
	shader5.setUniform3f("objectColor", vec3(1.0f, 0.5f, 0.31f));		//this is object
	shader5.setUniform4f("colour", vec4(0.8f, 0.2f, 1, 1));
	shader5.disable();

	Texture texture("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\sample_gigatextures_4096_seamless2.png");
	Texture texture2("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\tileable-grass_clover_tt7010116-sample_1.png");
	Texture texture3("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\381f5a63791945.5abc4ccf1297d.png");
	Texture* textures[] = {	&texture, &texture2, &texture3 };

	shader->enable();
	TileLayer ground(shader);
	shader->disable();
	shader2->enable();
	TileLayer player(shader2);
	shader2->disable();

	for (float i = -16.0f; i < 16.0f; i++)
	{
		//layer.add(new Sprite(i, j, 0.9f, 0.9f, Math::vec4(rand() % 100 / 100.0f, 0.2f, 0.6f, 1))); // since you're creating these Sprites using the 'new' keyword, you need to pass them as pointers. New keyword returns a void* pointer.
		ground.add(new Sprite(i, -9, 0.9f, 0.9f, &texture2));
	}

	player.add(new Sprite(0, 0, 0.9f, 0.9f, &texture3));


	float gravity = 0;
	for (float i = -16.0f; i < 16.0f; i++)
	{
		for (float j = -9.0f; j < 9.0f; j++)
		{
			sprites.push_back(new StaticSprite(i, j, 0.9f, 0.9f, vec4(1, 1, 1, 1), *shader, texture));
		}
	}

	vec2 *translations = new vec2[900];
	int index = 0;
	float offset = 0.00;
	vec2 translation;
	for (int i = -15; i < 15; i++)
	{
		for (int j = -15; j < 15; j++)
		{
			
			translation.x = (float)i * (1  + offset);
			translation.y = (float)j * (1  + offset);
			translations[index++] = translation;

		}
	}

	shader3.enable();
	for (unsigned int i = 0; i < 900; i++)
	{
		shader3.setUniform2f("offsets[" + std::to_string(i) + "]", translations[i]);
	}
	shader3.disable();
	shader5.enable();
	for (unsigned int i = 0; i < 900; i++)
	{
		shader5.setUniform2f("offsets[" + std::to_string(i) + "]", translations[i]);
	}
	shader5.disable();

	delete[] translations;


	int layout[] = { UV_VALUE, POSITION_VALUE, NORMAL_VALUE};
	Renderable3D cube(Model::vertices, 8 * 6 * 6, layout, 3, 8 * sizeof(float));
	Renderable3D cube2(Model::vertices, 8 * 6 * 6, layout, 3, 8 * sizeof(float));
	int layout2[] = { POSITION_VALUE};
	Renderable3D light(Model::lightSourceVertx, 3 * 6 * 6, layout2, 1, 3 * sizeof(float));

	cube.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	cube2.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
	cube2.translate(15.0f, 0.0f, 0.0f);


	float lightX = 2, lightY = 1.5f;
	light.translate(lightX, lightY, 0.0f);
	light.scale(0.3f, 0.3f, 0.3f);


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	texture2.Bind(0);

	shader5.enable();
	shader5.setUniform3f("cameraPos", vec3(0.3f, 0.3f, 0.3f));
	shader5.setUniform2f("light_pos", vec2(lightX, lightY));
	shader5.disable();

	shader3.enable();
	shader3.setUniform3f("cameraPos", vec3(0.3f, 0.3f, 0.3f));
	shader3.setUniform2f("light_pos", vec2(lightX, lightY));
	shader3.disable();

	shader4.enable();
	shader4.setUniform2f("light_pos", vec2(lightX, lightY));
	shader4.disable();

	glEnable(GL_DEPTH_TEST);

	glm::mat4 camera;
	Renderer3D renderer3D;

	while (!window.closed()) {

		window.clear();
		camera = glm::lookAt(window.cameraPos, window.cameraPos + window.cameraFront, window.cameraUp); // camera movement operations on view matrix each frame.

		cube.rotate(0.00005f * 10.0f, 0.0f, .0f, 0.5f); // TODO: implement delta time 
		renderer3D.instancedDrawVertex(cube, shader3, camera, 36, 900);
		cube2.rotate(0.00005f * 50.0f, 0.0f, 1.0f, 0.0f);
		renderer3D.instancedDrawVertex(cube2, shader5, camera, 36, 900);

		renderer3D.singleDrawVertex(light, shader4, camera, 36);


		window.update();
	}
	return 0;
}