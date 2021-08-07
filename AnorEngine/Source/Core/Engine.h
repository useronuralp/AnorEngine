#pragma once
#include "core.h"

#include "EngineKeyCodes.h"
#include "MouseButtonCodes.h"

//-----Engine modules---------------
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/simplerenderer3D.h"
#include "../Assets/Models/cube.h"
#include "imgui.h"
#include "Graphics/Layers/ImGuiBase.h"
#include "Utility/Logger.h"
#include "Events/EventHandler.h"
#include "Graphics/OrthographicCameraController.h"
#include "Graphics/ParticleSystem.h"
#include "Utility/Timer.h"
#include "Graphics/Framebuffer.h"
#include "Scene/Entity.h"
#include "Scene/ScriptableEntity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "entt.h"
#include "Graphics/EditorCamera.h"
//Try to include this at the bottom. I don't know what kind of circular header inclusion fuckery goes on in the code base but I could not find the problem lol
#include "Renderer/Renderer.h"
