#include "RectTransform.h"
#include "Loopie/Core/Application.h"

void Loopie::RectTransform::GetParentDimensions(float& outW, float& outH, float& outX, float& outY)
{
	auto owner = GetOwner();
	if (!owner) return;

	std::shared_ptr<Entity> parent = owner->GetParent().lock();

	RectTransform* parentRect = nullptr;
	if (parent) {
		parentRect = parent->GetComponent<RectTransform>();
	}

	if (parentRect) {
		outW = parentRect->width;
		outH = parentRect->height;
		outX = parentRect->screenX;
		outY = parentRect->screenY;
	}
	else {
		outW = (float)Application::GetInstance().GetWindow().GetSize().x;
		outH = (float)Application::GetInstance().GetWindow().GetSize().y;
		outX = 0.0f;
		outY = 0.0f;;
	}
}

void Loopie::RectTransform::Init()
{
	CalculateRect();
}

Loopie::JsonNode Loopie::RectTransform::Serialize(Loopie::JsonNode& parent) const
{
	JsonNode rectTransformObj = parent.CreateObjectField("rect-transform");

	JsonNode node = rectTransformObj.CreateObjectField("properties");
	node.CreateField("anchorMinX", anchorMin.x);
	node.CreateField("anchorMinY", anchorMin.y);
	node.CreateField("anchorMaxX", anchorMax.x);
	node.CreateField("anchorMaxY", anchorMax.y);
	node.CreateField("pivotX", pivot.x);
	node.CreateField("pivotY", pivot.y);
	node.CreateField("anchoredPositionX", anchoredPosition.x);
	node.CreateField("anchoredPositionY", anchoredPosition.y);
	node.CreateField("sizeDeltaX", sizeDelta.x);
	node.CreateField("sizeDeltaY", sizeDelta.y);

	node = rectTransformObj.CreateObjectField("flags");
	node.CreateField("draggable", draggable);
	node.CreateField("invisible", invisible);
	node.CreateField("cut_childs", cut_childs);
	node.CreateField("interactive", interactive);

	return rectTransformObj;
}

void Loopie::RectTransform::Deserialize(const Loopie::JsonNode& data)
{
	JsonNode node = data.Child("properties");
	if (node.IsValid() && node.IsObject())
	{
		anchorMin.x = node.GetValue<float>("anchorMinX", anchorMin.x).Result;
		anchorMin.y = node.GetValue<float>("anchorMinY", anchorMin.y).Result;
		anchorMax.x = node.GetValue<float>("anchorMaxX", anchorMax.x).Result;
		anchorMax.y = node.GetValue<float>("anchorMaxY", anchorMax.y).Result;
		pivot.x = node.GetValue<float>("pivotX", pivot.x).Result;
		pivot.y = node.GetValue<float>("pivotY", pivot.y).Result;
		anchoredPosition.x = node.GetValue<float>("anchoredPositionX", anchoredPosition.y).Result;
		anchoredPosition.y = node.GetValue<float>("anchoredPositionY", anchoredPosition.y).Result;
		sizeDelta.x = node.GetValue<float>("sizeDeltaX", sizeDelta.x).Result;
		sizeDelta.y = node.GetValue<float>("sizeDeltaY", sizeDelta.y).Result;
	}

	node = data.Child("flags");
	if (node.IsValid() && node.IsObject())
	{
		draggable = node.GetValue<bool>("draggable", draggable).Result;
		invisible = node.GetValue<bool>("invisible", invisible).Result;
		cut_childs = node.GetValue<bool>("cut_childs", cut_childs).Result;
		interactive = node.GetValue<bool>("interactive", interactive).Result;
	}

	CalculateRect();
}

void Loopie::RectTransform::CalculateRect()
{
	float pW, pH, pX, pY;
	GetParentDimensions(pW, pH, pX, pY);

	float anchorLeft = pX + (pW * anchorMin.x);
	float anchorRight = pX + (pW * anchorMax.x);
	float anchorBottom = pY + (pH * anchorMin.y);
	float anchorTop = pY + (pH * anchorMax.y);

	width = (anchorRight - anchorLeft) + sizeDelta.x;
	height = (anchorTop - anchorBottom) + sizeDelta.y;

	screenX = anchorLeft + anchoredPosition.x - (width * pivot.x);
	screenY = anchorBottom + anchoredPosition.y - (height * pivot.y);
}

bool Loopie::RectTransform::IsMouseInside(float mouseX, float mouseY) const
{
	if (!GetIsActive() || invisible || !interactive) {
		return false;
	}
	return (mouseX >= screenX) && (mouseX <= screenX + width)
		&& (mouseY >= screenY) && (mouseY <= screenY + height);
}

const Loopie::vec2 Loopie::RectTransform::GetAnchorMin() const 
{
	return anchorMin;
}

const Loopie::vec2 Loopie::RectTransform::GetAnchorMax() const
{
	return anchorMax;
}

const Loopie::vec2 Loopie::RectTransform::GetAnchoredPosition() const
{
	return anchoredPosition;
}

const Loopie::vec2 Loopie::RectTransform::GetPivot() const
{
	return pivot;
}

const Loopie::vec2 Loopie::RectTransform::GetSizeDelta() const
{
	return sizeDelta;
}

void Loopie::RectTransform::SetAnchorMin(Loopie::vec2 anchMin)
{
	this->anchorMin = anchMin;
}

void Loopie::RectTransform::SetAnchorMax(Loopie::vec2 anchMax)
{
	this->anchorMax = anchMax;
}

void Loopie::RectTransform::SetPivot(Loopie::vec2 pivot) 
{
	this->pivot = pivot;
}

void Loopie::RectTransform::SetSizeDelta(Loopie::vec2 sizeDelta) 
{
	this->sizeDelta = sizeDelta;
}

void Loopie::RectTransform::SetAnchoredPosition(Loopie::vec2 anchPos)
{
	this->anchoredPosition = anchPos;
}

void Loopie::RectTransform::DrawDebugBorders() const
{

}
