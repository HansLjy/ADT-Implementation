#if !defined(FIBO_HEAP_HPP)
#define FIBO_HEAP_HPP

template <typename T>
class FibonacciHeap; // Forward declaration for friendship

template <typename T>
class FibonacciNode;

template <typename T>
FibonacciNode<T>* merge(FibonacciNode<T>* node_a, FibonacciNode<T>* node_b);

template <typename T>
void merge(FibonacciHeap<T>* fib_a, FibonacciHeap<T>* fib_b);

template <typename T>
class FibonacciNode {
private:
	using PtrNode = FibonacciNode*;
	T key;
	PtrNode parent = nullptr;
	PtrNode next_sibling = nullptr;
	PtrNode prev_sibling = nullptr;
	PtrNode first_child = nullptr;
	bool marked = false;
	int rank = 0;

public:
	FibonacciNode () = default;
	FibonacciNode (const T& key);

	friend class FibonacciHeap<T>;
	friend FibonacciNode* merge<T>(FibonacciNode* node_a, FibonacciNode* node_b);
	friend void merge<T>(FibonacciHeap<T>* fib_a, FibonacciHeap<T>* fib_b);
};

template <typename T>
class FibonacciHeap {
	using PtrNode = FibonacciNode<T>*;
	PtrNode head = nullptr;
	int size = 0;
public:
	bool empty();
	PtrNode push(const T &key);
	PtrNode decrease(PtrNode node, const T &key);
	T top();
	T pop();
	FibonacciHeap (const int capacity = 0);
	~FibonacciHeap () = default;
	void clear();

	void printHeap();

private:
	void rearrange();				// Rearrange the trees
	void removeRoot(PtrNode node);	// Remove a root of the tree
	void insertTree(PtrNode node);	// Insert a new tree
	void deleteTree(PtrNode node);	// For memory recollection
	void printTree(PtrNode node, int step);
	friend void merge<T>(FibonacciHeap* fib_a, FibonacciHeap* fib_b);
};

// Merge B into A
template <typename T>
void merge(FibonacciHeap<T>* fib_a, FibonacciHeap<T>* fib_b);


// Implementation below

#include <iostream>
#include <cmath>
#include <cstring>

template <typename T>
FibonacciNode<T>::FibonacciNode(const T& key) : key(key) {}

// Merge two trees
template <typename T>
FibonacciNode<T>* merge(FibonacciNode<T>* node_a, FibonacciNode<T>* node_b) {
	if (node_b == nullptr) {
		return node_a;
	}
	if (node_a != nullptr && !(node_a->key > node_b->key)) {	// transparent b into a
		// Insert b into the head of a's children list
		node_b->next_sibling = node_a->first_child;
		if (node_a->first_child) {
			node_a->first_child->prev_sibling = node_b;
		}
		node_b->prev_sibling = nullptr;
		node_a->first_child = node_b;
		node_b->parent = node_a;
		node_a->rank++;
		return node_a;
	} else {
		return merge(node_b, node_a);
	}
}

template <typename T>
FibonacciHeap<T>::FibonacciHeap (const int capacity) {}

template <typename T>
void FibonacciHeap<T>::clear() {
	if (head == nullptr) {
		return;
	}
	// Delete all the trees one by one
	for (PtrNode itr = head, nxt; itr != nullptr; itr = nxt) {
		nxt = itr->next_sibling;
		deleteTree(itr);
	}
}

template <typename T>
void FibonacciHeap<T>::deleteTree(PtrNode node) {
	if (node == nullptr) {
		return;
	}
	// Delete all the subtrees one by one
	for (PtrNode itr = node->first_child, nxt; itr != nullptr; itr = nxt) {
		nxt = itr->next_sibling;
		deleteTree(itr);
	}
	delete node;
}

template <typename T>
bool FibonacciHeap<T>::empty() {
	return head == nullptr;
}

// Push a new key into the heap
template <typename T>
typename FibonacciHeap<T>::PtrNode FibonacciHeap<T>::push(const T& key) {
	// Create a new tree
	size++;
	auto new_tree = new FibonacciNode<T>(key);
	// Insert the new tree among other trees
	new_tree->next_sibling = head;
	if (head != nullptr) {
		head->prev_sibling = new_tree;
	}
	head = new_tree;
	return new_tree;
}

template <typename T>
FibonacciNode<T>* FibonacciHeap<T>::decrease(PtrNode node, const T &key) {
	node->key = key;
	auto ret = node;
	// If this is a root or the change does not violate the heap order, do nothing
	if (node->parent == nullptr || node->parent->key < key) {
		return node;
	}

	node->marked = true;	// simplify the code
	
	// Cascading cut (including the original cut)
	while (node->marked) {
		// Cut the node from its parent
		if (node->prev_sibling) {
			node->prev_sibling->next_sibling = node->next_sibling;
		}
		if (node->next_sibling) {
			node->next_sibling->prev_sibling = node->prev_sibling;
		}
		if (node->parent && node->parent->first_child == node) {
			node->parent->first_child = node->next_sibling;
		}

		node->marked = false;	// Root should be unmarked

		// Insert the node back to the heap
		insertTree(node);

		// If this is not a root
		if (node->parent) {
			PtrNode tmp = node->parent;
			node->parent = nullptr;
			node = tmp;
			node->rank--;
		} else {
			break;
		}
	}

	// If the cascading cut ends at a non-root node
	if (node->parent) {
		node->marked = true;
	}

	return ret;
}

