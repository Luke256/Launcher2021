#pragma once
#include "Main.hpp"

class Title : public App::Scene{
private:
    Stopwatch watch;
    
public:
    Title(const InitData& init);
    void update() override;
    void draw() const override;
};