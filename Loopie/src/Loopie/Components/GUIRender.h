#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Resources/Types/Mesh.h"
#include "Loopie/Resources/Types/Material.h"

namespace Loopie {

    class GUIRender : public Component {
    public:
        DEFINE_TYPE(GUIRender)

        GUIRender();
        virtual ~GUIRender();

        std::shared_ptr<Mesh> GetMesh() const { return uiMesh; }
        std::shared_ptr<Material> GetMaterial() const { return material; }
        void SetMaterial(std::shared_ptr<Material> mat) { material = mat; }

        void Init() override;

        //Functions
        void SetAsQuad();

		//Set up a simple quad mesh and material
        std::shared_ptr<Mesh> CreateQuad();
        static std::shared_ptr<Material> GetDefaultGUI();

    private:
        std::shared_ptr<Mesh> uiMesh = nullptr;
        std::shared_ptr<Material> material = nullptr;
    };
}