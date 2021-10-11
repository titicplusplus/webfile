#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include<unistd.h>
#include <unordered_map>
#include <thread>

#include <cstdio>
#include <memory>
//#include <stdexcept>
#include <sstream>      // std::stringstream, std::stringbuf
#include <array>

#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif


void remove_file(std::string dir_n);

class webserver
{
	public:
		webserver(std::string &path);

		std::string remove( std::string path );

		std::string index(std::string folder);
		std::string getpath(std::string folder);
		std::string file_tostring( const fs::path &entry, std::string &folder );

		std::string convert( std::string path , int i);


		std::string img_index(std::string img);
		std::string next(std::string img_n, std::string why);

		std::string search();		
		std::string search_file( std::string name );		

		~webserver();

	private:
		std::string &m_path;
		//std::unordered_map<std::string, std::thread> lethread;
		std::vector< std::thread > lethread;

		


};


#endif
