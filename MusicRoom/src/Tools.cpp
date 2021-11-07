#include "Tools.hpp"


void LoadMusic(String name){
    if(!AudioAsset::IsRegistered(name)) return;
    AudioAsset::LoadAsync(name);
    while(!AudioAsset::IsReady(name)){
        System::Sleep(0.1s);
    }
    AudioAsset(name).seekSamples(0);
}

namespace Tools{
    MusicPlayer::MusicPlayer(){
        isPlaying = false;
        PlayingIndex = 0;
        History << U".";
        SeekBarTransition = Transition(0.5s, .5s);
        Played = false;
    }

    void MusicPlayer::update(){
        // Print << History;
        // Print << History;
        // Print << U"pos : " << AudioAsset(History[PlayingIndex]).posSample();
        // Print << U"all : " << (double)AudioAsset(History[PlayingIndex]).samples();
        // Print << U"played : " << AudioAsset(History[PlayingIndex]).samplesPlayed();
        // Print << U"Active : " << AudioAsset(History[PlayingIndex]).isActive();
        if(ShowPlayList.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
        if(AudioAsset(History[PlayingIndex]).posSample() == 0 && Played){
            // Console << U"End";
            if(!AudioAsset(History[PlayingIndex]).isLoop()){ // ループ再生なら無視
                // 再生終了
                // Console << U"NotLoop";
                if(PlayingIndex < History.size() - 1){
                    Console << U"Next";
                    ++PlayingIndex;
                    lpTask->~AsyncTask();
                    lpTask = Async(LoadMusic, History[PlayingIndex]);
                    isPlaying = false;
                }
                else isPlaying = false;
                AudioAsset(History[PlayingIndex]).stop();
            }
        }

        if(lpTask && lpTask->isReady()){
            lpTask->get();
            AudioAsset(History[PlayingIndex]).play();
            Played = false;
            isPlaying = true;
        }
        if(!Played && AudioAsset(History[PlayingIndex]).posSample()){
            Played = true;
        }

        {
            const Transformer2D t2(Mat3x2::Translate(DrawErea.scaled(.9).pos), TransformCursor::Yes);
            SeekBarTransition.update(SeekBar.mouseOver());
            if(ToPrevMusic.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
            if(StartStop.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
            if(ToNextMusic.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);

            if(StartStop.leftReleased()){
                isPlaying = !isPlaying;
                if(isPlaying) AudioAsset(History[PlayingIndex]).play();
                else AudioAsset(History[PlayingIndex]).pause();
            }
            if(ToPrevMusic.leftReleased()){
                if(AudioAsset(History[PlayingIndex]).posSec() < 3 && PlayingIndex > 1){ // 前の曲に
                    AudioAsset(History[PlayingIndex]).stop();
                    PlayingIndex = Max(0, PlayingIndex - 1);
                    lpTask->~AsyncTask();
                    Played = false;
                    lpTask = Async(LoadMusic, History[PlayingIndex]);
                    // AudioAsset(History[PlayingIndex]).setLoop(false);
                }
                else{ // 曲の先頭に
                    AudioAsset(History[PlayingIndex]).seekSamples(0);
                }
            }
            if(ToNextMusic.leftReleased()){ // 次の曲に
                AudioAsset(History[PlayingIndex]).seekSamples(AudioAsset(History[PlayingIndex]).samples() - 1);
            }

            if(SeekBar.leftClicked()){
                AudioAsset(History[PlayingIndex]).pause();
            }
            else if(SeekBar.leftReleased()){
                double seek = Cursor::Pos().x / (double)SeekBar.w * AudioAsset(History[PlayingIndex]).samples();
                AudioAsset(History[PlayingIndex]).seekSamples(seek);
                AudioAsset(History[PlayingIndex]).play();
            }
        }
    }

    void MusicPlayer::draw()const{
        DrawErea.scaled(.9).draw(ColorF(1, 0, 0, 0.3));
        {
            const Transformer2D t2(Mat3x2::Translate(DrawErea.scaled(.9).pos), TransformCursor::Yes);
            if(ToPrevMusic.mouseOver()) ToPrevMusic.draw(ColorF(1,1,1,.1));
            if(StartStop.mouseOver()) StartStop.draw(ColorF(1,1,1,.1));
            if(ToNextMusic.mouseOver()) ToNextMusic.draw(ColorF(1,1,1,.1));
            FontAsset(U"Albums.Detail")(U"\xF04AB").drawAt(ToPrevMusic.center);
            if(!isPlaying || SeekBar.leftPressed()) FontAsset(U"Albums.Detail")(U"\xF040A").drawAt(StartStop.center);
            else FontAsset(U"Albums.Detail")(U"\xF03E4").drawAt(StartStop.center);
            FontAsset(U"Albums.Detail")(U"\xF04AC").drawAt(ToNextMusic.center);

            if(AudioAsset::IsReady(History[PlayingIndex])){
                int32 sec = AudioAsset(History[PlayingIndex]).posSec();
                FontAsset(U"Albums.Detail")(U"{}:{:0>2}"_fmt(sec/60, sec%60)).draw(Arg::leftCenter(ToNextMusic.x + 30, ToNextMusic.y), ColorF(0.9));
            }
            else{
                FontAsset(U"Albums.Detail")(U"-:--").draw(Arg::leftCenter(ToNextMusic.x + 30, ToNextMusic.y), ColorF(0.9));
            }


            SeekBar.stretched(0, 0, -EaseInQuint(1 - SeekBarTransition.value()) * 6, 0).draw(ColorF(0.6));
            const double Progress = AudioAsset(History[PlayingIndex]).posSample() / (double)AudioAsset(History[PlayingIndex]).samples();
            SeekBar.stretched(0, (Progress - 1) * SeekBar.w, -EaseInQuint(1 - SeekBarTransition.value()) * 6, 0).draw(ColorF(1, .2, .2));
        }

        {
            if(PlayingIndex){
                const ScopedViewport2D viewport{ NameErea.x + DrawErea.x + 50, NameErea.y, NameErea.w, NameErea.h };
                const RectF region = FontAsset(U"Player.Title")(History[PlayingIndex].replaced(U".", U" - ")).region();
                const int32 pos_x = int32(Scene::Time() * 100) % int32(region.w * 2 + NameErea.w);
                FontAsset(U"Player.Title")(History[PlayingIndex].replaced(U".", U" - ")).draw(Arg::rightCenter(NameErea.w - pos_x + region.w, 33));
            }
        }

        // ShowPlayList.draw();
        FontAsset(U"Albums.Detail")(U"\xF0CB8").drawAt(ShowPlayList.center());

        if(ShowPlayList.leftPressed()){
            PlayListBox.draw(ColorF(1, 0.8));
            {
                const ScopedViewport2D viewport(PlayListBox);
                for(int32 index = 0; auto Name : History){
                    if(!index){
                        ++index;
                        continue;
                    }

                    if(index == PlayingIndex) FontAsset(U"Albums.Title2")(Name.replaced(U".", U" - ")).draw(RectF(Vec2(20, 20 + (index - 1) * 40), PlayListBox.w - 40, 40), ColorF(0.2));
                    FontAsset(U"Albums.Detail")(Name.replaced(U".", U" - ")).draw(RectF(Vec2(20, 20 + (index - 1) * 40), PlayListBox.w - 40, 40), ColorF(0.2));
                    // Print << index << U" : " << Name;
                    ++index;
                }
            }
        }
    }

    void MusicPlayer::Playnow(String name){
        AudioAsset(History[PlayingIndex]).stop();
        History.insert(History.begin() + PlayingIndex + 1, name);
        ++PlayingIndex;
        isPlaying = true;
        lpTask->~AsyncTask();
        lpTask = Async(LoadMusic, History[PlayingIndex]);
        // Console << lpTask->isValid();
        // while(!(lpTask->isReady())){}
        // // Console << U"Hi";
        // AudioAsset(History[PlayingIndex]).setLoop(false);
    }

    void MusicPlayer::Append(String name){
        History << name;
        isPlaying = true;
    }
};
