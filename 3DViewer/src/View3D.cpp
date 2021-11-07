#include "View3D.hpp"

View::View(const InitData& init) : IScene(init){
    camera = DebugCamera3D{ Graphics3D::GetRenderTargetSize(), 25_deg, Vec3{ 0, 0, -5 } };
    Graphics3D::SetGlobalAmbientColor(ColorF{ 0.5 });
	Graphics3D::SetSunColor(ColorF{ 1.0 });

    // モデル読み込み
    model = Model(getData().Dataset[getData().SelectedIndex].ModelPath);
    Model::RegisterDiffuseTextures(model, TextureDesc::MippedSRGB);

    InfoTransition = Transition(0.1s, 0.1s);
    InfoActive = false;
    InfoIndex = Rect(Detail::detail.x - 60, 0, 60);
}

/*update---------------------------------------------------------------------*/
void View::update() {
    camera.update(4.0);
    InfoTransition.update(InfoActive);

    {
        const Transformer2D t(Mat3x2::Translate((1 - InfoTransition.value()) * Detail::detail.w, 0), TransformCursor::Yes);
        if(Detail::ShowButton.scaled(0.9).rounded(10).mouseOver()){
            Cursor::RequestStyle(CursorStyle::Hand);
        }
        if(Detail::ShowButton.scaled(0.9).rounded(10).leftReleased()){
            changeScene(U"List");
        }
        if(InfoIndex.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
        if(InfoIndex.leftReleased()) InfoActive = !InfoActive;
    }
    if(InfoActive && MouseL.up() && Cursor::Pos().x < Detail::detail.x) InfoActive = false;
}

/*draw---------------------------------------------------------------------*/
void View::draw() const {
    {
		Graphics3D::SetCameraTransform(camera);
		renderTexture.clear(ColorF{0}.removeSRGBCurve());
		const ScopedRenderTarget3D target{ renderTexture };

		// draw models
		{
            const Transformer3D t{ Mat4x4::Translate(0, 0, 0) };

	        const auto& materials = model.materials();

	        for (const auto& object : model.objects())
	        {
                object.draw(materials);
	        }
		}
	}

    // RenderTexture を 2D シーンに描画
	{
		Graphics3D::Flush();
		renderTexture.resolve();
		Shader::LinearToScreen(renderTexture);
	}

    {
        const Transformer2D t(Mat3x2::Translate((1 - InfoTransition.value()) * Detail::detail.w, 0), Transformer2D::Target::SetLocal);

        InfoIndex.rounded(10, 0, 0, 10).draw(ColorF(.15));
        FontAsset(U"List.Title")(U"\xF035C").drawAt(InfoIndex.center(), ColorF(0.9));

        Detail::detail.draw(ColorF(.15));
        if(InfoActive) Line(Detail::ItemBase.x * 2 + Detail::ItemSize.x * 2, 80, Detail::ItemBase.x * 2 + Detail::ItemSize.x * 2, Scene::Height()).draw(3, Color(230, 176, 0));
        const auto item = getData().Dataset[getData().SelectedIndex];
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
        Detail::ShowButton.scaled(0.9).rounded(10).draw(Color(11, 156, 230));
        FontAsset(U"List.Title")(U"\xF0206 戻る").drawAt(Detail::ShowButton.center());


        Rect(Detail::detail.x, 0, Scene::Width(), 80).draw(ColorF(.1));
        if(InfoActive) Line(Detail::detail.x, 80, Scene::Width(), 80).draw(5, Color(230, 176, 0));
        FontAsset(U"List.Head")(U"3Dビュー").draw(Detail::detail.x + 10, 10, ColorF(0.9));
    }
}
