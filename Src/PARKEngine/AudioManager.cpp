#include "AudioManager.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <fmod.h>
#include "ResourceManager.h"

std::unique_ptr<AudioManager> AudioManager::instance_;


void AudioManager::initInstance(std::string audioSourceFile, float doppler, float rolloff)
{
	//Devuelve la instancia si exise, si no crea una nueva
	if (instance_.get() == nullptr)
		instance_.reset(new AudioManager(audioSourceFile, doppler, rolloff));
}

AudioManager * AudioManager::instance()
{
	//Devuelve la instancia
	assert(instance_.get() != nullptr);
	return instance_.get();
}


//Constructora, le llegan los parametros del mundo que afecta a cualquier entidad con sonido 3D
AudioManager::AudioManager(std::string audioSourceFile, float doppler, float rolloff) : doppler_(doppler), rolloff_(rolloff)
{
	result_ = FMOD::System_Create(&system_);
	FMOD_OK_ERROR_CHECK();
	result_ = system_->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	FMOD_OK_ERROR_CHECK();
	system_->set3DSettings(doppler_, 1.0, rolloff_);

	READ_JSON_SOUNDS(audioSourceFile);
}

AudioManager::~AudioManager()
{
	std::cout << "Destructora de AudioManager" << std::endl;
	result_ = system_->release();
	FMOD_OK_ERROR_CHECK();

	instance_.release();
}

void AudioManager::FMOD_OK_ERROR_CHECK()
{
	assert(result_ == FMOD_OK);
}

void AudioManager::ADD_2D_SOUND(std::string fileName, std::string id, int loopCount, float volume, float pan)
{
	FMOD::Sound* snd;
	FMOD_CREATESOUNDEXINFO* ErrInf = 0;

	if(loopCount < 0)
	result_ = system_->createSound(fileName.c_str(), FMOD_LOOP_NORMAL, ErrInf, &snd);
	else result_ = system_->createSound(fileName.c_str(), FMOD_DEFAULT, ErrInf, &snd);

	FMOD_OK_ERROR_CHECK();

	soundValues aux;
	aux.loopCount = loopCount; aux.pan = pan; aux.volume = volume; aux.snd = snd, aux.id_ = id;

	soundList_.insert(std::make_pair(id, aux));
}

void AudioManager::ADD_SONG(std::string fileName, std::string id, int loopCount, float volume, float pan)
{
	FMOD::Sound* snd;
	FMOD_CREATESOUNDEXINFO* ErrInf = 0;

	if (loopCount < 0)
		result_ = system_->createSound(fileName.c_str(), FMOD_LOOP_NORMAL | FMOD_CREATESTREAM, ErrInf, &snd);
	else result_ = system_->createSound(fileName.c_str(), FMOD_DEFAULT | FMOD_CREATESTREAM, ErrInf, &snd);

	FMOD_OK_ERROR_CHECK();

	soundValues aux;
	aux.loopCount = loopCount; aux.pan = pan; aux.volume = volume; aux.snd = snd, aux.id_ = id;

	Songs_.insert(std::make_pair(id, aux));
}

void AudioManager::ADD_3D_SOUND(std::string fileName, std::string id, int loopCount, float volume)
{
	FMOD::Sound* snd;
	FMOD_CREATESOUNDEXINFO* ErrInf = 0;

	if (loopCount < 0)
		result_ = system_->createSound(fileName.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, ErrInf, &snd);
	else result_ = system_->createSound(fileName.c_str(), FMOD_3D | FMOD_DEFAULT, ErrInf, &snd);

	FMOD_OK_ERROR_CHECK();

	soundValues3D aux;
	aux.loopCount = loopCount; aux.volume = volume; aux.snd = snd, aux.id_ = id;

	soundList3D_.insert(std::make_pair(id, aux));
}

void AudioManager::PLAY_2D_SOUND(std::string AudioID)
{
	auto iter = SoundsChannels_.find(AudioID);
	if (iter != SoundsChannels_.end()) {
		SoundsChannels_.erase(AudioID);
	}
	FMOD::Channel* chn;

	auto it = soundList_.find(AudioID);
	if (it != soundList_.end()) {

		result_ = system_->playSound((*it).second.snd, 0, false, &chn);

		chn->setVolume((*it).second.volume*masterSoundVolume); //Se multiplica por el sonido master para que se le aplique el volumen seleccionado
		chn->setPan((*it).second.pan);
		chn->setLoopCount((*it).second.loopCount);

		FMOD_OK_ERROR_CHECK();
		SoundsChannels_.insert(std::make_pair(AudioID, chn));
	}
	
}

