#include "PlayfairCipher.hpp"


#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <map>
#include <vector>

PlayfairCipher::PlayfairCipher(const std::string& key) 
{
    setKey(key);
}

void PlayfairCipher::setKey(const std::string& key)
{
    // store the original key
    key_ = key;

    //Append the alphabet, add alphabet onto string
    key_ = key_ + alphabet_;
    //Make sure the key is upper case std::stransform algorithm 
    //first two things give thing want to copy over, transform takes each element in that range, copy put to output range, output range is overwritin key_ variable.
    std::transform(key_.begin(), key_.end(),key_.begin(), ::toupper);
    
    // Remove non-alpha characters 

    auto rem_iter = std::remove_if(key_.begin(), key_.end(), [] (char c) {return !std::isalpha(c);});

    // actually erase
    key_.erase(rem_iter, key_.end());
    
    // Change J-> I

    std::transform(key_.begin(), key_.end(), key_.begin(), [] (char c) {if (c== 'J') return 'I'; return c;} );
    
    // Remove duplicated letters
    std::string encountered_letters{""};
    auto detect_duplicates = [&] (char c) {
        if(encountered_letters.find(c) == std::string::npos)
        {
            encountered_letters += c;
            return false;
        }
        else
        {
            return true;
        }
    };
    auto rem_iter2 = std::remove_if(key_.begin(), key_.end(), detect_duplicates);
    key_.erase(rem_iter2, key_.end());

    // Store the coords of each letter
    // loop over each letter
    //calculate row and colunn numbers and store both letter and std::pair of coords in map

    //create vectors storing column and row numbers for each element in key_ 
    std::vector<int> column_number = {};
    std::vector<int> row_number = {};
    column_number.resize(key_.size());
    row_number.resize(key_.size());
    int i{-1};
    int j{-1};
    int k{0};
    std::generate(column_number.begin(), column_number.end(), [&] () {i+=1; return i%gridsize;} );
    std::generate(row_number.begin(), row_number.end(), [&] () {if (j%gridsize == gridsize-1) k+=1; j+=1; return k;});
    // Store the playfair cipher key map
    // need two maps stored as members of the class one to go from letter to coord and one to go from coord to letter
    for (size_t l{0}; l < key_.size(); l++){
        auto coords { std::make_pair( row_number[l], column_number[l] ) };
        auto element1 { std::make_pair(key_[l], coords) };
        auto element2 { std::make_pair(coords, key_[l]) };
        map_letter_to_coords.insert(element1);
        map_coords_to_letters.insert(element2);
    }

}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                      const CipherMode cipherMode) const
{   std::string input = inputText;
    std::string output{""};
    
    // Change J → I
    std::transform(input.begin(), input.end(), input.begin(), [] (char c) {if (c== 'J') return 'I'; return c;} );
    // If repeated chars in a digraph add an X or Q if X
    // handle duplicated letters   
    for (size_t i{0}; i < input.size(); i++){
        if (input[i]==input[i+1] && input[i]=='X'){
            input.insert(i+1, "Q");}
        else if (input[i]==input[i+1]){
            input.insert(i+1, "X");
        }    
    }

   // if the size of input is odd, add a trailing Z
    if (input.size()%2!=0){
        input+= "Z";
    }
    // Loop over the input in Digraphs
    for (size_t c{0}; c < input.size()-1; c+=2){
    // - Find the coords in the grid for each digraph
    
        auto d1_coords_iter = map_letter_to_coords.find(input[c]);
        auto d2_coords_iter = map_letter_to_coords.find(input[c+1]);
        int d1_row = ((*d1_coords_iter).second).first;
        int d1_column = ((*d1_coords_iter).second).second;
        int d2_row = ((*d2_coords_iter).second).first;
        int d2_column = ((*d2_coords_iter).second).second;
        // - Apply the rules to these coords to get 'new' coords
        // FOR encryption and decryption
        switch (cipherMode) {
            case CipherMode::Encrypt:
                   
                if(d1_row == d2_row ){
                    //replace d1 and d2 with ones directly to right
                    
                    d1_column = (1 + d1_column) % gridsize;
                    d2_column = (1 + d2_column) % gridsize;; 
                }
                else if(d1_column == d2_column){
                    //replace d1 and d2 with ones directly underneath
                    d1_row = (1 + d1_row) % gridsize;;
                    d2_row = (1 + d2_row) % gridsize;;
                }
                else {
                    //replace d1 with row d1 column d2 and d2 with row d2 column d1, basically swap column values
                    int temp = d1_column;
                    d1_column = d2_column;
                    d2_column = temp;
                }
                break;
            case CipherMode::Decrypt:
                if(d1_row == d2_row ){
                    //replace d1 and d2 with ones directly to left
                    d1_column = ( d1_column + gridsize - 1) % gridsize;
                    d2_column = (d2_column + gridsize - 1) % gridsize; 
                }
                else if(d1_column == d2_column){
                    //replace d1 and d2 with ones directly above
                    d1_row = (gridsize + d1_row -1) % gridsize;
                    d2_row = (gridsize + d2_row -1) % gridsize;
                }
                else {
                    //replace d1 with row d1 column d2 and d2 with row d2 column d1, basically swap column values
                    int temp = d1_column;
                    d1_column = d2_column;
                    d2_column = temp;  
                }
                break;
        }   
       

        // - Find the letter associated with the new coords
         auto coords_transformed_d1 { std::make_pair(d1_row, d1_column) };
         auto coords_transformed_d2 { std::make_pair(d2_row, d2_column) };
         auto d1_transformed_coords_iter = map_coords_to_letters.find(coords_transformed_d1);
         auto d2_transformed_coords_iter = map_coords_to_letters.find(coords_transformed_d2);        
         char d1_transformedletter = (*d1_transformed_coords_iter).second;
         char d2_transformedletter = (*d2_transformed_coords_iter).second;
        
         // return the output text
       
        output += d1_transformedletter;
        output += d2_transformedletter;
        }
    
   
    return output;
}