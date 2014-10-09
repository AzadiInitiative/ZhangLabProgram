#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <queue>
#include "Triangle.h"

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H
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
    std::cout << "========================" << std::endl;
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

// returns vector of the proper order of vertices to check (useful in implementing triangle algorithm)
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
void find_triangles(std::map<std::string, std::set<std::string> >& adj_list, std::vector<std::string>& vertex_order, std::vector< std::set< std::set<std::string> > >& triangle_holder){
    
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
            std::set<std::string> intersection;
            std::set_intersection(algo_arr[vertex_order[i]].begin(),algo_arr[vertex_order[i]].end(),
                                  algo_arr[*itr_begin].begin(),algo_arr[*itr_begin].end(),
                                  std::inserter(intersection,intersection.begin()));
            
            if(!intersection.empty()){
                
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
    }
    
    if(!triangles.empty())
        triangle_holder.push_back(triangles);
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
        
        // Omit leading and trailing file whitespace
        if((vertex_one.compare("")==0)&&(vertex_two.compare("")==0))
            continue;
        
        add_edge(adj_list, vertex_one, vertex_two);
        add_edge(adj_list, vertex_two, vertex_one);
    }
    
    edge_list.close();
    
    return adj_list;
}

// This function will return false if a single element exists in the vector that isn't marked 
bool vector_covered(std::vector<std::pair<std::string, std::set<std::string> > >& comp_vector, std::string mark_symbol){
    for(std::vector<std::pair<std::string, std::set<std::string> > >::iterator itr_begin = comp_vector.begin();
        itr_begin != comp_vector.end();
        itr_begin++)
    {
        if(itr_begin->first.substr(0,mark_symbol.size()) != mark_symbol)
            return false;
    }
    return true;
}

// Add any source node to the queue marked. Do not alter the vector map
bool enqueue_source_node(std::queue<std::string>& vertex_container, std::vector<std::pair<std::string, std::set<std::string> > >& comp_vector, std::string mark_symbol){
     for(std::vector<std::pair<std::string, std::set<std::string> > >::iterator itr_begin = comp_vector.begin();
        itr_begin != comp_vector.end();
        itr_begin++)
    {
        if(itr_begin->first.substr(0,mark_symbol.size()) != mark_symbol){
            std::string source = mark_symbol+itr_begin->first;
            vertex_container.push(source);
            return true; // Success
        }          
    }
    return false; // Failure. This point should have been caught by other code  
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
        
// A basic implementation of BFS to find the connected components 
void connected_components(std::map<std::string, std::set<std::string> >& adj_list, std::vector<std::map<std::string, std::set<std::string> > >& c_components){
    
    std::vector<std::pair<std::string, std::set<std::string> > > map_vect(adj_list.begin(), adj_list.end());
    std::string mark_symbol = "!!+@##@+!!"; // Symbol used to mark
    
    while(!vector_covered(map_vect,mark_symbol)){
        std::queue<std::string> vertex_container;   // Should only contain marked vertices
        std::set<std::string> vertices_encountered; // Maintain list of encountered vertices 
        std::map<std::string, std::set<std::string> > connected_comp_map;
        
        if(!enqueue_source_node(vertex_container, map_vect, mark_symbol)){
            std::cout << "Error in connected_components function" << std::endl;
            return;
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

void triangles(std::string file_path, std::vector< std::set< std::set<std::string> > >& triangle_holder){
    
    std::map<std::string, std::set<std::string> > adj_list;
    std::vector<std::map<std::string, std::set<std::string> > > c_components;
  
    load_adj_list(adj_list, file_path);
    connected_components(adj_list, c_components);
    
    // Find triangles of each connected component
    for(std::vector<std::map<std::string, std::set<std::string> > >::iterator itr_begin = c_components.begin();
        itr_begin != c_components.end();
        itr_begin++)
    {
        std::vector<std::string> vertex_order;
        degree_vector(*itr_begin, vertex_order);
        find_triangles(*itr_begin, vertex_order, triangle_holder);
    }
    
}

std::string trim_end_commas(std::string gene){
    if(gene.size() >=2)
        return gene.substr(1, gene.size()-2);
    else
        return gene;
}

std::vector<std::string> string_split(std::string s, const char delimiter, bool header)
{
    size_t start=0;
    size_t end=s.find_first_of(delimiter);
    
    std::vector<std::string> output;
    
    while (end <= std::string::npos)
    {
        if(header)
	    output.push_back(trim_end_commas(trim(s.substr(start, end-start))));
        else
            output.push_back(trim(s.substr(start, end-start)));

	    if (end == std::string::npos)
	    	break;

    	start=end+1;
    	end = s.find_first_of(delimiter, start);
    }
    
    return output;
}

void load_correlation_map(std::map<std::string, double> &correlation_map, std::string cor_file_path){
    
    std::ifstream reader;
    reader.open(cor_file_path.c_str());
    std::string line = "";
    std::getline(reader,line);
    std::vector<std::string> header = string_split(line, '\t', true);
    
    while(!reader.eof()){
        std::getline(reader,line);
        std::vector<std::string> gene_row_data = string_split(line, '\t', false);
        
        for(int i = 1; i < gene_row_data.size(); i++){
            std::string gene_tag = header[i-1] + "_" + trim_end_commas(gene_row_data[0]);
            correlation_map.insert(std::pair<std::string, double>(gene_tag, std::atof(gene_row_data[i].c_str())));
        }
    }
}

void print_triangle_types(std::vector<Triangle> all_triangles){
    
    int neg_neg_neg = 0; 
    int pos_neg_pos = 0; 
    int neg_pos_neg = 0; 
    int pos_pos_pos = 0;
    
    for(std::vector<Triangle>::iterator itr_begin = all_triangles.begin();
        itr_begin != all_triangles.end();
        itr_begin++)
    {
        if(itr_begin->a_valid_triangle()){
            
            if(itr_begin->neg_neg_neg())
                neg_neg_neg++;
            
            if(itr_begin->neg_pos_neg())
                neg_pos_neg++;
            
            if(itr_begin->pos_pos_pos())
                pos_pos_pos++;
            
            if(itr_begin->pos_neg_pos())
               pos_neg_pos++; 
        }
    }
    
    std::cout << "neg_neg_neg: ";
    std::cout << neg_neg_neg << std::endl;
    
    std::cout << "pos_neg_pos: ";
    std::cout << pos_neg_pos << std::endl;
    
    std::cout << "neg_pos_neg: ";
    std::cout << neg_pos_neg << std::endl;
    
    std::cout << "pos_pos_pos: ";
    std::cout << pos_pos_pos << std::endl;
}

#endif	/* FUNCTIONS_H */

