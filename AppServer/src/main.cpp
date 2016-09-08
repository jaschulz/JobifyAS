#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <mongoose/Server.h>
#include <mongoose/JsonController.h>
#include <fstream>
#include <iostream>

using namespace std;
using namespace Mongoose;

class MyController: public JsonController {
public:
	int jsonContainsValue(const Json::Value& root, string value) {
		const Json::Value& users = root["users"];
		for (Json::ValueConstIterator it = users.begin(); it != users.end();
				++it) {
			const Json::Value& user = *it;
			if (value.compare(user["email"].asString()) == 0)
			{
				return 0;
			}
		}
		return 1;
	}

	void registerUser(Request &request, JsonResponse &response) {

		Json::Value users;
		Json::Reader reader;

		std::ifstream file("users.json", std::ifstream::binary);
		bool parsingSuccessful2 = reader.parse(file, users, true); //parse process
		if (!parsingSuccessful2) {
			std::cout << "Failed to parse"
					<< reader.getFormattedErrorMessages();

		}
		std::string data = request.getData();

		Json::Value root;

		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse"
					<< reader2.getFormattedErrorMessages();

		}
		string username = root.get("email", "").asString();
		string password = root.get("password", "").asString();

		Json::Value user1;
		user1["email"] = username;
		user1["password"] = password;

		file.close();

		if (jsonContainsValue(users, username) == 1) { //  == 0 && password.compare(user1["pass"].asString()) == 0) {
			users["users"].append(user1);
			Json::StyledStreamWriter writer;
			std::ofstream out("users.json");
			writer.write(out, users);
			out.close();
			response["result"].append("Registration Ok");
		} else {
			response["result"].append("El usuario ya existe.");
		}
	}

	void login(Request &request, JsonResponse &response) {
		/*		Json::Value users;
		 Json::Value user1;
		 Json::Value user2;
		 user1["login"] = "locolope";
		 user1["pass"] = "123456";
		 user2["login"] = "lima";
		 user2["pass"] = "qwerty";
		 users.append(user1);
		 users.append(user2);
		 */
		Json::Value users;
		Json::Reader reader;

		std::ifstream file("users.json", std::ifstream::binary);
		//file >> users;

		bool parsingSuccessful2 = reader.parse(file, users, true); //parse process
		if (!parsingSuccessful2) {
			std::cout << "Failed to parse"
					<< reader.getFormattedErrorMessages();

		}

		std::string data = request.getData();

		Json::Value root;

		Json::Reader reader2;
		bool parsingSuccessful = reader2.parse(data.c_str(), root); //parse process
		if (!parsingSuccessful) {
			std::cout << "Failed to parse"
					<< reader2.getFormattedErrorMessages();

		}
		string username =
				root.get("email", "A Default Value if not exists").asString();

		string password = root.get("password", "").asString();

		if (jsonContainsValue(users, username) == 0) { //  == 0 && password.compare(user1["pass"].asString()) == 0) {
			response["result"].append("login Ok");
		} else {
			response["result"].append("login Failed");
		}
	}

	void setup() {
		// Example of prefix, putting all the urls into "/api"
		setPrefix("/api");

		addRouteResponse("GET", "/login", MyController, login, JsonResponse);
		addRouteResponse("POST", "/registerUser", MyController, registerUser,
				JsonResponse);
	}
};

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

	MyController myController;
	Server server(8080);
	server.registerController(&myController);
	server.setOption("enable_directory_listing", "false");
	server.start();

	cout << "Server started, routes:" << endl;
	myController.dumpRoutes();

	while (running) {
		sleep(10);
	}

	server.stop();

	return EXIT_SUCCESS;
}
