#include <Siv3D.hpp> // Siv3D v0.6 beta2
#include "Main.hpp"
#include "View3D.hpp"
#include "View2D.hpp"
#include "List.hpp"

void Main(){

    Window::Resize(GameInfo::Width, GameInfo::Height);
    Scene::Resize(GameInfo::Width, GameInfo::Height);
    Window::SetTitle(GameInfo::Title);
    Window::SetStyle(WindowStyle::Frameless);
    Scene::SetBackground(ColorF(0.2));

    App manager;
    manager.add<List>(U"List");
    manager.add<View>(U"View");
    manager.add<View2D>(U"View2D");

    const Font iconFont(36, Typeface::Icon_MaterialDesign);
    FontAsset::Register(U"List.Head", 42);
    FontAsset::Register(U"List.Title", 36, Typeface::Bold);
    FontAsset::Register(U"List.Detail", 24);
    FontAsset::Register(U"List.Title2", 24, Typeface::Bold);
    FontAsset(U"List.Title").addFallback(iconFont);

    while(System::Update()){
        if(!manager.update()) break;
    }
}
