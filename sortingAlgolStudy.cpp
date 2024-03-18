//
// Created by KAVOSH on 3/16/2024.
//
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <functional>
#include <chrono>


#define RUN_IS_SORTED
#define PRINT_ARRAY

const std::string DESCRIPTION{"insertion sort, quick sort, quick insertion with cutoff k and merge sort will be examined "
                              "by a randomly generated array of a specified size. "
                              "Program may output the sorted array each of algorithms produce, run std::is_sorted to double check "
                              "if it is really sorted and print the running time of the algorithm.\n"
                              "Note: make sure that k is a positive integer(k >= 1)."};
constexpr int MIN_RANDOM_N{0};
constexpr int MAX_RANDOM_N{100};


enum WhichAlgol{
    INSERTION,
    QUICK_SORT,
    QUICK_INSERTION,
    MERG,
};


template<typename T>
using CompFunc = std::function<bool(const T&, const T&)>;

class Timer
{
private:
    using Clock = std::chrono::steady_clock;
    using Second = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<Clock> m_beg{ Clock::now() };

public:

    void reset()
    {
        m_beg = Clock::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<Second>(Clock::now() - m_beg).count();
    }
};

template<typename T>
void printArray(std::vector<T>& a, std::ostream& out){
    for(int i{0}; i<a.size(); ++i){
        out<<a[i]<<' ';
    }
    out<<std::endl;
}


template<typename T>
void generateRandomVector(std::vector<T>& a){

    static std::random_device rd;
    static std::mt19937 mtEng(rd());

    std::uniform_int_distribution<> uid(MIN_RANDOM_N, MAX_RANDOM_N);

    for(int i{0}; i < a.size(); ++i)
        a[i] = uid(mtEng);
}


template<typename T>
void insertionSort(std::vector<T>& a, const CompFunc<T>& compFunc, int start, int end){
    for(int i{start + 1}; i<end; ++i){

        auto temp = std::move(a[i]);
        int j;

        for(j = i; j>start; --j){
            if(compFunc(temp, a[j - 1])){
                a[j] = std::move(a[j - 1]);
            }else{
                break;
            }
        }
        a[j] = std::move(temp);
    }
}


template <typename T>
const T & median3(std::vector <T> &a, int left, int right)
{
    int center = (left+right)/2;

    if (a[center] < a[left])
        std::swap(a[left], a[center]);

    if (a[right] < a[left])
        std::swap(a[left], a[right]);

    if (a[right] < a[center])
        std::swap(a[center], a[right]);

    std::swap(a[center], a[right - 1]); // Place pivot at position right - 1 10
    return a[right - 1];
}


template<typename T>
void quickSort(std::vector<T>& a, const CompFunc<T>& compFunc, int left, int right){

    if (left + 1 < right){

        const T& pivot = median3(a, left, right);
        int i=left, j=right - 1;

        for (;;){
            while(compFunc(a[++i] , pivot)) {}
            while(compFunc(pivot , a[--j])) {}
            if (i < j)
                std::swap(a[i], a[j]);
            else
                break;
        }
        std::swap(a[i], a[right-1]); // Restore pivot
        quickSort(a, compFunc, left, i-1); // Sort small elements
        quickSort(a, compFunc, i+1, right);
    }else{ // if the left and right are beside each other than manually sort them
        if(!compFunc(a[left], a[right]))
            std::swap(a[left], a[right]);
    }
}


template<typename T>
void quickInsertion(std::vector<T>& a, const CompFunc<T>& compFunc, int left, int right, int k){
    if (left + k < right) {

        const auto& pivot {median3(a, left, right)};

        int i {left};
        int j {right - 1};

        while (true) {
            while (compFunc(a[++i] , pivot));
            while (compFunc(pivot , a[--j]));
            if (i < j)
                std::swap(a[i], a[j]);
            else
                break;
        }
        std::swap(a[i], a[right-1]); // restore the new pivot
        quickInsertion(a, compFunc, left, i-1, k);
        quickInsertion(a, compFunc, i + 1, right, k);
    }else // if the interval is less than the interval size then do insertion sort because it is faster
        insertionSort<int>(a, compFunc, left, right+1);
}


