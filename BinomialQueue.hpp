#if !defined(BINOMIAL_QUEUE_HPP)
#define BINOMIAL_QUEUE_HPP

template <typename T>
class BinomialTree {
public:
	friend class BinomialQueue <T>;
	using PtrBinomialTree = BinomialTree<T>*;
	BinomialTree();
	BinomialTree(const T& key);
	~BinomialTree();

	T getKey();

private:
	T key;
	PtrBinomialTree left_child;
	PtrBinomialTree next_sibling;
};

template <typename T>
BinomialTree<T>::PtrBinomialTree Merge(BinomialTree<T>::PtrBinomialTree T1, BinomialTree<T>::PtrBinomialTree T2);

template <typename T>
class BinomialQueue {
public:
	BinomialQueue();	// By default generate a Queue of 30
	BinomialQueue(int capacity);
	BinomialQueue(int capacity, const T& key);
	BinomialQueue(int capacity, BinomialTree<T>* old_tree);

	int getSize();
	void Insert(const T& key);
	void DeleteMin();
	T Top();

	~BinomialQueue();
private:
	using PtrNode = BinomialTree<T>*;
	PtrNode* Queue;
	int size;

	// Merge a smaller Binomial Queue Q into this one
	// Merge Q into this one, Q WILL BE DELETED AFTER THIS
	void Merge(BinomialQueue<T>* Q);
};

#endif // BINOMIAL_QUEUE_HPP


