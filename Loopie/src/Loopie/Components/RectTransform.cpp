#include "RectTransform.h"
#include "Canvas.h"
#include "CanvasScaler.h"
#include "Loopie/Core/Application.h"
#include "Transform.h"

void Loopie::RectTransform::UpdateMatrix(const vec2& parentSize, float scaleFactor, Canvas* root)
{
	// Scale to dimensions desired
	float currentWidth = Width * scaleFactor;
	float currentHeight = Height * scaleFactor;

	// Calculate area between anchors in pixels
	float anchorLeft = parentSize.x * AnchorMin.x;
	float anchorRight = parentSize.x * AnchorMax.x;
	float anchorBottom = parentSize.y * AnchorMin.y;
	float anchorTop = parentSize.y * AnchorMax.y;

	// Determin base position
	vec2 anchorCenter = vec2((anchorLeft + anchorRight) * 0.5f,
							 (anchorBottom + anchorTop) * 0.5f);

	// Final position at world space
	vec3 finalPos = vec3(anchorCenter.x + (AnchoredPosition.x * scaleFactor),
						 anchorCenter.y + (AnchoredPosition.y * scaleFactor),
						 AnchoredPosition.z);

	if (root && root->PixelPerfect)
	{
		finalPos.x = std::round(finalPos.x);
		finalPos.y = std::round(finalPos.y);
	}

	// Matrix Calculations (T * R * S)
	matrix4 translation = translate(matrix4(1.0f), finalPos);
	matrix4 rotation = toMat4(quaternion(radians(Rotation)));
	matrix4 scale = glm::scale(matrix4(1.0f), vec3(currentWidth * Scale.x, currentHeight * Scale.y, 1.0f));
	
	// Apply pivot offset
	vec3 pivotOffset = vec3((Pivot.x - 0.5f) * currentWidth, (Pivot.y - 0.5f) * currentHeight, 0.0f);
	matrix4 pivotTransform = glm::translate(matrix4(1.0f), -pivotOffset);

	// Final Matrix
	m_LocalToWorldMatrix = translation * rotation * scale * pivotTransform;
	m_CalculatedSize = vec2(currentWidth, currentHeight);

	if (auto transform = GetOwner()->GetTransform())
	{
		transform->SetLocalPosition(finalPos);
		transform->SetLocalRotation(quaternion(radians(Rotation)));
		transform->SetLocalScale(vec3(currentWidth * Scale.x, currentHeight * Scale.y, 1.0f));
	}

	// Recursiveley update children
	for (auto& child : GetOwner()->GetChildren())
	{
		if (!child) continue;
		if (auto rt = child->GetComponent<RectTransform>()) {
			rt->UpdateMatrix(m_CalculatedSize, scaleFactor, root);
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
		scaleFactor = scaler->GetScaleFactor();
	}

	vec2 pSize = GetParentSize();

	UpdateMatrix(pSize, scaleFactor, root);
}

Loopie::JsonNode Loopie::RectTransform::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("rectTransform");

	// Position and size
	JsonNode pos = node.CreateObjectField("anchoredPosition");
	pos.CreateField("x", AnchoredPosition.x);
	pos.CreateField("y", AnchoredPosition.y);
	pos.CreateField("z", AnchoredPosition.z);

	node.CreateField("width", Width);
	node.CreateField("height", Height);

	// Anchors and Pivot
	JsonNode ancMin = node.CreateObjectField("anchorMin");
	ancMin.CreateField("x", AnchorMin.x); 
	ancMin.CreateField("y", AnchorMin.y);

	JsonNode ancMax = node.CreateObjectField("anchorMax");
	ancMax.CreateField("x", AnchorMax.x); 
	ancMax.CreateField("y", AnchorMax.y);

	JsonNode piv = node.CreateObjectField("pivot");
	piv.CreateField("x", Pivot.x); 
	piv.CreateField("y", Pivot.y);

	// Rotation transform
	JsonNode rot = node.CreateObjectField("rotation");
	rot.CreateField("x", Rotation.x); 
	rot.CreateField("y", Rotation.y); 
	rot.CreateField("z", Rotation.z);

	// Scale transform
	JsonNode scale = node.CreateObjectField("scale");
	scale.CreateField("x", Scale.x); 
	scale.CreateField("y", Scale.y); 
	scale.CreateField("z", Scale.z);

	return node;
}

void Loopie::RectTransform::Deserialize(const JsonNode& data)
{
	JsonNode node = data.Child("rectTransform");
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
	
	// Anchors and Pivot
	JsonNode ancMin = node.Child("anchorMin");
	if (ancMin.IsValid()) {
		AnchorMin.x = ancMin.GetValue<float>("x", 0.0f).Result;
		AnchorMin.y = ancMin.GetValue<float>("y", 0.0f).Result;
	}
	JsonNode ancMax = node.Child("anchorMax");
	if (ancMax.IsValid()) {
		AnchorMax.x = ancMax.GetValue<float>("x", 0.0f).Result;
		AnchorMax.y = ancMax.GetValue<float>("y", 0.0f).Result;
	}
	JsonNode piv = node.Child("pivot");
	if (piv.IsValid()) {
		Pivot.x = piv.GetValue<float>("x", 0.5f).Result;
		Pivot.y = piv.GetValue<float>("y", 0.5f).Result;
	}
	
	// Rotation transform
	JsonNode rot = node.Child("rotation");
	if (rot.IsValid()) {
		Rotation.x = rot.GetValue<float>("x", 0.0f).Result;
		Rotation.y = rot.GetValue<float>("y", 0.0f).Result;
		Rotation.z = rot.GetValue<float>("z", 0.0f).Result;
	}
	
	// Scale transform
	JsonNode scale = node.Child("scale");
	if (scale.IsValid()) {
		Scale.x = scale.GetValue<float>("x", 1.0f).Result;
		Scale.y = scale.GetValue<float>("y", 1.0f).Result;
		Scale.z = scale.GetValue<float>("z", 1.0f).Result;
	}
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

Loopie::vec2 Loopie::RectTransform::GetParentSize()
{
	if (auto parent = GetOwner()->GetParent().lock())
	{
		if (auto parentRT = parent->GetComponent<RectTransform>())
		{
			return parentRT->GetSize();
		}

		if (auto canvas = parent->GetComponent<Canvas>()) 
		{
			return canvas->GetCanvasSize();
		}
	}
	return vec2(0.0f, 0.0f);
}
