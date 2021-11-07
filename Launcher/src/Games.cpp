#include "Games.hpp"

Games::Games(const InitData& init) : IScene(init){
    StartAnimation
        .set(U"ShapeX", {0.5s, 640}, {1s, 100})
        .set(U"ShapeY", {0.5s, 250}, {1s, 100})
        .set(U"ShapeW", {0.5s, 422}, {1s, 200})
        .set(U"ShapeH", {0.5s, 430}, {1s, 200})
        .set(U"Logo", {0.5s, 640}, {1s, 206})
        .set(U"FadeIn", {0.5s, 0}, {1s, 1})
        .set(U"FadeOut", {0.5s, 1}, {1s, 0})
        .start()
        ;

    SwitchAnimation
        .set(U"Up", {0s, -550}, {0.4s, 0})
        .set(U"Down", {0s, 550}, {0.4s, 0})
        .set(U"None", {0s, 0}, {0.4s, 0})
    ;

    const String CurrentDirectory = FileSystem::FullPath(FileSystem::CurrentDirectory());
    for (const auto& path : FileSystem::DirectoryContents(CurrentDirectory + U"data/Games/", Recursive::No)){
        if(FileSystem::FileName(path) == U".DS_Store") continue;
        const INI ini(path + U"launcher_info.ini");
        Product product;
        product.path = path + ini.get<String>(U"Game.path");
        product.title = ini.get<String>(U"Game.title");
        product.Desc = ini.get<String>(U"Game.desc");
        product.Staff = ini.get<String>(U"Game.staff");
        product.Time = ini.get<String>(U"Game.time");
        product.Tools = ini.get<String>(U"Game.tools");
        TextureAsset::Register(product.title, path + ini.get<String>(U"Game.image"));

        getData().Products << product;
    }


    StartButton = Image(U"data/Assets/Game_Play.png").alphaToPolygon();
    StartButton.moveBy(1009, 549);
    DownButton = Image(U"data/Assets/ArrowDown.png").alphaToPolygon();
    DownButton.moveBy(260, 100);
    UpButton = Image(U"data/Assets/ArrowUp.png").alphaToPolygon();
    UpButton.moveBy(260, 100);
    Selecting = 0;
    BaseY = 13;
    AnimationType = U"None";
}

/*update---------------------------------------------------------------------*/
void Games::update() {
    if (getData().process){
		// プロセスが実行中なら
		if (getData().process->isRunning()){
			// ウィンドウを最小化
			Window::Minimize();
			return;
		}
		else { // プロセスが終了したら
			// ウィンドウを復帰
			Window::Restore();
            Window::Centering();
			getData().process.reset();
		}
	}

    if(StartButton.mouseOver() || Back.mouseOver()){
        Cursor::RequestStyle(CursorStyle::Hand);
    }
    if(DownButton.mouseOver() && Selecting + 1 < getData().Products.size()) Cursor::RequestStyle(CursorStyle::Hand);
    if(UpButton.mouseOver() && Selecting > 0) Cursor::RequestStyle(CursorStyle::Hand);

    if(Back.leftReleased()){
        changeScene(U"Title");
    }

    if(StartButton.leftReleased()){
        getData().process = ChildProcess(getData().Products[Selecting].path);
    }

    if(UpButton.leftReleased()){
        --Selecting;
        if(InRange(Selecting, 0, (int32)getData().Products.size() - 1))SwitchAnimation.restart();
        Selecting = Clamp(Selecting, 0, (int32)getData().Products.size() - 1);
        AnimationType = U"None";
    }
    if(DownButton.leftReleased()){
        ++Selecting;
        if(InRange(Selecting, 0, (int32)getData().Products.size() - 1))SwitchAnimation.restart();
        Selecting = Clamp(Selecting, 0, (int32)getData().Products.size() - 1);
        AnimationType = U"None";
    }

    if(KeyUp.up()){
        --Selecting;
        if(InRange(Selecting, 0, (int32)getData().Products.size() - 1))SwitchAnimation.restart();
        Selecting = Clamp(Selecting, 0, (int32)getData().Products.size() - 1);
        AnimationType = U"Up";
    }
    if(KeyDown.up()){
        ++Selecting;
        if(InRange(Selecting, 0, (int32)getData().Products.size() - 1))SwitchAnimation.restart();
        Selecting = Clamp(Selecting, 0, (int32)getData().Products.size() - 1);
        AnimationType = U"Down";
    }
}

