#pragma once
#include "Main.hpp"

class Albums : public App::Scene{
private:
    const Vec2 ItemBase = Vec2(80, 100);
    const Vec2 ItemSize = Vec2(360, 340);
    const RectF Quit = RectF(Arg::topRight=Vec2(GameInfo::Width - 10, 10), 150, 60);
    double Scroll;
    int32 Selecting;
    Transition QuitTransition;
    
public:
    Albums(const InitData& init);
    void update() override;
    void draw() const override;
    void drawItem(const Album &item, const int32 index) const;
};