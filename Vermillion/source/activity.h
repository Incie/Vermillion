#pragma once

#include"servicelocator.h"
#include"uiview.h"
#include"windowstate.h"

class Activity {
public:
	virtual ~Activity() = default;

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;

	virtual void Resize(const glm::ivec2& newWindowSize);
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

	void SetServiceLocator(ServiceLocator& locator);
protected:
	void AddView(UIView* view, int id);
	bool UpdateUI(float deltaTime);
	void RenderUI();
	void DeinitializeUI();
	UIView* GetViewById(int id);

	template<class T>  T* GetViewById(int id) {
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