template <typename T>
T FibonacciHeap<T>::top() {
	// Did not check underflow!
	rearrange();
	PtrNode min_itr = head;
	for (PtrNode itr = head->next_sibling; itr != nullptr; itr = itr->next_sibling) {
		if (itr->key < min_itr->key) {
			min_itr = itr;
		}
	}
	return min_itr->key;
}

template <typename T>
T FibonacciHeap<T>::pop() {
	// Did not check underflow!
	rearrange();
	PtrNode min_itr = head;
	for (PtrNode itr = head->next_sibling; itr != nullptr; itr = itr->next_sibling) {
		if (itr->key < min_itr->key) {
			min_itr = itr;
		}
	}
	T min_key = min_itr->key;
	removeRoot(min_itr);
	size--;
	return min_key;
}

template <typename T>
void FibonacciHeap<T>::rearrange() {
	static PtrNode list_size_of[30];
	static const double constant = std::log(1.5);
	int max_rank = std::log(size + 1) / constant + 1;

	// Link all the trees of the same size
	// PtrNode* list_size_of = new PtrNode[max_rank + 1];
	// memset(list_size_of, 0, sizeof(PtrNode) * (max_rank + 1));
	for (PtrNode itr = head, nxt; itr != nullptr; itr = nxt) {
		nxt = itr->next_sibling;
		PtrNode& list_head = list_size_of[itr->rank];
		itr->next_sibling = list_head;
		list_head = itr;
	}

	for (int i = 0; i < max_rank; i++) {
		PtrNode& cur_tree = list_size_of[i];	// current tree being processed
		PtrNode next_tree = (cur_tree == nullptr ? nullptr : cur_tree->next_sibling);
		PtrNode& next_list = list_size_of[i + 1];
		while (next_tree != nullptr) {
			PtrNode next_next_tree = next_tree->next_sibling;	// Save the next tree in advance
			PtrNode carry = merge(cur_tree, next_tree);
			carry->next_sibling = next_list;
			next_list = carry;

			cur_tree = next_next_tree;	// current tree being processed
			next_tree = (cur_tree == nullptr ? nullptr : cur_tree->next_sibling);
		}
	}

	head = nullptr;
	PtrNode prev = nullptr;	// The tree inserted last time
	for (int i = 0; i < max_rank; i++) {
		PtrNode cur = list_size_of[i];
		if (cur == nullptr) {
			continue;
		}
		cur->next_sibling = prev;
		if (prev) {
			prev->prev_sibling = cur;
			// reverse the order, actually
		}
		prev = cur;
		list_size_of[i] = nullptr;
	}
	head = prev;
	if (prev) {
		prev->prev_sibling = nullptr;
	}
	// delete [] list_size_of;
}

template <typename T>
void FibonacciHeap<T>::insertTree(PtrNode node) {
	node->next_sibling = head;
	node->prev_sibling = nullptr;
	if (head != nullptr) {
		head->prev_sibling = node;
	}
	head = node;
}

template <typename T>
void FibonacciHeap<T>::removeRoot(PtrNode node) {
	if (node->prev_sibling != nullptr) {
		node->prev_sibling->next_sibling = node->next_sibling;
	}
	if (node->next_sibling != nullptr) {
		node->next_sibling->prev_sibling = node->prev_sibling;
	}
	if (head == node) {
		head = node->next_sibling;
	}
	// cut the tree from the heap

	for (PtrNode itr = node->first_child, nxt; itr != nullptr; itr = nxt) {
		nxt = itr->next_sibling;
		itr->parent = nullptr;	// Make every subtree of the current node a tree
		insertTree(itr);
	}
	delete node;
}

template <typename T>
void FibonacciHeap<T>::printHeap() {
	std::cout << "================================" << std::endl;

	std::cout << "Fibonacci Heap: size = " << size << std::endl;
	int cnt = 0;
	for (PtrNode itr = head; itr != nullptr; itr = itr->next_sibling, cnt++) {
		std::cout << "Tree " << cnt << ":" << std::endl;
		printTree(itr, 2);
	}
}

template <typename T>
void FibonacciHeap<T>::printTree(PtrNode node, int step) {
	for (int i = 0; i < step; i++) {
		std::cout << '-';
	}
	node->key.print();
	// std::cout << node->key << std::endl;
	for (PtrNode itr = node->first_child; itr != nullptr; itr = itr->next_sibling) {
		printTree(itr, step + 2);
	}
}

template <typename T>
void merge(FibonacciHeap<T>* fib_a, FibonacciHeap<T>* fib_b) {
	if (fib_b->head == nullptr) {
		return;
	}
	for (decltype(fib_b->head) itr = fib_b->head, nxt; itr != nullptr; itr = nxt) {
		nxt = itr->next_sibling;
		fib_a->insertTree(itr);	// Insert all the elements of B into A
	}
}

#endif // FIBO_HEAP_HPP
