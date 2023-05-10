@echo off 
g++ main.cpp imgui.cpp imgui_impl_dx11.cpp imgui_impl_win32.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp ImageInfo.cpp -ld3dcompiler -ld3d11 -ldwmapi -lgdi32 -IC:\Python310\include -LC:\Python310\libs -lpython310 -Wall
