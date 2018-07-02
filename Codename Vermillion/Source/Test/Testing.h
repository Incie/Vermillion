#pragma once

class Testing
{
public:
	Testing();
	~Testing();

	void Update(double deltaTime);
	void Render();
	
private:
	double rotation;
};