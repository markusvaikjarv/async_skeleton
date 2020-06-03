#include "include/Simple-Web-Server/client_http.hpp"
#include "include/Simple-Web-Server/server_http.hpp"
#include <future>

using namespace std;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

int main() {
	HttpServer server;
	server.config.port = 8080;

	server.resource["^/match/([0-9]+)$"]["GET"] = [](shared_ptr<HttpServer::Response> res, shared_ptr<HttpServer::Request> req) {
		res->write(req->path_match[1].str());
	};

	promise<unsigned short> server_port;
	thread server_thread([&server, &server_port]() {
		server.start([&server_port](unsigned short port) {
			server_port.set_value(port);
		});
	});
	cout << "Server listening on PORT " << server_port.get_future().get() << endl << endl;

	server_thread.join();
}
