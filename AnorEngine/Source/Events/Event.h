#pragma once
namespace AnorEngine
{
	namespace Input
	{	
		enum class ANOR_API EventType
		{
			None = 0, KeyPressEvent, KeyReleaseEvent,
			MouseClickEvent, MouseReleaseEvent, MouseMoveEvent, MouseScrollEvent,
			WindowResizeEvent
		};

		class ANOR_API Event
		{
		protected:
			EventType m_EventType = EventType::None;
		public:
			virtual void Log() { INFO("Event"); }
			virtual EventType GetEventType() { return m_EventType; }
			virtual  ~Event() {}
		public:
			bool m_Handled = false;
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API KeyPressEvent : public Event
		{
		public:
			KeyPressEvent(int KeyCode, int Action)
				:m_KeyCode(KeyCode), m_Action(Action)
			{
				Event::m_EventType = EventType::KeyPressEvent;
			}
		public:
			void Log() override { INFO("Key Press Event. Pressed Key: {0}" , m_KeyCode); }
			int GetKeyCode() { return m_KeyCode; }
			int GetAction() { return m_Action; }
		private:
			int m_KeyCode, m_Action;
		public:
			virtual ~KeyPressEvent(){} 
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API KeyReleaseEvent : public Event
		{
		public:
			KeyReleaseEvent(int KeyCode)
				:m_KeyCode(KeyCode)
			{
				Event::m_EventType = EventType::KeyReleaseEvent;
			}
		public:
			void Log() override { INFO("Key Release Event. Pressed Key: {0}", m_KeyCode); }
			int GetKeyCode() { return m_KeyCode; }
		private:
			int m_KeyCode;
		public:
			virtual ~KeyReleaseEvent(){}
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API MouseClickEvent: public Event
		{
		public:
			MouseClickEvent(int MouseCode, int Action)
				:m_MouseCode(MouseCode), m_Action(Action)
			{
				Event::m_EventType = EventType::MouseClickEvent;
			}
		public:
			void Log() override { INFO("Mouse Click Event. Clicked Mouse Button: {0}", m_MouseCode); }
			int GetMouseCode() { return m_MouseCode; }
			int GetMouseAction() { return m_Action; }
		public:
			virtual ~MouseClickEvent() override {}
		private:
			int m_MouseCode, m_Action;
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API MouseReleaseEvent : public Event
		{
		public:
			MouseReleaseEvent(int MouseCode, int Action)
				:m_MouseCode(MouseCode), m_Action(Action)
			{
				Event::m_EventType = EventType::MouseReleaseEvent;
			}
		public:
			void Log() override { INFO("Mouse Release Event. Released Mouse Button: {0}", m_MouseCode); }
			int GetMouseCode() { return m_MouseCode; }
			int GetMouseAction() { return m_Action; }
		public:
			virtual ~MouseReleaseEvent() override {}
		private:
			int m_MouseCode, m_Action;
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API MouseMoveEvent : public Event
		{
		public:
			MouseMoveEvent(float mouseXpos, float mouseYpos)
				:m_MouseXPosition(mouseXpos), m_MouseYPosition(mouseYpos)
			{
				Event::m_EventType = EventType::MouseMoveEvent;
			}
		public:
			void Log() override { INFO("Mouse Move Event. Mouse Position: X: {0}, Y: {1}", m_MouseXPosition, m_MouseYPosition); }
			float GetMouseXPosition() { return m_MouseXPosition; }
			float GetMouseYPosition() { return m_MouseYPosition; }
		public:
			virtual ~MouseMoveEvent() override {}
		private:
			float m_MouseXPosition, m_MouseYPosition;
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API MouseScrollEvent : public Event
		{
		public:
			MouseScrollEvent(float mouseXpos, float mouseYpos)
				:m_MouseXOffset(mouseXpos), m_MouseYOffset(mouseYpos)
			{
				Event::m_EventType = EventType::MouseScrollEvent;
			}
		public:
			void Log() override { INFO("Mouse Scroll Event."); }
			float GetXOffset() { return m_MouseXOffset; }
			float GetYOffset() { return m_MouseYOffset; }
		private:
			float m_MouseXOffset, m_MouseYOffset;
		};
		//--------------------------------------------------------------------
		//--------------------------------------------------------------------
		class ANOR_API WindowResizeEvent : public Event
		{
		public:
			WindowResizeEvent(int width, int height)
				:m_Width(width), m_Height(height)
			{
				Event::m_EventType = EventType::WindowResizeEvent;
			}
		public:
			void Log() override { INFO("Window Resize Event. Width: {0}, Height: {1}", m_Width, m_Height); }
			int GetWidth() { return m_Width; }
			int GetHeight() { return m_Height; }
		private:
			int m_Width, m_Height;
		};
	}
}