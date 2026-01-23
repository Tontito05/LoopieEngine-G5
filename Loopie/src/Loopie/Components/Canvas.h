#pragma once

#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Render/VertexArray.h"
#include "Loopie/Resources/Types/Material.h"

namespace Loopie
{
	enum class RenderMode
	{
		ScreenSpaceOverlay,
		WorldSpace
	};

	class Canvas : public Component
	{
	public:
		RenderMode Mode = RenderMode::ScreenSpaceOverlay;
		bool PixelPerfect = false;

		DEFINE_TYPE(Canvas)

		Canvas() = default;

		virtual void Init() override;

		// Getters
		vec2 GetCanvasSize() const;

		void UpdateUIHierarchy();

		std::shared_ptr<VertexArray> GetVAO() const { return m_QuadVAO; }
		void SetVAO(std::shared_ptr<VertexArray> vao);

		void NotifyHierarchyDirty(Entity* entity = nullptr);
		void SetPixelPerfect(bool enable);

		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;

	private:
		vec2 m_LastScreenSize{ 0, 0 };
		std::shared_ptr<VertexArray> m_QuadVAO;
		std::shared_ptr<Material> m_material;
		std::shared_ptr<Texture> Sprite = nullptr;
		std::string pathSprite = "assets/icons/canvas.png";
	};
}