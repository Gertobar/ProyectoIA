all:
	g++ Fuente.cpp Principal.cpp -I/usr/local/include/opencv4/ -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio -lopencv_features2d -lopencv_xfeatures2d -std=c++11 -o vision.bin
run:
	./vision.bin


