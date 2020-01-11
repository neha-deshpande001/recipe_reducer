#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <unordered_map>
#include <string>


//This function converts an input string (ex. "* 1" or "* 1/4"), converts it into an integer, then changes the yield size.
double convert(std::string myData, double factor){
	double value;

	//The space between * and 1 is actually a no-break space, so the actual number starts at index 3, not 2
	myData = myData.substr(3,myData.size());

	if(myData.find("/") != std::string::npos){ //the string contains /, meaning that it is a fraction
		int slashpos = myData.find("/");
    	int numerator = std::stoi(myData.substr(0,slashpos));
    	int denominator = std::stoi(myData.substr(slashpos+1));

    	value = factor * numerator/(double)denominator;
	}
	else{ // whole number
		value = factor * std::stoi(myData);
	}
	return value;
}

int main(int argc, char* argv[]) {
	int servings;
	int originalServings;
	std::string data = "";
	std::string input_file = "";
	std::string output_file = "";
	std::vector<std::string> allData;
  	bool saveString = false;
  	bool instructionsFound = false;
  	std::string firstWord;
  	bool firstWordFound = false;
  	bool nutritionFound = false;
  	bool insideIngredients = false;
  	bool cookFound = false;
  	bool prepFound = false;

	// parse the arguments
  	for (int i = 1; i < argc; i++) {
    	if (argv[i] == std::string("-i")) {
      		i++;
      		assert (i < argc);
      		input_file = argv[i];
    	}
    	else if (argv[i] == std::string("-o")) {
      		i++;
      		assert (i < argc);
      		output_file = argv[i];
    	}
    	else if (argv[i] == std::string("-servings")) {
      		i++;
      		assert (i < argc);
      		servings = std::stoi(argv[i]);
    	}
  	}

	// convert the strings to input and output files
	std::ifstream inFile(input_file);
	std::ofstream outFile(output_file);

	// transfers data from the input file
	if(inFile.is_open()){
	    while (inFile >> data) { // transfer all the data from the input file to the allData vector.
	    	if(!firstWordFound){
	    		firstWord = data;
	    		firstWordFound = true;
	    	}
	  		if(data == "Instructions"){
	  			if(allData.back() == "Save"){
	  				allData.pop_back();
	  			}
	  			instructionsFound = true;
	  		}
	  		if(data == "Blue"){ //transfer only the important parts of the input file
	  			saveString = false;
	  		}
	    	if(saveString && data != "Serving:"){
	  			allData.push_back(data);
	  		}
	  		if(data == "Yield:"){
	  			saveString = true;
	  			allData.push_back("Yield:");
	  		}
	    }
	    inFile.close();
	}
	else{ // If the input file cannot be opened, it throws an error message
	    std::cerr << "Unable to open input file." << std::endl;
	    exit(1);
	}

	if(!outFile.is_open()) {// If the output file cannot be opened, it throws an error message
		std::cerr << "Unable to open output file." << std::endl;
		exit(2);
	}

	//save how many servings the original recipe is written for
	originalServings = stoi(allData[1]);

	
	// now go through everything, eliminating unnecessary detail
	for(int i = 0; i < allData.size(); i++){
		if(allData[i].find("*") != std::string::npos) {
			outFile << std::endl << "    ";
		}
		// change the recipe's serving amount
		if(insideIngredients && allData[i].find("*") != std::string::npos){
			if(allData[i].find_first_of("0123456789") != std::string::npos){
				outFile << "  * " << convert(allData[i],servings/(double)originalServings) << " ";
				allData.erase(allData.begin() + i);	
			}
			else{
				outFile << "  ";
			}
		}
		if(allData[i+4] == "votes" || allData[i+4] == "vote"){
			allData.erase(allData.begin() + i + 4);
			allData.erase(allData.begin() + i + 3);
			allData.erase(allData.begin() + i + 2);
			allData.erase(allData.begin() + i + 1);
		}
		if(allData[i] == "* Divide"){
			allData[i+2] = std::to_string(servings);
		}
		if(allData[i] == "Prep" && !prepFound){
			outFile << std::endl << std::endl << "  ";
			prepFound = true;
		}
		if(allData[i] == "Cook" && !cookFound){
			outFile << std::endl << "  ";
			cookFound = true;
		}
		if(allData[i] == "Total" || allData[i] == "Course:" || allData[i] == "Cuisine:"){
			outFile << std::endl << "  ";
		}
		if(allData[i] == firstWord){
			outFile << std::endl << std::endl << "  ";
		}
		if(allData[i] == "Ingredients"){
			outFile << std::endl << std::endl << std::endl << "  ";
			insideIngredients = true;
		}
		if(allData[i] == "Instructions"){
			outFile << std::endl << std::endl << std::endl << "  ";
		}
		if(allData[i] == "Nutrition"){
			outFile << std::endl << std::endl << std::endl << "  ";
		}
		if(insideIngredients && allData[i].find(":") != std::string::npos){
			outFile << std::endl << std::endl << "  ";
		}
		if(allData[i] == "Calories:"){
			outFile << " ";
		}
		if(allData[i] == "Yield:"){
			allData[i+1] = std::to_string(servings);
		}

//===================== print to the output file =========================
		outFile << allData[i] << " ";
//========================================================================


		if(insideIngredients && allData[i].find(":") != std::string::npos){
			outFile << "  ";
		}
		if(nutritionFound && allData[i].find(",") != std::string::npos){
			outFile << std::endl << "  ";
		}
		if(allData[i] == "servings"){
			outFile << std::endl << std::endl;
		}
		if(allData[i] == "Ingredients"){
			outFile << std::endl;
		}
		if(allData[i] == "Instructions"){
			outFile << std::endl;
			insideIngredients = false;
		}
		if(allData[i] == "Nutrition"){
			outFile << std::endl << std::endl;
			nutritionFound = true;
			allData[i+1].erase();
		}
	}
	return 0;
}
