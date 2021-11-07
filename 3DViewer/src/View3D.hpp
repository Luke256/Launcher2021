#pragma once
#include "Main.hpp"

class View : public App::Scene{
private:
	const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
    Model model;
    DebugCamera3D camera;

    Transition InfoTransition;
    bool InfoActive;
    Rect InfoIndex;
    
public:
    View(const InitData& init);
    void update() override;
    void draw() const override;
};