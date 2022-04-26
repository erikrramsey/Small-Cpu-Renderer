#include "Application.h"

int main(int argc, char** argv) {
	auto app = new Application();
	app->run();
	delete app;
	return 0;
}