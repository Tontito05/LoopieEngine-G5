#pragma once
#include "Component.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Resources/Types/Texture.h"
#include "Loopie/Resources/Types/Material.h"
#include "Loopie/Render/VertexArray.h"

namespace Loopie
{
	class Image : public Component
	{
	public:
		DEFINE_TYPE(Image)

		Image() = default;

		vec4 Color = vec4(1.0f, 1.0f, 255.0f, 1.0f);
		std::shared_ptr<Texture> Sprite = nullptr;
		std::string pathSprite = "assets/icons/defaultTexture.png";

		bool UseTexture = true;

		virtual void Init() override;

		void SetSprite(const std::shared_ptr<Texture>& texture) { Sprite = texture; }
		void SetSprite(const std::string& path);
		void SetColor(const vec4& color) { Color = color; }

		std::shared_ptr<Material> GetMaterial() const { return m_material; }

		virtual JsonNode Serialize(JsonNode& parent) const override;
		virtual void Deserialize(const JsonNode& data) override;

	private:
		std::shared_ptr<Material> m_material;
	};
}