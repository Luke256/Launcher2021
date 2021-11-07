#include "Musics.hpp"

Musics::Musics(const InitData& init) : IScene(init){
    QuitTransition = Transition(.1s, .1s);
    album = getData().Albums[getData().SelectedIndex];
    Selecting = -1;
    Scroll = 0;
    isPlaying = false;
    playing_music_index = -1;
}

/*update---------------------------------------------------------------------*/
void Musics::update() {
    ClearPrint();
    // Print << playing_music;
    // Print << AudioAsset::IsRegistered(playing_music);
    // Print << AudioAsset::IsReady(playing_music);
    QuitTransition.update(Quit.mouseOver());
    if(Quit.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
    if(Quit.leftReleased()){
        // アルバム内の曲をメモリから解放
        for(auto name : album.musics){
            AudioAsset(name.music).release();
        }
        changeScene(U"Albums");
    }

    for(auto [index, music] : Indexed(album.musics)){
        const RectF itemErea(UI::ItemBase.x, UI::ItemBase.y + UI::ItemSize.y * index - Scroll, UI::ItemSize);
        if(itemErea.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
        if(itemErea.leftReleased()){
            Selecting = index;
        }
    }

    {
        const Transformer2D t2(Mat3x2::Translate(UI::Detail.pos), TransformCursor::Yes);
        if(UI::Playnow.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
        if(UI::Playnow.leftReleased()) {
            getData().Player.Playnow(album.Name + U"." + album.musics[Selecting].Title);
        }
        if(UI::AddQue.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
        if(UI::AddQue.leftReleased()) {
            getData().Player.Append(album.Name + U"." + album.musics[Selecting].Title);
        }
    }

    getData().Player.update();

    // if(isPlaying && AudioAsset::IsReady(playing_music)){
    //     AudioAsset(playing_music).play();
    // }
    // else {
    //     AudioAsset(playing_music).pause();
    // }


    // {
    //     const Transformer2D t2(Mat3x2::Translate(UI::TuneErea.scaled(.9).pos), TransformCursor::Yes);
    //     if(UI::ToPrevMusic.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
    //     if(UI::StartStop.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
    //     if(UI::ToNextMusic.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);

    //     if(UI::StartStop.leftReleased()){
    //         if(playing_music == album.Name + U"." + album.musics[Selecting].Title || playing_music == U"") isPlaying = !isPlaying;
    //         else AudioAsset(playing_music).stop();
    //         playing_music_index = Selecting;
    //         playing_music = album.Name + U"." + album.musics[playing_music_index].Title;
    //         AudioAsset::LoadAsync(playing_music);
    //         // AudioAsset(playing_music).play();
    //     }
    //     if(UI::ToPrevMusic.leftReleased()){
    //         if(AudioAsset(playing_music).posSec() < 3){ // 前の曲に
    //             AudioAsset(playing_music).stop();
    //             playing_music_index = (playing_music_index - 1 + album.musics.size()) % album.musics.size();
    //             playing_music = album.Name + U"." + album.musics[playing_music_index].Title;
    //             AudioAsset::LoadAsync(playing_music);
    //             // AudioAsset(playing_music).play();
    //         }
    //         else{ // 曲の先頭に
    //             AudioAsset(playing_music).seekSamples(0);
    //         }
    //     }
    //     if(UI::ToNextMusic.leftReleased()){ // 次の曲に
    //         AudioAsset(playing_music).stop();
    //         playing_music_index = (playing_music_index + 1) % album.musics.size();
    //         playing_music = album.Name + U"." + album.musics[playing_music_index].Title;
    //         AudioAsset::LoadAsync(playing_music);
    //         // AudioAsset(playing_music).play();
    //     }
    // }


    Scroll += Mouse::Wheel() * 10;
    Scroll = Min(((double)album.musics.size() - 1) * UI::ItemSize.y, Scroll);
    Scroll = Max(0.0, Scroll);
}

/*draw---------------------------------------------------------------------*/
void Musics::draw() const {

    for(auto [index, music] : Indexed(album.musics)){
        drawItem(music.Title, index);
    }

    Line(500, 80, 500, Scene::Height()).draw(5, Color(230, 176, 0));
    UI::Detail.draw(ColorF(.15));
    if(Selecting+1)
    {
        const ScopedViewport2D viewport(UI::Detail);
        const Transformer2D ct(Mat3x2::Identity(), Mat3x2::Translate(UI::Detail.pos));
        if(TextureAsset(album.Name)) UI::ImageErea.scaled(.81)(TextureAsset(album.Name)).draw();
        else{
            UI::ImageErea.scaled(.81).draw(ColorF(0.6));
            FontAsset(U"Albums.Title")(U"No Image").drawAt(UI::ImageErea.center(), ColorF(0.9));
        }
        FontAsset(U"Albums.Title2")(U"タイトル").draw(Arg::bottomLeft(UI::TitleErea.scaled(.81).pos));
        FontAsset(U"Albums.Title")(album.musics[Selecting].Title).draw(UI::TitleErea.scaled(.81));
        FontAsset(U"Albums.Title2")(U"クレジット").draw(Arg::bottomLeft(UI::StaffErea.scaled(.81).pos));
        FontAsset(U"Albums.Detail")(album.musics[Selecting].Staff).draw(UI::StaffErea.scaled(.81));
        FontAsset(U"Albums.Title2")(U"説明").draw(Arg::bottomLeft(UI::DescErea.scaled(.9).pos));
        FontAsset(U"Albums.Detail")(album.musics[Selecting].Desc).draw(UI::DescErea.scaled(.9));
        FontAsset(U"Albums.Title2")(U"アルバム - アーティスト").draw(Arg::bottomLeft(UI::HeadErea.scaled(.9).pos));
        FontAsset(U"Albums.Head")(album.Name + U" - " + album.Auth).draw(UI::HeadErea.scaled(.9));

        if(UI::Playnow.mouseOver()) UI::Playnow.draw(ColorF(1,1,1,.1));
        FontAsset(U"Albums.Title")(U"\xF040A").drawAt(UI::Playnow.center);
        if(UI::AddQue.mouseOver()) UI::AddQue.draw(ColorF(1,1,1,.1));
        FontAsset(U"Albums.Title")(U"\xF0412").drawAt(UI::AddQue.center);

        // UI::ImageErea.scaled(.81).draw(ColorF(1, 0, 0, 0.3));
        // UI::TitleErea.scaled(.81).draw(ColorF(1, 0, 0, 0.3));
        // UI::StaffErea.scaled(.81).draw(ColorF(1, 0, 0, 0.3));
        // UI::DescErea.scaled(.9).draw(ColorF(1, 0, 0, 0.3));
        // UI::HeadErea.scaled(.9).draw(ColorF(1, 0, 0, 0.3));
    }

    Rect(0, 0, Scene::Width(), 80).draw(ColorF(.1));
    Line(0, 80, Scene::Width(), 80).draw(5, Color(230, 176, 0));


    getData().Player.draw();
    // UI::TuneErea.scaled(.9).draw(ColorF(1, 0, 0, 0.3));
    // {
    //     const Transformer2D t2(Mat3x2::Translate(UI::TuneErea.scaled(.9).pos), TransformCursor::Yes);
    //     if(UI::ToPrevMusic.mouseOver()) UI::ToPrevMusic.draw(ColorF(1,1,1,.1));
    //     if(UI::StartStop.mouseOver()) UI::StartStop.draw(ColorF(1,1,1,.1));
    //     if(UI::ToNextMusic.mouseOver()) UI::ToNextMusic.draw(ColorF(1,1,1,.1));
    //     FontAsset(U"Albums.Detail")(U"\xF04AB").drawAt(UI::ToPrevMusic.center);
    //     if(!isPlaying) FontAsset(U"Albums.Detail")(U"\xF040A").drawAt(UI::StartStop.center);
    //     else FontAsset(U"Albums.Detail")(U"\xF03E4").drawAt(UI::StartStop.center);
    //     FontAsset(U"Albums.Detail")(U"\xF04AC").drawAt(UI::ToNextMusic.center);
    // }
    Quit.rounded(10).draw(ColorF(.1 + QuitTransition.value() * .2));
    FontAsset(U"Albums.Detail")(U"\xF004D もどる").drawAt(Quit.center());
}

void Musics::drawItem(String name, int32 index) const{
    const RectF ItemErea(UI::ItemBase.x, UI::ItemBase.y + UI::ItemSize.y * index - Scroll, UI::ItemSize);
    ItemErea.scaled(.9).rounded(10).draw();
    FontAsset(U"Albums.Title")(name).draw(ItemErea.scaled(.9).stretched(-10,-5,-5,-10), ColorF(0.2));
}