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

void InitiativeTrackerUI::Resize(const glm::vec2& windowSize, const TextService& text)
{
	UIView::Resize(windowSize, text);
}

void InitiativeTrackerUI::Measure(const glm::vec2& dimensions, const TextService& text)
{
	int count = initiativeTracker.Count();

	if( count == 0 )
		count = 1;

	size.x = 200;
	size.y = 16 + (float)count * 20 + (float)count * 4;
}

void InitiativeTrackerUI::Update()
{
}

#include"GL/glew.h"
#include"../render.h"
#include"../entity/Entity.h"
void InitiativeTrackerUI::Render(ServiceLocator& Services)
{
	Measure(glm::vec2(), Services.Text());
	UIView::Render(Services);

	auto initiatives = initiativeTracker.Initiatives();
	auto currentTurn = initiativeTracker.CurrentInitiativeIndex();
	glTranslatef(8, 8, 0);
	for( int i = 0; i < initiativeTracker.Count(); ++i ){
		auto color = glm::vec3(0.68f);
		if (i == currentTurn)
			color = glm::vec3(0.78f, 0.93f, 0.78f);

		auto a = initiatives[i];
		if(a.first->HasMoved())
			color = glm::vec3(1, 0.5f, 0.5f);

		Render::Quad(0,0, size.x - 16.0f, 20, color);

		std::string name;

		auto player = dynamic_cast<Player*>(a.first);
		auto enemy = dynamic_cast<Enemy*>(a.first);
		if(player != nullptr) {
			name = player->PlayerName();
		}
		else if(enemy != nullptr) {
			name = fmt::format("{0} [{1}]", enemy->Name(), enemy->EnemyId() );
		}
		else {
			name = "unknown";
			throw "InitiativeTrackerUI did not find actor type";
		}


		Services.Text().Print(0,-2.0f, fmt::format("{0} {1}", a.first->Initiative(), name), 20, Colors::Black, false, true);
		glTranslatef(0,4,0);
	}
}
