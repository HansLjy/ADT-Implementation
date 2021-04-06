#include "BinomialQueue.hpp"
#include <cmath>
#include <algorithm>


template <typename T>
BinomialTree<T>* Merge(BinomialTree<T>* T1, BinomialTree<T>* T2) {
	if (T1->key < T2->key) {
		T2->next_sibling = T1->left_child;
		T1->left_child = T2;
		return T1;
	} else {
		return Merge (T2, T1);
	}
}

template <typename T>
T BinomialTree<T>::getKey() {
	return key;
}

template <typename T>
BinomialTree<T>::BinomialTree() : left_child(nullptr), next_sibling(nullptr) {}

template <typename T>
BinomialTree<T>::BinomialTree(const T& key) : left_child(nullptr), next_sibling(nullptr), key(key) {}

template <typename T>
BinomialTree<T>::~BinomialTree() {}

template <typename T>
BinomialQueue<T>::BinomialQueue() {
	size = 0;
	Queue = new BinomialTree<T>[30];
}

template <typename T>
BinomialQueue<T>::BinomialQueue(int capacity) {
	size = 0;
	Queue = new BinomialTree<T>[capacity];
}

template <typename T>
BinomialQueue<T>::BinomialQueue(int capacity, const T& key) {
	size = 1;
	Queue = new BinomialTree<T>[capacity];
	Queue[0] = new BinomialTree(key);
}

template <typename T>
BinomialQueue<T>::BinomialQueue(int capacity, BinomialTree<T>* old_tree) : BinomialQueue<T>(capacity) {
	int rank = 0;
	for (auto it = old_tree->left_child; it != nullptr; it = it->next_sibling) {
		rank++;
	}	// Calculate how many child does old_tree has
	this->size = rank--;
	for (auto it = old_tree->left_child; it != nullptr; it = it->next_sibling) {
		this->Queue[rank--] = it;
	}
}


template <typename T>
BinomialQueue<T>::~BinomialQueue() {
	delete [] Queue;
}

template <typename T>
int BinomialQueue<T>::getSize() {
	return size;
}

template <typename T>
void BinomialQueue<T>::Insert(const T& key) {
	auto new_queue = new BinomialQueue(size + 1, key);
	this->Merge(new_queue);
}

template <typename T>
T BinomialQueue<T>::Top() {
	if (size == 0) {
		// Error Handling
	}
	T min_key = Queue[0]->getKey();
	for (int i = 1; i < size; i++) {
		if (Queue[i]->getKey() < min_key) {
			min_key = Queue[i]->getKey();
		}
	}
}

template <typename T>
void BinomialQueue<T>::Merge(BinomialQueue<T>* Q) {

	BinomialTree<T>* carry = nullptr;
	int max_size = std::max(size, Q->getSize());
	for (int i = 0; i < max_size; i++) {
		auto T1 = Queue[i];
		auto T2 = Q->Queue[i];
		int flag = 4 * (carry != nullptr) + 2 * (T1 != nullptr) + (T2 != nullptr)
		switch (flag) {
			case 0:
			case 1:
				Queue[i] = T2;
				break;
			case 2:
				Queue[i] = T1;
				break;
			case 3:	// 11
			case 7:
				Queue[i] = carry;
				carry = Merge(T1, T2);
				break;
			case 4:
				Queue[i] = carry;
				carry = nullptr;
				break;
			case 5:
				Queue[i] = nullptr;
				carry = Merge(carry, T2);
				break;
			case 6:
				Queue[i] = nullptr;
				carry = Merge(carry, T1);
				break;
		}
	}
	// Change of the size
	if (carry != nullptr) {
		size = max_size;
	}
	delete Q;
}

template <typename T>
void BinomialQueue<T>::DeleteMin() {
	if (size <= 0) {
		return;
	}
	int min_key_id = 0;
	for (int i = 1; i < size; i++) {
		if (Queue[i]->getKey() < Queue[min_key_id]->getKey()) {
			min_key_id = i;
		}
	}
	BinomialQueue<T>* new_queue = new BinomialQueue<T>(size + 1, Queue[i]);
	Queue[i] = nullptr;
	this->Merge(new_queue);
}