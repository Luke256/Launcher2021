#pragma once
#include "Main.hpp"

class View2D : public App::Scene{
private:
    const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
    Texture texture;
    Camera2D camera;
    

    Transition InfoTransition;
    bool InfoActive;
    Rect InfoIndex;
    
public:
    View2D(const InitData& init);
    void update() override;
    void draw() const override;
};
