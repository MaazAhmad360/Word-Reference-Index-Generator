#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <iomanip>
using namespace std;


//-----------------------------------------------------------
//----------------------Occurence Class----------------------
//-----------------------------------------------------------
class Occurence
{
	friend std::ostream& operator<<(std::ostream& out, Occurence const& obj);
private:
	int chap;
	int page;
	int line;

public:
	Occurence()
	{
		chap = 0;
		page = 0;
		line = 0;
	}
	Occurence(int c = 0, int p = 0, int l = 0)
	{
		chap = c;
		page = p;
		line = l;
	}
	Occurence(const Occurence& rhs)
	{
		chap = rhs.chap;
		page = rhs.page;
		line = rhs.line;
	}
	void operator=(const Occurence& rhs)
	{
		chap = rhs.chap;
		page = rhs.page;
		line = rhs.line;
	}
	void setLine(int l)
	{
		line = l;
	}
	~Occurence()
	{

	}
};

//operator>> is a global function. This is not a member function of class
//overloaded stream extraction operator
std::ostream& operator<<(std::ostream& out, Occurence const& obj)
{
	/*if (obj.chap == 32)
		out << "\tChapter Number: " << 1 << "\n";
	else*/
		out << "\tChapter Number: " << obj.chap << "\n"; //assuming the number were initially string and are stored as ASCII
	out << "\tPage Number: " << obj.page << "\n";
	out << "\tLine Number: " << obj.line << "\n";
	return out;	// to enable cascading
}


/// <summary>
/// BST Template Class to Store a Key and a Value
/// </summary>
/// <typeparam name="k">main element to store and order BST</typeparam>
/// <typeparam name="v">the main data</typeparam>
template <typename k, typename v>
class BST {
protected:
	class TNode
	{
	public:
		k key;
		v value;
		TNode* leftChild;
		TNode* rightChild;

		TNode(k ke, v va)
		{
			key = ke;
			value = va;
		}
		TNode(TNode& rhs)
		{
			key = rhs.key;
			value = rhs.value;
		}
	};


public:
	TNode* root;
	BST();
	BST(k key, v value);
	BST(BST& rhs);
	void deepCopy(TNode*& lhs, TNode*& rhs);
	virtual void insert(k const key, v const value);
	virtual void insert(TNode*& root, k const key, v const value);
	void insert(TNode const temp);
	void insert(TNode*& root, TNode const temp);
	bool search(k const key);
	bool search(TNode*& root, k const key);
	void inorderPrintKeys() const;
	void inorderPrintKeys(TNode* root) const;
	void inorderPrintValues() const;
	void inorderPrintValues(TNode* root) const;
	int length() const;
	int length(TNode* root, int count) const;
};

class specialBST {
private:
	class TNode
	{
	public:
		string key;
		list<Occurence> value;
		TNode* leftChild;
		TNode* rightChild;
		TNode()
		{
			leftChild = rightChild = nullptr;
		}
		TNode(string ke, Occurence va)
		{
			key = ke;
			value.push_back(va);
		}
		TNode(string ke, list<Occurence> va)
		{
			key = ke;
			value = va;
		}
		TNode(TNode& rhs)
		{
			key = rhs.key;
			value = rhs.value;
		}
		void operator=(TNode& rhs)
		{
			key = rhs.key;
			value = rhs.value;
		}
	};


public:
	TNode* root;
	specialBST();
	specialBST(string key, list<Occurence> value);
	specialBST(const specialBST& rhs); //wrapper Copy Constructor that calls on Recursive Deep Copy
	void deepCopy(TNode*& lhs, TNode* rhs); // Recursive Deep Copy
	void insert(string const key, Occurence const value);
	void insert(TNode*& root, string const key, Occurence const value);
	void insert(TNode temp);
	void insert(TNode*& root, TNode temp);
	list<Occurence>* search(string const key); //returns pointer to the list of occurence that contains the string
	list<Occurence>* search(TNode*& root, string const key);
	void findReference(string const key);	// using the search function prints the list of occuerence
	//void inorderPrintKeys() const;
	//void inorderPrintKeys(TNode* root) const;
	void inorderPrintValues() const;		//wrapper that calls recursive inorder traversal function below
	void inorderPrintValues(TNode* root) const;
	int length() const;		// wrapper that calls recursive length function that returns the number of nodes in the tree
	int length(TNode* root) const;

