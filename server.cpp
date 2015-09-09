#include <string>
#include <memory>
#include <cstdint>
#include <iostream>
#include <evhttp.h>
#include <fstream>
#include <sys/stat.h>

std::string home_dir;

void OnReq(struct evhttp_request *req, void *arg){
	struct stat sb;
	auto *OutBuf = evhttp_request_get_output_buffer(req);
	
	if (!OutBuf) return;

	std::string file_name = evhttp_request_get_uri(req);
	std::string req_file = home_dir + file_name;
	std::string req_file_alt = home_dir + file_name + "/";

	if (stat(req_file.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){
		if (req_file.substr(req_file.length() - 1,1) == "/"){
			req_file += "index.html";
		}else{
			req_file += "/index.html";
		}
		printf("Client Requested to Access a dir \n");
	}

	std::ifstream myReadFile;
	myReadFile.open(req_file);
	std::string konten;
	std::string line;

	if (myReadFile.is_open()) {
		while (getline (myReadFile,line))
			konten += line;
			
		evbuffer_add_printf(OutBuf, "%s", konten.c_str());
	}else{
		evbuffer_add_printf(OutBuf, "<html><body><center><h1>404 - File not found</h1></center></body></html>");
	}
	
	myReadFile.close();

	evhttp_send_reply(req, HTTP_OK, "", OutBuf);
}


int main(int argc,char *argv[]){
	char const SrvAddress[] = "0.0.0.0";
	std::uint16_t SrvPort;

	if (argc < 3){
		std::cout << "Usage: server [HOME DIR] [PORT]\n";
		return 0;
	}else{
		home_dir = argv[1];
		SrvPort = std::stoi(argv[2]);
		std::cout << "Server is using directory : " << home_dir << "\nRunning on port " << SrvPort << "\n" << argc;
	}

	if (!event_init()){
		std::cerr << "Failed to init libevent." << std::endl;
		return -1;
	}

	std::unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(SrvAddress, SrvPort), &evhttp_free);
	if (!Server){
		std::cerr << "Failed to init http server." << std::endl;
		return -1;
	}


	evhttp_set_gencb(Server.get(), OnReq, nullptr);
	if (event_dispatch() == -1){
		std::cerr << "Failed to run messahe loop." << std::endl;
		return -1;
	}

	return 0;
}
