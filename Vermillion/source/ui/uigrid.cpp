#include"pch.h"
#include "uigrid.h"

UIGrid::UIGrid()
    : rowCount(1), columnCount(1)
{
}

UIGrid::~UIGrid()
{
}

constexpr float widgetSize = 50.0f;

void UIGrid::Measure(const glm::vec2& windowSize, const TextService& text)
{
    auto childCount = children.size();

    columnCount = 5;
    if(columnCount == 0)
        columnCount = 1;

    rowCount = 1 + (static_cast<int>(children.size()) / columnCount);

    SetSize(static_cast<float>(columnCount) * (widgetSize + 2.0f), static_cast<float>(rowCount) * (widgetSize + 2.0f));
}

void UIGrid::Resize(const glm::vec2& windowSize, const TextService& text)
{
    Measure(windowSize, text);

    auto coord = glm::vec2{0,0};
    int row = 0;
    int col = 0;

    for(auto child : children) {
        child->SetSize(widgetSize, widgetSize);
        child->SetPosition(coord);

        coord.x += widgetSize + 2.0f;
        col++;

        if(col == columnCount) {
            row++;
            col = 0;
            coord.x = 0.0f;
            coord.y += widgetSize + 2.0f;;
        }
    }
}


void UIGrid::Render(ServiceLocator& Services)
{
    this->StartRender();
    for(auto child : children)
        child->Render(Services);
    this->EndRender();
}