	void loadInputFile(ifstream& fin, BST<string, string> stopWords);
	void populateInputBST(TNode* inputWords, int size);

	void populateInputBST(TNode* inputWords, int low, int high);

	TNode* copyToArray();
	void copyToArray(TNode* root, TNode*& temp, int& index, int size);
	TNode* mergeSortedArray(TNode* arr1, TNode* arr2, int size1, int size2);
	
	void sort();
	specialBST mergeBSTs(specialBST lhs, specialBST rhs);
	void intersection(TNode* arr1, TNode* arr2, int size1, int size2);
	void findCommonWords(specialBST lhs);

	void deleteNode(string k);
	TNode* deleteNode(TNode* root, string k, bool& flag);

	void writeOnFile(ofstream& fout);
	void writeOnFile(ofstream& fout, TNode* root);

	void destroy(TNode* root)
	{
		if (!root)
		{

		}
		destroy(root->leftChild);
		destroy(root->rightChild);

		delete root;
	}

	~specialBST()
	{
		destroy(root);
	}
};


//-----------------------------------------------------------
//-----------------------------------------------------------
//----------------------Non-Class Functions------------------

/// <summary>
/// Loads all the stop words from file and returns a vector of strings
/// </summary>
/// <returns>vector of strings (stopWords)</returns>
vector<string> loadStopWords(ifstream& fin)
{

	if (fin.is_open())
	{
		string word;
		vector<string> stopWords;

		while (!fin.eof())
		{
			getline(fin, word);
			stopWords.push_back(word);
		}

		fin.close();

		return stopWords;
	}
	else
	{
		cout << "Error: Unable to open STOP WORDS.rtf" << endl;
	}
	
}

/// <summary>
/// populates BST of stop Words using its overloaded recursive function
/// </summary>
/// <returns>Stop Words BST</returns>
template<typename k, typename v>
void populateStopBST(ifstream& fin, BST<k, v>& stopWordsBST)
{
	if (fin.is_open())
	{
		vector<string> stopWords = loadStopWords(fin); //storing each stop word in a vector of strings
		int high = stopWords.size() - 1;
		int low = 0;

		//BST<string, string> stopWordsBST;
		populateStopBST(stopWords, low, high, stopWordsBST);

		cout << "-------------Stop Words Loaded and Ready To be Used-------------------\n\n";
	}
	else 
	{
		cout << "Unable to open File\n";
	}

	//return stopWordsBST;
}
template<typename k, typename v>
void populateStopBST(vector<string> stopWords, int low, int high, typename BST<k, v>& bst)
{
	if (low <= high)
	{
		int mid = low + (high - low) / 2;
		bst.insert(stopWords[mid], stopWords[mid]);

		populateStopBST(stopWords, low, mid - 1, bst);
		populateStopBST(stopWords, mid + 1, high, bst);
	}


}

