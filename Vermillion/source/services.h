#pragma once


#include<string>
#include"glm/glm.hpp"
#include"inputstate.h"
#include"keycodes.h"
#include<functional>
#include"file.h"


//MessageService
class Event {
public:
	Event(int eventId) : id(eventId) {}

	int Id() { return id; }
private:
	const int id;
};

class EventMessage : public Event {
public:
	EventMessage(int eventId, int eventMessage) : Event(eventId), eventMessage(eventMessage) {}

	int Message() { return eventMessage; }
private:
	int eventMessage;
};

class EventService {
public:
	virtual void DispatchEvent(const Event& eventData) = 0;
	virtual void RegisterEventHandler(int eventId, const std::function<void(Event*)>& func) = 0;
};



//SoundService
class SoundService {
public:
	virtual void Play(const FilePath& soundFile) = 0;
	virtual void PlayStream(const FilePath& musicFile) = 0;
	virtual void Volume(int volume) = 0;
	virtual int Volume() = 0;
};


//TextureService
class Texture {
public:
	unsigned int textureId;
	int width;
	int height;
	unsigned int channels;
	std::string file;
};

class TextureService {
public:
	virtual std::shared_ptr<Texture> LoadTexture(const FilePath& relativePath) = 0;
	virtual void UnloadAll() = 0;
};


//TextService
class TextService {
public:
	TextService() : serviceId(-1) {}
	
	int ServiceId() { return serviceId; }
	void SetServiceId(int id) { if(serviceId == -1) { serviceId = id; } else throw std::exception("service id already set"); }

	virtual float CalculateWidth(const std::string& text, unsigned int fontHeight) const = 0;
	virtual float Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center = false, bool newline = false) const = 0;
	virtual void NewLine(unsigned int fontHeight) const = 0;
	virtual void PrintCenter(const double x, const double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const = 0;

private:
	int serviceId;
	
};


//InputService
class InputService {
public:
	virtual void Update() = 0;

	virtual bool KeyDown(char keyCode) const = 0;
	virtual bool KeyOnce(char keyCode) const = 0;
	virtual bool KeyUp(char keyCode) const = 0;
	virtual int ScrollState() const = 0;

	virtual glm::vec2 GetMousePosition() const = 0;
	virtual glm::vec2 GetMousePositionNormalized() const = 0;
	virtual glm::vec2 GetMouseDelta() const = 0;

	virtual bool GamePadButtonDown(GamePadButton button) const = 0;
	virtual bool GamePadButtonOnce(GamePadButton button) const = 0;
	virtual bool GamePadButtonUp(GamePadButton button) const = 0;

	virtual double GamePadTriggerState(bool right) const = 0;
	virtual glm::vec2 GamePadRightStick() const = 0;
	virtual glm::vec2 GamePadLeftStick() const = 0;
};