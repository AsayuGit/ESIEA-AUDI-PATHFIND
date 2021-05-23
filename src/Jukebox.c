#include "Jukebox.h"

#include <SDL2/SDL_mixer.h>

#include "Load.h"

char* MusicPath[NumberOfMusicTracks][2] = {
    {"Assets/Audio/Music/BGM_INTRO.wav", "Assets/Audio/Music/BGM_LOOP.wav"}
};

char* EffectPath[NumberOfEffectTracks] = {
    "Assets/Audio/SFX/Yeah.wav"
};

static Mix_Music* Track_INTRO = NULL;
static Mix_Music* Track_LOOP = NULL;
static int PlayingTrack = -1;

/* MUSIC QUEUE SYSTEM */

void PlayTrackID(MusicPlaylistID TrackID){
    StopTrack();

    if (Track_INTRO){
        Mix_FreeMusic(Track_INTRO);
        Track_INTRO = NULL;
    }

    if (Track_LOOP){
        Mix_FreeMusic(Track_LOOP);
        Track_LOOP = NULL;
    }
    
    if (MusicPath[TrackID][0])
        Track_INTRO = LoadMusic(MusicPath[TrackID][0]);
    if (MusicPath[TrackID][1]){
        Track_LOOP = LoadMusic(MusicPath[TrackID][1]);
    }

    if (Track_INTRO)
        Mix_PlayMusic(Track_INTRO, 1);
    
    PlayingTrack = TrackID;
}

void MusicDaemon(void){
    if ((PlayingTrack >= 0) && (!Mix_PlayingMusic()) && Track_LOOP){
        Mix_PlayMusic(Track_LOOP, -1);
    }
}

unsigned int GetTrackID(void){
    return PlayingTrack;
}

void StopTrack(void){
    PlayingTrack = -1;
    Mix_HaltMusic();
}