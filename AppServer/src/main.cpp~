#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include "JobifyController.h"

using namespace std;
using namespace Mongoose;


volatile static bool running = true;

void handle_signal(int sig) {
	if (running) {
		cout << "Exiting..." << endl;
		running = false;
	}
}

int main() {
	srand(time(NULL));

	signal(SIGINT, handle_signal);

	JobifyController jController;
	Server server(8080);
	server.registerController(&jController);
	server.setOption("enable_directory_listing", "false");
	server.start();

	cout << "Server started, routes:" << endl;
	jController.dumpRoutes();

	while (running) {
		sleep(10);
	}

	server.stop();

	return EXIT_SUCCESS;
}
