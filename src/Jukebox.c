/*
    PATHFIND is a simple game where the player aims to find chests either
    by themself or by using the included pathfinding algorythm

    Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "Jukebox.h"

#include <SDL2/SDL_mixer.h>

#include "Load.h"

const char* MusicPath[NumberOfMusicTracks][2] = {
    {"Assets/Audio/Music/BGM_INTRO.wav", "Assets/Audio/Music/BGM_LOOP.wav"}
};

const char* EffectPath[NumberOfEffectTracks] = {
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