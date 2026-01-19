#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Resources/Types/Texture.h"
#include "Loopie/Resources/Types/Material.h"


namespace Loopie {

    class GUICanvas : public Component {
    public:

        DEFINE_TYPE(GUICanvas)

        enum class RenderMode {
            OVERLAY,
            CAMERA,
            WORLD_SPACE
        };

		GUICanvas();
		~GUICanvas();

		// Getters
		RenderMode GetRenderMode() const { return mode; }
		vec2 GetReferenceResolution() const { return referenceResolution; }
        std::shared_ptr<Material> GetMaterial() const { return material; }

		// Setters
		void SetRenderMode(RenderMode _mode) { mode = _mode; }
		void SetReferenceResolution(const vec2& resolution) { referenceResolution = resolution; }
		void SetMaterial(std::shared_ptr<Material> mat) { material = mat; }


        //Overrides
        void Init() override;
        JsonNode Serialize(JsonNode& parent) const override;
        void Deserialize(const JsonNode& data) override;


    private:

        RenderMode mode = RenderMode::OVERLAY;
        std::shared_ptr<Material> material = nullptr;
        vec2 referenceResolution = vec2(1920, 1080);
    };

}
