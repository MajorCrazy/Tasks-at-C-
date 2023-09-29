#include <iostream>
#include <math.h>
#include <vector>
#include <array>
#include <chrono> // для функций из std::chrono
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <set>
#include <unordered_set>

using namespace std;

class Timer
{
private:
	// Псевдонимы типов используются для удобного доступа к вложенным типам
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

// data structure that represents a node in the tree
struct Node {
	int data; // holds the key
	Node* parent; // pointer to the parent
	Node* left; // pointer to left child
	Node* right; // pointer to right child
};

typedef Node* NodePtr;

// class SplayTree implements the operations in Splay tree
class SplayTree {
private:
	NodePtr root;

	void preOrderHelper(NodePtr node) {
		if (node != nullptr) {
			cout << node->data << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != nullptr) {
			inOrderHelper(node->left);
			cout << node->data << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != nullptr) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout << node->data << " ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, int key) {
		if (node == nullptr || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	void deleteNodeHelper(NodePtr node, int key) {
		NodePtr x = nullptr;
		NodePtr t, s;
		while (node != nullptr) {
			if (node->data == key) {
				x = node;
			}

			if (node->data <= key) {
				node = node->right;
			}
			else {
				node = node->left;
			}
		}

		if (x == nullptr) {
			cout << "Couldn't find key in the tree" << endl;
			return;
		}
		split(x, s, t); // split the tree
		if (s->left) { // remove x
			s->left->parent = nullptr;
		}
		root = join(s->left, t);
		delete(s);
		s = nullptr;
	}

	void printHelper(NodePtr root, string indent, bool last) {
		// print the tree structure on the screen
		if (root != nullptr) {
			cout << indent;
			if (last) {
				cout << "└────";
				indent += "     ";
			}
			else {
				cout << "├────";
				indent += "|    ";
			}

			cout << root->data << endl;

			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
	}

	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// splaying
	void splay(NodePtr x) {
		while (x->parent) {
			if (!x->parent->parent) {
				if (x == x->parent->left) {
					// zig rotation
					rightRotate(x->parent);
				}
				else {
					// zag rotation
					leftRotate(x->parent);
				}
			}
			else if (x == x->parent->left && x->parent == x->parent->parent->left) {
				// zig-zig rotation
				rightRotate(x->parent->parent);
				rightRotate(x->parent);
			}
			else if (x == x->parent->right && x->parent == x->parent->parent->right) {
				// zag-zag rotation
				leftRotate(x->parent->parent);
				leftRotate(x->parent);
			}
			else if (x == x->parent->right && x->parent == x->parent->parent->left) {
				// zig-zag rotation
				leftRotate(x->parent);
				rightRotate(x->parent);
			}
			else {
				// zag-zig rotation
				rightRotate(x->parent);
				leftRotate(x->parent);
			}
		}
	}

	// joins two trees s and t
	NodePtr join(NodePtr s, NodePtr t) {
		if (!s) {
			return t;
		}

		if (!t) {
			return s;
		}
		NodePtr x = maximum(s);
		splay(x);
		x->right = t;
		t->parent = x;
		return x;
	}

	// splits the tree into s and t
	void split(NodePtr& x, NodePtr& s, NodePtr& t) {
		splay(x);
		if (x->right) {
			t = x->right;
			t->parent = nullptr;
		}
		else {
			t = nullptr;
		}
		s = x;
		s->right = nullptr;
		x = nullptr;
	}

public:
	SplayTree() {
		root = nullptr;
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
	}

	// search the tree for the key k
	// and return the corresponding node
	NodePtr searchTree(int k) {
		NodePtr x = searchTreeHelper(this->root, k);
		if (x) {
			splay(x);
		}
		return x;
	}

	// find the node with the minimum key
	NodePtr minimum(NodePtr node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	NodePtr maximum(NodePtr node) {
		while (node->right != nullptr) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	NodePtr successor(NodePtr x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != nullptr) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		NodePtr y = x->parent;
		while (y != nullptr && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	NodePtr predecessor(NodePtr x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != nullptr) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != nullptr && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// insert the key to the tree in its appropriate position
	void insert(int key) {
		// normal BST insert
		NodePtr node = new Node;
		node->parent = nullptr;
		node->left = nullptr;
		node->right = nullptr;
		node->data = key;
		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != nullptr) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			}
			else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		}
		else if (node->data < y->data) {
			y->left = node;
		}
		else {
			y->right = node;
		}

		// splay the node
		splay(node);
	}

	NodePtr getRoot() {
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		printHelper(this->root, "", true);
	}

};

template<class T>
class AVLNode
{
protected:
	//закрытые переменные AVLNode N; N.data = 10 вызовет ошибку
	T data;

	//не можем хранить AVLNode, но имеем право хранить указатель
	AVLNode* left;
	AVLNode* right;
	AVLNode* parent;

	//переменная, необходимая для поддержания баланса дерева
	int height;
public:
	//доступные извне переменные и функции
	virtual void setData(T d) { data = d; }
	virtual T getData() { return data; }
	int getHeight() { return height; }

	virtual AVLNode* getLeft() { return left; }
	virtual AVLNode* getRight() { return right; }
	virtual AVLNode* getParent() { return parent; }

	virtual void setLeft(AVLNode* N) { left = N; }
	virtual void setRight(AVLNode* N) { right = N; }
	virtual void setParent(AVLNode* N) { parent = N; }

	//Конструктор. Устанавливаем стартовые значения для указателей
	AVLNode<T>(T n)
	{
		data = n;
		left = right = parent = NULL;
		height = 1;
	}

	AVLNode<T>()
	{
		left = NULL;
		right = NULL;
		parent = NULL;
		height = 1;
	}


	virtual void print()
	{
		cout << "\n" << data;
	}

	virtual void setHeight(int h)
	{
		height = h;
	}

	template<class T> friend ostream& operator<< (ostream& stream, AVLNode<T>& N);

	AVLNode* successor()
	{
		AVLNode<T>* Current;
		if (right != NULL)
		{
			Current = right;
			while (Current->left != NULL)
				Current = Current->left;
		}
		else
		{
			Current = parent;
			while (Current != NULL && Current->data < data)
				Current = Current->parent;
		}
		return Current;
	}
	AVLNode* predecessor()
	{
		return NULL;
	}
};

template<class T>
ostream& operator<< (ostream& stream, AVLNode<T>& N)
{
	stream << "\nAVLNode data: " << N.data << ", height: " << N.height;
	return stream;
}
template<class T>
void print(AVLNode<T>* N) { cout << "\n" << N->getData(); }

template<class T>
class Tree
{
protected:
	//корень - его достаточно для хранения всего дерева
	AVLNode<T>* root;

	virtual AVLNode<T>* push_R(AVLNode<T>* N, AVLNode<T>* Current)
	{
		//не передан добавляемый узел
		if (N == NULL) return NULL;

		//пустое дерево - добавляем в корень
		if (root == NULL) { root = N; return N; }

		if (Current->getData() > N->getData())
		{
			//идем влево
			if (Current->getLeft() != NULL) return push_R(N, Current->getLeft());
			else { Current->setLeft(N); N->setParent(Current); }
		}
		if (Current->getData() < N->getData())
		{
			//идем вправо
			if (Current->getRight() != NULL) return push_R(N, Current->getRight());
			else { Current->setRight(N); N->setParent(Current); }
		}
		//if (Current->getData() == N->getData()) ?
		//вернуть добавленный узел
		return N;
	}

	//поиск узла в дереве. Второй параметр - в каком поддереве искать, первый - что искать
	virtual AVLNode<T>* Find_R(T data, AVLNode<T>* Current)
	{
		//база рекурсии
		if (Current == NULL) return NULL;

		if (Current->getData() == data) return Current;

		//рекурсивный вызов
		if (Current->getData() > data) return Find_R(data, Current->getLeft());
		if (Current->getData() < data) return Find_R(data, Current->getRight());
	}
public:
	//доступ к корневому элементу
	virtual AVLNode<T>* getRoot() { return root; }

	//конструктор дерева: в момент создания дерева ни одного узла нет, корень смотрит в никуда
	Tree<T>() { root = NULL; }

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual AVLNode<T>* push(AVLNode<T>* N)
	{
		return push_R(N, root);
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual AVLNode<T>* push(T n)
	{
		AVLNode<T>* N = new AVLNode<T>;
		N->setData(n);
		return push(N);
	}

	//удаление узла
	virtual void Remove(AVLNode<T>* N)
	{ }

	virtual AVLNode<T>* Min(AVLNode<T>* Current = NULL)
	{
		//минимум - это самый "левый" узел. Идём по дереву всегда влево
		if (root == NULL) return NULL;

		if (Current == NULL)
			Current = root;
		while (Current->getLeft() != NULL)
			Current = Current->getLeft();
		return Current;
	}

	virtual AVLNode<T>* Max(AVLNode<T>* Current = NULL)
	{
		//максимум - это самый "правый" узел. Идём по дереву всегда вправо
		if (root == NULL) return NULL;
		if (Current == NULL) Current = root;
		for (; Current->getRight() != NULL; Current = Current->getRight());
		return Current;
	}


	virtual AVLNode<T>* Find(T data)
	{
		return Find_R(data, root);
	}

	//три обхода дерева
	virtual void PreOrder(AVLNode<T>* N, void (*f)(AVLNode<T>*))
	{
		if (N != NULL)
			f(N);
		if (N != NULL && N->getLeft() != NULL)
			PreOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PreOrder(N->getRight(), f);
	}

	//InOrder-обход даст отсортированную последовательность
	virtual void InOrder(AVLNode<T>* N, void (*f)(AVLNode<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			InOrder(N->getLeft(), f);
		if (N != NULL)
			f(N);
		if (N != NULL && N->getRight() != NULL)
			InOrder(N->getRight(), f);
	}

	virtual void PostOrder(AVLNode<T>* N, void (*f)(AVLNode<T>*))
	{
		if (N != NULL && N->getLeft() != NULL)
			PostOrder(N->getLeft(), f);
		if (N != NULL && N->getRight() != NULL)
			PostOrder(N->getRight(), f);
		if (N != NULL)
			f(N);
	}
};

template<class T>
class AVL_Tree : public Tree<T>
{
protected:
	//определение разности высот двух поддеревьев
	int bfactor(AVLNode<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		return (hr - hl);
	}

	//при добавлении узлов в них нет информации о балансе, т.к. не ясно, куда в дереве они попадут
	//после добавления узла рассчитываем его высоту (расстояние до корня) и редактируем высоты в узлах, где это
	//значение могло поменяться
	void fixHeight(AVLNode<T>* p)
	{
		int hl = 0;
		int hr = 0;
		if (p->getLeft() != NULL)
			hl = p->getLeft()->getHeight();
		if (p->getRight() != NULL)
			hr = p->getRight()->getHeight();
		p->setHeight((hl > hr ? hl : hr) + 1);
	}

	//краеугольные камни АВЛ-деревьев - процедуры поворотов
	AVLNode<T>* RotateRight(AVLNode<T>* p) // правый поворот вокруг p
	{
		AVLNode<T>* q = p->getLeft();

		p->setLeft(q->getRight());
		q->setRight(p);

		if (p == Tree<T>::root)
			Tree<T>::root = q;
		else
		{
			if (p->getParent()->getLeft() == p)
				p->getParent()->setLeft(q);
			else
				p->getParent()->setRight(q);
		}

		q->setParent(p->getParent());
		p->setParent(q);
		if (p->getLeft() != NULL) p->getLeft()->setParent(p);

		fixHeight(p);
		fixHeight(q);
		return q;
	}

	AVLNode<T>* RotateLeft(AVLNode<T>* q) // левый поворот вокруг q
	{
		AVLNode<T>* p = q->getRight();

		q->setRight(p->getLeft());
		p->setLeft(q);

		if (q == Tree<T>::root)
			Tree<T>::root = p;
		else
		{
			if (q->getParent()->getLeft() == q)
				q->getParent()->setLeft(p);
			else
				q->getParent()->setRight(p);
		}

		p->setParent(q->getParent());
		q->setParent(p);
		if (q->getRight() != NULL) q->getRight()->setParent(q);

		fixHeight(q);
		fixHeight(p);
		return p;
	}

	//балансировка поддерева узла p - вызов нужных поворотов в зависимости от показателя баланса
	AVLNode<T>* Balance(AVLNode<T>* p) // балансировка узла p
	{
		fixHeight(p);
		if (bfactor(p) == 2)
		{
			if (bfactor(p->getRight()) < 0) RotateRight(p->getRight());
			return RotateLeft(p);
		}
		if (bfactor(p) == -2)
		{
			if (bfactor(p->getLeft()) > 0) RotateLeft(p->getLeft());
			return RotateRight(p);
		}

		return p; // балансировка не нужна
	}

public:
	//конструктор AVL_Tree вызывает конструктор базового класса Tree
	AVL_Tree<T>() : Tree<T>() {}

	virtual AVLNode<T>* push_R(AVLNode<T>* N)
	{
		return push_R(N, Tree<T>::root);
	}

	//рекуррентная функция добавления узла. Устроена аналогично, но вызывает сама себя - добавление в левое или правое поддерево
	virtual AVLNode<T>* push_R(AVLNode<T>* N, AVLNode<T>* Current)
	{
		//вызываем функцию push_R из базового класса
		AVLNode<T>* pushedAVLNode = Tree<T>::push_R(N, Current);
		//применяем к добавленному узлу балансировку
		if (Current != NULL)
			return Balance(Current);
		return pushedAVLNode;
	}

	//функция для добавления числа. Делаем новый узел с этими данными и вызываем нужную функцию добавления в дерево
	virtual AVLNode<T>* push(T n)
	{
		AVLNode<T>* N = new AVLNode<T>;
		N->setData(n);
		return push_R(N);
	}

	//удаление узла
	virtual void Remove(AVLNode<T>* N)
	{ }
};


//класс итератор по дереву
template<typename ValueType>
class TreeIterator : public std::iterator<std::input_iterator_tag, ValueType>
{
private:

public:
	AVLNode<ValueType>* ptr;

	TreeIterator() { ptr = NULL; }
	TreeIterator(AVLNode<ValueType>* p) { ptr = p; }
	TreeIterator(const TreeIterator& it) { ptr = it.ptr; }

	TreeIterator& operator=(const TreeIterator& it) { ptr = it.ptr; return *this; }
	TreeIterator& operator=(AVLNode<ValueType>* p) { ptr = p; return *this; }

	bool operator!=(TreeIterator const& other) const { return ptr != other.ptr; }
	bool operator==(TreeIterator const& other) const { return ptr == other.ptr; }
	AVLNode<ValueType>& operator*() { return *ptr; }
	TreeIterator& operator++() //++p;
	{
		if (ptr != NULL)
			ptr = ptr->successor();
		return *this;
	}
	TreeIterator operator++(int v) //x = p++;
	{
		AVLNode<ValueType>* p = ptr;
		if (ptr != NULL)
			ptr = ptr->successor();
		return TreeIterator<ValueType>(p);
	}
};
//класс итерируемое дерево поиска
template<class T>
class IteratedTree : public AVL_Tree<T>
{
public:
	IteratedTree<T>() : AVL_Tree<T>() {}

	TreeIterator<T> iterator;

	TreeIterator<T> begin() { return TreeIterator<T>(Tree<T>::Min()); }
	TreeIterator<T> end() { return TreeIterator<T>(Tree<T>::Max()); }
};

int main()
{
	SplayTree bst;
	for (int i = 0; i < 1000; i++)
		bst.insert(i);

	for (int i = 100; i < 200; i++)
		for (int j = 0; j < 10; j++)
			bst.searchTree(i);

	Timer t;

	double time = 0;
	for (int j = 0; j < 10; j++)
	{
		t.reset();
		for (int i = 100; i < 200; i += 10)
		{
			bst.searchTree(i);
		}
		bst.searchTree(300 + j);
		bst.searchTree(400 + j);
		time += t.elapsed();
	}

	
	std::cout << "Splay: " << time/10 << '\n';

	IteratedTree<int> T;
	for (int i = 0; i < 1000; i++)
		T.push(i);

	time = 0;
	for (int j = 0; j < 10; j++)
	{
		t.reset();
		for (int i = 100; i < 200; i += 10)
		{
			T.Find(i);
		}
		time += t.elapsed();
	}
	

	std::cout << "AVL: " << time / 10 << '\n';

	set<int> s;
	for (int i = 0; i < 1000; i++)
		s.insert(i);

	time = 0;
	for (int j = 0; j < 10; j++)
	{
		t.reset();
		for (int i = 100; i < 200; i += 10)
		{
			s.find(i);
		}
		time += t.elapsed();
	}


	std::cout << "red-black tree: " << time / 10 << '\n';

	unordered_set<int> h;
	for (int i = 0; i < 1000; i++)
		h.insert(i);

	time = 0;
	for (int j = 0; j < 10; j++)
	{
		t.reset();
		for (int i = 100; i < 200; i += 10)
		{
			h.find(i);
		}
		time += t.elapsed();
	}


	std::cout << "hashtable: " << time / 10 << '\n';

	char c1; std::cin >> c1;
	return 0;
}
