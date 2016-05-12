#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <iostream>

class Node
{
public:
	Node* left;                                                //pointer to left child
	Node* right;                                               //pointer to right child
	Node* next;                                                // this is used for queues when we level traverse
	Node** parentConnection;                                   //pointer to the pointer to this node
	int data;                                                  //the thing in question
	//our node constructor
	Node(const int& num) { data = num; left = right = next = 0; parentConnection = 0; }
};

class BinarySearchTree{
private:
	int size;
	int total;
public:
	Node* root;                                                // the top of the tree	

	BinarySearchTree();                                        //constructor
	~BinarySearchTree();                                       //destructor

	//void insert(int addThis);
	void insert(Node* addThis);                                //method for inserting an int, calls insert(Node*, Node*)
	void insert(Node* addThis, Node* subRoot);                 //resolves the location for the new node and adds it to the tree

	void del(const int&);

	void preorder();
	void preorder(Node* subRoot);

	void inorder();
	void inorder(Node* subRoot);

	void postorder();
	void postorder(Node* subRoot);

	Node* search(const int& num) { return search(root, num); }
	Node* search(Node*, const int&);

	void layerorder();

	void display();
	void display(Node*, const int&);

	//empty the tree
	void empty();

	Node* findIOP(Node*);

	//deletes the rightmost node
	void delMax(const Node*);

	//deletes the leftmost node
	void delMin(const Node*);

	void incSize(){ ++size; }
	void decSize(){ --size; }
	void setSize(int size){ this->size = size; }
	int getSize(){ return size; }
	void addTotal(int x){ total += x;}
	void subtractTotal(int x){ total -= x;}
	int getTotal(){ return total; }
	void setTotal(int total){ this->total = total; }
};

BinarySearchTree::BinarySearchTree(){
	root = 0;
}

BinarySearchTree::~BinarySearchTree(){
}

//void BinarySearchTree::insert(int addThis){
//	Node my_node = Node(addThis);
//	Node* nodePtr = &my_node;
//	insert(nodePtr);
//}

void BinarySearchTree::insert(Node* addThis){
	
	if (!root){
		root = addThis;
		root->parentConnection = &root;
		incSize();
		addTotal(addThis->data);
	}
	else insert(addThis, root); // start the recursion from the root of the tree
}

void BinarySearchTree::insert(Node* addThis, Node* subRoot){
	if (addThis->data <= subRoot->data){
		// make sure there is something to compare to in recursion call
		if (subRoot->left){
			insert(addThis, subRoot->left);
		}
		else{
			subRoot->left = addThis;
			subRoot->left->parentConnection = &(subRoot->left);
			addTotal(addThis->data);
			incSize();
		}
	}
	else{
		// see above
		if (subRoot->right){
			insert(addThis, subRoot->right);
		}
		else{
			subRoot->right = addThis;
			subRoot->right->parentConnection = &(subRoot->right);
			addTotal(addThis->data);
			incSize();
		}
	}
}

void BinarySearchTree::inorder(){
	inorder(root);  // start the recursion
	std::cout << std::endl;
}

void BinarySearchTree::inorder(Node* subRoot){

	if (!subRoot)
		return;

	// left child first
	inorder(subRoot->left);

	// self
	std::cout << subRoot->data << " ";

	// right child
	inorder(subRoot->right);
}

void BinarySearchTree::preorder(){
	preorder(root);
	std::cout << std::endl;
}

void BinarySearchTree::preorder(Node* subRoot){
	if (!subRoot)
		return;

	// self first
	std::cout << subRoot->data << " ";

	//then children
	preorder(subRoot->left);
	preorder(subRoot->right);
}

void BinarySearchTree::postorder(){
	postorder(root);
	std::cout << std::endl;
}

void BinarySearchTree::postorder(Node* subRoot){

	if (!subRoot)
		return;

	// children first
	postorder(subRoot->left);
	postorder(subRoot->right);

	// then self
	std::cout << subRoot->data << " ";
}

void BinarySearchTree::del(const int& num){

	Node* del = search(num);
	if (!del)
		return;

	subtractTotal(num);

	// if we have no children in del
	if (del->left == del->right)
	{
		// the node that points to del has an address that is stored in pC
		// so we need to de-reference pC to change the value that the
		// pointer points to instead of del
		*(del->parentConnection) = 0;
	}
	// if we have one child in del
	else if (!del->left || !del->right){

		if (del->left){
			// set the value of del's pC to point to del's child
			*(del->parentConnection) = del->left;
			// set del's child's pC equal to del's pC
			del->left->parentConnection = del->parentConnection;
		}

		else{
			// set the value of del's pC to point to del's child
			*(del->parentConnection) = del->right;
			// set del's child's pC equal to del's pC
			del->right->parentConnection = del->parentConnection;
		}
	}
	// if we have two children in del
	else
	{
		// find iop (inorder predecessor)
		Node* iop = findIOP(del->left);

		// we know iop has at most a left child
		// if iop has a left child
		if (iop->left){
			// set the pC pointer of the child equal to iop's
			iop->left->parentConnection = iop->parentConnection;
		}
		//set the value that the iop->pC points to to iop->left
		*(iop->parentConnection) = iop->left;

		// we know we have a left and a right child
		// set iop->left equal to del->left (adds del's left child to iop)
		iop->left = del->left;
		// set the address in del->left->pc equal to the address of iop->left
		if (iop->left)
		iop->left->parentConnection = &iop->left;

		// we know we have a left and a right child
		// see above
		iop->right = del->right;
		if (iop->right)
		iop->right->parentConnection = &iop->right;

		// whatever is pointing to del we now point to iop
		*(del->parentConnection) = iop;
		// set the parentConnection for iop to that of del's
		iop->parentConnection = del->parentConnection;
	}
	decSize();
	delete del;
}

Node* BinarySearchTree::search(Node* subRoot, const int& num){
	// if the node is null, num is not in the tree
	if (!subRoot)
		return 0;
	// if this is the node we are looking for
	if (subRoot->data == num)
		return subRoot;
	// if its in the left sub tree
	if (num < subRoot->data)
		return search(subRoot->left, num);
	// it HAS to be in the right subtree (if its in the tree at all)
	return search(subRoot->right, num);
}

void BinarySearchTree::display(){
	display(root, 0);
}

void BinarySearchTree::display(Node* subRoot, const int& indent){
	// use reverse in order traversal to get the order
	if (!subRoot){
		for (int i = 0; i < indent; ++i)
			std::cout << " ";
		std::cout << "NULL" << std::endl;
	}
	else{
		// right
		display(subRoot->right, indent + 5);
		// node
		for (int i = 0; i < indent; ++i)
			std::cout << " ";
		std::cout << subRoot->data << std::endl;
		// left
		display(subRoot->left, indent + 5);
	}
}

Node* BinarySearchTree::findIOP(Node* subRoot){
	// check if it is not null and it has a right node
	// if not, return it
	if (subRoot && !subRoot->right)
		return subRoot;
	// it has a right child
	return findIOP(subRoot->right);
}

//deletes the rightmost node
void BinarySearchTree::delMax(const Node* subroot){
	if (!subroot->right){
		del(subroot->data);
	}
	else delMax(subroot->right);
}

//deletes the leftmost node
void BinarySearchTree::delMin(const Node* subroot){
	if (!subroot->left){
		del(subroot->data);
	}
	else delMin(subroot->left);
}

void BinarySearchTree::empty(){
	while (root){del(root->data);}
}

#endif
