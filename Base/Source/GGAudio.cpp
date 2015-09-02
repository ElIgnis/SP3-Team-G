#include "SceneStealth.h"
ISoundEngine* engine;
//Initialize game audio here
int SceneStealth::InitAudio()
{
	engine = createIrrKlangDevice(ESOD_AUTO_DETECT,ESEO_MULTI_THREADED|ESEO_LOAD_PLUGINS|ESEO_USE_3D_BUFFERS);
	engine->setSoundVolume(1.0f);

	//Menu sounds here
	sound[MENU_BGM] = engine->play2D("../Base/Audio/Menu_bgm.mp3", true, true);
	sound[MENU_BGM]->setVolume(0.5f);

	sound[MENU_SELECT] = engine->play2D("../Base/Audio/Menu_select.ogg", false, true);
	sound[MENU_SELECT]->setVolume(0.5f);

	sound[MENU_HIGHSCORE] = engine->play2D("../Base/Audio/Menu_highscore.wav", true, true);
	sound[MENU_HIGHSCORE]->setVolume(0.8f);

	//Level things here
	sound[LEVEL_WIN] = engine->play2D("../Base/Audio/Level_win.mp3", false, true);
	sound[LEVEL_WIN]->setVolume(1.0f);

	sound[LEVEL_LOSE] = engine->play2D("../Base/Audio/Level_lose.wav", false, true);
	sound[LEVEL_LOSE]->setVolume(1.0f);

	sound[LEVEL_BUTTON] = engine->play2D("../Base/Audio/Level_button.wav", false, true);
	sound[LEVEL_BUTTON]->setVolume(0.7f);

	sound[LEVEL_CHECKPOINT] = engine->play2D("../Base/Audio/Level_checkpoint.mp3", false, true);
	sound[LEVEL_CHECKPOINT]->setVolume(0.5f);

	//Enemy sounds here
	sound[ENEMY_ALERT] = engine->play2D("../Base/Audio/Enemy_alert.mp3", false, true);
	sound[ENEMY_ALERT]->setVolume(1.0f);
	
	sound[ENEMY_SHOOT] = engine->play2D("../Base/Audio/Enemy_shoot.ogg", false, true);
	sound[ENEMY_SHOOT]->setVolume(0.5f);
	
	sound[ENEMY_BULLET_WALL] = engine->play2D("../Base/Audio/Enemy_shoot_hit_wall.wav", false, true);
	sound[ENEMY_BULLET_WALL]->setVolume(0.08f);
	
	sound[ENEMY_STUNNED] = engine->play2D("../Base/Audio/Enemy_stunned.wav", false, true);
	sound[ENEMY_STUNNED]->setVolume(0.5f);

	//Player Sounds here
	sound[PLAYER_DMG] = engine->play2D("../Base/Audio/Player_damaged.wav", false, true);
	sound[PLAYER_DMG]->setVolume(0.8f);
	
	sound[PLAYER_PICKUP] = engine->play2D("../Base/Audio/Player_pickup.wav", false, true);
	sound[PLAYER_PICKUP]->setVolume(0.8f);
	
	sound[PLAYER_HEALTH] = engine->play2D("../Base/Audio/Player_health.wav", false, true);
	sound[PLAYER_HEALTH]->setVolume(0.9f);
	
	sound[PLAYER_DISGUISE] = engine->play2D("../Base/Audio/Player_disguise.wav", false, true);
	sound[PLAYER_DISGUISE]->setVolume(0.9f);
	
	sound[PLAYER_SPEED] = engine->play2D("../Base/Audio/Player_speed.wav", false, true);
	sound[PLAYER_SPEED]->setVolume(0.9f);
	
	sound[PLAYER_DECOY] = engine->play2D("../Base/Audio/Player_decoy.wav", false, true);
	sound[PLAYER_DECOY]->setVolume(0.9f);

	return 0;
}

int SceneStealth::ExitAudio()
{
	if(engine)
	{
		engine->drop();
	}
	return 0;
}