template<typename T>
void merge(std::vector<T>& a, std::vector<T>& tempA,
                  int leftPos, int rightPos, int rightEnd,
                  const CompFunc<T>& compFunc){
    // left array
    int leftEnd{rightPos - 1};
    // and the leftPos

    // right array
    // the right End
    // the rightPos
    int tempIndex{leftPos};
    const int elementCount{rightEnd - leftPos + 1};

    while(leftPos <= leftEnd && rightPos <= rightEnd)
        if( compFunc(a[leftPos], a[rightPos]) )
            tempA[tempIndex++] = std::move(a[leftPos++]);
        else
            tempA[tempIndex++] = std::move(a[rightPos++]);

    // insert the remaining into tempA
    // inserting the remaining of left
    while(leftPos <= leftEnd)
        tempA[tempIndex++] = std::move(a[leftPos++]);
    // inserting the remaining of right
    while(rightPos <= rightEnd)
        tempA[tempIndex++] = std::move(a[rightPos++]);

    // insert the tempA to the a
    for(int i{0}; i<elementCount; ++i, --rightEnd){
        a[rightEnd] = std::move(tempA[rightEnd]);
    }
}


template<typename T>
void mergeSort(std::vector<T>& a, std::vector<T>& tempA, int start, int end, const CompFunc<T>& compFunc){

    if(start < end){
        int middle_n{ ( start + end ) / 2 };
        mergeSort(a, tempA, start, middle_n, compFunc);
        mergeSort(a, tempA, middle_n + 1, end, compFunc);
        merge(a, tempA, start, middle_n + 1, end, compFunc);
    }
}


// a set of deriver code for the internal implementations
template<typename T>
void mergeSort(std::vector<T>& a, const CompFunc<T>& compFunc){
    std::vector<T> tempA(a.size());
    mergeSort(a, tempA, 0, static_cast<int>(a.size()) - 1, compFunc);
}


template<typename T>
void quickSort(std::vector<T>& a, const CompFunc<T>& compFunc){
    // if its empty the second parameter would be -1 which breaks the algorithm
    if(a.empty())
        return;
    quickSort(a, compFunc, 0, static_cast<int>(a.size()) - 1);
}

// the value k must be an integer greater or equal to 1
// zero and negative will not work
template<typename T>
void quickInsertion(std::vector<T>& a, const CompFunc<T>& compFunc, int k){
    quickInsertion(a, compFunc, 0, static_cast<int>(a.size()) - 1, k);
}


template<typename T>
void insertionSort(std::vector<T>& a, const CompFunc<T>& compFunc){
    insertionSort(a, compFunc, 0, a.size());
}


template<typename T>
void runAlgorithm(std::vector<T> array, WhichAlgol algol, int k){
    Timer t;

    switch (algol) {
        case INSERTION:
            insertionSort<T>(array, std::less<>());
            break;
        case MERG:
            mergeSort<T>(array, std::less<>());
            break;
        case QUICK_SORT:
            quickSort<T>(array, std::less<>());
            break;
        case QUICK_INSERTION:
            quickInsertion<T>(array, std::less<>(), k);
            break;
    }

    std::cout<<"running time: "<<t.elapsed()<<std::endl; // important to run before the others
#ifdef PRINT_ARRAY
    std::cout<<"result:"<<std::endl;
    printArray(array, std::cout);
#endif
#ifdef RUN_IS_SORTED
    std::cout<<std::boolalpha<<"isSorted: "<<std::is_sorted(array.begin(), array.end())<<std::endl;
#endif
}


int main(){
    int arraySize;
    int k;

    std::cout<<std::endl<<DESCRIPTION<<std::endl<<std::endl;

    std::cout<<"Enter the array size:";
    std::cin>>arraySize;

    std::cout<<"Enter the k:";
    std::cin>>k;

    std::vector<int> array(arraySize);
    generateRandomVector(array);

#ifdef PRINT_ARRAY
    std::cout<<"\ngenerated array:"<<std::endl;
    printArray(array, std::cout);
#endif

    std::cout<<std::endl;
    std::cout<<"quicksort sort:"<<std::endl;
    runAlgorithm(array, QUICK_SORT, 0);

    std::cout<<std::endl;
    std::cout<<"merge sort sort:"<<std::endl;
    runAlgorithm(array, MERG, 0);

    std::cout<<std::endl;
    std::cout<<"quick insertion sort:"<<std::endl;
    runAlgorithm(array, QUICK_INSERTION, k);

    std::cout<<std::endl;
    std::cout<<"insertion sort:"<<std::endl;
    runAlgorithm(array, INSERTION, 0);

    return 0;
}