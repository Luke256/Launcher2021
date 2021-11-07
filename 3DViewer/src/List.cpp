#include "List.hpp"

List::List(const InitData& init) : IScene(init){
    if(!getData().Loaded){ // モデルのロード
        const FilePath homedirectory = FileSystem::CurrentDirectory() + U"Models/";

        for(auto &modelDirectory : FileSystem::DirectoryContents(homedirectory, Recursive::No)){
            if(!FileSystem::IsDirectory(modelDirectory)) continue;
            const INI ini(modelDirectory + U"config.ini");
            if(ini.isEmpty()) continue;

            Summary summary;
            summary.Desc = ini.get<String>(U"Model.desc");
            summary.image = Texture(modelDirectory + ini.get<String>(U"Model.image"), TextureDesc::Mipped);
            summary.ModelPath = modelDirectory + ini.get<String>(U"Model.path");
            summary.Name = ini.get<String>(U"Model.title");
            summary.Staff = ini.get<String>(U"Model.staff");
            summary.Tools = ini.get<String>(U"Model.tools");
            summary.Type = (ini.get<String>(U"Model.Type") == U"2D" ? 0 : 1);

            getData().Dataset << summary;
        }
        getData().Loaded = true;
    }
    Scroll = 0;
    Selecting = -1;
    QuitTransition = Transition(.1s, .1s);
}

/*update---------------------------------------------------------------------*/
void List::update() {
    QuitTransition.update(Quit.rounded(10).mouseOver());
    for(auto [index, item] : Indexed(getData().Dataset)){
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
        if(getData().Dataset[Selecting].Type == 0){
            changeScene(U"View2D");
        }
        else{
            changeScene(U"View");
        }
    }
    if(Quit.rounded(10).mouseOver()){
        Cursor::RequestStyle(CursorStyle::Hand);
    }
    if(Quit.rounded(10).leftReleased()){
        System::Exit();
    }

    Scroll += Mouse::Wheel() * 10;
    Scroll = Min((getData().Dataset.size() - 1) / 2 * ItemSize.y, Scroll);
    Scroll = Max(0.0, Scroll);
}

/*draw---------------------------------------------------------------------*/
void List::draw() const {
    for(auto[index, item] : Indexed(getData().Dataset)){
        drawItem(item, index);
    }

    Detail::detail.draw(ColorF(.15));
    Line(ItemBase.x * 2 + ItemSize.x * 2, 80, ItemBase.x * 2 + ItemSize.x * 2, Scene::Height()).draw(3, Color(230, 176, 0));
    if(Selecting + 1){
        const auto item = getData().Dataset[Selecting];
        if(item.image) Detail::imageErea.scaled(0.9)(item.image).draw();
        else{
            Detail::imageErea.draw(ColorF(0.6));
            FontAsset(U"List.Title")(U"No Image").drawAt(Detail::imageErea.center(), ColorF(0.9));
        }
        FontAsset(U"List.Title")(item.Name).draw(Detail::TitleErea.scaled(0.9));
        FontAsset(U"List.Title2")(U"説明").draw(Arg::bottomLeft = Detail::descErea.scaled(0.9).stretched(-30, 0, -10, 0).tl());
        FontAsset(U"List.Detail")(item.Desc).draw(Detail::descErea.scaled(0.9).stretched(-30, 0, -10, 0));
        FontAsset(U"List.Title2")(U"クレジット").draw(Arg::bottomLeft = Detail::StaffErea.scaled(0.8).tl());
        FontAsset(U"List.Detail")(item.Staff).draw(Detail::StaffErea.scaled(0.8));
        FontAsset(U"List.Title2")(U"使用ツール").draw(Arg::bottomLeft = Detail::ToolsErea.scaled(0.8).tl());
        FontAsset(U"List.Detail")(item.Tools).draw(Detail::ToolsErea.scaled(0.8));
        // Detail::TitleErea.scaled(0.9).draw(ColorF(1, 0, 0, 0.3));
        // Detail::descErea.scaled(0.9).stretched(-30, 0, -10, 0).draw(ColorF(1, 0, 0, 0.3));
        // Detail::StaffErea.scaled(0.8).draw(ColorF(1, 0, 0, 0.3));
        // Detail::ToolsErea.scaled(0.8).draw(ColorF(1, 0, 0, 0.3));
        Detail::ShowButton.scaled(0.9).rounded(10).draw(Color(11, 156, 230));
        if(item.Type == 1){
            FontAsset(U"List.Title")(U"\xF0EC7 3Dで見る").drawAt(Detail::ShowButton.center());
        }
        else{
            FontAsset(U"List.Title")(U"\xF0977 2Dで見る").drawAt(Detail::ShowButton.center());
        }
    }

    Rect(0, 0, Scene::Width(), 80).draw(ColorF(.1));
    Line(0, 80, Scene::Width(), 80).draw(5, Color(230, 176, 0));
    FontAsset(U"List.Head")(U"3D/2Dモデル一覧").draw(10, 10, ColorF(0.9));

    Quit.rounded(10).draw(ColorF(.1 + QuitTransition.value() * .2));
    FontAsset(U"List.Title")(U"\xF0206 終了").drawAt(Quit.center());
}


void List::drawItem(const Summary &item, const int32 index) const {
    const RectF ItemErea(ItemBase + ItemSize * Vec2(index % 2, index / 2) - Vec2(0, Scroll), ItemSize);
    ItemErea.scaled(0.9).rounded(10).draw();
    {
        const ScopedViewport2D viewport(ItemErea.scaled(0.9).asRect());
        const auto imageErea = RectF(Vec2(0, 0), ItemSize * 0.9 - Vec2(0, 100));
        const auto HeadErea = RectF(Vec2(0, ItemErea.scaled(0.9).h - 100), Vec2(ItemSize.x * 0.9, 60));
        const auto AuthErea = RectF(Vec2(0, ItemErea.scaled(0.9).h - 40), Vec2(ItemSize.x * 0.9, 40));
        if(item.image){
            imageErea.rounded(10,10,0,0).toBuffer2D(Vec2(0, 0), ItemSize * 0.9 - Vec2(0, 100)).draw(item.image);
        }
        else{
            imageErea.rounded(10,10,0,0).draw(ColorF(0.6));
            FontAsset(U"List.Title")(U"No Image").drawAt(imageErea.center(), ColorF(0.9));
        }
        FontAsset(U"List.Title")(item.Name).draw(HeadErea.scaled(0.9), ColorF(0.2));
        FontAsset(U"List.Detail")(item.Staff).draw(AuthErea.scaled(0.9), ColorF(0.2));
    }
}
