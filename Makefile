all: 
	g++ -Iimgui/SFML/include -Iimgui imgui/imgui-SFML.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/ImGuiFileDialog.cpp main.cpp -lsfml-graphics -lsfml-window -lsfml-system -lglut -lGLU -lGL -o backup_app
