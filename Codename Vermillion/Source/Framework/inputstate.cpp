#include"input.h"
#include"log.h"


unsigned char InputState::keyboardStates[256];


void InputState::Reset()
{
	Log::Info("InputState", "Reset");
	memset(keyboardStates, 0, 256);
}
