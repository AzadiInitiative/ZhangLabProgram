#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>

using namespace std;

// This function will return false if a single element exists in the vector that isn't marked. It accepts 
// as parameters a vectorized form of the adjacency list and the symbol used to mark vertices 
bool vector_covered(vector<pair<string, set<string> > >& comp_vector, string mark_symbol){
    
	for(vector<pair<string, set<string> > >::iterator itr_begin = comp_vector.begin();
        itr_begin != comp_vector.end();
        itr_begin++)
    {
		// Return false if a single element isn't marked 
        if(itr_begin->first.substr(0,mark_symbol.size()) != mark_symbol)
            return false;
    }
	
    return true;
}

// Add any source node to the queue marked. Do not alter the vector map
bool enqueue_source_node(queue<string>& vertex_container, vector<pair<string, set<string> > >& comp_vector, string mark_symbol){
	
     for(vector<pair<string, set<string> > >::iterator itr_begin = comp_vector.begin();
        itr_begin != comp_vector.end();
        itr_begin++)
    {
		// If an unmarked node exists, "mark it" and add it to the queue vertex_container 
        if(itr_begin->first.substr(0,mark_symbol.size()) != mark_symbol){
            string source = mark_symbol + itr_begin->first;
            vertex_container.push(source);
            return true;
        }          
    }
	
	cout << "Error: A node to enqueue should exist. Check enqueue_source_node() and previous code" << endl;
	exit(0); 
}

// This will remove the "mark" from the front of the vertex name
std::string remove_mark(std::string vertex_name, std::string mark_symbol){
    return vertex_name.substr(mark_symbol.size(), vertex_name.size()-mark_symbol.size());
}

// Predicate which will allow me to find a pair of interest 
struct find_pair{
    
    find_pair(std::string s_vertex) : vertex(s_vertex) {}
    
    bool operator () (std::pair<std::string, std::set<std::string> >& samp_pair){
        return (samp_pair.first == vertex);
    }
            
    std::string vertex;        
};

void update_map_vect(std::vector<std::pair<std::string, std::set<std::string> > >& map_vect, std::string key, std::string mark_symbol){
    std::vector<std::pair<std::string, std::set<std::string> > >::iterator it = std::find_if(map_vect.begin(), map_vect.end(), find_pair(key));
    it->first = mark_symbol+(it->first);        
}

// Should return true if the vertex is marked 
bool vertex_marked(std::vector<std::pair<std::string, std::set<std::string> > >& map_vect, std::string key, std::string mark_symbol){
    std::string marked_vertex = mark_symbol+key;
    
    for( std::vector<std::pair<std::string, std::set<std::string> > >::iterator itr_begin = map_vect.begin();
         itr_begin != map_vect.end();
         itr_begin++)
    {
        if((*itr_begin).first == marked_vertex)
            return true; 
    }
    return false;
}
        
// A basic implementation of BFS to find the connected components. The "connected components" will be stored in the parameter c_component
void connected_components(map<string, set<string> >& adj_list, vector<map<string, set<string> > >& c_components){
    
	// Convert adjacency list into vector format (useful for algorithm)
    vector<pair<string, set<string> > > map_vect(adj_list.begin(), adj_list.end());
    
	// The BFS algorithm requires a method to mark if a vertex has been reached. I use this symbol appended to 
	// the front of a vertex to indicate that it has been "covered"
	string mark_symbol = "!!+@##@+!!";
    
    while(!vector_covered(map_vect,mark_symbol))
	{
		// Should only contain marked vertices 
        queue<string> vertex_container; 
		
		  
        std::set<std::string> vertices_encountered; // Maintain list of encountered vertices 
        std::map<std::string, std::set<std::string> > connected_comp_map;
        
        if(!enqueue_source_node(vertex_container, map_vect, mark_symbol)){
            std::cout << "Error in connected_components function" << std::endl;
            exit(0);
        }
        
        while(vertex_container.size()!=0){
         
            // Remove mark from vertex(a) in the queue and find in vector. Set up connected component 
            std::string key = remove_mark(vertex_container.front(), mark_symbol);
            std::set<std::string> empty_set;
            connected_comp_map.insert(std::pair<std::string, std::set<std::string> > (key, empty_set));
            vertex_container.pop();
            std::vector<std::pair<std::string, std::set<std::string> > >::iterator it = std::find_if(map_vect.begin(), map_vect.end(), find_pair(key));
            vertices_encountered.insert(key);

            for( std::set<std::string>::iterator itr_begin = it->second.begin();
                 itr_begin != it->second.end();
                 itr_begin++)
            {
                connected_comp_map[key].insert(*itr_begin);
                if( !vertex_marked(map_vect, *itr_begin, mark_symbol) &&
                    (vertices_encountered.count(*itr_begin) == 0)
                  ){
                    vertex_container.push(mark_symbol+ *itr_begin);
                    vertices_encountered.insert(*itr_begin);
                }
            }

            // Update vector so that it has a marked vertex(a)
            update_map_vect(map_vect, key, mark_symbol);    
        }
        c_components.insert(c_components.end(), connected_comp_map);
    }
	    
}



int main(){

  cout << "Hello World" << endl;
  return 0; 

}