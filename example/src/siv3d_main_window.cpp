
#include <Siv3D.hpp>

#include "siv3d_main_window.h"

#include "../../siv3d-imgui/siv3d_imgui.h"

static void ImguiDemo()
{
	// Our state
	static bool show_demo_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGuiIO& io = ImGui::GetIO();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

static void SetEmbeddedFontForImgui()
{
	ImGuiIO& io = ImGui::GetIO();
	const auto& fontAtlas = io.Fonts;
	const ImWchar* glyph = fontAtlas->GetGlyphRangesChineseFull();
#if SIV3D_PLATFORM(WINDOWS) || SIV3D_PLATFORM(MACOS)
	s3d::Blob fontBlob = s3d::Compression::DecompressFile(s3d::Resource(U"engine/font/noto-cjk/NotoSansCJK-Regular.ttc.zstdcmp"));
	if (!fontBlob.isEmpty())
	{
		/* AddFontFromMemoryTTF() assumes that font data is to be held by user's hand. */
		s3d::Array<s3d::Byte> fontData = fontBlob.asArray();
		fontAtlas->AddFontFromMemoryTTF(fontData.data(), static_cast<int>(fontData.size()), 24.f, nullptr, glyph);
		ImGui_ImplSiv3d_HoldFontData(fontData);
	}
#endif
}

CSiv3dMainWindow::CSiv3dMainWindow(const char32_t* windowName)
{
	if (windowName != nullptr)
	{
		s3d::Window::SetTitle(windowName);
	}

	s3d::Scene::SetResizeMode(s3d::ResizeMode::Actual);
	s3d::Window::SetStyle(s3d::WindowStyle::Sizable);
}

CSiv3dMainWindow::~CSiv3dMainWindow()
{

}

void CSiv3dMainWindow::Display()
{
#if 0
	s3d::Graphics::SetVSyncEnabled(false);
#endif

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplSiv3d_Init();

	SetEmbeddedFontForImgui();

	while (s3d::System::Update())
	{
		ImGui_ImplSiv3d_NewFrame();
		ImGui::NewFrame();

		ImguiDemo();

		ImGui::Render();
		ImGui_ImplSiv3d_RenderDrawData();
	}

	ImGui_ImplSiv3d_Shutdown();
}