void AudioManager::PLAY_3D_SOUND(std::string AudioID, Vector3 pos_)
{
	auto iter = SoundsChannels_.find(AudioID);
	if (iter != SoundsChannels_.end()) {
		SoundsChannels_.erase(AudioID);
	}
	FMOD::Channel* chn;
	
	auto it = soundList3D_.find(AudioID);
	if (it != soundList3D_.end()) {

		result_ = system_->playSound((*it).second.snd, 0, false, &chn);

		FMOD_VECTOR pos = { pos_.x, pos_.y, pos_.z };
		chn->setVolume((*it).second.volume*masterSoundVolume); //Se multiplica por el sonido master para que se le aplique el volumen seleccionado
		chn->setLoopCount((*it).second.loopCount);
		FMOD_VECTOR* vel = new FMOD_VECTOR{ 0,0,0 };
		chn->set3DAttributes(&pos, vel);

		(*it).second.emitter.x = &pos_.x; (*it).second.emitter.y = &pos_.y; (*it).second.emitter.z = &pos_.z;

		
	
		FMOD_OK_ERROR_CHECK();
		SoundsChannels_.insert(std::make_pair(AudioID, chn));
	}
}

void AudioManager::PLAY_SONG(std::string AudioID)
{
	auto iter = SongChannels_.find(AudioID);
	if (iter != SongChannels_.end()) {
		SongChannels_.erase(AudioID);
	}
	FMOD::Channel* chn;

	auto it = Songs_.find(AudioID);
	if (it != Songs_.end()) {

		result_ = system_->playSound((*it).second.snd, 0, false, &chn);

		chn->setVolume((*it).second.volume*masterMusicVolume); //Se multiplica por el sonido master para que se le aplique el volumen seleccionado
		chn->setPan((*it).second.pan);
		chn->setLoopCount((*it).second.loopCount);

		FMOD_OK_ERROR_CHECK();
		SongChannels_.insert(std::make_pair(AudioID, chn));
	}
}

void AudioManager::STOP_ALL_SOUNDS()
{
	for (auto it = SoundsChannels_.begin(); it != SoundsChannels_.end(); it++)
	{
		(*it).second->stop();
	}
	SoundsChannels_.clear();
	for (auto it = SongChannels_.begin(); it != SongChannels_.end(); it++)
	{
		(*it).second->stop();
	}
	SongChannels_.clear();
}

void AudioManager::STOP_SOUND(std::string AudioID)
{
	auto it = SoundsChannels_.find(AudioID);
	if (it != SoundsChannels_.end()) {
		(*it).second->stop();
		SoundsChannels_.erase(AudioID);
	}
	auto it2 = SongChannels_.find(AudioID);
	if (it2 != SongChannels_.end()) {
		(*it2).second->stop();
		SongChannels_.erase(AudioID);
	}
}


void AudioManager::UP_MUSIC_VOLUME()
{
	masterMusicVolume += 0.1; 
	if (masterMusicVolume > 1) masterMusicVolume = 1;
	SET_MUSIC_VOLUME();
}

void AudioManager::DOWN_MUSIC_VOLUME()
{
	masterMusicVolume -= 0.1;
	if (masterMusicVolume < 0.1) masterMusicVolume = 0;
	if (masterMusicVolume < 0) masterMusicVolume = 0;
	SET_MUSIC_VOLUME();
}

void AudioManager::UP_EFFECTS_VOLUME()
{
	masterSoundVolume += 0.1;
	if (masterSoundVolume > 1) masterSoundVolume = 1;
	SET_SOUND_VOLUME();
}

void AudioManager::DOWN_EFFECTS_VOLUME()
{
	masterSoundVolume -= 0.1;
	if (masterSoundVolume < 0.1) masterSoundVolume = 0;
	if (masterSoundVolume < 0) masterSoundVolume = 0;
	SET_SOUND_VOLUME();
}


void AudioManager::SET_SOUND_VOLUME()
{
	for (auto it = SoundsChannels_.begin(); it != SoundsChannels_.end(); it++)
	{
		(*it).second->setVolume(masterSoundVolume);
	}
}

void AudioManager::SET_MUSIC_VOLUME()
{
	for (auto it = SongChannels_.begin(); it != SongChannels_.end(); it++)
	{
		(*it).second->setVolume(masterMusicVolume);
	}
}

void AudioManager::READ_JSON_SOUNDS(std::string file)
{
	json js = ResourceManager::instance()->getJsonByKey(file);
	for(json d2 : js["2DSounds"])
	ADD_2D_SOUND(d2["rute"], d2["Id"], d2["loopCount"], d2["Volume"], d2["Pan"]);
	for (json d3 : js["3DSounds"])//Sonidos 3D
		ADD_3D_SOUND(d3["rute"], d3["Id"], d3["loopCount"], d3["Volume"]);
	for (json song : js["Songs"])
 		ADD_SONG(song["rute"], song["Id"], song["loopCount"], song["Volume"], song["Pan"]);
}


void AudioManager::set3DFactors(float doppler, float rolloff)
{
	doppler_ = doppler;
	rolloff_ = rolloff;
	system_->set3DSettings(doppler_, 1.0, rolloff_);
}
