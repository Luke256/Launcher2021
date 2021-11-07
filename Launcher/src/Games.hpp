#pragma once
#include "Main.hpp"

class Games : public App::Scene{
private:
    const Rect TitleArea = Rect(907, 13, 360, 100);
    const Rect DescArea = Rect(915, 170, 340, 130);
    const Rect TimeArea = Rect(915, 340, 160, 50);
    const Rect ToolsArea = Rect(1085, 340, 170, 90);
    const Rect StaffArea = Rect(915, 440, 340, 60);
    const Rect Back = Rect(10, 500, 380, 132);

    SimpleAnimation StartAnimation;
    SimpleAnimation SwitchAnimation;
    Polygon StartButton;
    Polygon UpButton;
    Polygon DownButton;
    int32 Selecting;
    int32 BaseY;
    String AnimationType;
    
public:
    Games(const InitData& init);
    void update() override;
    void draw() const override;
    void drawFadeIn(double t) const override;
    void drawFadeOut(double t) const override;
};