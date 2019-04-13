#pragma once

#include"servicelocator.h"

class Activity {
public:
	virtual void Initialize() = 0;
	virtual void Deinitialize() = 0;

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