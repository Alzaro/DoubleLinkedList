#pragma once
////////////////////////////////////////////////////////////////////////////////////////////////
// File :			DLList.h
// Author :			Corey Feist
// Purpose :		Driver of the Doubly Linked List(DLList) and Doubly Linker Iterator(DLIter) classes
////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Type> class DLLIter;

template <typename Type = int> class DLList
{
private:
	friend class DLLIter<Type>;
	struct Node
	{
		Type data;
		Node* next, *prev;
		Node(Type _data, Node* _prev) : data(_data), prev(_prev), next(nullptr) { }
	};
	
	Node* head, *tail;
	unsigned int Size;

public:

	
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Notes : constructs an empty list
	/////////////////////////////////////////////////////////////////////////////
	DLList() { head = tail = nullptr; Size = 0; }

	/////////////////////////////////////////////////////////////////////////////
	// Function : Destructor
	// Notes : Destroys a list
	/////////////////////////////////////////////////////////////////////////////
	~DLList() { clear(); }
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : Assignment Operator
	/////////////////////////////////////////////////////////////////////////////
	DLList<Type>& operator=(const DLList<Type>& _other);
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : Copy Constructor
	/////////////////////////////////////////////////////////////////////////////
	DLList(const DLList<Type>& _other);
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : addHead
	// Parameters : v - the item to add to the head of the list
	/////////////////////////////////////////////////////////////////////////////
	void addHead(const Type& _data);
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : addTail
	// Parameters : v - the item to add to the tail of the list
	/////////////////////////////////////////////////////////////////////////////
	void addTail(const Type& _data);
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : clear
	// Notes : clears the list, freeing any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	void clear();
	
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : insert
	// Parameters :	index - an iterator to the location to insert at
	//				v - the item to insert
	// Notes : do nothing on a bad iterator
	/////////////////////////////////////////////////////////////////////////////
	void insert(DLLIter<Type>& _index, const Type& _data);
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : remove
	// Parameters :	index - an iterator to the location to remove from
	// Notes : do nothing on a bad iterator
	/////////////////////////////////////////////////////////////////////////////
	void remove(DLLIter<Type>& _index);

	unsigned int size() { return Size; }


};

template <typename Type>
DLList<Type>& DLList<Type>::operator=(const DLList<Type>& _other)
{
	
	if (this != &_other)
	{
		if (!_other.head)
		{
			clear();
			return *this;
		}
		clear();
		Node* temp1 = _other.head;
		while (temp1 != nullptr)
		{
			addTail(temp1->data);
			temp1 = temp1->next;
		}
	}
	
	return *this;
}

template <typename Type>
DLList<Type>::DLList(const DLList<Type>& _other) : DLList()
{
	if (this != &_other)
	{
		if (!_other.head)
		{
			return;
		}
		clear();
		Node* temp1 = _other.head;
		while (temp1 != nullptr)
		{
			addTail(temp1->data);
			temp1 = temp1->next;
		}
	}

}

template <typename Type>
void DLList<Type>::addHead(const Type& _data)
{
	Node* newNode = new Node(_data, nullptr);
	if (head == nullptr) 
	{
		head = newNode;
		tail = newNode;
	}
	else 
	{
		head->prev = newNode;
		newNode->next = head;
		head = newNode;
	}
	++Size;
}

template <typename Type>
void DLList<Type>::addTail(const Type& _data)
{
	Node* newNode = new Node(_data, nullptr);
	if (tail == nullptr)
	{
		tail = newNode;
		head = newNode;
	}
	else
	{
		tail->next = newNode;
		newNode->prev = tail;
		newNode->next = nullptr;
		tail = newNode;
	}
	++Size;
}

template <typename Type>
void DLList<Type>::clear()
{
	if (head != nullptr) 
	{
		Node* temp;
		while (head != nullptr)
		{
			temp = head;
			head = head->next;
			delete temp;
		}
	}
	head = tail = nullptr;
	Size = 0;
	
	
	
}

template <typename Type>
void DLList<Type>::insert(DLLIter<Type>& _index, const Type& _data)
{
	if (!_index.end())
	{
		if (_index.node == head) 
		{
			addHead(_data);
			_index.node = head;
		}
		else if (_index.node == tail)
		{
			Node* temp = new Node(_data, tail->prev);
			temp->next = tail;
			if (tail->prev != nullptr)
			{
				tail->prev->next = temp;
				tail->prev = temp;
			}
			_index.node = temp;
			++Size;
		}
		else 
		{
			Node* temp = head;
			while (temp != _index.node)
			{
				temp = temp->next;
			}
			Node* newNode = new Node(_data, temp->prev);
			newNode->next = temp;
			if (temp->prev != nullptr)
				temp->prev->next = newNode;
			temp->prev = newNode;
			_index.node = newNode;
			++Size;
		}
		
	}
}

template <typename Type>
void DLList<Type>::remove(DLLIter<Type>& _index)
{
	if (!_index.end())
	{
		Node* temp = head;

		if (head != _index.node)
		{
			while (temp != _index.node && temp != nullptr)
			{
				temp = temp->next;
			}
			_index.node = temp->prev;
			if (temp != nullptr)
			{
				if (temp->prev != nullptr)
				{
					temp->prev->next = temp->next;
				}
				if (temp->next != nullptr)
				{
					temp->next->prev = temp->prev;
				}
			}
		}
		else 
		{
			head = temp->next;
			_index.node = head;
		}
		if (head == nullptr)
		{
			head = nullptr;
			tail = nullptr;
		}
		delete temp;
		--Size;
	}
}

template <typename Type>
class DLLIter
{
	friend class DLList<Type>;
private:
	typename DLList<Type>::Node* node;
	DLList<Type>* list;
public:
	/////////////////////////////////////////////////////////////////////////////
	// Function : Constructor
	// Parameters :	listToIterate - the list to iterate
	/////////////////////////////////////////////////////////////////////////////
	DLLIter(DLList<Type>& listToIterate) { list = &listToIterate; node = nullptr; }
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : beginHead
	// Notes : moves the iterator to the head of the list
	/////////////////////////////////////////////////////////////////////////////
	void beginHead() { node = list->head; }
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : beginTail
	// Notes : moves the iterator to the tail of the list
	/////////////////////////////////////////////////////////////////////////////
	void beginTail() { node = list->tail; }
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : end
	// Notes : returns true if we are at the end of the list, false otherwise
	/////////////////////////////////////////////////////////////////////////////
	bool end() const;
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : operator++
	// Notes : move the iterator forward one node
	/////////////////////////////////////////////////////////////////////////////
	DLLIter<Type>& operator++();
	
	/////////////////////////////////////////////////////////////////////////////
	// Function : operator--
	// Notes : move the iterator backward one node
	/////////////////////////////////////////////////////////////////////////////
	DLLIter<Type>& operator--();

	Type& current() const { return node->data; }
};

template <typename Type>
bool DLLIter<Type>::end() const
{
	if (node == nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename Type>
DLLIter<Type>& DLLIter<Type>::operator++()
{
	if (node != nullptr)
	{
		node = node->next;
	}
	return *this;
}

template <typename Type>
DLLIter<Type>& DLLIter<Type>::operator--()
{
	if (node != nullptr)
	{
		node = node->prev;
	}
	return *this;
}