/** 
*  @file    decoding.cpp
*  @author  Alejandro Palomo Alepuz
*  @date    13/07/2018  
*  @version 1.0 
*  
*  @brief Application to decode Huffman encoding version of an input file.
*
*  @section DESCRIPTION
*  
*  An application which takes an input file  encoded with Huffman 
*  and produces an output file which contains the original information.
*
*/

#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <bitset>

using namespace std;

//map to store de code of each character
map<char, string> coding;
//map to store the frequency of each character
map<char, int> freq;

//Huffman tree node
struct Node
{
    char data;
    int freq;
    Node *left;
    Node *right;
 
    Node(char data, int freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

//compare the frequency between nodes
struct CompareNodes
{
    bool operator()(Node* l, Node* r)
    {
        return (l->freq > r->freq);
    }
};

//priority queue to store the nodes in order of frequencies
priority_queue<Node*, vector<Node*>, CompareNodes> HeapNodes;

void storeCodes(struct Node* root, string str)
{
    if (root==NULL)
        return;
    if (root->data != '#')
        coding[root->data]=str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

void HuffmanCodes()
{
    struct Node *left, *right, *top;
    for (map<char, int>::iterator v=freq.begin(); v!=freq.end(); v++)
    {
        HeapNodes.push(new Node(v->first, v->second));
    }

    while (HeapNodes.size() != 1)
    {
        left = HeapNodes.top();
        HeapNodes.pop();
        right = HeapNodes.top();
        HeapNodes.pop();
        top = new Node('#', left->freq + right->freq);
        top->left = left;
        top->right = right;
        HeapNodes.push(top);
    }
    storeCodes(HeapNodes.top(), "");
}

string decode_file(struct Node* root, string s)
{
    string ans = "";
    struct Node* curr = root;
    for (int i=0;i<s.size();i++)
    {
        if (s[i] == '0')
           curr = curr->left;
        else
           curr = curr->right;
 
        // Leaf node
        if (curr->left==NULL and curr->right==NULL)
        {
            ans += curr->data;
            curr = root;
        }
    }
    return ans+'\0';
}

int main()
{
    char name, number;
    int value;
    char * binary = (char*) malloc (2000);
    string encodedString, decodedString;

    //read and store the huffman encoded information from huffman.txt file
 	FILE *f = fopen("huffman.txt","r");
 	
 	if (f == NULL)
    {
        cout << "Error: huffman file does not exist. Closing" << endl;
		return -1;
    }
 	else
    {
 	    while (feof(f) == 0)
 	    {
 		    fgets(binary,1024,f);
            encodedString = binary;
            cout << "Huffman encoded imput data:" << endl << encodedString << endl << endl;
 	    }
    }
    fclose(f);

    //read the dictionary file with symbols and frequencies
    ifstream myfile("dictionary.txt", ifstream::in);
	if (!myfile)
	{
		cout << "File dictionary.txt does not exist" << endl;
		return -1;
	}
	if (myfile.peek() == std::ifstream::traits_type::eof())
	{
    		cout << "File dictionary.txt is empty" << endl;
		return -1;
	}

    while(myfile >> name >> value){
        freq.insert(std::pair<char, unsigned int>(name, value));
    }
    
    cout << "Characters and their codes:" << endl;
    for (auto v=freq.begin(); v!=freq.end(); v++)
        cout << v->first <<' ' << v->second << endl;

    //generate the huffman tree
    HuffmanCodes();

    //decoding using the huffman coded string and the huffman tree
    decodedString = decode_file(HeapNodes.top(), encodedString);
    cout << endl <<"Decoded Huffman Data:" << endl << decodedString << endl;

    //conversion from ASCII to binary an creation of the output file
    ofstream f2;
    f2.open ("output.txt");
    cout << "Original information in binary:" << endl;
    for (int i=0;i<decodedString.size()-1;i++)
    {
        bitset<8> decodedbits;
        decodedbits = bitset<8>(decodedString.c_str()[i]);
        cout << decodedbits;
        f2 << decodedbits;
    }
    cout << endl;
    f2.close();
    free(binary);
    return 0;
}