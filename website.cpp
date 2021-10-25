#include "website.hpp"
#include <unistd.h>
#include <sys/stat.h>
#include <cmath>
#include <algorithm>    // std::sort

void remove_file( std::string dir_n )
{
	sleep( 300 );
	fs::remove_all( dir_n );
}


bool IsPathExist(const std::string &s)
{
  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}

bool compareFunction (fs::path a, fs::path b) {return a<b;} 

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string toHuman( std::uintmax_t size )
{
	int i {0};
        double mantissa = size;
	//mantissa /= 8;
	for (; mantissa >= 1000.; mantissa /= 1000., ++i) { }
        mantissa = std::ceil(mantissa * 10.) / 10.;

	//mantissa =  floor( (x * 100) + 0.5) / 100;

	std::string result { std::to_string( mantissa )  };


	auto fsize = result.find(".");

	if (fsize != std::string::npos)
	{
		if ( result[fsize + 2] == '0' )
		{
			if ( result[fsize + 1] == '0')
			{
				result = result.substr(0, fsize);	
			}	
			else
			{
				result = result.substr(0, fsize+2);
			}
		}
		else
		{
			result = result.substr(0, fsize+3);
		}
	}

	
	result = result + "BKMGTPE"[i] + "o";

	return result;
}

webserver::webserver(std::string &path) : m_path { path }
{

}

std::string webserver::remove(std::string path)
{
	for (std::size_t i {0};i < path.size();i++)
	{
		if ( path[i] == '\'' )
		{
			path = path.substr(0, i) + "\\" + path.substr(i);
			i++;
		}

	}

	return path;

}


std::string webserver::index(std::string folder)
{
	std::string http {""};
	
	std::ifstream flux { "file/index.html" };

	std::string ligne;

	while ( std::getline(flux, ligne) )
	{
		if (ligne == "<!-- path -->")
		{
			http += getpath(folder);
		}
		else if (ligne == "<!-- script -->")
		{
			http += "<script type=\"text/javascript\" src=\"/file/script.js\"> </script>";
		}
		else
		{
			http += ligne;
		}

	}

	return http;
}
		
std::string webserver::onlypath(std::string folder)
{
	std::string all_path { m_path + folder };
	std::string html {""} ;
	
	std::vector<fs::path> v_folder;
	std::vector<fs::path> v_file;


	for (const auto &entry : fs::directory_iterator(all_path))
	{
		if (fs::is_directory( entry.path() ))
		{
			v_folder.emplace_back( entry );
		}
		else
		{
			v_file.emplace_back( entry );
		}	
	}


	std::sort( v_folder.begin(), v_folder.end(), compareFunction);//sort the vector
	std::sort( v_file.begin(), v_file.end(), compareFunction);//sort the vector

	if (folder != "")
		folder += "/";

	for (const auto &entry : v_folder)
	{
		std::string path_n = entry.filename();
	
		html += path_n + "\n" + remove(folder + path_n) + "\n/icon/folder.png\n";
	}

	for (const auto &entry : v_file)
	{
		html += file_tostring( entry, folder, false );
	}


	return html;
}

std::string webserver::getpath( std::string folder )
{
	std::string all_path { m_path + folder };
	std::string html {""} ;
	
	std::vector<fs::path> v_folder;
	std::vector<fs::path> v_file;


	for (const auto &entry : fs::directory_iterator(all_path))
	{
		if (fs::is_directory( entry.path() ))
		{
			v_folder.emplace_back( entry );
		}
		else
		{
			v_file.emplace_back( entry );
		}	
	}


	std::sort( v_folder.begin(), v_folder.end(), compareFunction);//sort the vector
	std::sort( v_file.begin(), v_file.end(), compareFunction);//sort the vector

	for (const auto &entry : v_folder)
	{
		std::string path_n = entry.filename();
	
		html += "<div onclick=\"go_dir('" + remove(folder + path_n) + "/')\"> <p> <img src=\"/icon/folder.png\" >" + path_n + "</p> </div>";
	}

	for (const auto &entry : v_file)
	{
		html += file_tostring( entry, folder, true );
	}

	return html;


}
		
std::string webserver::nomaj( std::string extname ) {
	for (int i = 0; i < extname.size(); i++) {
		if (extname[i] > 64 and extname[i] < 91) {
			extname[i] += 32;
		}
	}

	return extname;
}