//-----------------------------------------------------------
//-----------------------Menu Function-----------------------
//-----------------------------------------------------------
void menu()
{
	int n = 0;
	BST<string, string> stopWordsBST;
	specialBST index1;
	specialBST index2;
	specialBST mergedBst;

	while (n != -1)
	{
		cout << "\n\n-----------------------------------------------------------------\n";
		cout << "-------------------------INDEX CREATION SOFTWARE-----------------\n";
		cout << "-----------------------------------------------------------------\n\n";
		cout << setw(10) << "SELECT THE MENU TO PROCEED\n\n";
		cout << "0. Load Stop Words\n";
		cout << "1. Load 1st Index\n";
		cout << "2. Load 2nd Index\n";
		cout << "3. Print All Words\n";
		cout << "4. Search a Word's Reference\n";
		cout << "5. Delete a Word's Index\n";
		cout << "6. Merge two Indexes (First and Second)\n";
		cout << "7. Find Common Words between two Indexes (First and Second)\n";
		cout << "8. Export to .txt File\n";
		cout << "-1. Exit the Program\n";

		cin >> n;
		cin.clear();

		switch (n)
		{
		case -1:
			break;
		case 0:
		{
			string fileStr;
			/*cout << "Enter Stop Words Full File Name: ";
			cin >> fileStr;*/
			fileStr = "STOPWORDS.txt";
			//fileStr += ".txt";
			ifstream inFile(fileStr);
			populateStopBST(inFile, stopWordsBST);


			break;
		}
		case 1:
		{
			string fileStr;
			cout << "Enter Full File Name of First Input: ";
			cin >> fileStr;
			ifstream inFile(fileStr);
			index1.loadInputFile(inFile, stopWordsBST);

			cout << "--------------First Index Loaded and Ready to be Used--------------------\n\n";

			break;
		}
		case 2:
		{
			string fileStr;
			cout << "Enter Full File Name of Second Input: ";
			cin >> fileStr;
			ifstream inFile(fileStr);
			index2.loadInputFile(inFile, stopWordsBST);

			cout << "--------------Second Index Loaded and Ready to be Used--------------------\n\n";

			break;
		}
		case 3:
		{
			int m;
			cout << "Select Index to Show:\n1. Stop Words\n2. Index 1\n3. Index 2\n4. Merged Index\n";
			cin >> m;

			switch (m)
			{
			case 1:
			{
				cout << "------------------------Stop Words------------------------------\n";
				stopWordsBST.inorderPrintKeys();
				break;
			}
			case 2:
			{
				cout << "------------------------First Index Words------------------------------\n";
				index1.inorderPrintValues();
				break;
			}
			case 3:
			{
				cout << "------------------------Second Index Words------------------------------\n";
				index2.inorderPrintValues();
				break;
			}
			case 4:
			{
				cout << "------------------------Merged Index Words------------------------------\n";
				mergedBst.inorderPrintValues();
				break;
			}
			default:
				break;
			}

			break;
		}
		case 4:
		{
			int m;
			cout << "Select Index to Show:\n1. Index 1\n2. Index 2\n3. Merged Index\n";
			cin >> m;
			cin.clear();
			string str;
			cout << "Select Word to Find:\n";
			cin >> str;

			switch (m)
			{
			case 1:
			{
				cout << "------------------------Reference in First Index------------------------------\n";
				index1.findReference(str);
				break;
			}
			case 2:
			{
				cout << "------------------------Reference in Second Index------------------------------\n";
				index2.findReference(str);
				break;
			}
			case 3:
			{
				cout << "------------------------Reference in Merged Index------------------------------\n";
				mergedBst.findReference(str);
				break;
			}
			default:
				break;
			}
			break;
		}
		case 5:
		{
			int m;
			cout << "Select Index to Delete From:\n1. Index 1\n2. Index 2\n3. Merged Index\n";
			cin >> m;
			cin.clear();
			string str;
			cout << "Select Word to Delete:\n";
			cin >> str;

			switch (m)
			{
			case 1:
			{
				cout << "------------------------Deletion in First Index------------------------------\n";
				index1.deleteNode(str);
				break;
			}
			case 2:
			{
				cout << "------------------------Deletion in Second Index------------------------------\n";
				index2.deleteNode(str);
				break;
			}
			case 3:
			{
				cout << "------------------------Deletion in Merged Index------------------------------\n";
				mergedBst.deleteNode(str);
				break;
			}
			default:
				break;
			}
			break;
		}
		case 6:
		{
			cout << "----------------------------Merging Indexes-------------------\n";
			mergedBst = index1.mergeBSTs(index1, index2);
			cout << "Merged Successfully\n";
			break;
		}
		case 7:
		{
			cout << "------------------------Common Words B/W Two Indexes--------------------\n";
			index1.findCommonWords(index2);
			cout << endl;
			break;
		}
		case 8:
		{
			int m;
			cout << "Select Index to Write:\n1. Index 1\n2. Index 2\n3. Merged Index\n";
			cin >> m;
			cin.clear();
			string fileStr;
			cout << "Enter File Name: ";
			cin >> fileStr;
			ofstream outFile(fileStr);
			

			switch (m)
			{
			case 1:
			{
				cout << "--------------Index Written On File--------------------\n\n";
				index1.writeOnFile(outFile);
				break;
			}
			case 2:
			{
				cout << "--------------Index Written On File--------------------\n\n";
				index1.writeOnFile(outFile);
				break;
			}
			case 3:
			{
				cout << "--------------Index Written On File--------------------\n\n";
				mergedBst.writeOnFile(outFile);
				break;
			}
				
			default:
				break;
			}

			break;
		}
		default:
			break;
		}
	}
	
}

