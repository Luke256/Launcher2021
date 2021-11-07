# pragma once
# include <Siv3D.hpp>

inline namespace Tools{
    struct MusicPlayer{
        const Rect DrawErea = Rect(Arg::center(640, 40), 800, 80);
        const Circle ToPrevMusic = Circle(DrawErea.h / 2 * 0.9, DrawErea.h / 2 * 0.9, 20);
        const Circle StartStop = Circle(ToPrevMusic.x + 50, ToPrevMusic.y, 20);
        const Circle ToNextMusic = Circle(ToPrevMusic.x + 100, ToPrevMusic.y, 20);
        const RectF SeekBar = RectF(0, 0, DrawErea.w * 0.9, 10);
        const Rect NameErea = Rect(230, 10, 400, 62);
        const Rect ShowPlayList = Rect(650 + DrawErea.x + 50, 15, 50, 50);
        const Rect PlayListBox = Rect(500 + DrawErea.x + 50, 80, 400, 600);

        Array<String>History;
        int32 PlayingIndex;
        bool isPlaying;
        bool Played;
        Transition SeekBarTransition;

        Optional<AsyncTask<void>>lpTask;

        MusicPlayer();
        void update();
        void draw() const;
        void Playnow(String name);
        void Append(String name);
    };
};
