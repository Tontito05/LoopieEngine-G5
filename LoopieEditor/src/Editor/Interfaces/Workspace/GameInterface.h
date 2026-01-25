#include "Editor/Interfaces/Interface.h"
#include "Loopie/Render/FrameBuffer.h"
#include "Loopie/Math/Ray.h"

namespace Loopie {
	class Camera;
	class GameInterface : public Interface {
	public:
		GameInterface();
		~GameInterface() = default;
		void Init() override {}
		void Render() override;

		void StartScene();
		void EndScene();

		ivec2 GetWindowSize() const { return m_windowSize; }

		bool IsVisible() { return m_visible; }
		bool IsFocused() const { return m_focused; }
		Ray MouseRay();

		void HandleButtonLogic();
		void HandleTextLogic();

		std::shared_ptr<Entity> MousePick();

		Camera* GetCamera();

	private:
		bool m_visible = false;
		bool m_focused = false;

		bool options_show_test_window = false;

		ivec2 m_windowSize = ivec2(0);
		ivec2 m_mousePosition = ivec2(0);

		std::shared_ptr<Entity> m_activeTextEntity = nullptr;

		std::shared_ptr<FrameBuffer> m_buffer;
		std::shared_ptr<Entity> m_entityUnderMouse = nullptr;
		std::shared_ptr<Entity> m_lastHoveredEntity = nullptr;
		std::shared_ptr<Entity> m_lastPressedEntity = nullptr;
	};
}