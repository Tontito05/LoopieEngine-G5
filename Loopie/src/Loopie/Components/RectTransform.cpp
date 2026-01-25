#include "RectTransform.h"
#include "Canvas.h"
#include "CanvasScaler.h"
#include "Loopie/Core/Application.h"
#include "Transform.h"

void Loopie::RectTransform::UpdateMatrix(const vec2& parentSize, float scaleFactor, Canvas* root)
{
	float finalX = parentSize.x + (AnchoredPosition.x * scaleFactor);
	float finalY = parentSize.y + (AnchoredPosition.y * scaleFactor);

	float finalW = Width * scaleFactor;
	float finalH = Height * scaleFactor;

	if (root && root->PixelPerfect)
	{
		finalX = std::round(finalX);
		finalY = std::round(finalY);
	}

	vec3 pivotOffset(
		-(Pivot.x * finalW),
		-(Pivot.y * finalH),
		0.0f
	);
	matrix4 pivotTranslation = translate(matrix4(1.0f), pivotOffset);

	// Matrix Calculations (T * R * S)
	matrix4 rotation = toMat4(GetOwner()->GetTransform()->GetLocalRotation());
	matrix4 scale = glm::scale(matrix4(1.0f), vec3(finalW, finalH, 1.0f));

	matrix4 visualTRS = rotation * scale;
	matrix4 translation = translate(matrix4(1.0f), vec3(finalX, finalY, 0.0f));
	
	// Final Matrix
	m_LocalToWorldMatrix = translation * visualTRS * pivotTranslation;

	if (auto transform = GetOwner()->GetTransform())
	{
		transform->SetWorldPosition(vec3(finalX,finalY,transform->GetLocalPosition().z));
		transform->SetLocalScale(vec3(finalW, finalH, 1.0f));
	}

	// Recursiveley update children
	vec2 currentPos = vec2(finalX, finalY);
	for (auto& child : GetOwner()->GetChildren())
	{
		if (!child) continue;
		if (auto rt = child->GetComponent<RectTransform>()) {
			rt->UpdateMatrix(currentPos, scaleFactor, root);
		}
	}

	m_Dirty = false;
}

void Loopie::RectTransform::RefreshMatrix()
{
	Canvas* root = FindRootCanvas();
	if (!root) return;

	float scaleFactor = 1.0f;
	if (auto scaler = root->GetOwner()->GetComponent<CanvasScaler>())
	{
		scaleFactor = scaler->m_scaleFactor;
	}

	vec2 pPos = vec2(0.0f, 0.0f);
	if(auto parent = GetOwner()->GetParent().lock())
	{
		if (auto parentRT = parent->GetComponent<RectTransform>())
		{
			pPos.x = parentRT->AnchoredPosition.x;
			pPos.y = parentRT->AnchoredPosition.y;
		}
	}

	UpdateMatrix(pPos, scaleFactor, root);
}

Loopie::JsonNode Loopie::RectTransform::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("recttransform");

	// Position and size
	JsonNode pos = node.CreateObjectField("anchoredPosition");
	pos.CreateField("x", AnchoredPosition.x);
	pos.CreateField("y", AnchoredPosition.y);
	pos.CreateField("z", AnchoredPosition.z);

	node.CreateField("width", Width);
	node.CreateField("height", Height);

	return node;
}

void Loopie::RectTransform::Deserialize(const JsonNode& data)
{
	JsonNode node = data.Child("recttransform");
	if (!node.IsValid()) return;
	
	// Position and size
	JsonNode pos = node.Child("anchoredPosition");
	if (pos.IsValid()) {
		AnchoredPosition.x = pos.GetValue<float>("x", 0.0f).Result;
		AnchoredPosition.y = pos.GetValue<float>("y", 0.0f).Result;
		AnchoredPosition.z = pos.GetValue<float>("z", 0.0f).Result;
	}
	Width = node.GetValue<float>("width", 100.0f).Result;
	Height = node.GetValue<float>("height", 100.0f).Result;
}

Loopie::Canvas* Loopie::RectTransform::FindRootCanvas()
{
	if (auto canvas = GetOwner()->GetComponent<Canvas>())
		return canvas;

	Entity* current = GetOwner()->GetParent().lock().get();
	while (current)
	{
		if (auto canvas = current->GetComponent<Canvas>())
			return canvas;
		current = current->GetParent().lock().get();
	}
	return nullptr;
}
