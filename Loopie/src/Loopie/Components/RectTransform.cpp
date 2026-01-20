#include "RectTransform.h"
#include "Loopie/Components/Transform.h"
#include "Loopie/Core/Application.h"
#include "Loopie/Components/MeshRenderer.h"

using namespace Loopie;

void RectTransform::GetParentDimensions(float& outW, float& outH, float& outX, float& outY)
{
    auto owner = GetOwner();
    if (!owner) return;

    std::shared_ptr<Entity> parent = owner->GetParent().lock();
    RectTransform* parentRect = parent ? parent->GetComponent<RectTransform>() : nullptr;

    if (parentRect)
    {
        outW = parentRect->width;
        outH = parentRect->height;
        outX = parentRect->screenX;
        outY = parentRect->screenY;
    }
    else
    {
        auto size = Application::GetInstance().GetWindow().GetSize();
        outW = (float)size.x;
        outH = (float)size.y;
        outX = 0.0f;
        outY = 0.0f;
    }
}

void RectTransform::Init()
{
    CalculateRect();

    Transform* transform = GetTransform();
    if (transform)
    {
        transform->m_transformNotifier.AddObserver(this);
		transform->SetEulerAngles(vec3(90.0f,0.0f,0.0f));
    }
}

void RectTransform::OnNotify(const TransformNotification& notification)
{

}

JsonNode RectTransform::Serialize(JsonNode& parent) const
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

    node = rectTransformObj.CreateObjectField("transform");
    node.CreateField("posX", localPosition.x);
    node.CreateField("posY", localPosition.y);
    node.CreateField("posZ", localPosition.z);
    node.CreateField("scaleX", localScale.x);
    node.CreateField("scaleY", localScale.y);
    node.CreateField("scaleZ", localScale.z);
    node.CreateField("rotX", localRotation.x);
    node.CreateField("rotY", localRotation.y);
    node.CreateField("rotZ", localRotation.z);
    node.CreateField("rotW", localRotation.w);

    return rectTransformObj;
}

void RectTransform::Deserialize(const JsonNode& data)
{
    JsonNode node = data.Child("properties");
    if (node.IsValid())
    {
        anchorMin.x = node.GetValue<float>("anchorMinX", anchorMin.x).Result;
        anchorMin.y = node.GetValue<float>("anchorMinY", anchorMin.y).Result;
        anchorMax.x = node.GetValue<float>("anchorMaxX", anchorMax.x).Result;
        anchorMax.y = node.GetValue<float>("anchorMaxY", anchorMax.y).Result;
        pivot.x = node.GetValue<float>("pivotX", pivot.x).Result;
        pivot.y = node.GetValue<float>("pivotY", pivot.y).Result;
        anchoredPosition.x = node.GetValue<float>("anchoredPositionX", anchoredPosition.x).Result;
        anchoredPosition.y = node.GetValue<float>("anchoredPositionY", anchoredPosition.y).Result;
        sizeDelta.x = node.GetValue<float>("sizeDeltaX", sizeDelta.x).Result;
        sizeDelta.y = node.GetValue<float>("sizeDeltaY", sizeDelta.y).Result;
    }

    node = data.Child("transform");
    if (node.IsValid())
    {
        localPosition.x = node.GetValue<float>("posX", localPosition.x).Result;
        localPosition.y = node.GetValue<float>("posY", localPosition.y).Result;
        localPosition.z = node.GetValue<float>("posZ", localPosition.z).Result;

        localScale.x = node.GetValue<float>("scaleX", localScale.x).Result;
        localScale.y = node.GetValue<float>("scaleY", localScale.y).Result;
        localScale.z = node.GetValue<float>("scaleZ", localScale.z).Result;

        localRotation.x = node.GetValue<float>("rotX", localRotation.x).Result;
        localRotation.y = node.GetValue<float>("rotY", localRotation.y).Result;
        localRotation.z = node.GetValue<float>("rotZ", localRotation.z).Result;
        localRotation.w = node.GetValue<float>("rotW", localRotation.w).Result;
    }

    CalculateRect();
}

void RectTransform::CalculateRect()
{
    float pW, pH, pX, pY;
    GetParentDimensions(pW, pH, pX, pY);

    // Anchor region
    float anchorLeft = pX + pW * anchorMin.x;
    float anchorBottom = pY + pH * anchorMin.y;
    float anchorRight = pX + pW * anchorMax.x;
    float anchorTop = pY + pH * anchorMax.y;

    float anchorWidth = anchorRight - anchorLeft;
    float anchorHeight = anchorTop - anchorBottom;

    // Final size
    width = anchorWidth + sizeDelta.x;
    height = anchorHeight + sizeDelta.y;

    // Bottom-left screen position
    screenX = anchorLeft + anchoredPosition.x - width * pivot.x;
    screenY = anchorBottom + anchoredPosition.y - height * pivot.y;

    SyncRectToTransform();
}


void RectTransform::SyncRectToTransform()
{
    Transform* t = GetTransform();
    if (!t) return;

    // UI space → local transform
    vec3 pos;
    pos.x = screenX + width * pivot.x;
    pos.y = screenY + height * pivot.y;
    pos.z = 0.0f;

    t->SetLocalPosition(pos);
    t->SetLocalScale({ width, 0.1f , height });
}

bool RectTransform::IsMouseInside(float mouseX, float mouseY) const
{
    if (!GetIsActive() || invisible || !interactive)
        return false;

    return mouseX >= screenX && mouseX <= screenX + width &&
        mouseY >= screenY && mouseY <= screenY + height;
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
	CalculateRect();
}

void Loopie::RectTransform::SetAnchorMax(Loopie::vec2 anchMax)
{
	this->anchorMax = anchMax;
	CalculateRect();
}

void Loopie::RectTransform::SetPivot(Loopie::vec2 pivot) 
{
	this->pivot = pivot;
	CalculateRect();
}

void Loopie::RectTransform::SetSizeDelta(Loopie::vec2 sizeDelta) 
{
	this->sizeDelta = sizeDelta;
	CalculateRect();
}

void Loopie::RectTransform::SetAnchoredPosition(Loopie::vec2 anchPos)
{
	this->anchoredPosition = anchPos;
	CalculateRect();
}

void Loopie::RectTransform::SetWidth(float width_)
{
    width = width_;
    sizeDelta.x = width_ - (sizeDelta.x + (width_ - width));
    CalculateRect();
}
void Loopie::RectTransform::SetHeight(float height_)
{
    height = height_;
    sizeDelta.y = height_ - (sizeDelta.y + (height_ - height));
    CalculateRect();
}
void Loopie::RectTransform::SetLocalPosition(const Loopie::vec3& position)
{
	GetTransform()->SetLocalPosition(position);
}
void Loopie::RectTransform::SetLocalScale(const Loopie::vec3& scale)
{
	GetTransform()->SetLocalScale(scale);
}
void Loopie::RectTransform::SetLocalRotation(const Loopie::quaternion& rotation)
{
    GetTransform()->SetLocalRotation(rotation);
}

void RectTransform::DrawDebugBorders() const
{
	//TODO
}

