#include <optional>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <io2d.h>
#include "route_model.h"
#include "render.h"
#include "route_planner.h"

using namespace std::experimental;

static std::optional<std::vector<std::byte>> ReadFile(const std::string &path)
{   
    std::ifstream is{path, std::ios::binary | std::ios::ate};
    if( !is )
        return std::nullopt;
    
    auto size = is.tellg();
    std::vector<std::byte> contents(size);    
    
    is.seekg(0);
    is.read((char*)contents.data(), size);

    if( contents.empty() )
        return std::nullopt;
    return std::move(contents);
} 

int main(int argc, const char **argv)
{    
    std::string osm_data_file = "";
    if( argc > 1 ) {
        for( int i = 1; i < argc; ++i )
            if( std::string_view{argv[i]} == "-f" && ++i < argc )
                osm_data_file = argv[i];
    }
    else {
        std::cout << "To specify a map file use the following format: " << std::endl;
        std::cout << "Usage: [executable] [-f filename.osm]" << std::endl;
        osm_data_file = "../map.osm";
    }
    
    std::vector<std::byte> osm_data;
 
    if( osm_data.empty() && !osm_data_file.empty() ) {
        std::cout << "Reading OpenStreetMap data from the following file: " <<  osm_data_file << std::endl;
        auto data = ReadFile(osm_data_file);
        if( !data )
            std::cout << "Failed to read." << std::endl;
        else
            osm_data = std::move(*data);
    }
    
    // TODO 1 - done. : Declare floats `start_x`, `start_y`, `end_x`, and `end_y` and get
    // user input for these values using std::cin. Pass the user input to the
    // RoutePlanner object below in place of 10, 10, 90, 90.
  	
  //begin MyTask
  
	float start_x, start_y, end_x, end_y;
   // bool CoordCheckOK=(!(start_x>=0 && start_x<=100 && start_y>=0 && start_y<=100 && end_x>=0 && end_x<=100 && end_y>=0 && end_y<=100));
   //----PROBLEM: if I use this bool instead of copying the long condition given in the brackets, sometimes I get faulty results. I don't know why. That's why I used the long condition twice below instead of the bool.
  do {
  
  	std::cout << "Please enter start coordinates between 1 and 100." << "\n";
    std::cout << "start_x= " << "\n";
  	std::cin >> start_x;
  	std::cout << "start_y= " << "\n";
  	std::cin >> start_y;
  
    std::cout << "Please enter end coordinates between 1 and 100." << "\n";
    std::cout << "end_x= " << "\n";
  	std::cin >> end_x;
  	std::cout << "end_y= " << "\n";
  	std::cin >> end_y;
    
    //-------here I know this long expression  is not elegant, but substituting this below with the bool variable, I encountered problems sometimes.
    if ((!(start_x>=0 && start_x<=100 && start_y>=0 && start_y<=100 && end_x>=0 && end_x<=100 && end_y>=0 && end_y<=100)))
       {std::cout << "ERROR: At least one of your coordinates is not between 0 and 100!" << "\n";}
    else
       {
      std::cout << "Your start coordinates are: start_x= "<< start_x << ", start_y=" << start_y << "\n";
      std::cout << "Your end coordinates are: end_x= "<< end_x << ", end_y=" << end_y << "\n";
       }  
  }
  //-------here I know this is not elegant, but substituting this below with the bool variable, I encountered problems sometimes.
    while ((!(start_x>=0 && start_x<=100 && start_y>=0 && start_y<=100 && end_x>=0 && end_x<=100 && end_y>=0 && end_y<=100)));
  
  //end MyTask */
  
  
    // Build Model.
    RouteModel model{osm_data};
  //----this is a constructor, creating a RouteModel class named model. HM
  
    // Create RoutePlanner object and perform A* search.
    //RoutePlanner route_planner{model, 10, 10, 90, 90}; //OriginalLine
  	RoutePlanner route_planner{model, start_x, start_y, end_x, end_y}; // MyTask: user defined coordinates instead of constant ones - done.
    //----this is a constructor creating RoutePlanner class. HM 
  
  
  route_planner.AStarSearch();

    std::cout << "Distance: " << route_planner.GetDistance() << " meters. \n";

    // Render results of search.
    Render render{model};

    auto display = io2d::output_surface{400, 400, io2d::format::argb32, io2d::scaling::none, io2d::refresh_style::fixed, 30};
    display.size_change_callback([](io2d::output_surface& surface){
        surface.dimensions(surface.display_dimensions());
    });
    display.draw_callback([&](io2d::output_surface& surface){
        render.Display(surface);
    });
    display.begin_show();
}
