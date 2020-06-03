#include <future>
#include <memory>
#include <asio.hpp>

#include "../include/Simple-Web-Server/client_http.hpp"
#include "../include/Simple-Web-Server/server_http.hpp"

using namespace std;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

int main() {
	HttpServer server;

	// Externally created io_context can be shared by other
	// ASIO compatible libraries - e.g. async database drivers
	shared_ptr<asio::io_context> loop = make_shared<asio::io_context>();

	server.config.port = 8080;
	server.io_service = loop;

	server.default_resource["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
		response->write("SAMPLE RESPONSE");
	};

	// CPU bound operations can be done
	// on seperate threads to avoid blocking the event bus
	server.resource["^/heavy-operations$"]["GET"] = [](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request>) {
		thread worker_thread([response] {
			this_thread::sleep_for(chrono::seconds(5));
			response->write("FINISHED 5 SECONDS OF WORK");
		});
		worker_thread.detach();
	};

	promise<unsigned short> server_port;
	thread server_thread([&server, &server_port, &loop]() {
		server.start([&server_port](unsigned short port) {
			server_port.set_value(port);
		});
		loop->run();
	});
	cout << "Server listening on PORT " << server_port.get_future().get() << endl << endl;

	server_thread.join();
}
