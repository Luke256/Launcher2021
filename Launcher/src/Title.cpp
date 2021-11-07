#include "Title.hpp"

Title::Title(const InitData& init) : IScene(init){
    OpenAnimation
        .set(U"Shape0", {0.5s, 640}, {1s, 240})
        .set(U"Shape1", {0.5s, 640}, {1s, 640})
        .set(U"Shape2", {0.5s, 640}, {1s, 1040})
        .set(U"Side_l", {0.5s, 480}, {1s, 80})
        .set(U"Side_r", {0.5s, 800}, {1s, 1200})
        .set(U"Computer", {0.5s, 220}, {1s, 570})
        .set(U"Fade", {0.5s, 0}, {1s, 1})
        .set(U"FadeOut", {0.5s, 1}, {1s, 0})
        ;

    StartAnimation.set(U"Fade", {0s, 0}, {1s, 1});

    MusicAnimation
        .set(U"Shape0", {0s, 240}, {0.5s, 240})
        .set(U"Shape1", {0s, 640}, {0.5s, 240})
        .set(U"Shape2", {0s, 1040}, {0.5s, 240})
        .set(U"Fade", {0s, 1}, {.5s, 0})
        ;
    GameAnimation
        .set(U"Shape0", {0s, 240}, {0.5s, 640})
        .set(U"Shape1", {0s, 640}, {0.5s, 640})
        .set(U"Shape2", {0s, 1040}, {0.5s, 640})
        .set(U"Fade", {0s, 1}, {.5s, 0})
        ;
    CGAnimation
        .set(U"Shape0", {0s, 240}, {0.5s, 1040})
        .set(U"Shape1", {0s, 640}, {0.5s, 1040})
        .set(U"Shape2", {0s, 1040}, {0.5s, 1040})
        .set(U"Fade", {0s, 1}, {.5s, 0})
        ;

    StartButton = Image(U"data/Assets/Game_Play.png").alphaToPolygon();
    StartButton.moveBy(559, 454);

    ChildRunning = false;
    Opened = false;
    Running = U"Main";

    StartAnimation.start();
    if(!getData().initial){
        Opened = true;
        OpenAnimation.restart();
    }

    getData().initial = false;
    watch = Stopwatch();
    watch.start();
}

