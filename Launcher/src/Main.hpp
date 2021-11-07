#include <Siv3D.hpp>
#pragma once

struct Product{
    String path;
    String title;
    String image;
    String Desc;
    String Staff;
    String Tools;
    String Time;
};

struct GameData{
    Optional<ChildProcess> process;
    Array<Product>Products;
    Stopwatch watch;
    bool initial = true;
    int32 TimeLimit;
};

using App = SceneManager<String, GameData>;

namespace GameInfo{
    const int32 Width = 1280;
    const int32 Height = 720;
    const String Title = U"2021年度海城コンピュータ部　ゲームランチャー";
};
