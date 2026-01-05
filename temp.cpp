
#include "prototypes.h"

//////////////////////////////////////////      build_prim_tree       //////////////////////////////////////////

// Creating prim tree according to the given algorithm
// Returns array where prim[i] is the parent of vertex i in MST
int* build_prim_tree(EdgeList net[ ], int n) {

    // Create arrays
    int* min = (int*)malloc(sizeof(int) * n);
    checkMemoryAllocation(min);

    int* prim = (int*)malloc(sizeof(int) * n);
    checkMemoryAllocation(prim);

    CandidateNode** location = (CandidateNode**)malloc(sizeof(CandidateNode*) * n);
    checkMemoryAllocation(location);

    // Calculate size needed for bit array (8 bits per byte)
    int k;
    if (n % 8 == 0) k = n / 8;
    else k = (n / 8) + 1;
    unsigned char* inT = (unsigned char*)malloc(sizeof(unsigned char) * k);
    checkMemoryAllocation(inT);

    // Initialize all data structures
    resetStructs(min, prim, inT, n, k);

    // Create a priority queue with all vertices
    CandidateList* priority = createPriority(n, min, location);

    // Set a root to Prim
    min[0] = 0;
    prim[0] = -1;
    DecreaseKey(location[0], min[0]);

    while (priority->head->next != NULL) {

        // Get the vertex with minimum cost that's not yet in Prim
        int u = DeleteMin(priority);

        // Check if min[u] is defined
        // If it's not defined, it means that tree the graph is broken
        if (min[u] == INT_MAX) printf("No spanning tree available."), exit(1);

        // Mark vertex u as added to Prim
        bitWiseTurnToTrue(inT, u);

        EdgeNode* edge = net[u].head->next;

        // Iterate through all edges from vertex u
        for (int i = 0; edge != NULL; i++) {
            int v = edge->e.neighbor;
            int costUV = edge->e.cost;

            if (!bitWiseCheck(inT,v) && costUV < min[v]) {
                min[v] = costUV;
                prim[v] = u;
                DecreaseKey(location[v], costUV);
            }
            edge = edge->next;  // move to next edge
        }
    }

    // Free arrays
    free(min);
    free(inT);
    free(location);
    free(priority);

    return prim;
}

//////////////////////////////////////////       createPriority       //////////////////////////////////////////

// Creates priority queue with all vertices as candidates
CandidateList* createPriority(int n, int* min, CandidateNode** location) {

    CandidateList* priority = createNewList(n);

    for (int v = 0; v < n; v++) {
        CandidateNode* candidate = createCandidate(v, min);
        addToCandidateListEnd(priority, candidate);
        location[v] = candidate;
    }

   return priority;

}

//////////////////////////////////////////       resetStructs        //////////////////////////////////////////

// Initializes all data structures
// Sets minimum distances to the maximum int value possible, parents to -1, and bitset to false
void resetStructs(int* min, int* prim, unsigned char* inT, int n, int k) {

    for (int i = 0; i < n; i++) {
        min[i] = INT_MAX;
        prim[i] = -1;
    }

    for (int i = 0; i < k; i++) {
        inT[i] = 0;
    }
}

//////////////////////////////////////////       createCandidate      //////////////////////////////////////////

// Creates a new candidate node for the priority queue
CandidateNode* createCandidate(int v, int* min) {

    CandidateNode* newNode = (CandidateNode*)malloc(sizeof(CandidateNode));
    checkMemoryAllocation(newNode);

    newNode->c.computer = v;
    newNode->c.min = min[v];

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

//////////////////////////////////////////        createNewList       //////////////////////////////////////////

// Creates empty doubly-linked priority queue with dummy head
CandidateList* createNewList(int n) {

    CandidateList* newList = (CandidateList*)malloc(sizeof(CandidateList));
    checkMemoryAllocation(newList);

    CandidateNode* dummyHead = (CandidateNode*)malloc(sizeof(CandidateNode));
    checkMemoryAllocation(dummyHead);
    dummyHead->next = NULL;
    dummyHead->prev = NULL;

    newList->head = dummyHead;
    newList->tail = NULL;

    return newList;
}

//////////////////////////////////////////   addToCandidateListEnd    //////////////////////////////////////////

// Adds candidate node to end of priority queue
void addToCandidateListEnd(CandidateList* priority, CandidateNode* node) {

    if (priority->tail == NULL) {
        priority->tail = node;
        priority->head->next = node;
        node->prev = priority->head;
    }

    else {
        priority->tail->next = node;
        node->prev = priority->tail;
        priority->tail = node;
    }

}

//////////////////////////////////////////     bitWiseTurnToTrue      //////////////////////////////////////////

// Sets v's bit to true using bitwise operations
void bitWiseTurnToTrue(unsigned char* inT, int v) {

    unsigned char mask = 1 << (v % 8);
    inT[v/8] |= mask;

}

//////////////////////////////////////////         DeleteMin          //////////////////////////////////////////

// Finds and removes vertex with minimum distance from priority queue
// Returns the computer ID of the minimum candidate
int DeleteMin(CandidateList* priority) {

    int minVal = INT_MAX;
    CandidateNode* curr = priority->head->next;
    CandidateNode* minNode = priority->head->next;

    while (curr != NULL) {
        if (curr->c.min < minVal) {
            minVal = curr->c.min;
            minNode = curr;
        }
        curr = curr->next;
    }

    int computer = minNode->c.computer;
    removeFromPriority(priority, minNode);

    return computer;
}

//////////////////////////////////////////     removeFromPriority     //////////////////////////////////////////

// Removes specified node from doubly-linked priority queue
void removeFromPriority(CandidateList* priority, CandidateNode* toRemove) {

    CandidateNode* prev = toRemove->prev;
    CandidateNode* next = toRemove->next;

    prev->next = next;

    if (next != NULL)   next->prev = prev;
    else                priority->tail = prev;

    free(toRemove);
}

//////////////////////////////////////////        DecreaseKey        //////////////////////////////////////////

// Updates minimum distance for a candidate node in the priority queue
void DecreaseKey(CandidateNode* node, int newMin) {

    node->c.min = newMin;

}

//////////////////////////////////////////       bitWiseCheck        //////////////////////////////////////////

// Checks if v is already in the MST using bitwise operations
// Returns non-zero if vertex is in MST, zero otherwise
int bitWiseCheck(unsigned char* inT, int v) {

    unsigned char mask = 1 << (v % 8);
    return (inT[v/8] & mask) != 0;

}