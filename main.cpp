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

//[INSERTION SORT]
void insertionSort(vector<Barang>& data) {
    int n = data.size();
    for (int i = 1; i < n; i++) {
        Barang key = data[i];
        int j = i - 1;

        while (j >= 0 && data[j].berat > key.berat) {
            data[j + 1] = data[j];
            j--;
        }
        data[j + 1] = key;
    }
}

//[MERGE SORT]
//Bagian combine
void merge(vector<Barang>& data, int left, int mid, int right) {
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
//bagian divide
void mergeSort(vector<Barang>& data, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(data, left, mid);
        mergeSort(data, mid + 1, right);
        merge(data, left, mid, right);
    }
}

int main() {
    srand(time(0));

    int i;
    int JUMLAH_BARANG = 100;
    for (i = 1; i <= 4; i++){
        cout << "\n\nGenerate data inventori...\n";
        vector<Barang> dataAwal = generateBarang(JUMLAH_BARANG);

        vector<Barang> dataInsertion = dataAwal;
        vector<Barang> dataMerge = dataAwal;

        cout << "Menjalankan Insertion Sort...\n";
        auto startInsertion = high_resolution_clock::now();
        insertionSort(dataInsertion);
        auto endInsertion = high_resolution_clock::now();
        auto waktuInsertion = duration_cast<milliseconds>(
            endInsertion - startInsertion
        ).count();

        cout << "Menjalankan Merge Sort...\n";
        auto startMerge = high_resolution_clock::now();
        mergeSort(dataMerge, 0, dataMerge.size() - 1);
        auto endMerge = high_resolution_clock::now();
        auto waktuMerge = duration_cast<milliseconds>(
            endMerge - startMerge
        ).count();

        cout << "\n===== TEST CASE NO." << i << " =====\n";
        cout << "Jumlah Barang   : " << JUMLAH_BARANG << endl;
        cout << "Insertion Sort : " << waktuInsertion << " ms\n";
        cout << "Merge Sort     : " << waktuMerge << " ms\n";
        JUMLAH_BARANG *= 7;
    }
    return 0;
}
