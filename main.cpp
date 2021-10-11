#include <iostream>
#include <string>
#include <simple-web-server/server_http.hpp>
#include "website.hpp"
#include <curl/curl.h> 

// g++ main.cpp website.cpp -o exe -pthread -lboost_system  -lstdc++fs -lcurl


using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

std::string path_basic { "/home/famillevincent/" };
std::vector<std::string> agree;

webserver web(path_basic );

int downlaod_all_file()
{

}

int main()
{
	system("rm temp/* -r");
	
	HttpServer server;
	server.config.port = 5000;


	server.resource["^/$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
    		response->write( web.index("") );
 	};

	server.resource["^/search$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
    		response->write( web.search() );
 	};

	server.resource["^/favicon.ico$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		std::ifstream flux { "favicon.ico" };

		std::string http {""};
		std::string ligne;

		if (flux)
			while ( getline(flux, ligne))
			{
				http += ligne;
				http += "\n";
			}
		flux.close();

		response->write( http );

 	};


	server.resource["^/dir$"]["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request)
	{
		auto query_fields = request->parse_query_string();
		std::cout << request->path << std::endl;

    		response->write( web.index( query_fields.find("path")->second ) );
 	};

	  server.default_resource["GET"] = [](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) 
	{
		std::string path { request->path };
		std::cout << path << std::endl;

		if (path.substr(0, 6) == "/icon/" || path.substr(0, 5) == "icon/")
		{
			std::ifstream flux { path.substr(1) };

			std::string http {""};
			std::string ligne;

			if (flux)
			while ( getline(flux, ligne))
			{
				http += ligne;
				http += "\n";
			}
			else
			{
				flux.close();
				flux.open("icon/default.png");

				while ( getline(flux, ligne))
				{
					http += ligne;
					http += "\n";
				}
			}
			flux.close();

			response->write( http );

		}
		else if (path.substr(0, 6) == "/file/" || path.substr(0, 5) == "file/" || path.substr(0, 6) == "/temp/" || path.substr(0, 5) == "temp/")
		{
			std::ifstream flux { path.substr(1) };

			std::string http {""};
			std::string ligne;

			if (flux)
			while ( getline(flux, ligne))
			{
				http += ligne;
				http += "\n";
			}
			flux.close();

			response->write( http );
		}
		else if (path.substr(0, 5) == "/dir/")
		{
			std::string folder { path.substr(5) };
			CURL *curl;
			std::string folder2 {  curl_easy_unescape( curl, folder.c_str() , 0, NULL) };

			response->write( web.index( folder2 ));

		}
		else if (path.substr(0, 6) == "/sdir/")
		{
			std::string folder { path.substr(6) };
			CURL *curl;
			std::string folder2 {  curl_easy_unescape( curl, folder.c_str() , 0, NULL) };

			response->write( web.search_file( folder2 ));

		}
		else if (path.substr(0, 10) == "/download/")
		{
			std::string folder { path.substr(10) };
			CURL *curl;
			std::string folder2 {  curl_easy_unescape( curl, folder.c_str() , 0, NULL) };

			if (folder.size() > 20 &&  folder2.substr( folder2.size() - 19 ) == ".titicplusplus.html" )
			{
				response->write( web.convert(folder2, 0) );
			}
			if (folder.size() > 19 &&  folder2.substr( folder2.size() - 18 ) == ".titicplusplus.mp3" )
			{
				response->write( web.convert(folder2, 1) );
			}
			else
			{

				std::ifstream flux { path_basic +  folder2 };

				std::string http {""};
				std::string ligne;

				if (flux)
				while ( getline(flux, ligne))
				{
					http += ligne;
					http += "\n";
				}

				response->write( http );

			}			


		}
		else if (path.substr(0, 12 ) == "/web_images/")
		{
			std::string img { path.substr(12) };
			response->write( web.img_index( img ) );
		}
		else if (path.substr(0, 15 ) == "/next/download/")
		{
			auto query_fields = request->parse_query_string();
			std::string img { path.substr(15) };
			response->write( web.next( img, query_fields.find("next")->second ) );

		}
	};

	server.start();
	return 0;
	
}
