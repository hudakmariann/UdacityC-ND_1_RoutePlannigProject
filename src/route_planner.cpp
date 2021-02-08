#include "route_planner.h"
#include <algorithm>


RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) 
{
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2 done: Use the m_Model.FindCFindClosestNode method to find the closest nodes to the starting and ending coordinates.
 RoutePlanner::start_node=&model.FindClosestNode(start_x, start_y);
 RoutePlanner::end_node=&model.FindClosestNode(end_x, end_y);
      // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
}


// TODO 3 done: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value. //ez a route_model.h-ban van, a Node class public fuggvenye:
// - Node objects have a distance method to determine the distance to another node.
float RoutePlanner::CalculateHValue(RouteModel::Node const* node)  
{
return RouteModel::Node (*node).distance(*RoutePlanner::end_node);
}


// TODO 4 done: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.
bool CheckValidCell(int x, int y) {
  bool on_grid_x = (x >= 0 && x <= 100);
  bool on_grid_y = (y >= 0 && y <= 100);
  if (on_grid_x && on_grid_y)
  { return true;}
  else
  {return false;}
}

void RoutePlanner::AddNeighbors(RouteModel::Node* current_node) 
{
current_node->visited=true;
current_node->FindNeighbors();
    
 for (RouteModel::Node * nbr : current_node->neighbors) {
          
        nbr->parent = current_node;
        nbr->h_value = CalculateHValue(nbr);        
        nbr->g_value = current_node->g_value+nbr->distance(*current_node);
 
    		
   if ((!nbr->visited)&&(CheckValidCell(nbr->x,nbr->y)))
    		{
    		open_list.push_back(nbr);
    		nbr->visited=true;
       		}
  }  
} 
//-----------TWO QUESTIONS HERE BELOW-------------------
/* QUESTION 1: Okay, I know I should use the new type of for loop (I did it above even in my previously submitted work), however, I still don't understand why the following does not do the same as the new type of for loop above.

Sorry, I have never learnt programming officially, I learned on my own and did some projects in Delphi. There we have only the classical for loops with a counter, as far as I know. 
So I think the task should be possible to be solved with the classical kind of (i++) for loop too, and I just would like to understand how could we write this in c++ with a classical for loop, even though the new type is more effective or elegant. I'd like to know this just for the sake of understanding c++ better and for learning.

What is wrong with the following? It fails the test, however, it seems for me that it should do the same as the for loop above.
  
  int n= current_node->neighbors.size();
  for (int i=0; i<<n;i++) {
   
        current_node->neighbors[i]->parent = current_node;
        current_node->neighbors[i]->h_value = CalculateHValue(current_node->neighbors[i]);        
        current_node->neighbors[i]->g_value = current_node->g_value+current_node->neighbors[i]->distance(*current_node);
    		if (!current_node->neighbors[i]->visited)
    		{
    		open_list.push_back(open_list_row);
    		current_node->neighbors[i]->visited=true;
    		}
  }*/

//QUESTION 2: thanks :)
//-------------------------------------------------------------
//QUESTION 3: Is it compusory to use the "auto" keyword? To be honest, I don't really like it because for me it makes reading and understanding the code much more difficult. I like to know the type of the variable at first glance. Is it a problem if I don't use it, I make explicit the type instead?

bool Compare(RouteModel::Node * a, RouteModel::Node * b)
  {
 float fa=a->g_value+a->h_value;
 float fb=b->g_value+b->h_value;
 return fa>fb; 
  //QUESTION 4: this gives a good result only if I use fa>fb. (Instead of fa<fb).
  //However, in Lesson 3, it was f1<f2, and in your suggestion, it was also the first argument which had to be smaller. But why?
  //I had a long struggle by the time I figured out that this was the fault.
  }

RouteModel::Node *RoutePlanner::NextNode() 

{
RouteModel::Node * NextNode;
/* MY OLD SOLUTION
RouteModel::Node* NextNodeTemp;
int MinIndex;
float min_f=0;
for (int i=0; i<open_list.size(); i++)
  {min_f+=(open_list[i]->g_value+open_list[i]->h_value);}

  for (int i = 0; i<open_list.size(); i++)
	{
  	float f=(open_list[i]->h_value+open_list[i]->g_value);//+(OpenNode->h_value);
  	if (f < min_f)
    	{
      	min_f=f;
      	NextNodeTemp=open_list[i];
      	MinIndex=i;
    	}
    
	}
NextNode=NextNodeTemp;
open_list.erase (open_list.begin()+(MinIndex));
*/ 

  //NEW SOLUTION
std::sort (open_list.begin(), open_list.end(), Compare);
NextNode=open_list.back();  
open_list.pop_back(); 

return NextNode;
//------  
//QUESTION 5: thanks for the answer, I like the lambda solution too, though I have not tried it yet. It is also possible to embed a function into another in Delphi, which I learned on my own before this c++ course.
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
  path_found.push_back(*current_node);
  while (((current_node)!=start_node))
    //QUESTION 6: Is (!(current_node)==start_node) is not correct here instead?
    {
    distance+=current_node->distance(*(current_node->parent));
    path_found.push_back(*current_node->parent);
    current_node=current_node->parent;
    if (current_node==start_node){break;}
    }
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
  std::reverse(path_found.begin(),path_found.end());
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
   RouteModel::Node *current_node = nullptr;

    // TODO: Implement your solution here.
 
  AddNeighbors(start_node);
 
  while (!open_list.empty()) 
    //QUESTION: Is  (open_list->size>0) not correct here instead?
  	{
  	current_node=NextNode();
    
  	if (current_node==end_node) 
    	{
      m_Model.path=ConstructFinalPath(current_node);
       break;  
    	}
    else{
    AddNeighbors(current_node);}
  	} //end of while
if (open_list.size()==0)
	{
  	std::cout<<"No path found!"<<"\n";
	}
 }