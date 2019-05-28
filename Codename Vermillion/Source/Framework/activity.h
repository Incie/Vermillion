#pragma once

#include"servicelocator.h"

class Activity {
public:
	virtual ~Activity() {}

	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;

	virtual void Resize() {}
	virtual void Update(double delta) = 0;
	virtual void Render() = 0;

	void SetServiceLocator(ServiceLocator& locator) {
		serviceLocator = &locator;
	}
protected:
	ServiceLocator& Services() { return *serviceLocator; }

private:
	ServiceLocator *serviceLocator;
};