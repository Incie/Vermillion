#pragma once

#include"services.h"

class SoundSoLoud : public SoundService 
{
public:
	void Init();
	void Deinit();

	void Play(const FilePath& soundFile);
	void PlayStream(const FilePath& musicFile);
	void Volume(int volume);
	int Volume();

private:
	int volume;
};