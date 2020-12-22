#include"pch/pch.h"
#include"tilewindow.h"
#include"ui/uitext.h"
#include"ui/uigrid.h"
#include"ui/uibutton.h"

UITileWindow::UITileWindow()
{
	SetAnchor(UIView::WindowAnchor::TOP | UIView::WindowAnchor::RIGHT);
	SetSize(150, 200);
	Invalidate();

	auto header = AddChild<UIText>();
	header->SetId(0);
	header->SetMargin(0, 4.0f);
	header->Text("Tiles");
	header->FontHeight(32.0f);

	auto tileGrid = AddChild<UIGrid>();

	int i = 0;
	while(i < 150) {
		auto b = tileGrid->AddChild<UIButton>();
		b->SetTextSize(8);
		b->SetText(fmt::format("{}", i));

		i++;
	}
}

UITileWindow::~UITileWindow()
{
}

void UITileWindow::Measure(const glm::vec2& windowSize, const TextService& text)
{
	VerticalWindow::Measure(windowSize, text);
}

void UITileWindow::Resize(const glm::vec2& windowSize, const TextService& text)
{
	VerticalWindow::Resize(windowSize, text);
}

void UITileWindow::SetTexture(Texture texture)
{
	this->texture = texture;
	Invalidate();
}
