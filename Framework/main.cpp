#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>

// trim carriage and newline symbols 
std::string trim(std::string name){    
    if (  (!name.empty() && name[name.length()-1] == '\n')   ||
          (!name.empty() && name[name.length()-1] == '\r\n') ||
          (!name.empty() && name[name.length()-1] == '\r')
       )
    {
        name.erase(name.length()-1);
        return name;
    }
    return name;
}

// add edge to adjacency list 
std::map<std::string, std::set<std::string> >& add_edge(std::map<std::string, std::set<std::string> >& adj_list, std::string vertex_one, std::string vertex_two){

    if(adj_list.count(vertex_one) == 0){            
        std::set<std::string>  empty_set;
        adj_list.insert(std::pair<std::string, std::set<std::string> >(vertex_one, empty_set) );           
        adj_list[vertex_one].insert(vertex_two); 
    }else{ 
        adj_list[vertex_one].insert(vertex_two);
    }
    
    return adj_list;
}

// output formatted adjacency list
void print_adj_list(std::map<std::string, std::set<std::string> >& adj_list){
    
    for(  std::map<std::string, std::set<std::string> >::iterator it = adj_list.begin();
          it != adj_list.end();
          it++
       )
    {
        std::cout << it->first << "-> { ";
        
        for(  std::set<std::string>::iterator it_ = it->second.begin(); 
              it_ != it->second.end();
              it_++
            )
        {
          std::cout << *it_ << " ";
        }
        std::cout << "}" << std::endl;
    }
}

// output formatted triangles 
void print_trianlges(std::set<std::set<std::string> >& triangles){

    for(  std::set<std::set<std::string> >::iterator itr_begin = triangles.begin();
          itr_begin != triangles.end();
          itr_begin++
       )
    {
        std::cout << "{";
        for(  std::set<std::string>::iterator itr_begin_ = (*itr_begin).begin();
              itr_begin_ != (*itr_begin).end();
              itr_begin_++
            )
        {
          std::cout << " "<< *itr_begin_;     
        }
        std::cout << " }" << std::endl;
    }
}

// returns vector of the proper order of vertices to check
void degree_vector(std::map<std::string, std::set<std::string> >& adj_list, std::vector<std::string>& vertex_order){
    
    // fill this by running through map once 
    std::map<std::string, int> vertex_degree;
    
    for(  std::map<std::string, std::set<std::string> >::iterator itr_begin_adj = adj_list.begin();
            itr_begin_adj != adj_list.end();
            itr_begin_adj++
       )
    {
        vertex_degree.insert(std::pair<std::string, int>(itr_begin_adj->first, itr_begin_adj->second.size()));
    }
    
    // Vectors that map degree to string. Can be sorted easily 
    std::vector<std::pair<int, std::string> > degree_vertex;
    
    for(  std::map<std::string, int>::iterator itr_begin_adj = vertex_degree.begin();
            itr_begin_adj != vertex_degree.end();
            itr_begin_adj++
       )
    {
        degree_vertex.push_back(std::pair<int, std::string>(itr_begin_adj->second, itr_begin_adj->first));
    }
    
    std::sort(degree_vertex.rbegin(), degree_vertex.rend());
   
    for(int i = 0; i < degree_vertex.size(); i++){
        vertex_order.push_back(degree_vertex[i].second);
    }
}

// implement algorithm 
void find_triangles(std::map<std::string, std::set<std::string> >& adj_list, std::vector<std::string>& vertex_order){
    
    std::map<std::string, std::set<std::string> > algo_arr;
    std::set<std::set<std::string> > triangles;
    
    // Initialize "empty array" for algorithm 
    for(int i = 0; i < vertex_order.size(); i++){
        std::set<std::string> empty_set;
        algo_arr.insert(std::pair<std::string, std::set<std::string> >(vertex_order[i], empty_set));
    }

    for(int i = 0; i < vertex_order.size(); i++){
        
        for(  std::set<std::string>::iterator itr_begin = adj_list[vertex_order[i]].begin();
              itr_begin != adj_list[vertex_order[i]].end();  
              itr_begin++
           )
        {
            //std::cout << *itr_begin << " ";
            
            std::set<std::string> intersection;
            std::set_intersection(algo_arr[vertex_order[i]].begin(),algo_arr[vertex_order[i]].end(),
                                  algo_arr[*itr_begin].begin(),algo_arr[*itr_begin].end(),
                                  std::inserter(intersection,intersection.begin()));
            
            if(!intersection.empty()){
                //std::cout << vertex_order[i] << ", " << *itr_begin << ", " << intersection.
                //std::cout << "Found Triangle" << std::endl;
                
                for(  std::set<std::string>::iterator itr_begin_ = intersection.begin();
                        itr_begin_!= intersection.end();
                        itr_begin_++
                   )
                {
                  std::set<std::string> my_triangle;
                  my_triangle.insert(*itr_begin_);
                  my_triangle.insert(*itr_begin);
                  my_triangle.insert(vertex_order[i]);
                  triangles.insert(my_triangle);
                }
            }
            
            algo_arr[*itr_begin].insert(vertex_order[i]);
        }
        //std::cout << std::endl;
    }
    
    print_trianlges(triangles);
}

// generate adjacency list
std::map<std::string, std::set<std::string> >& load_adj_list(std::map<std::string, std::set<std::string> > &adj_list, std::string file_path){
    
    std::ifstream edge_list;
    edge_list.open(file_path.c_str());
    std::string edge = "";

    // Add all the edges to the adjacency list
    while(!edge_list.eof()){
        std::string delimiter = "\t";
        
        std::getline(edge_list,edge);
        edge = trim(edge);

        std::string vertex_one = edge.substr(0, edge.find(delimiter));
        std::string vertex_two = edge.substr(edge.find(delimiter) + 1, edge.size() - edge.find(delimiter));
        
        add_edge(adj_list, vertex_one, vertex_two);
        add_edge(adj_list, vertex_two, vertex_one);
    }
    
    edge_list.close();
    
    return adj_list;
}

int main() {
    
    std::string file_path = "\\Users\\Michael\\Desktop\\edgelist.txt";
    std::map<std::string, std::set<std::string> > adj_list;
    std::vector<std::string> vertex_order;
    
    load_adj_list(adj_list, file_path);
    std::cout << std::endl;
    degree_vector(adj_list, vertex_order);

    find_triangles(adj_list, vertex_order);

    
    return 0;
}

