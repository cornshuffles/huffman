/*
AUTHOR: Vladislav Nikolic
DATE: November 28, 2020
COMPILATION: g++ *.cpp
SYNOPSIS: Once compiled, execute the program using ./a.out - replace "a.out" with whatever the name of the executable you generated when compiling is
ALGORITHM: This program implements the Huffman code generating algorithm
COMPLEXITY: O(nlog(n)) - where n is the number of unique elements
BUGS: None known
REFERENCES: https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/ - This code is a modified version of the code available here
NOTES: The input, compressed, and decompressed file names are specified by macros in compress.cpp
*/

#include "huffman.h"

using namespace std;

/*------------------------------------------------------*
 * Function:    newNode                                 *
 * Params:      string data - string this node will     *
 *                          represent                   *
 *              uint freq - frequency of the character  *
 *                                                      *
 * Returns:     MinHeapNode* - Pointer to created node  *
 *                                                      *
 * Description: returns a pointer to a newly allocated  *
 *              node specified by the params            *
 * -----------------------------------------------------*/
struct MinHeapNode* newNode(string data, uint freq){
    struct MinHeapNode* node = new MinHeapNode();
    node->left = node->right = NULL;
    node->data = data;
    node->freq = freq;
    return node;
}

/*------------------------------------------------------*
 * Function:    createMinHeap                           *
 * Params:      uint capacity - capacity of the heap    *
 *                                                      *
 * Returns:     MinHeap* - Pointer to created heap      *
 *                                                      *
 * Description: returns a pointer to a newly allocated  *
 *              heap specified by the param             *
 * -----------------------------------------------------*/
