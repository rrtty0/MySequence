#include "stdafx.h"
#include "Sequence.h"


/*Sequence::Sequence()
{
}*/


Sequence::Sequence(int v1, ...)
{
	int *ptr = &v1;
	while (*ptr != -1)
	{
		push_back(*ptr);
		++ptr;
	}
}


Sequence::Sequence(const Sequence& sequence)
{
	Sequence::Node::List::Element* ptr = sequence.pointersToKeys->head;

	while (ptr)
	{
		push_back(ptr->data->value);
		ptr = ptr->next;
	}
}


Sequence::Sequence(Sequence&& sequence)
{
	root = sequence.root;
	sequence.root = nullptr;
	size = sequence.size;
	sequence.size = 0;
	pointersToKeys = sequence.pointersToKeys;
	sequence.pointersToKeys = nullptr;
}


Sequence& Sequence::operator=(const Sequence& sequence)
{
	if (this != &sequence)
	{
		removeAll();

		Sequence::Node::List::Element* ptr = sequence.pointersToKeys->head;

		while (ptr)
		{
			push_back(ptr->data->value);
			ptr = ptr->next;
		}
	}

	return *this;
}


Sequence& Sequence::operator=(Sequence&& sequence)
{
	if (this != &sequence)
	{
		root = sequence.root;
		sequence.root = nullptr;
		size = sequence.size;
		sequence.size = 0;
		pointersToKeys = sequence.pointersToKeys;
		sequence.pointersToKeys = nullptr;
	}

	return *this;
}

Sequence::~Sequence()
{
	delete root;
}

void Sequence::push_back(int value)
{
	root = push_back(root, value);
}


Sequence::Node* Sequence::push_back(Sequence::Node* current_node, int value)
{
	if (current_node == nullptr)
	{
		Node* node = new Node(value);
		push_back_pointer(node);
		++size;
		return node;
	}

	if (current_node->value == value)
	{
		push_back_pointer(current_node);
		++size;
		return current_node;
	}

	if (value > current_node->value)
		current_node->right = push_back(current_node->right, value);

	if (value < current_node->value)
		current_node->left = push_back(current_node->left, value);

	return rebalance(current_node);
}


void Sequence::push_back_pointer(Sequence::Node* node)
{
	Sequence::Node::List::Element* pointerToNewElement = pointersToKeys->push_back(node);
	node->toPtrs.push_back(pointerToNewElement);
}


