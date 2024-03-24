#include"pSS.h"

void randPermutation(vector<int> &O) {
    int n = O.size();

    {
        int i;
#pragma omp parallel for shared(O, n) private(i)
        for (i = 0; i < n; i++)
            O[i] = i;
    }

    shuffle(O.begin(), O.end(), gen);
}

int greedyColoring(vector<vector<int>> &A, vector<int> &O, vector<int> &C) {
    int n = C.size();

    {
        int i;
#pragma omp parallel for shared(C, n) private(i)
        for (i = 0; i < n; i++)
            C[i] = 0;
    }
    int max_c = (n > 0);

    vector<bool> N_v(n);

    for (int i = 0; i < n; i++) {
        int v = O[i];

        {
            int j;
#pragma omp parallel for shared(N_v, i) private(j)
            for (j = 0; j < i; j++)
                N_v[j] = 0;
        }

        int j = 0;
        for (int x : A[v]) {
            if (C[x] > j) {
                N_v[C[x] - 1] = 1;
                while (N_v[j])
                    j++;
            }
        }

        max_c = max(max_c, C[v] = j + 1);
    }
    return max_c;
}

int fast_recoloring(vector<vector<int>> &A, vector<int> &O, vector<int> &C,
                    int max_c) {
    vector<int> I(max_c), It(max_c), Ot(max_c);

    {
        int i;
#pragma omp parallel for shared(I, It, max_c) private(i)
        for (i = 0; i < max_c; i++) {
            I[i] = It[i] = 0;
        }
    }

    int n = C.size();

    randPermutation(Ot);

    {
        int i;
#pragma omp parallel for shared(I, Ot, C, n) private(i)
        for (i = 0; i < n; i++) {
            I[Ot[C[i] - 1]]++;
        }
    }

    for (int i = 1, sum = I[0]; i < max_c; i++) {
        I[i] = (sum += I[i]) - I[i];
    }

    I[0] = 0;

    for (int i = 0; i < n; i++) {
        int c = Ot[C[i] - 1];
        O[I[c] + It[c]] = i;
        It[c]++;
    }

    return greedyColoring(A, O, C);
}

int SS(vector<vector<int>> &A, int n) {
    vector<int> O(n), C(n);

    randPermutation(O);
    int max_c = greedyColoring(A, O, C);

    int max_c_master = max_c;

    while (1) {
        max_c = fast_recoloring(A, O, C, max_c);
        if (max_c < max_c_master) {
            max_c_master = max_c;
            cout << max_c_master << endl;
        }
    }
    return max_c_master;
}

bool readfile(string fileName, vector<vector<int>> &A, int &n, int &e) {
    ifstream file(fileName);
    if (!(file.is_open()))
        return 0;
    string st[2], line;
    while (getline(file, line)) {
        if (line[0] == 'p') {
            stringstream s(line);
            s >> st[0] >> st[1] >> n >> e;
            A.resize(n);
            for (int i = 0; i < n; i++)
                A[i].resize(0);
        } else if (line[0] == 'e') {
            stringstream s(line);
            int x, y;
            s >> st[0] >> x >> y;
            A[x - 1].push_back(y - 1);
            A[y - 1].push_back(x - 1);
        }
    }
    file.close();
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    int n, e;
    vector<vector<int>> A;

    if (readfile(argv[1], A, n, e)) {
        SS(A, n);
    }

    return 0;
}
