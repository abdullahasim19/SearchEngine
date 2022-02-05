#include <iostream>
#include "List.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;
class Doc_Info
{
	int DocID;
	int tFrequency;
	friend class Search_Engine;
public:
	Doc_Info(int a = 0, int b = 0)
	{
		DocID = a;
		tFrequency = b;
	}
	friend ostream& operator<<(ostream& out, const Doc_Info& obj)
	{
		out << " Doc " << obj.DocID << " Tf " << obj.tFrequency;
		return out;
	}
};
class Term_Info
{
private:
	string key;
	list <Doc_Info> obj;
	friend class Search_Engine;
public:
	Term_Info()
	{
		key = ' ';
	}
	Term_Info(string n)
	{
		key = n;
	}
	friend ostream& operator<<(ostream& out, Term_Info& o)
	{
		out << o.key<<": ";
		list <Doc_Info>::Iterator it;
		list <Doc_Info>::Iterator it1;
		it1 = o.obj.begin();
		for (it = o.obj.begin(); it != o.obj.end(); ++it)
		{
			out << *it ;
			if (++it1 != o.obj.end())
				out << " ->";
		}
		return out;
	}
	bool operator==(const Term_Info& obj)
	{
		if (key == obj.key)
			return true;
		else
			return false;
	}
	friend class Search_Engine;
	char operator[](int k)
	{
		if (k >= 0 && k < key.length())
			return key[k];
		return ' ';
	}
	int length()
	{
		return key.length();
	}
	Term_Info operator=(const Term_Info& o)
	{
		key = o.key;
		obj = o.obj;
		return *this;
	}
};
template <class T>
class HashTable
{
	list <T>* table;
	int tablesize;
	int maxsize;
	void reHashing()
	{
		typename list<T>::Iterator it;
		int temp1 = maxsize;
		list <T>* temp = new list <T>[maxsize * 2];
		maxsize = maxsize * 2;
		int index = 0;
		for (int i = 0; i < temp1; i++)
		{
			if (table[i].getSize() != 0)
			{
				for (it = table[i].begin(); it != table[i].end(); ++it)
				{
					index = HF(*it);
					temp[index].insertatTail(*it);
				}
			}
		}
		delete[]table;
		table = temp;
	}
	int HF(T n)
	{
		int max = maxsize; //table size
		int p = 1;
		int ans = 1;
		int x = 37; //choice of x factor
		for (int i = 0; i < n.length(); i++)
		{
			ans = (ans+n[i] * p) % max;
			p = (p * x) % max;
		}
		return ans;
	}
	int LoadFactor()
	{
		return tablesize / maxsize;
	}
public:
	HashTable()
	{
		tablesize = 0;
		maxsize = 37;
		table = new list <T>[maxsize];
	}
	HashTable(const HashTable& o)
	{
		maxsize = o.maxsize;
		tablesize = o.tablesize;
		table = new list <T>[maxsize];
		for (int i = 0; i < maxsize; i++)
		{
			if (o.table[i].getSize() != 0)
				table[i] = o.table[i];
		}
	}
	HashTable operator = (const HashTable& o)
	{
		//if tablesize is 0 it means no the hashtable is empty so we do not delete and reallocate the memory
		//if tablesize is not 0 then we have to delete the previous data and the copy the new one
		if (tablesize != 0) 
		{
			delete[]table;
			table = new list <T>[o.maxsize];
		}
		maxsize = o.maxsize;
		tablesize = o.tablesize;
		for (int i = 0; i < maxsize; i++)
		{
			if (o.table[i].getSize() != 0)
				table[i] = o.table[i];
		}
		return* this;
	}
	void insert(T d)
	{
		tablesize++;
		if (LoadFactor() >= 2)
			reHashing();
		int index = HF(d);
		table[index].insertatTail(d);
	}
	void print()
	{
		for (int i = 0; i < maxsize; i++)
		{
			if (table[i].getSize() != 0)
			{
				table[i].print();
			}
		}
	}
	T* search(T d)
	{
		int index = HF(d);
		if (table[index].getSize() != 0)
			return table[index].search(d);
		return nullptr;
	}
	~HashTable()
	{
		delete[]table;
		table = nullptr;
		maxsize = 0;
		tablesize = 0;
	}
	int collision()
	{
		int count = 0;
		for (int i = 0; i < maxsize; i++)
		{
			if (table[i].getSize() > 1)
				count++;
		}
		return count;
	}
};
class Search_Engine
{
	vector <string> original;
	HashTable <Term_Info> index;
	vector <Term_Info> words;

