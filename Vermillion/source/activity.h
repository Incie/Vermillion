#pragma once

#include"servicelocator.h"
#include"ui/basewindow.h"
#include"windowstate.h"
#include<memory>

class ActivityInterface;

class Activity {
public:
	Activity();
	virtual ~Activity() = default;

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;

	virtual void Resize(const glm::ivec2& newWindowSize);
	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

	void SetServiceLocator(ServiceLocator& locator);
	void SetActivityInterface(ActivityInterface* activityInterface);

	bool Finished() const;
protected:
	void Finish();

	void AddView(BaseWindow* view, int id);

	template<class T = BaseWindow> 
	std::shared_ptr<BaseWindow> AddView(int id) {
		auto view = std::make_shared<T>();
		view->Id(id);
		windows.push_back(std::dynamic_pointer_cast<BaseWindow>(view));
		return view;
	}

	bool UpdateUI(float deltaTime);
	void RenderUI();
	void DeinitializeUI();
	std::shared_ptr<BaseWindow> GetViewById(int id);

	template<class T>  
	std::shared_ptr<T> GetViewById(int id) {
		for(auto view : windows) {
			if(view->Id() == id) {
				auto v = std::dynamic_pointer_cast<T>(view);
				
				if(v)
					return v;
			}
		}

		return nullptr;
	}

	ServiceLocator& Services() { return *serviceLocator; }
	void StartActivity(const std::string& activityId);

private:
	std::vector<std::shared_ptr<BaseWindow>> windows;
	ServiceLocator* serviceLocator;
	ActivityInterface* activityInterface;

	bool markedAsFinished;
};