//------------------------------------------------------------
//------------------------------------------------------------
//---------------------Main Function--------------------------
int main()
{
	menu();

	return 0;
}


//-----------------------------------------------------------
//-----------------------------------------------------------
//----------------------Special BST Methods------------------

specialBST::specialBST()
{
	root = nullptr;
}
specialBST::specialBST(string key, list<Occurence> value)
{
	this->root->key = key;
	this->root->value = value;
	this->root->leftChild = this->root->rightChild = nullptr;
}
specialBST::specialBST(const specialBST& rhs)
{
	deepCopy(root, rhs.root);
}
void specialBST::deepCopy(TNode*& lhs, TNode* rhs)
{
	if (!rhs)
	{

	}
	else
	{
		lhs = new TNode(rhs->key, rhs->value);
		deepCopy(lhs->leftChild, rhs->leftChild);
		deepCopy(lhs->rightChild, rhs->rightChild);
	}
}
void specialBST::insert(string key, Occurence value)
{
	insert(this->root, key, value);
}
void specialBST::insert(TNode*& root, string key, Occurence value)
{
	if (!root)
	{
		root = new TNode(key, value);
		/*root->key = key;
		root->value = value;*/
		root->rightChild = root->leftChild = nullptr;

	}

	if (key > root->key)
	{
		insert(root->rightChild, key, value);
	}
	else if (key < root->key)
	{
		insert(root->leftChild, key, value);
	}
}
void specialBST::insert(specialBST::TNode temp)
{
	insert(this->root, temp);
}
void specialBST::insert(specialBST::TNode*& root, TNode temp)
{
	if (!root)
	{
		root = new TNode(temp);
		/*root->key = key;
		root->value = value;*/
		root->rightChild = root->leftChild = nullptr;

	}

	if (temp.key > root->key)
	{
		insert(root->rightChild, temp);
	}
	else if (temp.key < root->key)
	{
		insert(root->leftChild, temp);
	}
}
void specialBST::inorderPrintValues() const
{
	cout << "ALL WORDS\n";
	inorderPrintValues(this->root);
}
void specialBST::inorderPrintValues(TNode* rhs) const
{
	if (!rhs)
	{

	}
	else
	{
		inorderPrintValues(rhs->leftChild);

		cout << "Key: " << rhs->key << "\n";
		list<Occurence>::iterator it = rhs->value.begin();
		cout << "Reference:\n";
		for (auto const& v : rhs->value)
			std::cout << v << "\n";

		inorderPrintValues(rhs->rightChild);
	}
}
list<Occurence>* specialBST::search(string const key)
{
	return search(this->root, key);
}
list<Occurence>* specialBST::search(TNode*& root, string const key)
{
	if (!root)
	{
		return nullptr;
	}
	else if (key == root->key)
	{
		return &root->value;
	}
	else
	{
		if (key > root->key)
		{
			return search(root->rightChild, key);
		}
		else if (key < root->key)
		{
			return search(root->leftChild, key);
		}
	}
}
void specialBST::findReference(string const key)
{
	list<Occurence>* ref = search(key);
	
	if (ref)
	{
		cout << "\"" << key << "\" found in following text\n";
		cout << "Reference:\n";
		for (list<Occurence>::iterator it = ref->begin(); it != ref->end(); it++)
		{
			cout << *it << "\n";
		}
	}
	else
	{
		cout << "\"" << key << "\" Not found in BST\n";
	}
}
int specialBST::length() const
{
	return length(this->root);
}
int specialBST::length(TNode* root) const
{
	/*if (!root)
	{
		return 0;
	}
	else
	{
		count = 1 + length(root->leftChild, count) + length(root->rightChild, count);
		return count;
	}*/

	if (root == NULL)
		return 0;
	else
		return(length(root->leftChild) + 1 + length(root->rightChild));
}

