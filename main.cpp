#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <unordered_map>
#include <string>



//"* 2"
//"* 1/2"

// do all the math to find the ideal ingredient amounts, and add that to the output file

double convert(std::string myData, double factor){
	double value;

	// why should this start at 3??????? super-mysterious-devil-character-which-is-actually-two-characters?
	//std::string s = ; 
	myData = myData.substr(3,myData.size());

	if(myData.find("/") != std::string::npos){ //the string contains /, meaning that it is a fraction
		int slashpos = myData.find("/");
    	int numerator = std::stoi(myData.substr(0,slashpos));
    	int denominator = std::stoi(myData.substr(slashpos+1));

    	value = factor * numerator/(double)denominator;
	}
	else{
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
	  		if(data == "Blue"){
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
	else{ // If the file cannot be opened, it throws an error message
	    std::cerr << "Unable to open input file." << std::endl;
	    exit(1);
	}

	if(!outFile.is_open()){
		std::cerr << "Unable to open output file." << std::endl;
		exit(2);
	}

	originalServings = stoi(allData[1]);

	
	for(int i = 0; i < allData.size(); i++){
		if(allData[i].find("*") != std::string::npos) {
			outFile << std::endl << "    ";
		}
		
		if(insideIngredients && allData[i].find("*") != std::string::npos){
			outFile << "  * " << convert(allData[i],servings/(double)originalServings) << " ";
			allData.erase(allData.begin() + i);	
		}
		if(allData[i+4] == "votes"){
			allData.erase(allData.begin() + i + 4);
			allData.erase(allData.begin() + i + 3);
			allData.erase(allData.begin() + i + 2);
			allData.erase(allData.begin() + i + 1);
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

//===================== this is the important part =======================
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
			//words.insert(std::make_pair("servings",true));
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



//std::stoi("1/4") //1
//std::stoi("/4") //error


//this programa ssumes that all the input recipe files are correct

//the below converts 

//http://elinks.or.cz/documentation/manpages/elinks.conf.5.html#document.dump.color_mode
//http://elinks.or.cz/documentation/manpages/elinks.1.html

//sudo apt-get install elinks
//elinks -dump -no-numbering -no-references a.html > original_recipe_file.txt
//elinks -dump -no-numbering -no-references https://www.skinnytaste.com/wprm_print/49040 > original_recipe_file.txt


