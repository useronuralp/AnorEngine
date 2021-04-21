#pragma once

//For use only by Client programs. Do not include this in actual engine code.
#include "core.h"

#include "Application.h"

#include "ImGui/ImGuiLayer.h"

//-----Engine modules---------------
#include "Graphics/window.h"
#include "Math/maths.h"
#include "Graphics/shader.h"
#include "Graphics/Buffers/buffer.h"
#include "Graphics/Buffers/indexbuffer.h"
#include "Graphics/Buffers/vertexarray.h"
#include "Graphics/renderer2D.h"
#include "Graphics/renderable2D.h"
#include "Graphics/simpleRenderer2D.h"
#include "Graphics/static_sprite.h"
#include "Graphics/batchrenderer2D.h"
#include "Graphics/sprite.h"
#include "Graphics/Layers/tilelayer.h"
#include "Graphics/texture.h"
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include "Graphics/simplerenderer3D.h"
#include "Graphics/renderable3D.h"
#include "vendor/Models/cube.h"
#include "Graphics/mesh.h"
#include "Graphics/model.h"

#include "utils/Logger.h"

#include "Graphics/Layers/Layer.h"

#include "Graphics//Layers/LayerStack.h"

//--------------------Entry point--------------------
#include "EntryPoint.h"
//--------------------Entry point--------------------