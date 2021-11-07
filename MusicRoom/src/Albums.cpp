#include "Albums.hpp"

Albums::Albums(const InitData& init) : IScene(init){
    Scroll = 0;
    Selecting = -1;
    QuitTransition = Transition(.1s, .1s);
}

/*update---------------------------------------------------------------------*/
void Albums::update() {
    QuitTransition.update(Quit.rounded(10).mouseOver());
    for(auto [index, item] : Indexed(getData().Albums)){
        const RectF ItemErea(ItemBase + ItemSize * Vec2(index % 2, index / 2) - Vec2(0, Scroll), ItemSize);
        if(ItemErea.scaled(0.9).rounded(10).mouseOver()){
            Cursor::RequestStyle(CursorStyle::Hand);
        }
        if(ItemErea.scaled(0.9).rounded(10).leftReleased()){
            Selecting = index;
        }
    }
    if(Selecting + 1 && Detail::ShowButton.scaled(0.9).rounded(10).mouseOver()){
        Cursor::RequestStyle(CursorStyle::Hand);
    }
    if(Selecting + 1 && Detail::ShowButton.scaled(0.9).rounded(10).leftReleased()){
        getData().SelectedIndex = Selecting;
        changeScene(U"Musics");
    }
    if(Quit.rounded(10).mouseOver()){
        Cursor::RequestStyle(CursorStyle::Hand);
    }
    if(Quit.rounded(10).leftReleased()){
        System::Exit();
    }

    getData().Player.update();

    Scroll += Mouse::Wheel() * 10;
    Scroll = Min(Floor(((double)getData().Albums.size() - 1) / 2) * ItemSize.y, Scroll);
    Scroll = Max(0.0, Scroll);
}

/*draw---------------------------------------------------------------------*/
void Albums::draw() const {
    for(auto[index, item] : Indexed(getData().Albums)){
        drawItem(item, index);
    }

    Detail::detail.draw(ColorF(.15));
    Line(ItemBase.x * 2 + ItemSize.x * 2, 80, ItemBase.x * 2 + ItemSize.x * 2, Scene::Height()).draw(3, Color(230, 176, 0));
    if(Selecting + 1){
        const auto item = getData().Albums[Selecting];
        if(TextureAsset(item.Name)) TextureAsset(item.Name).resized(Detail::imageErea.h, Detail::imageErea.h).drawAt(Detail::imageErea.center());
        else{
            Detail::imageErea.draw(ColorF(0.6));
            FontAsset(U"Albums.Title")(U"No Image").drawAt(Detail::imageErea.center(), ColorF(0.9));
        }
        FontAsset(U"Albums.Title")(item.Name).draw(Detail::TitleErea.scaled(0.9));
        FontAsset(U"Albums.Title2")(U"説明").draw(Arg::bottomLeft = Detail::descErea.scaled(0.9).stretched(-30, 0, -10, 0).tl());
        FontAsset(U"Albums.Detail")(item.Desc).draw(Detail::descErea.scaled(0.9).stretched(-30, 0, -10, 0));
        FontAsset(U"Albums.Title2")(U"アーティスト").draw(Arg::bottomLeft = Detail::StaffErea.scaled(0.8).tl());
        FontAsset(U"Albums.Detail")(item.Auth).draw(Detail::StaffErea.scaled(0.8));
        FontAsset(U"Albums.Title2")(U"収録曲").draw(Arg::bottomLeft = Detail::ToolsErea.scaled(0.8).tl());
        FontAsset(U"Albums.Detail")(item.musics.size()).draw(Detail::ToolsErea.scaled(0.8));
        Detail::ShowButton.scaled(0.9).rounded(10).draw(Color(11, 156, 230));
        FontAsset(U"Albums.Title")(U"\xF135A 曲一覧へ").drawAt(Detail::ShowButton.center());
    }

    Rect(0, 0, Scene::Width(), 80).draw(ColorF(.1));
    Line(0, 80, Scene::Width(), 80).draw(5, Color(230, 176, 0));
    getData().Player.draw();
    FontAsset(U"Albums.Head")(U"アルバム一覧").draw(10, 10, ColorF(0.9));

    Quit.rounded(10).draw(ColorF(.1 + QuitTransition.value() * .2));
    FontAsset(U"Albums.Title")(U"\xF0206 終了").drawAt(Quit.center());
}

void Albums::drawItem(const Album &item, const int32 index) const {
    const RectF ItemErea(ItemBase + ItemSize * Vec2(index % 2, index / 2) - Vec2(0, Scroll), ItemSize);
    ItemErea.scaled(0.9).rounded(10).draw();
    {
        const ScopedViewport2D viewport(ItemErea.scaled(0.9).asRect());
        const auto imageErea = RectF(Vec2(0, 0), ItemSize * 0.9 - Vec2(0, 100));
        const auto HeadErea = RectF(Vec2(0, ItemErea.scaled(0.9).h - 100), Vec2(ItemSize.x * 0.9, 60));
        const auto AuthErea = RectF(Vec2(0, ItemErea.scaled(0.9).h - 40), Vec2(ItemSize.x * 0.9, 40));
        if(TextureAsset(item.Name)){
            TextureAsset(item.Name).resized(imageErea.h, imageErea.h).drawAt(imageErea.center());
        }
        else{
            imageErea.rounded(10,10,0,0).draw(ColorF(0.6));
            FontAsset(U"Albums.Title")(U"No Image").drawAt(imageErea.center(), ColorF(0.9));
        }
        FontAsset(U"Albums.Title")(item.Name).draw(HeadErea.scaled(0.9), ColorF(0.2));
        FontAsset(U"Albums.Detail")(item.Auth).draw(AuthErea.scaled(0.9), ColorF(0.2));
    }
}
