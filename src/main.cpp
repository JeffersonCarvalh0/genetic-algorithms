# include "ga.hpp"
# include <fstream>
# include <vector>

using namespace std;

int main(int argc, char **argv) {
    int n, c;
    vector<int> weights, values;
    fstream file;

    if (argc == 1) {
        cout << "usage: " << argv[0] << " FILE...\n";
        return -1;
    }

    for (int i = 1; i < argc; ++i) {
        weights.clear(); values.clear();
        file.open(argv[i]);

        if (!file) {
            cout << "Unable to open file " << argv[i] << '\n';
        } else {
            cout << "File loaded successfully\n";

            file >> n >> c;
            weights.resize(n); values.resize(n);
            for (int i = 0; i < n; ++i) file >> values[i] >> weights[i];

            cout << "Finding the solution...\n";
            GA ga(weights, values, c);
            ga.start();
        }
    }

    return 0;
}
