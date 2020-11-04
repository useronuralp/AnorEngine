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

int main() {


	std::vector<StaticSprite*> sprites;
	srand(time(NULL));

	Window window("Onuralp" , 1920, 1080);	


	Shader *shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	
	shader->setUniform2f("light_pos", vec2(4.0f, 1.5f));
	
	Texture texture("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\sample_gigatextures_4096_seamless2.png");
	texture.Bind(0);

	shader->setUniform1f("u_Texture", 0);

	shader->setUniformMat4("pr_matrix", Math::mat4::orthographic(0, 16.0f, 0, 9.0f, -1.0f, 1.0f));

	TileLayer layer(shader);

	for(float j = -9.0f; j < 9.0f; j++)
	{	
		for (float i = -16.0f; i < 16.0f; i ++)
		{
			layer.add(new Sprite(i, j, 0.9f, 0.9f, Math::vec4(rand() % 100 / 100.0f, 0.2f, 0.6f, 1))); // since you're creating these Sprites using the 'new' keyword, you need to pass them as pointers. New keyword returns a void* pointer.
		}
	}

	SimpleRenderer2D simpleRenderer;
	
	double x, y;
	int z = 0;

	for (float i = -16.0f; i< 16.0f; i++)
	{
		for (float j = -9.0f; j < 9.0f; j++)
		{
			sprites.push_back(new StaticSprite(i, j, 0.9f, 0.9f, vec4(1, 1, 1, 1), *shader, texture));
	}

	while (!window.closed()) {
	
		window.clear();

		window.getMousePosition(x, y);
		shader->setUniform2f("light_pos", vec2((x * 32.0f / 1920.0f - 16.0f), (9.0f - y * 18.0f / 1080.0f)));

		for (float i = 0; i < sprites.size(); i++)
		{
			simpleRenderer.submit(sprites[i]);
		}

		simpleRenderer.flush();
		//layer.render();


		//batch.begin();
		//for(int i= 0; i < sprites.size(); i++)
		//{
		//	batch.submit(sprites[i]);
		//}


		//batch.submit(&sprite);
		//batch.submit(&sprite2);
		//z++;


		//batch.end();
		//batch.flush();
		//texture.Unbind();
	
		window.update();	
	}
	return 0;
	
}