#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>

typedef long long ll;
ll N,K,P;
ll MOD = 1e9+7;
ll calculateNumOfPINs(std::vector<std::vector<ll>>& dp);

void read_file(std::string filename){
std::ifstream file(filename);
    if (!file.is_open()) {
        //std::cout << "Napaka pri odpiranju fajla" << std::endl;
        return;
    }
    file >> N >> K >> P;
    //std::cout << "N: " << N << " K: " << K << " P: " << P << std::endl;
    std::vector<std::vector<ll>> dp(P + 1, std::vector<ll>(N + 1, 0));
    dp[0][0] = 1;
    std::cout << calculateNumOfPINs(dp) << std::endl;
}

ll calculateNumOfPINs(std::vector<std::vector<ll>>& dp){
    ll output = 0;
    for (ll p = 1; p <= P; p++){
        for (ll n= 1; n <= N; n++){
            dp[p][n] = dp[p - 1][n] * n + (N - (n-1)) * dp[p - 1][n - 1];
            //std::cout << dp[p][n] << " ";
            dp[p][n] %= MOD;
        }
        //std::cout << std::endl;
    }
    for (ll k = K; k <= N; k++){
        output += dp[P][k];
        output %= MOD;
    }
    return output;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Napaka: ./optimal <file_name>" << std::endl;
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