Sequence::Node* Sequence::rebalance(Sequence::Node* node)
{
	changeHeight(node);

	if (getBalance(node) == 2)
	{
		if (getBalance(node->right) == -1)
			node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	if (getBalance(node) == -2)
	{
		if (getBalance(node->left) == 1)
			node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	return node;
}


int Sequence::changeHeight(Node* node)
{
	int leftHeight;
	int rightHeight;

	if (node == nullptr)
		return 0;

	leftHeight = node->left ? node->left->height : 0;
	rightHeight = node->right ? node->right->height : 0;

	node->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;

	return node->height;
}


int Sequence::getBalance(Node* node)
{
	int leftHeight;
	int rightHeight;

	if (node == nullptr)
		return 0;

	leftHeight = node->left ? node->left->height : 0;
	rightHeight = node->right ? node->right->height : 0;

	return rightHeight - leftHeight;
}


Sequence::Node* Sequence::rotateRight(Sequence::Node* node)
{
	Sequence::Node* q = node->left;
	node->left = q->right;
	q->right = node;
	changeHeight(node);
	changeHeight(q);

	return q;
}

Sequence::Node* Sequence::rotateLeft(Sequence::Node* node)
{
	Sequence::Node* q = node->right;
	node->right = q->left;
	q->left = node;
	changeHeight(node);
	changeHeight(q);

	return q;
}

void Sequence::remove_back(int value)
{
	root = remove_back(root, value);
}

Sequence::Node* Sequence::remove_back(Sequence::Node* current_node, int value)
{
	if (current_node == nullptr)
		return nullptr;

	if (value > current_node->value)
		current_node->right = remove_back(current_node->right, value);
	else
		if (value < current_node->value)
			current_node->left = remove_back(current_node->left, value);
		else
		{
			pointersToKeys->remove(current_node->toPtrs[current_node->toPtrs.size() - 1]);
			current_node->toPtrs.erase(current_node->toPtrs.begin() + current_node->toPtrs.size() - 1);
			
			--size;

			if (current_node->toPtrs.size() == 0)
			{
				if (!current_node->right)
					return current_node->left;
				Node* rightMin = findRightMin(current_node->right);
				rightMin->right = removeRightMin(current_node->right);
				rightMin->left = current_node->left;
				return rebalance(rightMin);
			}
		}

	return rebalance(current_node);
}


Sequence::Node* Sequence::findRightMin(Sequence::Node* node)
{
	return node->left ? findRightMin(node->left) : node;
}


Sequence::Node* Sequence::removeRightMin(Sequence::Node* node)
{
	if (node->left == nullptr)
		return node->right;
	node->left = removeRightMin(node->left);
	return rebalance(node);
}


bool Sequence::contains(int value)
{
	return contains(root, value);
}


bool Sequence::contains(Sequence::Node* current_node, int value)
{
	if (current_node == nullptr)
		return false;

	if (current_node->value == value)
		return true;

	if (value > current_node->value)
		return contains(current_node->right, value);

	if (value < current_node->value)
		return contains(current_node->left, value);

}

void Sequence::show()
{
	cout << "< ";
	pointersToKeys->show();
	cout << ">\nsize: " << size;
}

void Sequence::EXCL(const Sequence& sequence)
{
	Sequence::Node::List::Element* this_seq_keys = pointersToKeys->head;
	Sequence::Node::List::Element* seq_keys = sequence.pointersToKeys->head;

	while (this_seq_keys)
	{
		Sequence::Node::List::Element* this_seq_keys_copy = this_seq_keys;
		while (seq_keys && this_seq_keys && seq_keys && this_seq_keys_copy->data->value == seq_keys->data->value)
		{
			this_seq_keys_copy = this_seq_keys_copy->next;
			seq_keys = seq_keys->next;
		}

		if (!seq_keys)
		{
			while (this_seq_keys != this_seq_keys_copy)
			{
				Sequence::Node::List::Element* p = this_seq_keys;
				this_seq_keys = this_seq_keys->next;
				remove_back(p->data->value);
			}
			break;
		}
			
		seq_keys = sequence.pointersToKeys->head;
		this_seq_keys = this_seq_keys->next;
	}

}

void Sequence::CHANGE(const Sequence& sequence, int index)
{
	Sequence::Node::List::Element* p = sequence.pointersToKeys->head;

	if (index >= 0 && index < size)
	{
		while (p)
		{
			add(index++, p->data->value);
			p = p->next;
		}

		p = sequence.pointersToKeys->head;

		while (p)
		{
			remove(index);
			p = p->next;
		}
	}
}

void Sequence::CONCAT(const Sequence& sequence)
{
	Sequence::Node::List::Element* element = sequence.pointersToKeys->head;

	while (element)
	{
		push_back(element->data->value);
		element = element->next;
	}
}

void Sequence::sort()
{
	Sequence::Node::List::Element* it = pointersToKeys->head;
	Sequence::Node::List::Element* it1 = pointersToKeys->head;
	Sequence::Node::List::Element* it2;

	while (it)
	{
		it2 = it1->next;
		while (it2)
		{
			if (it1->data->value > it2->data->value)
			{
				swapElement(it1, it2);
				
				it2 = it2->next->next;
			}
			else
			{
				it1 = it1->next;
				it2 = it2->next;
			}
		}
		it1 = pointersToKeys->head;
		it = it->next;
	}
}

void Sequence::swapElement(Sequence::Node::List::Element* it1, Sequence::Node::List::Element* it2)
{
	Sequence::Node::List::Element* it = new Sequence::Node::List::Element;

	it->previous = it1->previous;
	it->next = it2->next;

	if (it2->next)
		it2->next->previous = it1;
	
	if (it1->previous)
		it1->previous->next = it2;
	else
		pointersToKeys->head = it2;

	it2->previous = it1->previous;
	it2->next = it1;
	it1->previous = it2;
	it1->next = it->next;
}

void Sequence::MERGE(const Sequence& sequence)
{
	CONCAT(sequence);
	sort();
}

void Sequence::add(int index, int value)
{
	Sequence::Node::List::Element* ptr = pointersToKeys->head;
	int offset = 0;

	while (ptr && index > 0)
	{
		if (ptr->data->value == value)
			++offset;
		ptr = ptr->next;
		--index;
	}

	root = add(root, ptr, value, offset);
}

Sequence::Node* Sequence::add(Sequence::Node* current_node, Sequence::Node::List::Element* ptr, int value, int offset)
{
	if (current_node == nullptr || current_node->value == value)
	{
		Node* node = nullptr;

		if (current_node)
			node = current_node;
		else
			node = new Node(value);

		Sequence::Node::List::Element* element;

		if (!ptr)
			element = pointersToKeys->push_back(node);
		else
		{
			if (!ptr->previous)
				element = pointersToKeys->toStart(node);
			else
				element = pointersToKeys->insert(ptr->previous, node);
		}
		
		node->toPtrs.insert(node->toPtrs.begin() + offset, element);

		++size;
		return node;
	}


	if (value > current_node->value)
		current_node->right = add(current_node->right, ptr, value, offset);

	if (value < current_node->value)
		current_node->left = add(current_node->left, ptr, value, offset);

	return rebalance(current_node);
}

int Sequence::getSize()
{
	return size;
}

void Sequence::remove(int index)
{
	Sequence::Node::List::Element* ptr = pointersToKeys->head;

	if (index >= 0 && index < size)
	{
		while (index > 0)
		{
			ptr = ptr->next;
			--index;
		}

		root = remove(root, ptr);
	}
}

Sequence::Node* Sequence::remove(Sequence::Node* current_node, Sequence::Node::List::Element* ptr)
{
	if (current_node == nullptr)
		return nullptr;

	if (ptr->data->value > current_node->value)
		current_node->right = remove(current_node->right, ptr);
	else
		if (ptr->data->value < current_node->value)
			current_node->left = remove(current_node->left, ptr);
		else
		{
			for (int i = 0; i < current_node->toPtrs.size(); ++i)
				if (current_node->toPtrs[i] == ptr)
				{
					current_node->toPtrs.erase(current_node->toPtrs.begin() + i);
					break;
				}
			pointersToKeys->remove(ptr);

			--size;

			if (current_node->toPtrs.size() == 0)
			{
				if (!current_node->right)
					return current_node->left;
				Node* rightMin = findRightMin(current_node->right);
				rightMin->right = removeRightMin(current_node->right);
				rightMin->left = current_node->left;
				return rebalance(rightMin);
			}
		}

	return rebalance(current_node);
}

void Sequence::removeAll()
{
	while (pointersToKeys->head)
		remove(0);
}