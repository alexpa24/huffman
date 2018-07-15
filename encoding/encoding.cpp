/** 
*  @file    encoding.cpp
*  @author  Alejandro Palomo Alepuz
*  @date    13/07/2018  
*  @version 1.0 
*  
*  @brief Application to create compact version of an input file.
*
*  @section DESCRIPTION
*  
*  An application which takes an input file and produces an output 
*  file which contains a compact representation of the input file,
*  using Huffman Encoding.
*
*/

#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <bitset>
#include <fstream>
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
    if (root->data != '#') //internal node
        coding[root->data]=str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

void HuffmanCodes()
{
    struct Node *left, *right, *top;
    for (map<char, int>::iterator it=freq.begin(); it!=freq.end(); it++)
    {
        HeapNodes.push(new Node(it->first, it->second));
    }
    while (HeapNodes.size() != 1)
    {
        left = HeapNodes.top();
        HeapNodes.pop();
        right = HeapNodes.top();
        HeapNodes.pop();
        //'#' is used to define internal nodes
        top = new Node('#', left->freq + right->freq);
        top->left = left;
        top->right = right;
        HeapNodes.push(top);
    }
    storeCodes(HeapNodes.top(), "");
}

//stores each character and its frequency in the map
void Frequency(string str, int n)
{
    for (int i=0; i<n; i++)
    {
        freq[str[i]]++;
    }
}

int main()
{
    char binary[1024];
    string encoding;

    //read and store the original binary information from input.txt file
 	FILE *f = fopen("input.txt","r");
 	
 	if (f == NULL)
    {
        cout << "Error: input file does not exist. Closing" << endl;
		return -1;
    }
 	else
    {
 	    while (feof(f) == 0)
 	    {
 		    fgets(binary,1024,f);
            cout << "Raw imput data:" << endl << binary << endl << endl;
 	    }
    }
    fclose(f);

    //conversion from the binary raw data to ASCII
    stringstream sstream(binary);
    string str;
    while(sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        str += c;
    }

    cout << "The input information in ASCII is:" << endl;
    cout << str << endl << endl;

    //perform the frequency analysis to the ASCII data
    Frequency(str, str.length()-1);
    ofstream f3;
    f3.open ("dictionary.txt");
    cout << "Frequencies:" << endl;
    for (auto v=freq.begin(); v!=freq.end(); v++){
        cout << v->first <<' ' << v->second << endl;
        f3 << v->first <<' ' << v->second << endl;
    }
    f3.close();

    //compact the information using Huffman encoding
    HuffmanCodes();
    cout << endl << "Characters and their codes:" << endl;
    for (auto v=coding.begin(); v!=coding.end(); v++)
        cout << v->first <<' ' << v->second << endl;

    //generate the huffman encoded version of the input information
    //using the coded version of the symbols
    for (auto i: str)
        encoding+=coding[i];
 
    cout << endl << "Huffman encoded data:" << endl << encoding << endl;

    //save the result of the huffman coding in a file
    FILE *f2 = fopen("huffman.txt", "w");
    fprintf(f2, "%s", encoding.c_str());
    fclose(f2);
    
    return 0;
}