#ifndef EFP_BISIMULATION_DEF_H
#define EFP_BISIMULATION_DEF_H

typedef int BIS_indexType;

//adjacency list

struct adjList
{
    BIS_indexType node;
    struct counter *countxS; //pointer to the count(x,S) of Paige&Tarjan
    struct adjList *next;
};

//adjacency list of G_1

struct adjList_1
{
    BIS_indexType node;
    struct adjList *adj; //pointer to the corresponding edge in G
    struct adjList_1 *next;
};

//counters for Paige and Tarjan

struct counter
{
    BIS_indexType value;
    BIS_indexType node;
};

//graph: array of nodes

struct graph
{
    int label; //label of the node
    BIS_indexType rank; //rank of the node
    bool WFflag; //identifies if the node is WF(0 NWF,1 WF)
    BIS_indexType nextInBlock; //pointer to the next node in the same block
    BIS_indexType prevInBlock; //pointer to the previous node in the same block
    BIS_indexType block; //pointer to the block
    struct counter *countxB; //pointer to count(x,B)
    struct adjList *adj; //pointer to the adjacency list
    struct adjList_1 *adj_1; //pointer to the adjacency list of G-1
};

//information related to Q-Blocks

struct qPartition
{
    BIS_indexType size; //number of nodes in the block
    //normalisation of the rank values: normalisation array
    BIS_indexType nextBlock; //pointer to the next Q-Block
    //normalisation of the rank values: copy of the forefather's rank
    BIS_indexType prevBlock; //pointer to the prvious Q-Block
    //during DFS-visit: color of the nodes
    BIS_indexType superBlock; //pointer to the X-Block containing the current Q-Block
    //during DFS: forefather
    BIS_indexType firstNode; //pointer to the first node in the block
    //during the first DFS visit in SCC(): finishing time
};

//information related to X-Blocks

struct xPartition
{
    BIS_indexType nextXBlock; //pointer to the next X-Block
    BIS_indexType prevXBlock; //pointer to the previous X-Block
    BIS_indexType firstBlock; //pointer to the first Block in X
};

/****************Automa related******************/
//DATA STRUCTURES BIS_USED FOR THE INPUT AND THE OUTPUT FROM FC2 FILES
typedef struct e_elem_struct e_elem;
typedef struct v_elem_struct v_elem;
// typedef struct bhtab_struct bhtab;
typedef struct automa_struct automaton;

struct e_elem_struct
{
    int nbh; // Number of lables (behaviors) of a single Edge
    int *bh; // Array of Behaviors
    int tv; // Index (of the array of vertices) of the "To" Vertex
};

struct v_elem_struct
{
    int ne; // Number of Edges
    e_elem *e; // Vettore di edges
};

struct automa_struct
{
    unsigned long Nvertex;
    unsigned long Nbehavs;
    unsigned long Nagents;
    v_elem *Vertex;
};

#endif //EFP_BISIMULATION_DEF_H
