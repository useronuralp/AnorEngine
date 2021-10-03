#pragma once
#include "core.h"
//TODO: the order of these files can cause circular dependency. Check these thoroughly sometime.
//-----Engine modules---------------
#include "Graphics/OpenGLWindow.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/simplerenderer3D.h"
#include "Graphics/EditorCamera.h"
#include "Graphics/OrthographicCameraController.h"
#include "Graphics/ParticleSystem.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Layers/Layer.h"
#include "Graphics/Layers/LayerStack.h"
#include "Graphics/Layers/ImGuiBase.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/ScriptableEntity.h"
#include "Utility/Logger.h"
#include "Utility/Timer.h"
#include "Events/Event.h"
#include "Events/EventHandler.h"

#include "Renderer/Renderer2D.h" //Try to include this at bottom for now. Some of the above files need to be there first for this inclusion to work properly. TODO : Solve this problem.

//Trivial Stuff
#include "EngineKeyCodes.h"
#include "MouseButtonCodes.h"

//External stuff
#include <imgui.h>
#include <entt.h>
