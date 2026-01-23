#include "Editor/Interfaces/Interface.h"
#include "Loopie/Render/FrameBuffer.h"

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

		Camera* GetCamera();

	private:
		bool m_visible = false;
		std::shared_ptr<FrameBuffer> m_buffer;
		ivec2 m_windowSize = ivec2(0);
	};
}