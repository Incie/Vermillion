#include "testwindow.h"
#include"pch/pch.h"

TestWindow::TestWindow()
{
	auto group = AddView<LayoutHorizontal>();

	{
		auto subgroup = group->AddView<LayoutVertical>(-1);
		auto button = subgroup->AddView<Button>(0);
		button->Size({0, 30.0f});
		button->Text("Button0");

		int id = 1;
		for(auto i : {1,2,3,4,5,6,7}) {
			button = subgroup->AddView<Button>(id++);
			button->Size({0, 30.0f});
			button->Text(fmt::format("Button{}", id));
		}
	}

	{
		auto subgroup = group->AddView<LayoutVertical>(-1);
		auto button = subgroup->AddView<Button>(0);
		button->Size({0, 30.0f});
		button->Text("Button0");

		int id = 1;
		for(auto i : {1,2,3,4,5,6,7}) {
			button = subgroup->AddView<Button>(id++);
			button->Size({0, 30.0f});
			button->Text(fmt::format("Button{}", id));
		}
	}
}

TestWindow::~TestWindow()
{
}
