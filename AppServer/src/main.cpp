#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include "../Handlers/ProfileController.h"
#include "../Handlers/AccountController.h"

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

	ProfileController pController;
	AccountController aController;
	Server server(8081);
	server.registerController(&pController);
	server.registerController(&aController);
	server.setOption("enable_directory_listing", "false");
	server.start();

	cout << "Server started, routes:" << endl;
	aController.dumpRoutes();
	pController.dumpRoutes();

	while (running) {
		sleep(10);
	}

	server.stop();

	return EXIT_SUCCESS;
}
