#pragma once
# include <Siv3D.hpp>
# include "Tools.hpp"

struct Music{
    String Title;
    String Desc;
    String Tools;
    String Staff;
    String music;
};

struct Album{
    String Jacket;
    String Name;
    String Auth;
    String Desc;
    Array<Music>musics;
};

struct GameData{
    Array<Album>Albums;
    int32 SelectedIndex;
    Tools::MusicPlayer Player;
};

using App = SceneManager<String, GameData>;

namespace GameInfo{
    const int32 Width = 1280;
    const int32 Height = 720;
};

namespace Detail{
    const Vec2 ItemBase = Vec2(80, 100);
    const Vec2 ItemSize = Vec2(360, 340);
    const RectF detail(ItemBase.x * 2 + ItemSize.x * 2, 80, GameInfo::Width - ItemBase.x * 2 - ItemSize.x * 2, GameInfo::Height - 80);
    const RectF imageErea(detail.x + 20, detail.y + 20, detail.w - 40, (detail.w - 20) / 3 * 2);
    const RectF TitleErea(imageErea.bl(), imageErea.w, 60);
    const RectF descErea(TitleErea.bl(), imageErea.w, 150);
    const RectF StaffErea(descErea.bl(), imageErea.w / 2, 100);
    const RectF ToolsErea(StaffErea.tr(), imageErea.w / 2, 100);
    const RectF ShowButton(StaffErea.bl(), imageErea.w, 50);
};
