// Wordcounter in C++

#include "header.hpp"


#include <iostream>
#include <unordered_map>
#include <string>


std::unordered_map<char, int> counter(std::string input)
{
    std::unordered_map<char, int> map;
    
    for (char c : input)
    {
        map[c] += 1;
    }
    
    return map;
}


void printresult(std::unordered_map<char, int> map)
{
    for (const auto& pair : map)
    {
        std::cout << pair.first << " -> " << pair.second << "\n";
    }
}


int wordcountermain()
{
    std::string msg = "halloooo";
    
    //std::string msg2 = "https://www.amazon.de/soundcore-Kabellose-Anpassbarer-wasserfest-verwendbar-Schwarz/dp/B0BTYCJXBK/ref=cs_sr_dp_5?__mk_de_DE= M %7D  &crid=CN8HPDHROY2N&dib=eyJ2IjoiMSJ9.6ft8qa33NN_j_hAvQQEK_ncKubACOn6lGlankF-cIXGAQwkUQ7RiA8F2Cv8e5NfPiXatWnFDlmdl6NMnk0Q51F7fu3DUyiqGlThymzJdsl4u5xACrcUdYVo2IVzyhAlogjIZPK06v5eRzD0Yo7YyzDtBWbNJd1IudcGTElVdmM9dKmlOLadyADH-9tXZJ21urwA1hin-0oCFJExPMMO4MfjE6tbAKi6S1N2m2OHhOp4.q0Rgay3B_-G8gyWg1L2W4XbdQW9sHklERPp83NJ7s_E&dib_tag=se&keywords=soundcore+p20i&qid=1722952363&s=instant-video&sprefix=soundcore+p20i%2Cinstant-video%2C87&sr=1-1-catcorr";
    
    std::cout << "WordCounter\n";
    std::cout << "Input String: " << msg << "\n";
    
    printresult(counter(msg));
    
    return 0;
}