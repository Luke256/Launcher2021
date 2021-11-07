#include "Title.hpp"

Title::Title(const InitData& init) : IScene(init){
    watch = Stopwatch();
    watch.start();
}

/*update---------------------------------------------------------------------*/
void Title::update() {
    if(watch.sF() >= 3) changeScene(U"Albums");
}

/*draw---------------------------------------------------------------------*/
void Title::draw() const {
    TextureAsset(U"Title").draw(0, 0);
}