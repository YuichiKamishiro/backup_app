all: 
	g++ imgui/imgui-SFML.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/ImGuiFileDialog.cpp main.cpp -Iimgui -lsfml-graphics -lsfml-window -lsfml-system -lglut -lGLU -lGL -o backup_app
install:
	mkdir /usr/share/backup_app && mv script.sh /usr/share/backup_app && mv recovery.sh /usr/share/backup_app && mv config /usr/share/backup_app && cp backup_app /bin &&

