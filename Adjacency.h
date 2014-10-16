// Notes: The key use of this file is in the construction of an adjacency list representation via load_adj_list(). Given the appropriate 
// 	 	  parameters this function will load an adjacency list. 

// Required directives: <iostream>, <map>, <set>, <fstream>

// Future Goals for Code: 

// Example Use:
// 		string file_path = "edgelist_big.txt";
//		map<string, set<string> > adj_list;
//		load_adj_list(adj_list, file_path);
//		print_adj_list(adj_list);

using namespace std;

// Trim newline and other carriage symbols  
string trim(string name){    
    if (  (!name.empty() && name[name.length()-1] == '\n')   ||
          (!name.empty() && name[name.length()-1] == '\r')
       )
    {
        name.erase(name.length()-1);
        return name;
    }
    return name;
}

// add edge to adjacency list. Note that vertex one is the key and vertex two is the value, therefore, this 
// code will be need to run twice to add each edge to the adjacency list  
map<string, set<string> >& add_edge(map<string, set<string> >& adj_list, string vertex_one, string vertex_two){

    if(adj_list.count(vertex_one) == 0){            
    	set<string>  empty_set;
        adj_list.insert(pair<string, set<string> >(vertex_one, empty_set) );           
        adj_list[vertex_one].insert(vertex_two); 
    }else{ 
        adj_list[vertex_one].insert(vertex_two);
    }
    
    return adj_list;
}

// generate adjacency list
void load_adj_list(map<string, set<string> > &adj_list, string file_path){
    
	cout << "beginning adjacency list construction" << endl;
	
    ifstream edge_list;
	
	edge_list.open(file_path.c_str());
    
	if(!edge_list.is_open()){
		cout << "adjacency list construction failed" << endl;
		cout << "The edge list file could not be read. Check load_adj_list() and whether the edge list file exists" << endl;
		exit(0);
	}
	
	string edge = "";
	
    // Add all the edges to the adjacency list
    while(!edge_list.eof()){
		
        string delimiter = "\t";
        
        getline(edge_list,edge);
        
		edge = trim(edge);

        string vertex_one = edge.substr(0, edge.find(delimiter));
        string vertex_two = edge.substr(edge.find(delimiter) + 1, edge.size() - edge.find(delimiter));
		
        // Omit leading and trailing file whitespace
        if( ((vertex_one.compare("")==0)&&(vertex_two.compare("")==0)) ||  vertex_one.compare(vertex_two) == 0)
            continue;
		               
        add_edge(adj_list, vertex_one, vertex_two);
        add_edge(adj_list, vertex_two, vertex_one);
    }
	   
    edge_list.close();
	
	cout << "successfully constructed adjacency list" << endl; 
    
    return;
}

// output formatted adjacency list
void print_adj_list(map<string, set<string> >& adj_list){
	
	cout << "Printed Adjacency List" << endl;
	
    for(  map<string, set<string> >::iterator itr_begin = adj_list.begin();
          itr_begin != adj_list.end();
          itr_begin++)
    {
        cout << itr_begin->first << "-> { ";
        
        for(  set<string>::iterator s_itr_begin = itr_begin->second.begin(); 
              s_itr_begin != itr_begin->second.end();
              s_itr_begin++)
        {
          std::cout << *s_itr_begin << " ";
        }
		
        cout << "}" << endl;
    }
	
	cout << "Printed Adjacency List Complete" << endl;
}