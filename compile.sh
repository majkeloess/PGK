param=$1

g++ "$param.cpp" -o "$param" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
