#pragma once
#include <Siv3D.hpp>

struct Summary{
    Texture image;
    FilePath ModelPath;
    String Desc;
    String Staff;
    String Tools;
    String Name;
    size_t Type; // 0:2D 1:3D
};


struct GameData{
    Array<Summary>Dataset;
    bool Loaded = false;
    int32 SelectedIndex;
};

using App = SceneManager<String, GameData>;

namespace GameInfo{
    const int32 Width = 1280;
    const int32 Height = 720;
    const String Title = U"3DViewer";
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
