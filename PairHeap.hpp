static const int MaxSiblings = 100000;

template <typename T>
class PairHeap;

template <typename T>
class PairHeapNode;

template <typename T>
PairHeapNode<T>* compareAndMerge (PairHeapNode<T>* F, PairHeapNode<T>* S);

template <typename T>
class PairHeapNode {
	using PtrNode = PairHeapNode<T>*;
	T key;
	PtrNode first_child;
	PtrNode next_sibling, prev;
	PairHeapNode () = default;
	PairHeapNode (const T& key) : first_child(nullptr), next_sibling(nullptr), prev(nullptr), key(key) {}
	friend PairHeapNode* compareAndMerge<T> (PairHeapNode* first , PairHeapNode* second);
	friend class PairHeap<T>;
};

template <typename T>
class PairHeap {
	using PtrNode = PairHeapNode<T>*;
	PtrNode head = nullptr;

public:
	bool empty();
	PtrNode push(const T &key);
	PtrNode decrease(PtrNode node, const T &key);
	T top();
	T pop();
	PairHeap (const int capacity = 0);
	~PairHeap () = default;
	// void clear();
	void printHeap();
private:
	PtrNode combineSiblings(PtrNode first);
	void printHeap(PtrNode node, int step);
};

template <typename T>
PairHeap<T>::PairHeap (const int capacity) {}

// F is guaranteed to have no sibling
template <typename T>
PairHeapNode<T>* compareAndMerge (PairHeapNode<T>* F, PairHeapNode<T>* S) {
	if (S == nullptr) {
		return F;
	}
	if (F->key > S->key) {
		S->prev = F->prev;
		F->prev = S;
		F->next_sibling = S->first_child;
		if (S->first_child) {
			S->first_child->prev = F;
		}
		S->first_child = F;
		return S;
	} else {
		S->prev = F;
		F->next_sibling = S->next_sibling;
		if (S->next_sibling) {
			S->next_sibling->prev = F;
		}
		S->next_sibling = F->first_child;
		if (F->first_child) {
			F->first_child->prev = S;
		}
		F->first_child = S;
		return F;
	}
}

template <typename T>
PairHeapNode<T>* PairHeap<T>::push(const T& key) {
	PtrNode new_node;
	new_node = new PairHeapNode<T>(key);

	if (head == nullptr) {
		head = new_node;
	} else {
		head = compareAndMerge(head, new_node);
	}
	return new_node;
}

template <typename T>
PairHeapNode<T>* PairHeap<T>::decrease(PtrNode node, const T& key) {
	node->key = key;
	if (head == node) {
		return node;
	}
	// cut the node from the heap
	if (node->next_sibling) {
		node->next_sibling->prev = node->prev;
	}
	// Notice that node->prev is guaranteed to exist since it is not the root
	if (node->prev->first_child == node) {	// node is the first child
		node->prev->first_child = node->next_sibling;
	} else {	// node has a left sibling
		node->prev->next_sibling = node->next_sibling;
	}
	node->next_sibling = nullptr;
	head = compareAndMerge (head, node);
	return node;
}

template <typename T>
T PairHeap<T>::top() {
	return head->key;
}

template <typename T>
bool PairHeap<T>::empty() {
	return head == nullptr;
}

template <typename T>
T PairHeap<T>::pop() {
	// Did not check underflow
	T ret = head->key;
	if (head->first_child != nullptr) {
		auto new_head = combineSiblings(head->first_child);
		delete head;
		head = new_head;
	} else {
		delete head;
		head = nullptr;
	}
	return ret;
}

// combine siblings of first, return what results
template <typename T>
PairHeapNode<T>* PairHeap<T>::combineSiblings (PtrNode first) {
	static PtrNode Array[MaxSiblings];
	if (first->next_sibling == nullptr) { // only one sibling
		return first;
	}

	// Put all siblings in the array
	int cnt_siblings = 0;
	while (first != nullptr) {
		Array[cnt_siblings++] = first;
		first->prev->next_sibling = nullptr;	// break the link
		first = first->next_sibling;
	}

	// Combine them from left to right and then back
	int pos;
	for (pos = 0; pos + 1 < cnt_siblings; pos += 2) {
		Array[pos] = compareAndMerge(Array[pos], Array[pos + 1]);
	}

	pos -= 2;
	if (pos == cnt_siblings - 3) { // there is one left in the first pass
		Array[pos] = compareAndMerge (Array[pos], Array[pos + 2]);
	}

	while (pos) {
		Array[pos - 2] = compareAndMerge(Array[pos - 2], Array[pos]);
		pos -= 2;
	}
	return Array[0];
}

template <typename T>
void PairHeap<T>::printHeap () {
	std::cout << "===================" << std::endl;
	printHeap(head, 2);
}

template <typename T>
void PairHeap<T>::printHeap(PtrNode node, int step) {
	if (node == nullptr) {
		return;
	}
	for (int i = 1; i <= step; i++) {
		std::cout << "-";
	}
	if (node->prev) {
		std::cout << node->key << " " << node->prev->key << std::endl;
	} else {
		std::cout << node->key << std::endl;
	}
	for (auto itr = node->first_child; itr != nullptr; itr = itr->next_sibling) {
		printHeap(itr, step + 2);
	}
}