	string* tokenize(string in, int& result)
	{
		int tsize = 0;
		int size = in.length();
		for (int i = 0; i < size; i++)   //counting words
		{
			if (in[i] == ' ')
				tsize++;
		}
		tsize++;
		string* token = new string[tsize]; //tokenizing
		int k = 0;
		for (int i = 0; i < size; i++)
		{
			if (in[i] != ' ')
			{
				token[k].push_back(in[i]);
			}
			else
			{
				k++;
			}
		}
		int count = 0;										//removing common words
		for (int j = 0; j < tsize; j++)
		{
			if (token[j] != "of" && token[j] != "for" && token[j] != "the")
				count++;
		}
		if (count != tsize)
		{
			string* temp = new string[count];
			k = 0;
			for (int j = 0; j < tsize; j++)
			{
				if (token[j] != "of" && token[j] != "for" && token[j] != "the")
					temp[k++] = token[j];
			}
			result = count;
			return temp;
		}
		else
		{
			result = tsize;
			return token;
		}
	}
	int CountOccurrences(string str, string word)
	{
		int count = 0;
		int stren = str.length();
		int wordLen = word.length();
		int j;
		for (int i = 0; i <= stren; i++)
		{
			for (j = 0; j < wordLen; j++)
			{
				if (str[i + j] != word[j])
				{
					break;
				}
			}
			if (j == wordLen)
			{
				count++;
			}
		}
		return count;
	}
	void SortInt(vector <Doc_Info>& arr)
	{
		for (int i = 0; i < arr.size() - 1; i++)
		{
			for (int j = 0; j < arr.size() - i - 1; j++)
			{
				if (arr[j].DocID > arr[j + 1].DocID)
				{
					Doc_Info temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
				}
			}
		}
	}
	void SortDocs(vector <Doc_Info>& arr)
	{
		for (int i = 0; i < arr.size() - 1; i++)
		{
			for (int j = i+1; j < arr.size() - i - 1; j++)
			{
				if (arr[j].DocID > arr[j + 1].DocID&&arr[j].tFrequency==arr[i].tFrequency)
				{
					Doc_Info temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
				}
			}
		}
	}
	void SortMax(vector<Doc_Info>& arr)
	{
		for (int i = 0; i < arr.size() - 1; i++)
		{
			for (int j = 0; j < arr.size() - i - 1; j++)
			{
				if (arr[j].tFrequency < arr[j + 1].tFrequency)
				{
					Doc_Info temp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = temp;
				}
			}
		}
	}
	Doc_Info maxFrequency(list <Doc_Info> o1, int size, bool* flag)
	{
		int max = 1;
		list <Doc_Info>::Iterator it;
		Doc_Info ob;
		Doc_Info l;
		int d = 0;
		for (int i = 0; i < size; i++)
		{
			for (it = o1.begin(); it != o1.end(); ++it)
			{
				ob = *it;
				if (max < ob.tFrequency && flag[ob.DocID])
				{
					max = ob.tFrequency;
					d = ob.DocID;
					l = ob;
				}
			}
		}
		flag[d] = false;
		return l;
	}
	void remove_duplicates()
	{
		for (int i = 0; i < words.size(); i++)
		{
			for (int j = i + 1; j < words.size(); j++)
			{
				if (words[i] == words[j])
				{
					words.erase(words.begin() + j);
					j--;
				}
			}
		}
	}
public:
	void Create_Index(string* names, int n)
	{
		for (int p = 0; p < n; p++)
		{
			Add_Doc_to_Index(names[p]);
		}
	}
	void Add_Doc_to_Index(string name)
	{
		ifstream fin;
		fin.open(name);
		if (fin)
		{
			string ap;  //temporary 
			string in;
			int conv = 0;
			while (!fin.eof()) //extracting data from files
			{
				if (conv > 0)
					in.push_back(' ');
				fin >> ap;
				in = in + ap;
				conv++;
			}
			int size1 = in.length();
			for (int a = 0; a < size1; a++) //removing uppercase letters
			{
				if (in[a] >= 65 && in[a] <= 90)
					in[a] = in[a] + 32;
			}
			original.push_back(in);

			int count = 0;
			string* temp = tokenize(in, count); //tokenizing 
			
			Term_Info obj1;
			int before = words.size();
			for (int i =0; i < count; i++) //putting the data in vector 
			{
				obj1.key = temp[i];
				words.push_back(obj1);
			}
			remove_duplicates();

			for (int i = before; i < words.size(); i++) //after removing duplicates inserting in the hashtable
				index.insert(words[i]);

			//now computing the doc list of each key term in hashtable
			for (int i=0;i<words.size();i++) 
			{
				Term_Info *o =index.search(words[i]);
				if (o != nullptr)
				{
					count = CountOccurrences(original[original.size() - 1], o->key);
					if (count > 0)
					{
						Doc_Info d(original.size(), count);
						o->obj.insertatTail(d);
					}
				}
			}
			fin.close();
			delete[]temp;
		}
		else
			cout << "Input File " << name << " not found\n";
	}
	void printList()
	{
		index.print();
	}
	void Search_Documents(string n)
	{
		bool* flags = new bool[original.size() + 1]; //bool array to check which docs have been printed
		for (int q = 0; q < original.size() + 1; q++)
			flags[q] = true;

		int size = n.length();
		for (int a = 0; a < size; a++) //removing uppercase letters
		{
			if (n[a] >= 65 && n[a] <= 90)
				n[a] = n[a] + 32;
		}

		size = 0;
		string* temp = tokenize(n, size);
		list<Doc_Info>* arr = new list<Doc_Info>[size];

		int checking = 0;
		Term_Info term;
		for (int u = 0; u < size; u++)
		{
			term.key = temp[u];
			if (index.search(term) == nullptr)
			{
				cout << "Word " << temp[u] << " not found\n";
				checking++;
			}
		}
		if (checking < size)
		{
			int size1 = 0;
			Term_Info* tptr = nullptr;
			for (int i = 0; i < size; i++) //storing the doc lists of query words
			{
				term.key = temp[i];
				tptr = index.search(term);
				if (tptr != nullptr)
				{
					arr[i] = tptr->obj;
					size1++;
				}
			}
			//size = size1;
			list<Doc_Info>::Iterator it;
			vector <Doc_Info> v;
			if (size > 1)
			{
				for (int i = 0; i < size; i++) //controlling size of each word's list
				{
					for (it = arr[i].begin(); it != arr[i].end(); ++it) //starting traversal through each word
					{
						for (int j = i + 1; j < size; j++) //checking if there is repetion of documents
						{
							Doc_Info obj;
							Doc_Info obj1;
							obj = *it;
							list<Doc_Info>::Iterator it1;
							for (it1 = arr[j].begin(); it1 != arr[j].end(); ++it1)
							{
								obj1 = *it1;
								if (obj.DocID == obj1.DocID && flags[obj.DocID])
								{
									obj.tFrequency += obj1.tFrequency;
									v.push_back(obj);
									flags[obj.DocID] = false;
								}
							}
						}
					}
				}
				if (!v.empty())
					SortMax(v);
				for (int s = 0; s < v.size(); s++)
				{
					cout << "Doc: " << v[s].DocID << endl;
				}
			}
			if(!v.empty())
				v.clear();

			for (int i = 0; i < size; i++)
			{
				for (int t = 0; t < arr[i].getSize(); t++)
				{
					Doc_Info maxf = maxFrequency(arr[i], size, flags);  //ranking rule 2
					if (maxf.tFrequency != 0)
						v.push_back(maxf);
				}
			}
			if (!v.empty())
			{
				SortMax(v);
				SortDocs(v);
				for (int i = 0; i < v.size(); i++)
					cout << "Doc: " << v[i].DocID << endl;
			}


			list<Doc_Info>::Iterator f1;
			Doc_Info f2;
			vector <Doc_Info>arr1;
			for (int k = 0; k < size; k++)   //ranking rule 3
			{
				for (f1 = arr[k].begin(); f1 != arr[k].end(); ++f1)
				{
					f2 = *f1;
					if (flags[f2.DocID])
					{
						arr1.push_back(f2);
					}
				}
			}
			if (arr1.size() > 0)
			{
				SortInt(arr1);
				for (int t = 0; t < arr1.size(); t++)
					cout << "Doc: " << arr1[t].DocID << endl;
			}
			delete[]temp;
			delete[]arr;
			delete[]flags;
		}
		else
			cout << "Query not found\n";
	}
	int collisions()
	{
		return index.collision();
	}
};
int main()
{
	string arr[4] = { "Doc1.txt","Doc2.txt","Doc3.txt","Doc4.txt" };
	Search_Engine obj;
	obj.Create_Index(arr, 4);
	//obj.Add_Doc_to_Index("Doc5.txt");
	obj.printList();
	cout << "\n**********************\n\n";
	cout << "Enter a Query\n";
	string query;
	getline(cin, query);
	obj.Search_Documents(query);
	system("pause");
	return 0;
}