#include"pch/pch.h"
#include"activitymapeditor.h"
#include"render.h"

#include"ui/uiview.h"
#include"ui/uiview_vertical.h"
#include"ui/uibutton.h"

#include"tilewindow.h"

glm::vec2 ToGrid(const glm::vec2& point) {
	return {floorf(point.x / 50.0f), floorf(point.y / 50.0f)};
}

ActivityMapEditor::ActivityMapEditor()
{
	highlight = glm::vec2{};
	mousepos = glm::vec2{};
	camera.SetPositionCenter({0,0});
}

ActivityMapEditor::~ActivityMapEditor()
{
}

void ActivityMapEditor::Initialize()
{
	//AddView<UITileWindow>(0);
	//auto view = GetViewById<UITileWindow>(0);
	//view->SetCallback([this](int id) {
	//	auto v = this->GetViewById<VerticalWindow>(1);
	//});


	//AddView<VerticalWindow>(1);
	//auto view = GetViewById<VerticalWindow>(1);

	//view->SetAnchor(UIView::WindowAnchor::RIGHT | UIView::WindowAnchor::CENTER_VERTICAL);
	//auto button = view->AddChild<UIButton>();
	//button->SetId(11);
	//button->SetSize(10, 20);
	//button->SetText("Meow");
}

void ActivityMapEditor::Deinitialize()
{
	DeinitializeUI();
}

void ActivityMapEditor::Update(float delta)
{
	auto& input = Services().Input();

	auto inputHandled = UpdateUI(delta);

	mousepos = camera.ScreenToViewCoords(input.GetMousePositionNormalized());
	if(inputHandled) {
		highlight = {999999, 999999};
	}

	if(!inputHandled) {
		highlight = ToGrid(mousepos);

		if(input.KeyOnce(VKey_RETURN)) {
			level.AddPage();
		}

		if(input.KeyDown(VKey_LBUTTON) && input.KeyDown(VKey_SHIFT)) {
			level.RemoveTileAt(highlight);
		}
		else if(input.KeyDown(VKey_LBUTTON)) {
			level.AddTileAt(highlight);
		}

		if(input.KeyOnce(VKey_INSERT)) {
			level.AddPage();
		}

		if(input.KeyOnce(VKey_DELETE)) {

		}

		if(input.KeyDown(VKey_RBUTTON)) {
			camera.Move(input.GetMouseDelta());
		}

		if(input.ScrollState() < 0) {
			camera.ZoomByFactor(std::powf(1.1f, static_cast<float>(input.ScrollState() * -1)));
		}

		if(input.ScrollState() > 0) {
			camera.ZoomByFactor(std::powf(0.9f, static_cast<float>(input.ScrollState())));
		}

		if(input.KeyOnce(VKey_SUBTRACT)) {
			camera.ZoomByFactor(1.1f);
		}

		if(input.KeyOnce(VKey_ADD)) {
			camera.ZoomByFactor(0.9f);
		}
	}
}

#include"GL/glew.h"
void ActivityMapEditor::Render()
{
	camera.Push();
		glColor3f(1, 1, 1);
		level.Render();

		Render::Quad(highlight * 50.0f, {50,50}, Colors::Red); glDisable(GL_TEXTURE_2D);
		Render::Translate3D(glm::vec3{0,0,1});
		Render::LineGrid(ToGrid(camera.GetTopLeft() + glm::vec2{1.0f, 1.0f}) * 50.0f, camera.GetBottomRight(), {0.3f, 0.3f, 0.3f}, 50.0f);

	camera.Pop();

	auto& text = Services().Text();
	text.Print(15, 15, fmt::format("h[{},{}] {}", highlight.x, highlight.y, level.TileCount()), 15, Colors::White);

	RenderUI();
}
