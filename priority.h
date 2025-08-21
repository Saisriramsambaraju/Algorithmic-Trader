#ifndef PRIORITY_H  // Include guard to prevent multiple inclusions
#define PRIORITY_H

#include<string>
#include<vector>
using namespace std;

struct broker_details{
int bought;
int sold;
int net_transfer;
broker_details() : bought(0) , sold(0) , net_transfer(0){}
void operator=(int a){
    bought = 0;
    sold = 0;
    net_transfer = 0;
}
};


struct broker{
int start_time;
std::string Broker_name;
int Price;
int Quantity;
int end_time;

    bool operator<(const broker& other) const {
        // Compare based on Price
        if (Price != other.Price) {
            return Price < other.Price;
       }
        if (start_time != other.start_time) {
            return start_time < other.start_time;
        }
        // If Prices are equal, compare based on Broker_name
        return Broker_name < other.Broker_name;
    }

	bool operator>(const broker& other) const {
        // Use the negation of the < operator logic
        return !operator<(other);
    }
    void operator=(const broker& other){
        Price = other.Price;
        Quantity = other.Quantity;
        start_time = other.start_time;
        end_time = other.end_time;
        Broker_name = other.Broker_name;
    }

};

bool compareinminheap(broker b1,broker b2){
     if (b1.Price != b2.Price) {
            return b1.Price < b2.Price;
       }
    if (b1.start_time != b2.start_time) {
            return b1.start_time < b2.start_time;
        }
    return b1.Broker_name < b2.Broker_name;
}

bool compareinmaxheap(broker b1,broker b2){
     if (b1.Price != b2.Price) {
            return b1.Price > b2.Price;
       }
    if (b1.start_time != b2.start_time) {
            return b1.start_time < b2.start_time;
        }
    return b1.Broker_name < b2.Broker_name;
}

class pq {
private:
    void swapElements(int i, int j){
        broker temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
public:
    std::vector<struct broker> arr;
    int size;
    bool isMinHeap;

    pq(bool isMin = true) : size(0), isMinHeap(isMin) {}

    void heapify(int n, int i) {
		if(n==1)return;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (isMinHeap) {
            if (left < n && compareinminheap(arr[left],arr[smallest])) {
                smallest = left;
            }
            if (right < n && compareinminheap(arr[right],arr[smallest])) {
                smallest = right;
            }
        } else {
            if (left < n && compareinmaxheap(arr[left],arr[smallest])) {
                smallest = left;
            }
            if (right < n && compareinmaxheap(arr[right],arr[smallest])) {
                smallest = right;
            }
        }

        if (smallest != i) {
            swapElements(i,smallest);
            heapify(n, smallest);
        }
    }

    void insertNode(broker C1) {
        size = size + 1;
        arr.push_back(C1); // Push back the new element
        int i = size - 1;
        while (i != 0 && (isMinHeap ? (compareinminheap(arr[i],arr[(i - 1) / 2])) : compareinmaxheap(arr[i],arr[(i-1)/2]))) {
            swapElements(i,(i - 1) / 2);
            i = (i - 1) / 2;
        }
    }


    void pop_max() {
        if (size > 0) {
            swapElements(0, size - 1);
            arr.pop_back();
            size -= 1;
            heapify(size, 0);
        }
    }

    broker get_top() {
        if (size > 0) {
            return arr[0];
        }
        // Return a default broker or handle this case as needed
        return broker();
    }
};




#endif  // MYHEADER_H