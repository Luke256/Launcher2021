# include <Siv3D.hpp> // OpenSiv3D v0.6.1
# include "Main.hpp"
# include "Title.hpp"
# include "Games.hpp"

void Main()
{
    Window::Resize(GameInfo::Width, GameInfo::Height);
    Window::SetTitle(GameInfo::Title);
    Window::SetStyle(WindowStyle::Sizable);
    Window::SetStyle(WindowStyle::Frameless);

    TextureAsset::Register(U"Char_Computer", U"data/Assets/Cmputer.png", TextureDesc::Mipped);
    TextureAsset::Register(U"Char_Music", U"data/Assets/MUSIC.png", TextureDesc::Mipped);
    TextureAsset::Register(U"Char_Game", U"data/Assets/GAME.png", TextureDesc::Mipped);
    TextureAsset::Register(U"Char_3DCG", U"data/Assets/3DCG.png", TextureDesc::Mipped);
    TextureAsset::Register(U"CopyRight", U"data/Assets/copyright.png", TextureDesc::Mipped);
    TextureAsset::Register(U"Char_Detail_l", U"data/Assets/Char_detail_l.png", TextureDesc::Mipped);
    TextureAsset::Register(U"Char_Detail_r", U"data/Assets/Char_Detail_r.png", TextureDesc::Mipped);
    TextureAsset::Register(U"LogoL", U"data/Assets/KAIJO_l.png", TextureDesc::Mipped);
    TextureAsset::Register(U"LogoR", U"data/Assets/KAIJO_r.png", TextureDesc::Mipped);
    TextureAsset::Register(U"Shape", U"data/Assets/shape.png", TextureDesc::Mipped);
    TextureAsset::Register(U"GamePlay", U"data/Assets/Game_Play.png", TextureDesc::Mipped);
    TextureAsset::Register(U"GameUI", U"data/Assets/Games_UI.png", TextureDesc::Mipped);
    TextureAsset::Register(U"BackGround", U"data/Assets/back.png", TextureDesc::Mipped);

    FontAsset::Register(U"Games.Title", 32, Typeface::Bold);
    FontAsset::Register(U"Games.SubTitle", 24, Typeface::Bold);
    FontAsset::Register(U"Games.Detail", 24);

    App manager;
    manager.add<Title>(U"Title");
    manager.add<Games>(U"Games");

    manager.get()->watch.start();

    INI ini(U"data/Assets/config.ini");
    bool Maxed = false;
    manager.get()->TimeLimit = ini.get<int32>(U"App.Limit");

	while (System::Update())
	{
        TextureAsset(U"BackGround").draw(0, 0);

        if(KeyAlt.pressed() && KeyControl.pressed() && KeyShift.pressed() && KeyQ.pressed()){
            Window::Restore();
            Window::Centering();
            manager.changeScene(U"Title");
            manager.get()->initial = true;
            manager.get()->watch.restart();
            Maxed = false;
        }
        if(manager.get()->watch.sF() > manager.get()->TimeLimit){
            if(!Maxed){
                Window::Restore();
                Window::Maximize();
                Window::Centering();
                Maxed = true;
            }
            if(manager.get()->process){ //プロセス起動中なら殺す
                manager.get()->process->terminate();
            }
            FontAsset(U"Games.Title")(U"時間制限に達しました。\n申し訳ありませんが、次の方への交代をお願いします。").drawAt(Scene::Center());
            continue;
        }


        if(!manager.update()) break;
	}
}