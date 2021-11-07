#pragma once
#include "Main.hpp"

namespace UI{
    const Vec2 ItemBase(10, 100);
    const Vec2 ItemSize(480, 80);
    const Rect Detail(500, 80, GameInfo::Width - 500, GameInfo::Height - 80);
    const Rect ImageErea(20, 120, 300);
    const Rect TitleErea(320, 165, 440, 130);
    const Rect StaffErea(320, 305, 440, 100);
    const Rect DescErea(20, 420, 740, 200);
    const Rect HeadErea(20, 50, 570, 80);
    const Circle Playnow(600, 90, 30);
    const Circle AddQue(670, 90, 30);

    // const Rect TuneErea(Arg::center(GameInfo::Width / 2, 40), 900, 80);

    // const Circle ToPrevMusic(TuneErea.h / 2 * 0.9, TuneErea.h / 2 * 0.9, 20);
    // const Circle StartStop(ToPrevMusic.x + 50, ToPrevMusic.y, 20);
    // const Circle ToNextMusic(ToPrevMusic.x + 100, ToPrevMusic.y, 20);
};

class Musics : public App::Scene{
private:
    const RectF Quit = RectF(10, 10, 150, 60);
    Transition QuitTransition;
    Album album;
    int32 Selecting;
    double Scroll;
    bool isPlaying;
    String playing_music;
    int32 playing_music_index;
    
public:
    Musics(const InitData& init);
    void update() override;
    void draw() const override;
    void drawItem(String name, int32 index) const;
};