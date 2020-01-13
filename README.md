# Recipe Reducer

## Overview
The purpose of the Recipe Reducer is to change a recipe's yield and ingredients proportionally, based on an input value. Rather than manually converting each ingredient's amount for every dish that I wanted to cook, I decided to write a program to do it automatically. The program takes in an input .txt file from [SkinnyTaste.com](https://www.skinnytaste.com/), which has a mostly consistent format for the each recipe.


The program reads the original recipe's intended servings through the input file. 
The relationship between the ingredients and servings is proportional:

```
(intended ingredient amount) / (original ingredient amount) = (intended servings) / (original servings)
```

Multiplying both sides by the original ingredient amount yields an equation for finding the conversion factor for the intended ingredient amount, given 3 inputs.

```
(intended ingredient amount) = (servings that you want) / (original servings) * (original ingredient amount)
```


## Installation and Running Instructions
1. Download [ELinks](http://elinks.or.cz/)
``` 
sudo apt-get install elinks
```

2. Find the recipe that you want to reduce from [SkinnyTaste.com](https://www.skinnytaste.com/)

3. Right click the black "PRINT" on the recipe button and click "Copy link address"
``` 
insert screenshot here
```

4. Convert .html file to a .txt file using ELinks
``` 
elinks -dump -no-numbering -no-references [input_link.html] > ranch_chicken_salad_original.txt
```

5. Download and unzip [this repository](https://github.com/neha-deshpande001/recipe_reducer/archive/master.zip)

6. compile ```main.cpp```
```
g++ main.cpp -o main.out
```

7. run the program with the following command line arguments
```
./main.out -i [input_file.txt] -o [output_file.txt] -servings [number]
```

## Example
I want to cook a salad for myself and my brother (2 people). First, I would find the recipe on skinnytaste.com. However, I realize that the original recipe is for 4 servings, but I only want to cook 2 servings. Rather than manually changing the amount of each ingredient, I would instead convert the recipe using the Recipe Reducer.

I would copy the link address from skinnytaste.com
``` 
https://www.skinnytaste.com/wprm_print/47600
```

After that, I would use ELinks to convert the website from a .html file to a .txt file
``` 
elinks -dump -no-numbering -no-references <link> > ranch_chicken_salad_original.txt
```

Then, I would compile ```main.cpp```
```
g++ main.cpp -o main.out
```

Finally, I would run the program with the following arguments:
```
./main.out -i ranch_chicken_salad_original.txt -o ranch_chicken_salad_2_servings.txt -servings 2
```

The output file, ```ranch_chicken_salad_2_servings.txt```, would have the recipe edited for the yield to be 2 servings. It preserves information about the preparation/cooking instructions as well as nutrition facts.


Suppose that my brother and I like the salad so much that I decide to make the the salad again, but for my whole extended family, 10 people. To convert the recipe to make 10 servings, I can use the same steps as above, except when running the program, I would change the number of servings in the command line argument to 10.


Note: this project has no affiliation with skinnytaste.com.