void specialBST::loadInputFile(ifstream& fin, BST<string, string> stopWords)
{
	string para;
	string chStr = "Chapter";
	string pgStr = "Page";
	int currCh, currPg;
	int currLn = 1;

	while (!fin.eof())
	{
		getline(fin, para);
		if (!para.empty())
		{
			if (para[0] == '#')
			{
				if (para.find(chStr) != string::npos)
				{
					
					currCh = stoi(para.substr(10));
					continue;
				}

				if (para.find(pgStr) != string::npos)
				{
					currPg = stoi(para.substr(7));
					continue;
				}
			}

			//Extracting words from string
			string word = "";
			Occurence occ(currCh, currPg, currLn);
			for (int i = 0; i < para.size(); i++)
			{
				if (para[i] == ' ' || para[i] == ',' || para[i] == '.' || para[i] == '\'' || para[i] == ')')
				{
					if (static_cast<int>(word[0]) != 0)	// to prevent storing null
					{
						if (static_cast<int>(word[0]) < 97) // check if the first letter is capitalized
							word[0] = static_cast<char>(static_cast<int>(word[0]) + 32);
						if (para[i] == ')')
							word.erase(0, 1);
						if (!stopWords.search(word))	//does this word exist in stopWords? Yes = skip, No = insert
						{
							list<Occurence>* temp = search(word);
							if (temp != nullptr) //does it already exist in the BST?
							{
								temp->push_back(occ);
							}
							else
							{
								insert(word, occ);
							}
						}
					}

					word = "";
					if (para[i] == '.')
					{
						occ.setLine(++currLn);
					}
				}
				else
				{
					word.push_back(para[i]);
				}
			}

		}
	}
	sort();

}
void specialBST::populateInputBST(specialBST::TNode* inputWords, int size)
{
	int high = size - 1;
	int low = 0;

	//BST<string, string> stopWordsBST;
	populateInputBST(inputWords, low, high);

	//return stopWordsBST;
}

void specialBST::populateInputBST(specialBST::TNode* inputWords, int low, int high)
{
	if (low <= high)
	{
		int mid = low + (high - low) / 2;
		this->insert(inputWords[mid]);

		populateInputBST(inputWords, low, mid - 1);
		populateInputBST(inputWords, mid + 1, high);
	}


}

specialBST::TNode* specialBST::copyToArray()
{
	int size = length();
	TNode* temp = new TNode[size];
	int index = 0;
	copyToArray(this->root, temp, index, size);

	return temp;
}
void specialBST::copyToArray(TNode* root, TNode*& temp, int& index, int size)
{
	if (!root)
	{

	}
	else
	{
		copyToArray(root->leftChild, temp, index, size);

		temp[index].key = root->key;
		temp[index].value = root->value;
		index++;

		copyToArray(root->rightChild, temp, index, size);
	}
}
specialBST::TNode* specialBST::mergeSortedArray(TNode* arr1, TNode* arr2, int size1, int size2)
{
	int i = 0, j = 0, k = 0;
	TNode* mergedArr = new TNode[size1 + size2];
	// Traverse both array
	while (i < size1 && j < size2)
	{
		// Check if current element of first
		// array is smaller than current element
		// of second array. If yes, store first
		// array element and increment first array
		// index. Otherwise do same with second array
		if (arr1[i].key < arr2[j].key)
			mergedArr[k++] = arr1[i++];
		else
			mergedArr[k++] = arr2[j++];
	}

	// Store remaining elements of first array
	while (i < size1)
		mergedArr[k++] = arr1[i++];

	// Store remaining elements of second array
	while (j < size2)
		mergedArr[k++] = arr2[j++];

	return mergedArr;
}

