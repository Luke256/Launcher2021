# include <Siv3D.hpp> // OpenSiv3D v0.6.0
# include "Main.hpp"
# include "Albums.hpp"
# include "Musics.hpp"
# include "Title.hpp"

Array<Album> LoadMusics(String CurrentDirectory){
    Array<Album> res;
    const String HomeDirectory = CurrentDirectory + U"Albums/";
    for(auto dir : FileSystem::DirectoryContents(HomeDirectory, Recursive::No)){
        if(!FileSystem::IsDirectory(dir))continue;
        Album album;
        const INI album_info(dir + U"album.ini");
        album.Jacket = dir + album_info.get<String>(U"Album.image");
        album.Name = album_info.get<String>(U"Album.title");
        album.Auth = album_info.get<String>(U"Album.name");
        album.Desc = album_info.get<String>(U"Album.desc");
        TextureAsset::Register(album.Name, album.Jacket);
        TextureAsset::LoadAsync(album.Name);

        for(auto detail : FileSystem::DirectoryContents(dir, Recursive::No)){
            if(!FileSystem::IsDirectory(detail)) continue;

            Music music;
            const INI detail_info(detail + U"config.ini");
            music.Desc = detail_info.get<String>(U"Music.desc");
            music.music = detail + detail_info.get<String>(U"Music.path");
            music.Staff = detail_info.get<String>(U"Music.staff");
            music.Tools = detail_info.get<String>(U"Music.tools");
            music.Title = detail_info.get<String>(U"Music.title");
            AudioAsset::Register(album.Name+U"."+music.Title, music.music);
            album.musics << music;
        }

        res << album;
    }

    res.sort_by([](const Album& a, const Album& b) { return a.Name < b.Name; });

    return res;
}

void Main()
{
    Window::Resize(GameInfo::Width, GameInfo::Height);
    Window::SetTitle(U"Music Room");
    Scene::SetBackground(ColorF(0.1));

    AsyncTask loadTask(LoadMusics, FileSystem::CurrentDirectory());
    bool done = false;

    App manager;
    manager.add<Title>(U"Title");
    manager.add<Albums>(U"Albums");
    manager.add<Musics>(U"Musics");


    const Font iconFont(36, Typeface::Icon_MaterialDesign);
    FontAsset::Register(U"Albums.Head", 42);
    FontAsset::Register(U"Albums.Title", 36, Typeface::Bold);
    FontAsset::Register(U"Player.Title", 36);
    FontAsset::Register(U"Albums.Detail", 24);
    FontAsset::Register(U"Albums.Title2", 24, Typeface::Bold);
    FontAsset(U"Albums.Title").addFallback(iconFont);
    FontAsset(U"Albums.Detail").addFallback(iconFont);

    TextureAsset::Register(U"BackGround", U"Assets/BackGround.png");
    TextureAsset::Register(U"Title", U"Assets/Title.png");


	while(System::Update())
	{
        TextureAsset(U"BackGround").draw(0, 0);
        if(loadTask.isReady()){
            manager.get()->Albums = loadTask.get();
            done = true;
        }
        if(done){
            if(!manager.update()) break;
        }
        else{
            Circle(GameInfo::Width / 2, GameInfo::Height / 2, 50).drawArc(Scene::Time() * 120_deg, 240_deg, 10);
        }
	}
}