std::string webserver::file_tostring( const fs::path &entry, std::string &folder, bool html_render )
{
	std::string path_n = entry.filename();

	std::string ext { entry.extension() };
	std::string add { "" };
	std::string before { "/download/" };

	std::string image { "/icon/default.png" };

	std::string file_size { "?" };

	try {
		std::uintmax_t size = fs::file_size(entry);
		file_size = toHuman( size ) ; 
	}
	catch(fs::filesystem_error& e) {
		std::cout << e.what() << '\n';
	}



	if (ext != "" && ext != ".")
	{
		if( fs::exists("icon/" + nomaj(ext.substr(1)) + ".png"))
			image = "/icon/" + nomaj(ext.substr(1)) + ".png";
	}
	

	if (ext == ".pdf" && html_render)
	{
		add += ".titicplusplus.html";
	}
	else if (ext == ".wav" || ext == ".flac")
	{
		add += ".titicplusplus.mp3";
	}


	if (html_render)
	{

		if (ext == ".jpg" || ext == ".JPG" || ext == ".jpeg" || ext == ".JPEG" || ext == ".png" || ext == ".PNG" || ext == ".gif" || ext == ".GIF" || ext == ".bmp" || ext == ".BMP" || ext == ".svg" || ext == ".SVG")
		{
			image = before +remove( folder + path_n + add);
		}
	
		return "<div> <p> <a href=\"" + before +remove( folder + path_n + add)
		 + "\"> <img src=\"" + image + "\" > " + path_n + "</a> </p> <p>" 
		 //+ "\"> <img src=\"" + before +remove( folder + path_n + add) + "\" > " + path_n + "</a> </p> <p>" 
		+ file_size + " </p>  </div>";
	}
	
	return path_n + "\n" + before +remove( folder + path_n + add) + "\n" + image + "\n";
}


std::string webserver::convert( std::string path, int i)
{

	std::string file_name { "index" };
	std::string file_add { "" };

	std::string file { "" };

	if (i == 0)
	{
		file = "\"" + m_path + path.substr( 0, path.size() - 19 ) + std::string("\"");
		file_add = "-html.html";
	}
	else if (i == 1)
	{
		file = "\"" + m_path + path.substr( 0, path.size() - 18 ) + std::string("\"");		
		file_add = ".mp3";
	}


	std::string dir { "temp/0folder/" };

	while ( IsPathExist( dir ) )
	{
		dir[5]++;
		switch (dir[5]) {
			case ';':
				dir[5] = 'A';
				break;
			case '[':
				dir[5] = 'a';
				break;
			case '{':
				dir[5] = '0';
				break;
			default:
				break;
		}
	}
		
	
	fs::create_directory( dir );


	std::string comligne { "" }; 

	if (i == 0)
	{
		comligne = " pdftohtml  -s " + file + " " + dir + file_name;
	}
	else if (i == 1)
	{
		comligne = " ffmpeg -y -i " + file + " " + dir + file_name + file_add;
	}
	

	system( comligne.c_str() );
	
	std::string http {""};
	
	std::ifstream flux { dir + file_name + file_add };

	std::string ligne;


	while ( std::getline(flux, ligne) )
	{
		std::size_t size = ligne.find("src=\"");

		if (size != std::string::npos)
		{
			ligne.insert( size + 5, "/" + dir );

		}


		http += ligne + "\n";
	}

	flux.close();

	lethread.emplace_back( remove_file, dir );

	return http;
}


std::string webserver::img_index(std::string img)
{
	std::string http {""};

	img = "/download/" + img;
	
	std::ifstream flux { "file/img_index.html" };

	std::string ligne;

	while ( std::getline(flux, ligne) )
	{
		if (ligne == "<!-- img -->")
		{
			http += "<img id=\"image_f\" src=\"" + img + "\" alt=\"image\" />";
		}
		else 
		{
			http += ligne;
		}

	}

	return http;


}