void specialBST::sort()
{
	int size = length();
	TNode* wordsArray = copyToArray();

	for (int i = 0; i < size - 1; i++)// we will sort the array
	{

		// Last i elements are already in place   
		for (int j = 0; j < size - i - 1; j++)
		{
			if (wordsArray[j].key > wordsArray[j + 1].key)
			{
				wordsArray[j].key = wordsArray[j + 1].key;
				wordsArray[j].value = wordsArray[j + 1].value;
			}
		}
	}
	populateInputBST(wordsArray, size);
}
specialBST specialBST::mergeBSTs(specialBST lhs, specialBST rhs)
{
	TNode* arrayBST1 = lhs.copyToArray();
	TNode* arrayBST2 = rhs.copyToArray();
	int size1 = lhs.length();
	int size2 = rhs.length();

	TNode* mergedArr = mergeSortedArray(arrayBST1, arrayBST2, size1, size2);

	specialBST mergedBst;
	mergedBst.populateInputBST(mergedArr, size1 + size2);

	return mergedBst;
}
void specialBST::intersection(specialBST::TNode* arr1, specialBST::TNode* arr2, int size1, int size2)
{
	int i = 0, j = 0;
	while (i < size1 && j < size2) {
		if (i > 0 && arr1[i].key == arr1[i - 1].key) {        //For Handling duplicates
			i++;
			continue;
		}
		if (arr1[i].key < arr2[j].key)
			i++;
		else if (arr2[j].key < arr1[i].key)
			j++;
		else /* if arr1[i] == arr2[j] */
		{
			cout << arr2[j].key << "\t|\t";
			i++;
			j++;
		}
	}
}
void specialBST::findCommonWords(specialBST lhs)
{
	TNode* arrayBST1 = lhs.copyToArray();
	TNode* arrayBST2 = this->copyToArray();
	int size1 = lhs.length();
	int size2 = this->length();

	cout << "Common Words:\n\n";
	intersection(arrayBST1, arrayBST2, size1, size2);
}

void specialBST::deleteNode(string k)
{
	bool flag = 0;
	TNode* temp = deleteNode(this->root, k, flag);
	if (!flag)
		cout << "\"" << k << "\" does NOT exist in Index\n";
	else
		cout << "\"" << k << "\" successfully deleted in Index\n";
}
specialBST::TNode* specialBST::deleteNode(TNode* root, string k, bool& flag)
{
	if (root == NULL)
		return root;

	if (root->key > k) {
		root->leftChild = deleteNode(root->leftChild, k, flag);
		return root;
	}
	else if (root->key < k) {
		root->rightChild = deleteNode(root->rightChild, k, flag);
		return root;
	}

	// Deletion of node

	if (root->leftChild == NULL) {
		TNode* temp = root->rightChild;
		list<Occurence>::iterator itStart = root->value.begin();
		list<Occurence>::iterator itEnd = root->value.end();
		root->value.erase(itStart, itEnd);
		delete root;
		flag = 1;
		return temp;
	}
	else if (root->rightChild == NULL) {
		TNode* temp = root->leftChild;
		list<Occurence>::iterator itStart = root->value.begin();
		list<Occurence>::iterator itEnd = root->value.end();
		root->value.erase(itStart, itEnd);
		delete root;
		flag = 1;
		return temp;
	}

	else {

		TNode* succParent = root;

		TNode* succ = root->rightChild;
		while (succ->leftChild != NULL) {
			succParent = succ;
			succ = succ->leftChild;
		}

		if (succParent != root)
			succParent->leftChild = succ->rightChild;
		else
			succParent->rightChild = succ->rightChild;

		// Copy Successor Data to root
		root->key = succ->key;
		root->value = succ->value;

		// Delete Successor and return root
		list<Occurence>::iterator itStart = succ->value.begin();
		list<Occurence>::iterator itEnd = succ->value.end();
		succ->value.erase(itStart, itEnd);
		delete succ;
		flag = 1;
		return root;
	}
}

void specialBST::writeOnFile(ofstream& fout)
{
	writeOnFile(fout, this->root);
}
void specialBST::writeOnFile(ofstream& fout, TNode* root)
{
	if (!root)
	{

	}
	else
	{
		writeOnFile(fout, root->leftChild);

		fout << "Key: " << root->key << "\n";
		fout << "References:\n";
		for (list<Occurence>::iterator it = root->value.begin(); it != root->value.end(); it++)
			fout << *it << "\n";
		writeOnFile(fout, root->rightChild);
	}
}