/*draw---------------------------------------------------------------------*/
void Games::draw() const {
    for(int32 i : step(getData().Products.size())){
        TextureAsset(getData().Products[i].title).resized(500, 500).draw(390, 13 + (i - Selecting) * 550 + SwitchAnimation[AnimationType], ColorF(1, StartAnimation[U"FadeIn"]));
    }

    FontAsset(U"Games.Title")(getData().Products[Selecting].title).draw(TitleArea.stretched(-10), ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.SubTitle")(U"説明").draw(915, 130, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.Detail")(getData().Products[Selecting].Desc).draw(DescArea, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.SubTitle")(U"製作期間").draw(915, 300, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.Detail")(getData().Products[Selecting].Tools).draw(ToolsArea, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.SubTitle")(U"使用ツール").draw(1085, 300, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.Detail")(getData().Products[Selecting].Time).draw(TimeArea, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.SubTitle")(U"クレジット").draw(915, 400, ColorF(1, StartAnimation[U"FadeIn"]));
    FontAsset(U"Games.Detail")(getData().Products[Selecting].Staff).draw(StaffArea, ColorF(1, StartAnimation[U"FadeIn"]));



    TextureAsset(U"Shape").resized(StartAnimation[U"ShapeW"], StartAnimation[U"ShapeH"]).drawAt(StartAnimation[U"ShapeX"], StartAnimation[U"ShapeY"]);
    TextureAsset(U"Char_Computer").drawAt(StartAnimation[U"Logo"], 570);
    TextureAsset(U"CopyRight").drawAt(StartAnimation[U"Logo"], 690);
    TextureAsset(U"Char_Detail_l").drawAt(75, 262, ColorF(1, StartAnimation[U"FadeIn"]));
    TextureAsset(U"GameUI").drawAt(314, 261, ColorF(1, StartAnimation[U"FadeIn"]));
    TextureAsset(U"GamePlay").drawAt(1090, 630, ColorF(1, StartAnimation[U"FadeIn"]));

    FontAsset(U"Games.Title")(getData().TimeLimit - getData().watch.s()).drawAt(100, 80, ColorF(1, StartAnimation[U"FadeIn"]));
    Line(390, 546, 890, 546).draw(2, ColorF(1, StartAnimation[U"FadeIn"])); // line
    Rect(390, 13, 500).drawFrame(5, 0, ColorF(1, StartAnimation[U"FadeIn"])); // pic1
    Rect(390, 563, 500).drawFrame(5, 0, ColorF(1, StartAnimation[U"FadeIn"])); // pic2
    Rect(907, 123, 360, 390).drawFrame(5, 0, ColorF(1, StartAnimation[U"FadeIn"])); // detail
    Rect(907, 13, 360, 100).drawFrame(5, 0, ColorF(1, StartAnimation[U"FadeIn"])); // title

    if(Selecting == 0) UpButton.draw(ColorF(0.8, StartAnimation[U"FadeIn"]));
    if(Selecting == getData().Products.size() - 1) DownButton.draw(ColorF(0.8, StartAnimation[U"FadeIn"]));


    TextureAsset(U"LogoR").mirrored().drawAt(80, 500, ColorF(1, StartAnimation[U"FadeOut"]));
    TextureAsset(U"LogoR").drawAt(1200, 500, ColorF(1, StartAnimation[U"FadeOut"]));
    TextureAsset(U"Char_Detail_l").drawAt(80 + 16, 345, ColorF(1, StartAnimation[U"FadeOut"]));
    TextureAsset(U"Char_Detail_r").drawAt(1200 - 16, 345, ColorF(1, StartAnimation[U"FadeOut"]));
}


void Games::drawFadeIn(double t) const {
    draw();
}
void Games::drawFadeOut(double t) const {
    draw();
}