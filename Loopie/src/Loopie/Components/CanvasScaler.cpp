#include "CanvasScaler.h"
#include "Loopie/Core/Application.h"

Loopie::JsonNode Loopie::CanvasScaler::Serialize(JsonNode& parent) const
{
	JsonNode node = parent.CreateObjectField("canvasscaler");
	node.CreateField("scaleMode", (int)Mode);
	node.CreateField("matchWidthOrHeight", MatchWidthOrHeight);

	JsonNode refRes = node.CreateObjectField("referenceResolution");
	refRes.CreateField("x", ReferenceResolution.x);
	refRes.CreateField("y", ReferenceResolution.y);
	
	return node;
}

void Loopie::CanvasScaler::Deserialize(const JsonNode& data)
{
	JsonNode node = data.Child("canvasscaler");
	if (!node.IsValid()) return;

	Mode = (ScaleMode)node.GetValue<int>("scaleMode", (int)ScaleMode::ScaleWithScreenSize).Result;
	MatchWidthOrHeight = node.GetValue<float>("matchWidthOrHeight", 0.5f).Result;

	JsonNode refRes = node.Child("referenceResolution");
	if (refRes.IsValid()) {
		ReferenceResolution.x = refRes.GetValue<float>("x", 1920.0f).Result;
		ReferenceResolution.y = refRes.GetValue<float>("y", 1080.0f).Result;
	}
}
