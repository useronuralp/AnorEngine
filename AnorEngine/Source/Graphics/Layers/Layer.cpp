#include "pch.h"
#include "Layer.h"

namespace AnorEngine {
	namespace Graphics
	{
		Layer::Layer()
		{
			m_LayerName = "Layer";
			WARN("Layer Default contstructor!!");
		}
		Layer::Layer(const char* name)
			:m_LayerName(name)
		{
			WARN("Layer constructor with name initialization!!");
		}
		void Layer::logInfoDebug()
		{
			//CRITICAL("Created SuperClass Layer...");
			//CRITICAL("Layer name: {0}", m_LayerName);
		}
		Layer::~Layer()
		{	
			WARN("Layer Destructor completed!!!");
		}
	}
}