#pragma once
#include "Loopie/Components/Component.h"
#include "Loopie/Math/MathTypes.h"
#include "Loopie/Scene/Entity.h"
#include "Loopie/Events/IObserver.h"
#include "Loopie/Events/EventTypes.h"

namespace Loopie {

	class RectTransform : public Component, public IObserver<TransformNotification>
	{
	public:
		DEFINE_TYPE(RectTransform)

		RectTransform() = default;
		virtual ~RectTransform() = default;

		void Init() override;
		JsonNode Serialize(JsonNode& parent) const override;
		void Deserialize(const JsonNode& data) override;

		void CalculateRect();
		bool IsMouseInside(float mouseX, float mouseY) const;

		const vec2 GetAnchorMin() const;
		const vec2 GetAnchorMax() const;
		const vec2 GetPivot() const;
		const vec2 GetSizeDelta() const;
		const vec2 GetAnchoredPosition() const;

		void SetAnchorMin(vec2 anchMin);
		void SetAnchorMax(vec2 anchMax);
		void SetPivot(vec2 pivot);
		void SetSizeDelta(vec2 sizeDelta);
		void SetAnchoredPosition(vec2 anchPos);
		void SetWidth(float width);
		void SetHeight(float height);
		void SetLocalPosition(const vec3& position);
		void SetLocalScale(const vec3& scale);
		void SetLocalRotation(const quaternion& rotation);

		void DrawDebugBorders() const;

		// Implementar el método del Observer
		void OnNotify(const TransformNotification& notification) override;

		void SyncRectToTransform();

	public:
		vec2 anchorMin = { 0.0f, 0.0f };
		vec2 anchorMax = { 0.0f, 0.0f };
		vec2 pivot = { 0.0f, 0.0f };
		vec2 anchoredPosition = { 0.0f, 0.0f };
		vec2 sizeDelta = { 10.0f, 10.0f };

		float screenX = 0;
		float screenY = 0;
		float width = 0;
		float height = 0;

		vec3 localPosition;
		vec3 localScale;
		quaternion localRotation;

		bool draggable = false;
		bool invisible = false;
		bool cut_childs = false;
		bool interactive = true;

	private:
		void GetParentDimensions(float& outW, float& outH, float& outX, float& outY);
		void SyncWithTransform();
		
		// Flag para prevenir recursión infinita
		bool m_syncingWithTransform = false;
	};
}

