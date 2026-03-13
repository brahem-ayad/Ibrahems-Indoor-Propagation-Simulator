build:
	g++ main.cpp ./src/rlImGui.cpp -lraylib -limgui -o Program
	./Program

render_video:
	ffmpeg -framerate 60 -i output/frame%05d.png -c:v libx264 -pix_fmt yuv420p video.mp4
