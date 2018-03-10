all: main.cpp
	g++ main.cpp -o main
clean:
	rm main output.ppm output-perspective.ppm