std::string webserver::next(std::string img_n, std::string why)
{
	std::vector<std::string> img;

	auto it = img_n.find_last_of("/");
	std::string folder_img { "" };
	std::string name_img { "" };

	long pos { -1 };

	if (it != std::string::npos)
	{	
		folder_img += img_n.substr(0, it+1);
		name_img = img_n.substr(it+1);
	}
	else
	{
		name_img = img_n;
	}


	

	for ( const auto &entry : fs::directory_iterator(m_path + folder_img))
	{
		std::string ext { entry.path().extension() };

		if (ext == ".jpg" || ext == ".JPG" || ext == ".jpeg" || ext == ".JPEG" || ext == ".png" || ext == ".PNG" || ext == ".gif" || ext == ".GIF" || ext == ".bmp" || ext == ".BMP" || ext == ".svg" || ext == ".SVG")
		{
			img.push_back( entry.path().filename() );
			if (entry.path().filename() == name_img)
				pos = img.size() -1;
		}

	}


	if (pos == -1)
		return "";


	if (why == "-1")
	{
		if (pos == 0)
			pos = img.size() -1;
		else
			pos--;
	}
	else if (why == "1")
	{
		if (pos+1 == img.size())
			pos = 0;
		else
			pos++;
	}

	std::string result { "/download/" + folder_img + img[pos] };
	


	return result;
}


std::string webserver::getallimg(std::string img)
{
	std::cout << "cc" << std::endl;
	std::vector<std::string> img_list;
	std::cout << "cc" << std::endl;
	std::string html = "";
	std::cout << img << std::endl;

	if (img.size() > 11 && (img.substr(0, 10) == "/download/"))// || img.substr(0, 10) == "/download/"))
	{
		img.substr(11);
		std::cout << "mais ui" << std::endl;
	}

	auto it = img.find_last_of("/");
	std::string folder_img;
	std::string name_img;



	if (it != std::string::npos)
	{	
		folder_img = img.substr(0, it+1);
		name_img = img.substr(it+1);
	}
	else
	{
		folder_img = "";
		name_img = img;
	}


	std::cout << m_path + folder_img << std::endl;


	for ( const auto &entry : fs::directory_iterator(m_path + folder_img))
	{
		std::string ext { entry.path().extension() };

		if (ext == ".jpg" || ext == ".JPG" || ext == ".jpeg" || ext == ".JPEG" || ext == ".png" || ext == ".PNG" || ext == ".gif" || ext == ".GIF" || ext == ".bmp" || ext == ".BMP" || ext == ".svg" || ext == ".SVG")
		{
			/**if (status == 0) {
				std::string name = entry.path().filename();
				if (name == name_img)
				{
					html += "/download/" + folder_img + name + "\n";
					status = 1;
				}
				else {
					img_list.push_back(name);
				}
			}
			else if (status == 1) {
				std::string name = entry.path().filename();
				html += "/download/" + folder_img + name + "\n";
			}**/
			img_list.push_back(entry.path().filename());
		}

	}
	std::sort( img_list.begin(), img_list.end(), compareFunction);//sort the vector

	
	/**int end = 0;
	bool status = false;

	for (int i = 0; i < img_list.size(); i++) {
		if (status) {
			html += "/download/" + folder_img + img_list[i] + "\n";
		}
		else {
			if (img_list[i] == name_img) {
				html += "/download/" + folder_img + img_list[i] + "\n";
				status = 1;
				end = i;
			}
		}

		
	}**/

	int place = 0;
	for (int i = 0; i < img_list.size(); i++) {
		html += "/download/" + folder_img + img_list[i] + "\n";
		if (img_list[i] == name_img) {
			place = i;
		}
	}

	std::cout << "The place is " << place << std::endl;

	html += std::to_string(place) + "\n";

	return html;

}

std::string webserver::search()
{
	std::string http {""};
	
	std::ifstream flux { "file/search.html" };

	std::string ligne;

	while ( std::getline(flux, ligne) )
	{
		http += ligne;
	}

	return http;

}

std::string webserver::search_file( std::string name )
{
	std::string com { "find " + m_path + " -name \"*" + name + "*\""};
	std::stringstream result;
	result.str( exec(com.c_str() ) );

	std::string line;
	std::string html;

	while (std::getline(result, line))
	{
		std::string path_n = line.substr( m_path.size() );
		if (fs::is_directory( line ))
		{
			html += "<div onclick=\"go_dir('" + remove(path_n) + "/')\"> <p> <img src=\"/icon/folder.png\" >" + path_n + "</p> </div>";
		}
		else
		{
			const fs::path path { line };
			std::string folder { path.parent_path() };
			folder += "/";
			
			folder = folder.substr( m_path.size() );
			html += file_tostring( path, folder, true );
		}
	}

	return html;
}

webserver::~webserver() {}
