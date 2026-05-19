#include <iostream>
#include <fstream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <set>
#include <vector>
#include <climits>
long N,K,P;
long calculateNumOfPINs(long N, long K, long P);

void read_file(std::string filename){
std::ifstream file(filename);
    if (!file.is_open()) {
        //std::cout << "Napaka pri odpiranju fajla" << std::endl;
        return;
    }
    file >> N >> K >> P;
    //std::cout << "N: " << N << " K: " << K << " P: " << P << std::endl;

    std::cout << calculateNumOfPINs(N,K,P) << std::endl; 
    file.close();
}

long calculateNumOfPINs(long N, long K, long P){
    long output = 0;
    uint64_t numOfOptions = (long)pow(N,P);
    //std::cout << "St vseh kombinacij: " << numOfOptions<<std::endl;
    uint64_t count = 0;

    for (long i = 0; i < numOfOptions; i++){
        uint64_t temp = i;
        std::vector<int> pin(P);

        for (int j = 0; j < P; j++) {
            pin[j] = temp % N;
            temp = temp/N;
        }
        std::set<int> razlicne(pin.begin(), pin.end());

         if (razlicne.size() >= K) {
            count++;
        }
    }
    int modul = pow(10,9) - 7;
    output = (count%modul);
    return output;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Napaka: ./bruteforce <file_name>" << std::endl;
    }

    /*std::fstream file;
    for (size_t i = 1; i < 20; i++)
    {
        std::ostringstream fileName;
        fileName << "./testni_primeri/" << std::setw(2) << std::setfill('0') << i << ".in";
        read_file(fileName.str());
    }*/
    read_file(argv[1]);

    return 0;
}