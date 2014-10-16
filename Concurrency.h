
// Notes: The key use of this file is in its construction of the concurrent_edge_lookup() function. Given the 
// 		  the appropriate parameters this function will concurrently look for the given correlations.

// Required directives: <iostream>, <thread>, <fstream>, <vector>  

// Future Goals for Code: Make files generated by threads appear in a directory and then delete that
//						  directory when done 

using namespace std;

// Exception class 
class inverted_edge_name_error{
public:
	inverted_edge_name_error(){
		cout << "The edge names are not in the correct format." << endl;
		cout << "Check inverted_edge_name()  and make sure input edge names are correct" << endl;
		exit(0);
	}
};

// Expects an edge name of the form gene1_gene2_ and will return gene_2_gene_1
string inverted_edge_name(string edge_name){
	
	string gene1 = edge_name.substr(0, edge_name.find("_"));
	string gene2 = edge_name.substr(edge_name.find("_") + 1, edge_name.size() - edge_name.find("_") - 2);
	string inverted_edge = gene2 + "_" + gene1 + "_";
	
	int count_dashes = 0; 
	for(int i = 0; i < inverted_edge.size(); i++){
		if(inverted_edge[i] == '_')
			count_dashes++;
	}
	
	if(count_dashes == 2)
		return inverted_edge;
	
	throw inverted_edge_name_error(); 
		
	return "";
}

// Each thread should output the correlation value for the given edge to a file named "out_file_name".txt
void task1(string in_file_name, string out_file_name, string edge_name){
	
	ifstream edge_list;
	ofstream edge_corr;
    
	edge_list.open(in_file_name.c_str());

	if(!edge_list.is_open()){
		return; // If the correlation file doesn't exist you do not want to continue  
	}
	
	edge_corr.open(out_file_name.c_str());
	
    string edge = "";
	
    while(!edge_list.eof()){
        
        getline(edge_list,edge);

		// Exclude empty lines and determine if the current edge starts with either the string or its inverse 
		// remember that the correlation file only holds half of the full correlation matrix with all pairs 
		if((edge.size() > 0) && ( (edge.find(edge_name) == 0) || (edge.find(inverted_edge_name(edge_name)) == 0)) ){
			
			string pre_weight_one = edge.substr(edge.find("_") + 1, edge.size()-edge.find("_"));
			string pre_weight_two = pre_weight_one.substr(pre_weight_one.find("_") + 1, pre_weight_one.size()-pre_weight_one.find("_"));
			double edge_weight = stod(pre_weight_two);
			edge_corr << edge_weight; 
			
			break; // exit loop
		}
	}

	edge_list.close();
	edge_corr.close();
}

// The only danger of this function is that 'rm edge*.txt' is called. If any files exist that match this expression, it will be deleted 
void concurrent_edge_lookup(string correlation_list, string edge_1, string edge_2, string edge_3, vector<double>& edge_weight_container){
		
	double edge_weight_1 = 0; 
	double edge_weight_2 = 0; 
	double edge_weight_3 = 0; 
	
	string name_edge1_file = "edge1.txt";
	string name_edge2_file = "edge2.txt";
	string name_edge3_file = "edge3.txt";

	thread t1(task1, correlation_list, name_edge1_file, edge_1);
	thread t2(task1, correlation_list, name_edge2_file, edge_2);
	thread t3(task1, correlation_list, name_edge3_file, edge_3);
	
	t1.join(); 
	t2.join();
	t3.join();
	
	ifstream edge1_reader;
	ifstream edge2_reader;
	ifstream edge3_reader;
	
	edge1_reader.open(name_edge1_file.c_str());
	edge2_reader.open(name_edge2_file.c_str());
	edge3_reader.open(name_edge3_file.c_str());
	
	if(!edge1_reader.is_open() || !edge2_reader.is_open() || !edge3_reader.is_open()){
		cout << "A correlation file is missing (ie an edge#_reader stream failed to open)" << endl;
		cout << "Make sure that the file with the formatted correlations exists" << endl;
		return;
	}
	
	string edge1_line = "";
	string edge2_line = "";
	string edge3_line = "";
	
	while(!edge1_reader.eof()){
		getline(edge1_reader, edge1_line);
		try{
			edge_weight_1 = stod(edge1_line);
		}catch(invalid_argument &e){
			cout << "stod() failed to convert string to double" << endl;
			return; 
		}catch(...){
			cout << "Unknown error occurred when trying to convert string to double" << endl;
			return; 
		}
	}
	
	while(!edge2_reader.eof()){
		getline(edge2_reader, edge2_line);
		try{
			edge_weight_2 = stod(edge2_line);
		}catch(invalid_argument &e){
			cout << "stod() failed to convert string to double" << endl;
			return; 
		}catch(...){
			cout << "Unknown error occurred when trying to convert string to double" << endl;
			return; 
		}
	}
	
	while(!edge3_reader.eof()){
		getline(edge3_reader, edge3_line);
		try{
			edge_weight_3 = stod(edge3_line);
		}catch(invalid_argument &e){
			cout << "stod() failed to convert string to double" << endl;
			return; 
		}catch(...){
			cout << "Unknown error occured when trying to convert string to double" << endl;
			return; 
		}
	}
	
	edge_weight_container.push_back(edge_weight_1);
	edge_weight_container.push_back(edge_weight_2);
	edge_weight_container.push_back(edge_weight_3);
	
	system("rm edge*.txt"); // Warning! 
	
	return;
}