/*update---------------------------------------------------------------------*/
void Title::update() {
    if(!Opened){
        if(StartButton.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
        if(StartButton.leftReleased()){
            Opened = true;
            OpenAnimation.restart();
            watch.restart();
        }

        return;
    }
    if(watch.sF() < 1) return;
    if (getData().process){
		// プロセスが実行中なら
		if (getData().process->isRunning()){
			// ウィンドウを最小化
			Window::Minimize();
			return;
		}
		else { // プロセスが終了したら
			// ウィンドウを復帰
            ChildRunning = false;
			Window::Restore();
			getData().process.reset();
		}
	}

    if(ChildRunning){
        if(Running == U"Music"){
            # if SIV3D_PLATFORM(WINDOWS)
            getData().process = ChildProcess{ U"data/Games/MusicRoom/MusicRoom.exe" };
            # elif SIV3D_PLATFORM(MACOS)
            getData().process = ChildProcess{ U"data/Games/MusicRoom/MusicRoom.app/Contents/MacOS/MusicRoom" };
            # endif
        }
        else if(Running == U"Game"){
            changeScene(U"Games", 0.1s);
        }
        else if(Running == U"3DCG"){
            # if SIV3D_PLATFORM(WINDOWS)
            getData().process = ChildProcess{ U"data/Games/3DViewer/3DViewer.exe" };
            # elif SIV3D_PLATFORM(MACOS)
            getData().process = ChildProcess{ U"data/Games/3DViewer/3DViewer.app/Contents/MacOS/3DViewer" };
            # endif
        }
    }
    else{
        if(ToMusic.mouseOver() || ToGame.mouseOver() || To3DCG.mouseOver()){
            Cursor::RequestStyle(CursorStyle::Hand);
        }

        if(ToMusic.leftReleased()){
            Running = U"Music";
            ChildRunning = true;
            watch.restart();
            MusicAnimation.restart();
        }
        if(ToGame.leftReleased()){
            Running = U"Game";
            ChildRunning = true;
            watch.restart();
            GameAnimation.restart();
        }
        if(To3DCG.leftReleased()){
            Running = U"3DCG";
            ChildRunning = true;
            watch.restart();
            CGAnimation.restart();
        }
    }
}

/*draw---------------------------------------------------------------------*/
void Title::draw() const {
    const ScopedColorMul2D colorMul{ ColorF(1, StartAnimation[U"Fade"]) };
    for(auto i : step(3)){
        if(ChildRunning){
            if(Running == U"Music") TextureAsset(U"Shape").drawAt(MusicAnimation[U"Shape" + ToString(i)], 250);
            else if(Running == U"Game") TextureAsset(U"Shape").drawAt(GameAnimation[U"Shape" + ToString(i)], 250);
            else if(Running == U"3DCG") TextureAsset(U"Shape").drawAt(CGAnimation[U"Shape" + ToString(i)], 250);
        }
        else TextureAsset(U"Shape").drawAt(OpenAnimation[U"Shape" + ToString(i)], 250);
    }
    if(ChildRunning){
        if(Running == U"Music"){
            TextureAsset(U"Char_Music").drawAt(240, 220, ColorF(1, MusicAnimation[U"Fade"]));
            TextureAsset(U"Char_Game").drawAt(640, 220, ColorF(1, MusicAnimation[U"Fade"]));
            TextureAsset(U"Char_3DCG").drawAt(1040, 220, ColorF(1, MusicAnimation[U"Fade"]));
        }
        else if(Running == U"Game"){
            TextureAsset(U"Char_Music").drawAt(240, 220, ColorF(1, GameAnimation[U"Fade"]));
            TextureAsset(U"Char_Game").drawAt(640, 220, ColorF(1, GameAnimation[U"Fade"]));
            TextureAsset(U"Char_3DCG").drawAt(1040, 220, ColorF(1, GameAnimation[U"Fade"]));
        }
        else if(Running == U"3DCG"){
            TextureAsset(U"Char_Music").drawAt(240, 220, ColorF(1, CGAnimation[U"Fade"]));
            TextureAsset(U"Char_Game").drawAt(640, 220, ColorF(1, CGAnimation[U"Fade"]));
            TextureAsset(U"Char_3DCG").drawAt(1040, 220, ColorF(1, CGAnimation[U"Fade"]));
        }
    }
    else{
        TextureAsset(U"Char_Music").drawAt(240, 220, ColorF(1, OpenAnimation[U"Fade"]));
        TextureAsset(U"Char_Game").drawAt(640, 220, ColorF(1, OpenAnimation[U"Fade"]));
        TextureAsset(U"Char_3DCG").drawAt(1040, 220, ColorF(1, OpenAnimation[U"Fade"]));
    }
    TextureAsset(U"LogoR").mirrored().drawAt(OpenAnimation[U"Side_l"], 500);
    TextureAsset(U"LogoR").drawAt(OpenAnimation[U"Side_r"], 500);
    TextureAsset(U"Char_Detail_l").drawAt(OpenAnimation[U"Side_l"] + 16, 345);
    TextureAsset(U"Char_Detail_r").drawAt(OpenAnimation[U"Side_r"] - 16, 345);

    // StartButton.draw();
    TextureAsset(U"GamePlay").drawAt(640, 535, ColorF(1, OpenAnimation[U"FadeOut"]));

    TextureAsset(U"Char_Computer").drawAt(640, OpenAnimation[U"Computer"]);
    TextureAsset(U"CopyRight").drawAt(640, 690);
}

void Title::drawFadeIn(double t) const {
    draw();
}
void Title::drawFadeOut(double t) const {
    draw();
}