#pragma once
#include "Main.hpp"

class Title : public App::Scene{
private:
    const Circle ToMusic = Circle(240, 220, 180);
    const Circle ToGame = Circle(640, 220, 180);
    const Circle To3DCG = Circle(1040, 220, 180);

    Stopwatch watch;
    SimpleAnimation StartAnimation;
    SimpleAnimation OpenAnimation;
    SimpleAnimation MusicAnimation;
    SimpleAnimation GameAnimation;
    SimpleAnimation CGAnimation;
    String Running;
    Polygon StartButton;

    bool ChildRunning;
    bool Opened;
    
public:
    Title(const InitData& init);
    void update() override;
    void draw() const override;
    void drawFadeIn(double t) const override;
    void drawFadeOut(double t) const override;
};