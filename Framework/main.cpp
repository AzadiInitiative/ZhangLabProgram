#include "Functions.h"
#include "Triangle.h"


int main() {
    
    // Path to PPI Network Edge List & Correlation Data. The correlation threshold is also defined
    std::string file_path = "\\Users\\Michael\\Desktop\\Lab_Data\\edgelist.txt";
    std::string cor_file_path = "\\Users\\Michael\\Desktop\\Lab_Data\\test.txt";
    double correlation_threshold = 0.05;
    
    // Define container for triangle coordinates and fill using the triangles() function 
    std::vector< std::set< std::set<std::string> > > triangles_coord; 
    triangles(file_path, triangles_coord);
    
    // Define correlation table and fill using load_correlation_map() function  
    std::map<std::string, double> correlation_map;
    load_correlation_map(correlation_map, cor_file_path);
    
    // This container will hold all the triangles in the graph 
    std::vector<Triangle> all_triangles;  
    
    // Loop through all triangles in container and add to the vector 
    for(std::vector< std::set< std::set<std::string> > >::iterator vect_itr_b = triangles_coord.begin();
        vect_itr_b != triangles_coord.end();
        vect_itr_b++)
    {
        for(std::set< std::set<std::string> >::iterator set_itr_b = vect_itr_b->begin();
            set_itr_b != vect_itr_b->end();
            set_itr_b++)
        {
            Triangle graph_triangle(*set_itr_b, correlation_map, correlation_threshold);
            all_triangles.push_back(graph_triangle);
        }
    }
    
    // Output number of each type of triangle  
    print_triangle_types(all_triangles);
    
    return 0;
}

