#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

struct Barang {
    string nama;
    float berat;
};

#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"


//[GENERATOR]
// Ini generate barang dengan berat antara 0 sampai 1000
vector<Barang> generateBarang(int jumlah) {
    vector<Barang> data;
    data.reserve(jumlah);

    for (int i = 0; i < jumlah; i++) {
        Barang b;
        b.nama = "Barang_" + to_string(i + 1);
        b.berat = (rand() % 10000) / 10.0f; 
        data.push_back(b);
    }
    return data;
}



//Bagian combine ITERATIVE
void mergeIterative(vector<Barang>& data, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Barang> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = data[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = data[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].berat <= R[j].berat)
            data[k++] = L[i++];
        else
            data[k++] = R[j++];
    }

    while (i < n1)
        data[k++] = L[i++];

    while (j < n2)
        data[k++] = R[j++];
}
// merge sort ITERATIVE
void mergeSortIterative(vector<Barang>& data) {
    int n = data.size();

    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = min(left + size - 1, n - 1);
            int right = min(left + 2 * size - 1, n - 1);

            if (mid < right)
                mergeIterative(data, left, mid, right);
        }
    }
}



//Bagian combine RECURSIVE
void mergeRecursiveStep(
    vector<Barang>& data,
    vector<Barang>& L,
    vector<Barang>& R,
    int i, int j, int k
) {
    if (i == L.size() && j == R.size())
        return;

    if (i == L.size()) {
        data[k] = R[j];
        mergeRecursiveStep(data, L, R, i, j + 1, k + 1);
    }
    else if (j == R.size()) {
        data[k] = L[i];
        mergeRecursiveStep(data, L, R, i + 1, j, k + 1);
    }
    else if (L[i].berat <= R[j].berat) {
        data[k] = L[i];
        mergeRecursiveStep(data, L, R, i + 1, j, k + 1);
    }
    else {
        data[k] = R[j];
        mergeRecursiveStep(data, L, R, i, j + 1, k + 1);
    }
}
void mergeRecursive(vector<Barang>& data, int left, int mid, int right) {
    vector<Barang> L(data.begin() + left, data.begin() + mid + 1);
    vector<Barang> R(data.begin() + mid + 1, data.begin() + right + 1);

    mergeRecursiveStep(data, L, R, 0, 0, left);
}
// merge sort RECURSIVE
void mergeSortRecursive(vector<Barang>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRecursive(data, left, mid);
        mergeSortRecursive(data, mid + 1, right);
        mergeRecursive(data, left, mid, right);
    }
}

int main() {
    srand(time(0));

    int i;
    int JUMLAH_BARANG = 100;
    for (i = 1; i <= 30; i++){
        cout << "\n\nGenerate data inventori...\n";
        vector<Barang> dataAwal = generateBarang(JUMLAH_BARANG);

        vector<Barang> dataRecursive = dataAwal;
        vector<Barang> dataIterative = dataAwal;

        cout << "Menjalankan Merge Sort Recursive...\n";
        auto startRec = high_resolution_clock::now();
        mergeSortRecursive(dataRecursive, 0, dataRecursive.size() - 1);
        auto endRec = high_resolution_clock::now();
        auto waktuRec = duration_cast<milliseconds>(endRec - startRec).count();

        cout << "Menjalankan Merge Sort Iterative...\n";
        auto startIter = high_resolution_clock::now();
        mergeSortIterative(dataIterative);
        auto endIter = high_resolution_clock::now();
        auto waktuIter = duration_cast<milliseconds>(endIter - startIter).count();

        cout << "\n===== TEST CASE NO." << i << " =====\n"
        << "Items : " << BLUE << JUMLAH_BARANG << RESET
        << "\nRecursive : " << YELLOW << waktuRec << RESET << " ms"
        << "\nIterative : " << YELLOW << waktuIter << RESET << " ms";

        JUMLAH_BARANG *= 1.5;
    }
    return 0;
}
