#pragma once

#include"servicelocator.h"
#include"uiview.h"
#include"windowstate.h"

class Activity {
public:
	virtual ~Activity() = default;

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;

	virtual void Resize(const glm::ivec2& newWindowSize) {
		for(auto view : layers)
			view->Resize(newWindowSize, Services().Text());
	}
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

	void SetServiceLocator(ServiceLocator& locator) {
		serviceLocator = &locator;
	}
protected:
	void AddView(UIView* view, int id) {
		view->SetId(id);
		layers.push_back(view);
	}


	bool UpdateUI(float deltaTime) {
		bool inputHandled = false;
		for(auto layer : layers) {
			layer->Update(deltaTime, Services());

			if(layer->Active() == false)
				continue;

			if(layer->Invalidated()) {
				auto p = WindowState::Size();
				layer->Resize(glm::vec2(p.x, p.y), Services().Text());
			}

			if(!inputHandled)
				inputHandled = layer->HandleInput(Services().Input());
		}

		return inputHandled;
	}

	void RenderUI() {
		for(auto view : layers) {
			if(view->Active() == false)
				continue;
			view->StartRender();
			view->Render(Services());
			view->EndRender();
		}
	}

	void DeinitializeUI() {
		for(auto view : layers) {
			delete view;
		}

		layers.clear();
	}


	UIView* GetViewById(int id) {
		for(auto view : layers) {
			if(view->Id() == id)
				return view;
		}

		return nullptr;
	}

	template<class T> 
	T* GetViewById(int id) {
		for(auto view : layers) {
			if(view->Id() == id) {
				auto v = dynamic_cast<T*>(view);
				
				if(v != nullptr)
					return v;
			}
		}

		return nullptr;
	}


	ServiceLocator& Services() { return *serviceLocator; }

private:
	std::vector<UIView*> layers;
	ServiceLocator *serviceLocator;
};