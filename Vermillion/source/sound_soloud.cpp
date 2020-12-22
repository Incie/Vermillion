#include"pch.h"
#include "sound_soloud.h"

#include"../../vendor/soloud20200207/include/soloud.h"
#include"../../vendor/soloud20200207/include/soloud_wav.h"
#include"../../vendor/soloud20200207/include/soloud_wavstream.h"

SoLoud::Soloud soloud;
SoLoud::Wav wav;
SoLoud::WavStream stream;

void SoundSoLoud::Init()
{
    TRACE("SoundSoLoud");
    soloud.init();
    soloud.setGlobalVolume(0.5f);

    auto soundPath = FilePath(Paths::Sounds, "death.wav");
    wav.load(soundPath.tochar());

    auto musicPath = FilePath(Paths::Music, "Rawr - 01 - Entropy (Menu).mp3");
    stream.load(musicPath.tochar());
}

void SoundSoLoud::Deinit()
{
    TRACE("SoundSoLoud");
    soloud.deinit();
}

void SoundSoLoud::Play(const FilePath& soundFile)
{
    soloud.play(wav);
}

void SoundSoLoud::PlayStream(const FilePath& musicFile)
{
    auto result = soloud.play(stream);

}

void SoundSoLoud::Volume(int volume)
{
}

int SoundSoLoud::Volume()
{
    
    return 0;
}
