#ifndef _JUKEBOX_H
#define _JUKEBOX_H

    typedef enum{
        TRK_BGM,
        NumberOfMusicTracks
    } MusicPlaylistID;

    typedef enum{
        CHK_YEAH,
        NumberOfEffectTracks
    } EffectsPlaylistID;

    extern char* EffectPath[NumberOfEffectTracks];

    void PlayTrackID(MusicPlaylistID TrackID);
    void MusicDaemon(void);
    unsigned int GetTrackID(void);
    void StopTrack(void);

#endif
