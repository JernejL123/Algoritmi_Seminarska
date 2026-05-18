#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>

typedef long long ll;
ll N,K,P;

void read_file(std::string filename){
std::ifstream file(filename);
    if (!file.is_open()) {
        //std::cout << "Napaka pri odpiranju fajla" << std::endl;
        return;
    }
    file >> N >> K >> P;
    std::cout << "N: " << N << " K: " << K << " P: " << P << std::endl;
}

int main() {
    
    std::fstream file;
    for (size_t i = 1; i < 20; i++)
    {
        std::ostringstream fileName;
        fileName << "./testni_primeri/" << std::setw(2) << std::setfill('0') << i << ".in";
        read_file(fileName.str());
    }

return 0;

}