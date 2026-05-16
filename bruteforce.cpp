#include<iostream>
#include<fstream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <set>

long N,K,P;
long calculateNumOfPINs(long N, long K, long P);

void read_file(std::string filename){
std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Napaka pri odpiranju fajla" << std::endl;
        file.close(); //Misja hack: "Če fajl ni odprt ga zapri lol"
        return;
    }
    file >> N >> K >> P;
    //std::cout << "N: " << N << " K: " << K << " P: " << P << std::endl;

    std::cout << calculateNumOfPINs(N,K,P) << std::endl; 
    file.close();
}

long calculateNumOfPINs(long N, long K, long P){
    long output = 0;
    std::set<std::set<long>> allOptions;
    long numOfOptions = (long)pow(N,P);
    
    for (long i = 0; i < numOfOptions; i++){
        
    }
    
    return output;
}

int main() {
    
    std::fstream file;
    for (size_t i = 0; i < 20; i++)
    {
        std::ostringstream fileName;
        fileName << "./testni_primeri/" << std::setw(2) << std::setfill('0') << i << ".in";
        read_file(fileName.str());
    }

    return 0;
}