//-----------------------------------------------------------
//-----------------------------------------------------------
//----------------------Template BST Methods-----------------
template <typename k, typename v>
BST<k, v>::BST()
{
	root = nullptr;
}
template <typename k, typename v>
BST<k, v>::BST(k key, v value)
{
	this->root->key = key;
	this->root->value = value;
	this->root->leftChild = this->root->rightChild = nullptr;
}
template <typename k, typename v>
BST<k, v>::BST(BST& rhs)
{
	deepCopy(root, rhs.root);
}
template <typename k, typename v>
void BST<k, v>::deepCopy(TNode*& lhs, TNode*& rhs)
{
	if (rhs)
	{
		lhs = new TNode(rhs->key, rhs->value);
		deepCopy(lhs->leftChild, rhs->leftChild);
		deepCopy(lhs->rightChild, rhs->rightChild);
	}
}
//{
//	//root = rhs.root;
//	//root->leftChild = rhs.root->leftChild;
//	//root->rightChild = rhs.root->rightChild;
//
//
//	//TreeNode* temp = new TreeNode;
//	//if (root != nullptr)
//	//{
//	//	DeepCopy->key = rhs.root->key;
//	//	DeepCopy->value = rhs.root->value;
//	//	DeepCopy->leftChild = rhs.root->leftChild;
//	//	DeepCopy->rightChid = rhs.root->rightChild;
//	//}
//
//	if (rhs)
//	{
//		TreeNode* lhs = new TreeNode();
//		lhs->key = rhs.root->key;
//		lhs->left = deep_copy(rhs->left);
//		lhs->right = deep_copy(rhs->right);
//		return lhs;
//	}
//	else
//	{
//		return nullptr;
//	}
//
//}
template <typename k, typename v>
void BST<k, v>::insert(k key, v value)
{
	insert(this->root, key, value);
}
template <typename k, typename v>
void BST<k, v>::insert(TNode*& root, k key, v value)
{
	if (!root)
	{
		root = new TNode(key, value);
		/*root->key = key;
		root->value = value;*/
		root->rightChild = root->leftChild = nullptr;

	}

	if (key > root->key)
	{
		insert(root->rightChild, key, value);
	}
	else if (key < root->key)
	{
		insert(root->leftChild, key, value);
	}
}
template<typename k, typename v>
void BST<k ,v>::insert(BST::TNode const temp)
{
	insert(this->root, temp);
}
template<typename k, typename v>
void BST<k, v>::insert(BST::TNode*& root, TNode const temp)
{
	if (!root)
	{
		root = new TNode(temp);
		/*root->key = key;
		root->value = value;*/
		root->rightChild = root->leftChild = nullptr;

	}

	if (temp.key > root->key)
	{
		insert(root->rightChild, temp);
	}
	else if (temp.key < root->key)
	{
		insert(root->leftChild, temp);
	}
}
template <typename k, typename v>
bool BST<k, v>::search(k const key)
{
	return search(this->root, key);
}
template <typename k, typename v>
bool BST<k, v>::search(TNode*& root, k const key)
{
	if (root == nullptr)
	{
		return 0;
	}
	else if (key == root->key)
	{
		return 1;
	}
	else
	{
		if (key > root->key)
		{
			return search(root->rightChild, key);
		}
		else if (key < root->key)
		{
			return search(root->leftChild, key);
		}
	}
}
template <typename k, typename v>
void BST<k, v>::inorderPrintKeys() const
{
	inorderPrintKeys(this->root);
}
template <typename k, typename v>
void BST<k, v>::inorderPrintKeys(TNode* rhs) const
{
	if (!rhs)
	{

	}
	else
	{
		inorderPrintKeys(rhs->leftChild);
		cout << rhs->key << endl;
		inorderPrintKeys(rhs->rightChild);
	}
}
template <typename k, typename v>
void BST<k, v>::inorderPrintValues() const
{
	cout << "Key\t\tValues\n";
	inorderPrintValues(this->root);
}
template <typename k, typename v>
void BST<k, v>::inorderPrintValues(TNode* rhs) const
{
	if (!rhs)
	{

	}
	else
	{
		inorderPrintValues(rhs->leftChild);
		cout << rhs->key << "\t\t" << rhs->value << endl;
		inorderPrintValues(rhs->rightChild);
	}
}

template<typename k, typename v>
int BST<k, v>::length() const
{
	return length(this->root, -1);
}
template <typename k, typename v>
int BST<k, v>::length(TNode* root, int count) const
{
	if (!root)
	{
		return 0;
	}
	else
	{
		count = 1 + length(root->leftChild, count) + length(root->rightChild, count);
		return count;
	}

}
