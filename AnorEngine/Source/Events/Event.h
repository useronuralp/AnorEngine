#pragma once
namespace AnorEngine
{
	namespace Input
	{	
		enum class ANOR_API EventType
		{
			None = 0, KeyPressEvent, KeyReleaseEvent,
			MouseClickEvent, MouseReleaseEvent, MouseMoveEvent
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
				:m_MouseXOffset(mouseXpos), m_MouseYOffset(mouseYpos)
			{
				Event::m_EventType = EventType::MouseMoveEvent;
			}
		public:
			void Log() override { INFO("Mouse Move Event. Mouse Position: X: {0}, Y: {1}", m_MouseXOffset, m_MouseYOffset); }
			int GetMouseXOffset() { return m_MouseXOffset; }
			int GetMouseYOffset() { return m_MouseYOffset; }
		public:
			virtual ~MouseMoveEvent() override {}
		private:
			float m_MouseXOffset, m_MouseYOffset;
		};
	}
}