#include <iostream>
#include <fstream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <set>
#include <vector>
#include <climits>
#include <chrono>

long N,K,P;
long calculateNumOfPINs(long N, long K, long P);

void read_file(std::string filename){
std::ifstream file(filename);
    if (!file.is_open()) {
        //std::cout << "Napaka pri odpiranju fajla" << std::endl;
        file.close(); //Misja hack: "Če fajl ni odprt ga zapri lol"
        return;
    }
    file >> N >> K >> P;
    //std::cout << "N: " << N << " K: " << K << " P: " << P << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    std::cout <<"stevilo ok kombinacij: " << calculateNumOfPINs(N,K,P) << std::endl; 

    auto finish = std::chrono::high_resolution_clock::now();


    auto milliseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
    std::cout<<"cas delovanja: " << milliseconds.count() << "ms\n"<<std::endl;
    
    file.close();
}

long calculateNumOfPINs(long N, long K, long P){
    long output = 0;
    uint64_t numOfOptions = (long)pow(N,P);

    std::cout << "St vseh kombinacij: " << numOfOptions<<std::endl;
    
    uint64_t count = 0;

    for (long i = 0; i < numOfOptions; i++){
        uint64_t temp = i;
        std::vector<int> pin(P);

        uint64_t ost;

        for (int j = 0; j < P; j++) {
            pin[j] = temp % N;
            temp = temp/N;
        }

        std::set<int> razlicne(pin.begin(), pin.end());

         if (razlicne.size() >= K) {
            count++;
            /*
            if (count%1000 == 0){
                std::cout <<"stevilo resitev je: "<<count<<std::endl;
            }
            */
        }
       
    }

    int modul = pow(10,9) - 7;

    output = (count%modul);
    

    return output;
}

int main() {
    
    std::fstream file;
    for (size_t i = 1; i < 20; i++)
    {
        std::ostringstream fileName;
        fileName << "./testni_primeri/" << std::setw(2) << std::setfill('0') << i << ".in";
        std::cout << fileName.str()<<std::endl;
        read_file(fileName.str());
    }

    return 0;
}