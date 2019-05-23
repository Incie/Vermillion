#include"pch.h"
#include"../servicelocator.h"
#include "InitiativeUI.h"
#include"../director/Initiative.h"


InitiativeTrackerUI::InitiativeTrackerUI(InitiativeTracker& initiativeTracker)
	: initiativeTracker(initiativeTracker)
{
	SetAnchor(WindowAnchor::TOP|WindowAnchor::RIGHT);
}

InitiativeTrackerUI::~InitiativeTrackerUI()
{
}

bool InitiativeTrackerUI::HandleInput(const InputService& inputService)
{
	return false;
}

void InitiativeTrackerUI::Resize(const glm::vec2& windowSize)
{
	UILayer::Resize(windowSize);
}

void InitiativeTrackerUI::Measure(const glm::vec2& dimensions)
{
	int count = initiativeTracker.Count();

	if( count == 0 )
		count = 1;

	size.x = 200;
	size.y = 16 + count * 20 + (count) * 4;
}

void InitiativeTrackerUI::Update()
{
}

#include"GL/glew.h"
#include"../render.h"
void InitiativeTrackerUI::Render(ServiceLocator& Services)
{
	Measure(glm::vec2());
	UILayer::Render(Services);

	glTranslatef(8, 8, 0);
	for( int i = 0; i < initiativeTracker.Count(); ++i ){
		Render::Quad(0,0, size.x - 16.0f, 20, glm::vec3(0.88f));
		Services.Text().Print(0,-2.0f, fmt::format("{0}", i), 20, Colorf(0), false, true);
		glTranslatef(0,4,0);
	}
}
