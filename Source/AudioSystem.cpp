#include "AudioSystem.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <filesystem>

SoundHandle SoundHandle::Invalid;

// Create the AudioSystem with specified number of channels
// (Defaults to 8 channels)
AudioSystem::AudioSystem(int numChannels)
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	Mix_AllocateChannels(numChannels);

	mChannels.resize(numChannels);
}

// Destroy the AudioSystem
AudioSystem::~AudioSystem()
{
	for (auto& sound : mSounds) {
		Mix_FreeChunk(sound.second);
	}
	mSounds.clear();
	Mix_CloseAudio();
}

// Updates the status of all the active sounds every frame
void AudioSystem::Update(float deltaTime)
{
	for (SoundHandle& handle : mChannels){
		if (handle.IsValid()) {
			auto it = mHandleMap.find(handle);
			if (it != mHandleMap.end()) {
				int channel = it->second.mChannel;
				if (Mix_Playing(channel) == 0){
					mHandleMap.erase(it);
					handle.Reset();
				}
			}
		}
	}
}

// Plays the sound with the specified name and loops if looping is true
// Returns the SoundHandle which is used to perform any other actions on the
// sound when active
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
SoundHandle AudioSystem::PlaySound(const std::string& soundName, bool looping)
{
    // Get the sound with the given name
    Mix_Chunk *sound = GetSound(soundName);


	if (sound == nullptr)
	{
		SDL_Log("Failed to load sound: %s", soundName.c_str());
		return SoundHandle::Invalid;
	}

    int availableChannel = -1;

	int i = 0;
	for (SoundHandle& handle :  mChannels){
		if (!handle.IsValid()){
			availableChannel = i;
			break;
		}
		i++;
	}

	if (availableChannel == -1){
		for (auto& pair : mHandleMap){
			if (pair.second.mSoundName == soundName){
				availableChannel = pair.second.mChannel;
				SDL_Log("Stopping existing sound '%s' to play new instance", soundName.c_str());
				mHandleMap.erase(pair.first);
				mChannels[availableChannel].Reset();
				break;
			}
		}
	}

	if (availableChannel == -1){
		for (auto& pair : mHandleMap){
			if (pair.second.mIsLooping){
				StopSound(pair.first);
				availableChannel = pair.second.mChannel;
				SDL_Log("Stopping looping sound '%s' to play new sound", pair.second.mSoundName.c_str());
				break;
			}
		}
	}

	if (availableChannel == -1 && !mHandleMap.empty()){
		auto oldest = mHandleMap.begin();
		availableChannel = oldest->second.mChannel;
		SDL_Log("Stopping oldest sound '%s' to play new sound", oldest->second.mSoundName.c_str());
		StopSound(oldest->first);
	}

	mLastHandle++;

	HandleInfo info;
	info.mSoundName = soundName;
	info.mChannel = availableChannel;
	info.mIsLooping = looping;
	mHandleMap[mLastHandle] = info;
	mChannels[availableChannel] = mLastHandle;

	int loops = looping ? -1 : 0;
	Mix_PlayChannel(availableChannel, sound, loops);

    return mLastHandle;
}

// Stops the sound if it is currently playing
void AudioSystem::StopSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end()){
		SDL_Log("Error: Trying to stop invalid sound handle");
		return;
	}
	const HandleInfo& info = it->second;
	Mix_HaltChannel(info.mChannel);
	mHandleMap.erase(it);
	mChannels[info.mChannel].Reset();
}

// Pauses the sound if it is currently playing
void AudioSystem::PauseSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end()){
		SDL_Log("Error: Trying to pause invalid sound handle");
		return;
	}

	HandleInfo& info = it->second;
	if (!info.mIsPaused){
		Mix_Pause(info.mChannel);
		info.mIsPaused = true;
	}
}

// Resumes the sound if it is currently paused
void AudioSystem::ResumeSound(SoundHandle sound)
{
	auto it = mHandleMap.find(sound);
	if (it == mHandleMap.end()){
		SDL_Log("Error: Trying to resume invalid sound handle");
		return;
	}

	HandleInfo& info = it->second;
	if (info.mIsPaused){
		Mix_Resume(info.mChannel);
		info.mIsPaused = false;
	}

}

// Returns the current state of the sound
SoundState AudioSystem::GetSoundState(SoundHandle sound)
{
    if(mHandleMap.find(sound) == mHandleMap.end())
    {
        return SoundState::Stopped;
    }

    if(mHandleMap[sound].mIsPaused)
    {
        return SoundState::Paused;
    }

	return SoundState::Playing;
}

// Stops all sounds on all channels
void AudioSystem::StopAllSounds()
{
    Mix_HaltChannel(-1);

    for(auto & mChannel : mChannels)
    {
        mChannel.Reset();
    }

    mHandleMap.clear();
}

// Cache all sounds under Assets/Sounds
void AudioSystem::CacheAllSounds()
{
#ifndef __clang_analyzer__
	std::error_code ec{};
	for (const auto& rootDirEntry : std::filesystem::directory_iterator{"Assets/Sounds", ec})
	{
		std::string extension = rootDirEntry.path().extension().string();
		if (extension == ".ogg" || extension == ".wav")
		{
			std::string fileName = rootDirEntry.path().stem().string();
			fileName += extension;
			CacheSound(fileName);
		}
	}
#endif
}

// Used to preload the sound data of a sound
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
void AudioSystem::CacheSound(const std::string& soundName)
{
	GetSound(soundName);
}

// If the sound is already loaded, returns Mix_Chunk from the map.
// Otherwise, will attempt to load the file and save it in the map.
// Returns nullptr if sound is not found.
// NOTE: The soundName is without the "Assets/Sounds/" part of the file
//       For example, pass in "ChompLoop.wav" rather than
//       "Assets/Sounds/ChompLoop.wav".
Mix_Chunk* AudioSystem::GetSound(const std::string& soundName)
{
	std::string fileName = "../Assets/Sounds/";
	fileName += soundName;

	Mix_Chunk* chunk = nullptr;
	auto iter = mSounds.find(fileName);
	if (iter != mSounds.end())
	{
		chunk = iter->second;
	}
	else
	{
		chunk = Mix_LoadWAV(fileName.c_str());
		if (!chunk)
		{
			SDL_Log("[AudioSystem] Failed to load sound file %s", fileName.c_str());
			return nullptr;
		}

		mSounds.emplace(fileName, chunk);
	}
	return chunk;
}

// Input for debugging purposes
void AudioSystem::ProcessInput(const Uint8* keyState)
{
	// Debugging code that outputs all active sounds on leading edge of period key
	if (keyState[SDL_SCANCODE_PERIOD] && !mLastDebugKey)
	{
		SDL_Log("[AudioSystem] Active Sounds:");
		for (size_t i = 0; i < mChannels.size(); i++)
		{
			if (mChannels[i].IsValid())
			{
				auto iter = mHandleMap.find(mChannels[i]);
				if (iter != mHandleMap.end())
				{
					HandleInfo& hi = iter->second;
					SDL_Log("Channel %d: %s, %s, looping = %d, paused = %d",
							static_cast<unsigned>(i), mChannels[i].GetDebugStr(),
							hi.mSoundName.c_str(), hi.mIsLooping, hi.mIsPaused);
				}
				else
				{
					SDL_Log("Channel %d: %s INVALID", static_cast<unsigned>(i),
							mChannels[i].GetDebugStr());
				}
			}
		}
	}

	mLastDebugKey = keyState[SDL_SCANCODE_PERIOD];
}