struct MinHeap* createMinHeap(uint capacity){
    struct MinHeap* minHeap = new MinHeap();
    minHeap->size = 0; // Size is intially 0
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

/*------------------------------------------------------*
 * Function:    swapMinHeapNode                         *
 * Params:      MinHeapNode** a - pointer to a pointer  *
 *                                to a node to swap     *
 *              MinHeapNode** b - pointer to a pointer  *
 *                                to a node to swap     *
 * Returns:     void                                    *
 *                                                      *
 * Description: swaps the two specified nodes in the    *
 *              heap                                    *
 * -----------------------------------------------------*/
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){ 
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

/*------------------------------------------------------*
 * Function:    minHeapify                              *
 * Params:      MinHeap* minHeap - pointer to the       *
 *                                 minHeap to heapify   *
 *              int idx - index of the node to start on *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: Organizes the heap into a minHeap       *
 * -----------------------------------------------------*/
void minHeapify(struct MinHeap* minHeap, int idx){
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if(left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq){
        smallest = left;
    }

    if(right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq){
        smallest = right;
    }

    if(smallest != idx){
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

/*------------------------------------------------------*
 * Function:    isSizeOne                               *
 * Params:      MinHeap* minHeap - pointer to the       *
 *                                 minHeap to check     *
 *                                                      *
 * Returns:     bool - true if size is 1                *
 *                                                      *
 * Description: returns true if the size on the given   *
 *              heap is 1                               *
 * -----------------------------------------------------*/
bool isSizeOne(struct MinHeap* minHeap){ 
    return (minHeap->size == 1); 
}

/*------------------------------------------------------*
 * Function:    extractMin                              *
 * Params:      MinHeap* minHeap - pointer to the       *
 *                                 minHeap to use       *
 *                                                      *
 * Returns:     MinHeapNode* - pointer to the min node  *
 *                                                      *
 * Description: returns a pointer to the minimum node   *
 *              and removes that node from the heap     *
 * -----------------------------------------------------*/
struct MinHeapNode* extractMin(struct MinHeap* minHeap){
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return temp;
}

/*------------------------------------------------------*
 * Function:    insertMinHeap                           *
 * Params:      MinHeap* minHeap - pointer to the       *
 *                                 minHeap to use       *
 *              MinHeapNode* minHeapNode - pointer to   *
 *                                 the node to insert   *
 * Returns:     void                                    *
 *                                                      *
 * Description: inserts a node into the proper location *
 *              in a min heap                           *
 * -----------------------------------------------------*/
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){
    minHeap->size++;
    int i = minHeap->size - 1;
    while(i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq){
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

/*------------------------------------------------------*
 * Function:    buildMinHeap                            *
 * Params:      MinHeap* minHeap - pointer to the       *
 *                                 minHeap to use       *
 * Returns:     void                                    *
 *                                                      *
 * Description: calls minHeapify on each node to create *
 *              the min heap                            *
 * -----------------------------------------------------*/
void buildMinHeap(struct MinHeap* minHeap){
    int n = minHeap->size - 1;
    int i;
    for(i = (n - 1) / 2; i >= 0; --i){
        minHeapify(minHeap, i);
    }
}

/*------------------------------------------------------*
 * Function:    printArr                                *
 * Params:      int arr[] - array containing the code   *
 *                                                      *
 *              int n - length of the array             *
 *                                                      *
 * Returns:     string - the code that was passed in    *
 *                                                      *
 * Description: returns a string version of the passed  *
 *              in code                                 *
 * -----------------------------------------------------*/
string printArr(int arr[], int n){
    int i;
    string code;
    for(i = 0; i < n; i++){
        code += to_string(arr[i]);
    }
    return code;
}

/*------------------------------------------------------*
 * Function:    isLeaf                                  *
 * Params:      MinHeapNode* root - node to check       *
 *                                                      *
 * Returns:     bool - true if the node was a leaf      *
 *                                                      *
 * Description: returns a bool representing whether the *
 *              passed in node is a leaf                *
 * -----------------------------------------------------*/
int isLeaf(struct MinHeapNode* root){
    return !(root->left) && !(root->right);
}

/*------------------------------------------------------*
 * Function:    createAndBuildMinHeap                   *
 * Params:      string data[] - strings to insert       *
 *                                                      *
 *              int freq[] - frequencies of the chars   *
 *                                                      *
 *              int size - number of characters in data *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: Creates a min heap with specified data  *
 * -----------------------------------------------------*/
struct MinHeap* createAndBuildMinHeap(string data[], int freq[], int size){
    struct MinHeap* minHeap = createMinHeap(size);
    for(int i = 0; i < size; ++i){
        minHeap->array[i] = newNode(data[i], freq[i]);
    }

    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

/*------------------------------------------------------*
 * Function:    buildHuffmanTree                        *
 * Params:      string data[] - strings to insert       *
 *                                                      *
 *              int freq[] - frequencies of the chars   *
 *                                                      *
 *              int size - number of characters in data *
 *                                                      *
 * Returns:     void                                    *
 *                                                      *
 * Description: builds the Huffman tree with the passed *
 *              in data                                 *
 * -----------------------------------------------------*/
struct MinHeapNode* buildHuffmanTree(string data[], int freq[], int size){
    struct MinHeapNode *left, *right, *top;

    // Create min heap
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    // Iterate while size of heap doesn't become 1
    while(!isSizeOne(minHeap)){

        // Extract the 2 min size items from the heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        // Create a new internal
        // node with frequency equal to the
        // sum of the two nodes frequencies.
        // Make the two extracted node as
        // left and right children of this new node.
        // Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode("$", left->freq + right->freq);

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    // The remaining node is the
    // root node and the tree is complete.
    return extractMin(minHeap);
}
  
/*------------------------------------------------------*
 * Function:    getCodes                                *
 * Params:      MinHeapNode* root - the node to start at*
 *                                                      *
 *              int arr[] - array to generate codes in  *
 *                                                      *
 *              int top -index of the top of arr[] - 0  *
 *                                                      *
 * Returns:     map<string,string>* - map with elements *
 *                                    and codes         *
 *                                                      *
 * Description: generates Huffman codes for given tree  *
 * -----------------------------------------------------*/
map<string,string>* getCodes(struct MinHeapNode* root, int arr[], int top){ 

    static map<string,string>* stringCode = new map<string,string>;

    // Assign 0 to left edge and recur
    if(root->left){
        arr[top] = 0;
        getCodes(root->left, arr, top + 1);
    }

    // Assign 1 to right edge and recur
    if (root->right){
        arr[top] = 1;
        getCodes(root->right, arr, top + 1);
    }

    // If this is a leaf node, then
    // it contains one of the input
    // characters, print the character
    // and its code from arr[]
    if(isLeaf(root)){
        stringCode->insert(pair<string, string>(root->data, printArr(arr, top)));
    }

    return stringCode;
}

/*------------------------------------------------------*
 * Function:    HuffmanCodes                            *
 * Params:      string data[] - strings to insert       *
 *                                                      *
 *              int freq[] - frequencies of the chars   *
 *                                                      *
 *              int size - number of characters in data *
 *                                                      *
 * Returns:     map<char,string>* - map of chars and    *
 *                                  their codes         *
 *                                                      *
 * Description: returns a map of chars and their codes  *
 * -----------------------------------------------------*/
map<string,string>* HuffmanCodes(string data[], int freq[], int size){
    // Construct Huffman Tree
    struct MinHeapNode* root = buildHuffmanTree(data, freq, size); 
    int arr[size], top = 0;
    map<string,string>* stringCode = getCodes(root, arr, top);
    return stringCode;
}

/*------------------------------------------------------*
 * Function:    buildHuffmanTreeFromCodes               *
 * Params:      stringCode - strings/codes to insert    *
 *                                                      *
 * Returns:     MinHeapNode* - pointer to head of       *
 *                             created Huffman Tree     *
 *                                  their codes         *
 *                                                      *
 * Description: Builds a huffman tree given elements and*
 *              codes                                   *
 * -----------------------------------------------------*/
MinHeapNode* buildHuffmanTreeFromCodes(vector<pair<string, string>> stringCode){
    MinHeapNode *huffmanTree = new MinHeapNode();
    huffmanTree->left = NULL;
    huffmanTree->right = NULL;
    MinHeapNode *head;
    head = huffmanTree;

    for(auto elementIterator = stringCode.begin(); elementIterator != stringCode.end(); elementIterator++){
        for(int i = 0; i < elementIterator->second.length(); i++){
            if(elementIterator->second[i] == '1'){
                if(head->right == NULL){
                    MinHeapNode *newRightNode = new MinHeapNode();
                    newRightNode->left = NULL;
                    newRightNode->right = NULL;
                    head->right = newRightNode;
                }
                head = head->right;
            }
            else{
                if(head->left == NULL){
                    MinHeapNode *newLeftNode = new MinHeapNode();
                    newLeftNode->left = NULL;
                    newLeftNode->right = NULL;
                    head->left = newLeftNode;
                }
                head = head->left;
            }
        }
        head->data = elementIterator->first;
        head = huffmanTree;
    }

    return huffmanTree;
}