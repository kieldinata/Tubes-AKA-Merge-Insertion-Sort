#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

// ================== WARNA ==================
#define RESET   "\033[0m"
#define BLUE    "\033[34m"
#define YELLOW  "\033[33m"

// ================== STRUCT ==================
struct Barang {
    string nama;
    float berat;
};

// ================== MEMORY TRACKER ==================
size_t totalAllocated = 0;

void* operator new(size_t size) {
    totalAllocated += size;
    return malloc(size);
}

void* operator new[](size_t size) {
    totalAllocated += size;
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// untuk generate barang secara acak
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


void combine(vector<Barang>& data, int left, int mid, int right) {
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

void mergeSortRecursive(vector<Barang>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortRecursive(data, left, mid);
        mergeSortRecursive(data, mid + 1, right);
        combine(data, left, mid, right);
    }
}


void mergeSortIterative(vector<Barang>& data) {
    int n = data.size();

    for (int size = 1; size < n; size *= 2) {
        for (int left = 0; left < n - 1; left += 2 * size) {
            int mid = min(left + size - 1, n - 1);
            int right = min(left + 2 * size - 1, n - 1);

            if (mid < right)
                combine(data, left, mid, right);
        }
    }
}


int main() {
    srand(time(0));

    int jumlahTest;
    int JUMLAH_BARANG = 10;

    cout << "Jumlah testcase: ";
    cin >> jumlahTest;

    for (int i = 1; i <= jumlahTest; i++) {

        vector<Barang> dataAwal = generateBarang(JUMLAH_BARANG);
        vector<Barang> dataRecursive = dataAwal;
        vector<Barang> dataIterative = dataAwal;

        // rekursif
        totalAllocated = 0;
        auto startRec = high_resolution_clock::now();
        mergeSortRecursive(dataRecursive, 0, dataRecursive.size() - 1);
        auto endRec = high_resolution_clock::now();
        auto waktuRec = duration_cast<milliseconds>(endRec - startRec).count();
        size_t memRec = totalAllocated;

        // iteratif
        totalAllocated = 0;
        auto startIter = high_resolution_clock::now();
        mergeSortIterative(dataIterative);
        auto endIter = high_resolution_clock::now();
        auto waktuIter = duration_cast<milliseconds>(endIter - startIter).count();
        size_t memIter = totalAllocated;

        cout << "\n===== TEST CASE NO." << i << " ====="
             << "\nItems : " << BLUE << JUMLAH_BARANG << RESET
             << "\nRecursive : " << YELLOW << waktuRec << RESET << " ms"
             << " | Memory : " << YELLOW << memRec << RESET << " bytes"
             << "\nIterative : " << YELLOW << waktuIter << RESET << " ms"
             << " | Memory : " << YELLOW << memIter << RESET << " bytes\n";

        JUMLAH_BARANG *= 1.3;
    }

    return 0;
}
