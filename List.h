#pragma once
#include <iostream>
using namespace std;
template <class T>
class list
{
private:
	class Node
	{
	public:
		Node* next;
		Node* prev;
		T data;
		Node()
		{
			next = nullptr;
			prev = nullptr;
		}
	};
	Node* head;
	Node* tail;
	int lsize;
	void removeAll()
	{
		while (head->next != tail)
		{
			Iterator t = Delete(head->next);
			head->next = t.cur;
		}
		delete head;
		delete tail;
		head = nullptr;
		tail = nullptr;
		lsize = 0;
	}
public:
	list()
	{
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->prev = head;
		tail->next = nullptr;
		head->prev = nullptr;
		lsize = 0;
	}
	list(const list& obj)
	{
		head = new Node;
		tail = new Node;
		head->next = tail;
		tail->prev = head;
		tail->next = nullptr;
		head->prev = nullptr;
		Node* temp = obj.head->next;
		while (temp != obj.tail)
		{
			insertatTail(temp->data);
			temp = temp->next;
		}
		lsize = obj.lsize;
	}
	list operator=(const list& obj)
	{
		if (lsize != 0) //deleting previous data to avoid memory leak
		{
			removeAll();
		}
		if(head==nullptr)
			head = new Node;
		if(tail==nullptr)
			tail = new Node;
		head->next = tail;
		tail->prev = head;
		tail->next = nullptr;
		head->prev = nullptr;
		Node* temp = obj.head->next;
		while (temp != obj.tail)
		{
			insertatTail(temp->data);
			temp = temp->next;
		}
		lsize = obj.lsize;
		return *this;
	}
	class Iterator
	{
	private:
		Node* cur;
	public:
		Iterator()
		{
			cur = nullptr;
		}
		Iterator(Node* n)
		{
			cur = n;
		}
		Iterator(const Iterator& obj)
		{
			cur = obj.cur;
		}
		Iterator& operator++()
		{
			cur = cur->next;
			return*this;
		}
		Iterator& operator--()
		{
			cur = cur->prev;
			return*this;
		}
		bool operator == (const Iterator& rhs)const
		{
			if (cur == rhs.cur)
				return true;
			return false;
		}
		bool operator != (const Iterator& rhs)const
		{
			if (cur != rhs.cur)
				return true;
			return false;
		}
		T& operator*()
		{
			return cur->data;
		}
		Iterator& operator=(Node* pNode)
		{
			this->cur = pNode;
			return *this;
		}
		friend class list;
	};
	Iterator insert(Iterator it, T d)
	{
		Node* p = it.cur;
		Node* t = new Node;
		t->data = d;
		t->prev = p->prev;
		t->next = p;
		p->prev->next = t;
		p->prev = t;
		Iterator ret(t);
		return ret;
	}
	Iterator Delete(Iterator it)
	{
		Node* p = it.cur;
		Iterator ret(p->next);
		p->prev->next = p->next;
		p->next->prev = p->prev;
		delete p;
		return ret;
	}
	void print()
	{
		Node* temp = head->next;
		while (temp != tail)
		{
			cout << temp->data<<endl;
			temp = temp->next;
		}
		//cout << endl;
	}
	void insertatHead(T d)
	{
		Iterator it(head->next);
		insert(it, d);
		lsize++;
	}
	void insertatTail(T d)
	{
		Iterator it(tail);
		insert(it, d);
		lsize++;
	}
	void DeleteatHead()
	{
		if (head->next != tail) {
			Iterator it(head->next);
			Delete(it);
			lsize--;
		}
	}
	void DeleteatTail()
	{
		if (head->next != tail)
		{
			Iterator it(tail);
			Delete(--it);
			lsize--;
		}
	}
	Iterator begin()
	{
		Iterator it(head->next);
		return it;
	}
	Iterator end()
	{
		Iterator it(tail);
		return it;
	}
	int getSize()
	{
		return lsize;
	}
	T* search(T d)
	{
		Node* temp = head->next;
		while (temp != tail)
		{
			if (temp->data == d)
				return &temp->data;
			temp = temp->next;
		}
		return nullptr;
	}
	~list()
	{
		removeAll();
	}
};
