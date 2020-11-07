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

	Shader* shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader->enable();
	shader->setUniform2f("light_pos", vec2(4.0f, 1.5f));
	shader->setUniform1iv("u_Textures", texIDs, 10);




	Texture texture("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\sample_gigatextures_4096_seamless2.png");
	Texture texture2("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\tileable-grass_clover_tt7010116-sample_1.png");
	Texture texture3("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\381f5a63791945.5abc4ccf1297d.png");
	Texture* textures[] = 
	{
		&texture, &texture2, &texture3
	};



	TileLayer layer(shader);
	for (float j = -9.0f; j < 9.0f; j++)
	{
		for (float i = -16.0f; i < 16.0f; i++)
		{
			//layer.add(new Sprite(i, j, 0.9f, 0.9f, Math::vec4(rand() % 100 / 100.0f, 0.2f, 0.6f, 1))); // since you're creating these Sprites using the 'new' keyword, you need to pass them as pointers. New keyword returns a void* pointer.
			layer.add(new Sprite(i, j, 0.9f, 0.9f, textures[rand() % 3]));
		}
	}




	double x, y;

	for (float i = -16.0f; i < 16.0f; i++)
	{
		for (float j = -9.0f; j < 9.0f; j++)
		{
			sprites.push_back(new StaticSprite(i, j, 0.9f, 0.9f, vec4(1, 1, 1, 1), *shader, texture));
		}
	}
	
	while (!window.closed()) {

		window.clear();
		shader->enable(); // this enable thingy is very important BE CAREFUL. You can't set stuff in shaders without glBindProgram()
		window.getMousePosition(x, y);
		shader->setUniform2f("light_pos", vec2((x * 32.0f / 1920.0f - 16.0f), (9.0f - y * 18.0f / 1080.0f)));

		//for (float i = 0; i < sprites.size(); i++)
		//{
		//	simpleRenderer.submit(sprites[i]);   //simple renderer2D test render.
		//}
		//
		//simpleRenderer.flush();


		layer.render(); //layer rendering using batch renderer.


		//batch.begin();
		//for(int i= 0; i < sprites.size(); i++)
		//{
		//	batch.submit(sprites[i]);
		//}											 //batch rendering example

		//batch.end();
		//batch.flush();
		//texture.Unbind();

		window.update();
	}
	return 0;
}