#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include "Handlers/ProfileController.h"
#include "Handlers/AccountController.h"
#include "Handlers/SSController.h"
#include "Model/Location.h"

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
	SSController ssController;
	Server server(8081);
	server.registerController(&pController);
	server.registerController(&aController);
	server.registerController(&ssController);
	server.setOption("enable_directory_listing", "false");
	server.start();

	cout << "Server started, routes:" << endl;
	Location source(-34.7035466,-58.3930756);

	Location destination(-34.703450,-58.385126);

	cout<<"Distance: "<<	source.distanceTo(destination) <<endl;
	aController.dumpRoutes();
	pController.dumpRoutes();
	ssController.dumpRoutes();

	while (running) {
		sleep(10);
	}

	server.stop();

	return EXIT_SUCCESS;
}
