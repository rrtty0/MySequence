#pragma once
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <iostream>
using namespace std;


class Sequence
{
private:

	class Node
	{
	public:

		class List
		{
		public:

			class Element
			{
			public:
				Element() {};
				~Element() {};

				Node* data;
				Element* next;
				Element* previous;

				friend class List;
			};

			Element* head;

			List()
			{
				head = nullptr;
			}

			Element* push_back(Node* ptr)
			{
				Element* new_el = new Element;
				Element* last_el = head;

				new_el->next = nullptr;
				new_el->data = ptr;

				if (!head)
				{
					new_el->previous = nullptr;
					head = new_el;
				}
				else
				{
					while (last_el->next != nullptr)
						last_el = last_el->next;
					new_el->previous = last_el;
					last_el->next = new_el;
				}

				return new_el;
			}

			void remove(Element* ptr)
			{
				if (ptr->previous)
					ptr->previous->next = ptr->next;
				else
					head = ptr->next;

				if (ptr->next)
					ptr->next->previous = ptr->previous;

				ptr->next = nullptr;
				ptr->previous = nullptr;
			}

			~List()
			{
				while (head)
				{
					Element* el = head->next;
					delete head;
					head = el;
				}
			}

			void show()
			{
				Element* el = head;

				while (el)
				{
					cout << el->data->value << " ";
					el = el->next;
				}
			}

			Element* insert(Element* el, Node* ptr)
			{

				if (!el->next)
					return push_back(ptr);

				Element* element = new Element;

				element->data = ptr;
			
				element->next = el->next;
				element->previous = el;
				el->next = element;
				if (element->next->previous)
					element->next->previous = element;

				return element;
			}

			Element* toStart(Node* ptr)
			{
				Element* element = new Element;

				element->data = ptr;
				element->previous = nullptr;
				element->next = head;
				head->previous = element;
				head = element;

				return element;
			}

		};




		int value;                     //value at node
		Node* left;                    //left child
		Node* right;                   //right child
		int height;                    //number of nodes at tree, where this node is root
		vector<List::Element*> toPtrs; //array with pointers to array with pointers to keys
		
		
		Node() : left(nullptr), right(nullptr), height(1) {};                   //constructor without arguments
		Node(int v) : left(nullptr), right(nullptr), height(1), value(v) {};    //constructor with argument(int value)

		~Node()                                                                 //constructor without arguments
		{
			if (left)
				delete left;
			if (right)
				delete right;
		};      

	};


	Node * root;                                //root of Tree
	Node::List* pointersToKeys = new Node::List;//array with pointers to keys in Tree(keep sequence of elements)
	int size = 0;                               //number of elements of sequence

	Node* push_back(Node*, int);                           //push back element to sequnce(recursion)
	Node* remove_back(Node*, int);                         //delete element from sequence(recursion)
	bool contains(Node*, int);                             //check availability of element in sequence(recursion)
	int changeHeight(Node*);                               //change height of this node
	int getBalance(Node*);                                 //balance of this node
	Node* rebalance(Node*);                                //change balance of this node
	Node* rotateRight(Node*);                              //right turn around the node
	Node* rotateLeft(Node*);                               //left turn around the node
	Node* findRightMin(Node*);                             //find min element in right tree of deleted element
	Node* removeRightMin(Node*);                           //delete min element in right tree of deleted element
	void push_back_pointer(Node*);                         //add to back new pointer
	Node* add(Node*, Sequence::Node::List::Element*, int, int);//add new element(recursion)
	void swapElement(Sequence::Node::List::Element*, Sequence::Node::List::Element*);//swap two element in array of pointers
	Node* remove(Node*, Sequence::Node::List::Element*);                           //remove element from sequence(recursion)
	void removeAll();




public:
	//Sequence();                                //constructor with out arguments
	Sequence(vector<int> arr);                  //constructor with elements
	Sequence(const Sequence&);                  //constructor of copy
	Sequence(Sequence&&);                       //constructor with move
	Sequence& operator=(const Sequence&);       //= with copy
	Sequence& operator=(Sequence&&);            //= with move
	~Sequence();                                //destructor without arguments

	void push_back(int);               //push back element to Sequnce
	void remove_back(int);             //delete last(value) from Sequence
	bool contains(int);                //check availability of element in Sequence
	void show();                       //print Sequence on screen
	void EXCL(const Sequence&);        //delete equal part of this-sequence on argument-sequence
	void CHANGE(const Sequence&, int); //change this-sequence at index on argument-sequence
	void CONCAT(const Sequence&);      //concatenation argument-sequence to this-sequence
	void MERGE(const Sequence&);       //CONCAT + sort of result
	void sort();                       //sort elements of Sequence
	void add(int, int);                //add value element to Sequence to index
	int getSize();                     //return number of elements at Sequence
	void remove(int);                  //remove element from index
};

