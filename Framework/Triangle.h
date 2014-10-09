#include <set>
#include <math.h>

#ifndef TRIANGLE_H
#define	TRIANGLE_H

class Triangle{
    
public:    
    Triangle(std::set<std::string> triangle, std::map<std::string, double> correlation_map, double threshold){
        
        std::vector<std::string> vect_tri;
        std::copy(triangle.begin(), triangle.end(), std::back_inserter(vect_tri));
        valid_triangle = true; // assume true until proven otherwise
        
        std::string edge_1_name = vect_tri[0] + "_" + vect_tri[1];
        std::string edge_2_name = vect_tri[1] + "_" + vect_tri[2];
        std::string edge_3_name = vect_tri[2] + "_" + vect_tri[0];
        
        if(correlation_map.count(edge_1_name) != 0)
            edge_1 = correlation_map[edge_1_name];
        else
            valid_triangle = false;
               
        if(correlation_map.count(edge_2_name) != 0 )
            edge_2 = correlation_map[edge_2_name];
        else
            valid_triangle = false;
        
        if(correlation_map.count(edge_3_name) != 0 )
            edge_3 = correlation_map[edge_3_name];
        else
            valid_triangle = false;
        
        correlation_threshold = threshold;
    }
    
    bool pos_pos_pos(){
        if(( edge_1 >= fabs(correlation_threshold) ) && 
           ( edge_2 >= fabs(correlation_threshold) ) &&
           ( edge_3 >= fabs(correlation_threshold) )
          )
            return true;
        else
            return false;
    }
    
    bool neg_neg_neg(){
        if(( edge_1 <= (-1*fabs(correlation_threshold)) ) && 
           ( edge_2 <= (-1*fabs(correlation_threshold)) ) &&
           ( edge_3 <= (-1*fabs(correlation_threshold)) )
          )
            return true;
        else
            return false;    
    }
    
    bool pos_neg_pos(){
        if(( edge_1 >= (fabs(correlation_threshold)) ) && 
           ( edge_2 >= (fabs(correlation_threshold)) ) &&
           ( edge_3 <= (-1*fabs(correlation_threshold)) )
          )
            return true; 
        
        if(( edge_1 >= (fabs(correlation_threshold)) ) && 
           ( edge_2 <= (-1*fabs(correlation_threshold)) ) &&
           ( edge_3 >= (fabs(correlation_threshold)) )
          )
            return true; 
        
        if(( edge_1 <= (-1*fabs(correlation_threshold)) ) && 
           ( edge_2 >= (fabs(correlation_threshold)) ) &&
           ( edge_3 >= (fabs(correlation_threshold)) )
          )
            return true; 
        
        return false;
    }
    
    bool neg_pos_neg(){
        if(( edge_1 <= (-1*fabs(correlation_threshold)) ) && 
           ( edge_2 <= (-1*fabs(correlation_threshold)) ) &&
           ( edge_3 >= (fabs(correlation_threshold)) )
          )
            return true; 
        
        if(( edge_1 <= (-1*fabs(correlation_threshold)) ) && 
           ( edge_2 >= (fabs(correlation_threshold)) ) &&
           ( edge_3 <= (-1*fabs(correlation_threshold)) )
          )
            return true; 
        
        if(( edge_1 >= (-1*fabs(correlation_threshold)) ) && 
           ( edge_2 <= (1*fabs(correlation_threshold)) ) &&
           ( edge_3 <= (1*fabs(correlation_threshold)) )
          )
            return true; 
        
        return false;
    }
    
    bool a_valid_triangle(){
        return valid_triangle;
    }
    
private:
    double correlation_threshold;
    bool valid_triangle; 
    double edge_1;
    double edge_2;
    double edge_3;
};

#endif	/* TRIANGLE_H */

