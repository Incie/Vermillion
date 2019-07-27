#include"activity.h"


class BreakoutGame : public Activity {
public:
	BreakoutGame() = default;

	virtual void Initialize() override;
	virtual void Deinitialize() override;
	virtual void Resize(const glm::ivec2& newWindowSize) override;
	virtual void Update(float delta) override;
	virtual void Render() override;

private:

};