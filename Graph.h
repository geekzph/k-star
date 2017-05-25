//
// Created by zph on 2017/5/21.
//

#ifndef K_STAR_GRAPH_H
#define K_STAR_GRAPH_H


#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;

// This class represents a undirected graph using adjacency


struct EdgeNode
{//领接表节点
    int node;     //节点
    int t;        //时间
    bool operator < (const EdgeNode &a) const
    {
        //由小到大排列
        return node < a.node;
    }
};

struct KNode
{
    int u;
    int v;
    int t;
};

//extern int tau;
//extern int theta;
//extern vector<EdgeNode> *adjlist;
class Graph
{

    int k;
    int tau;
    int theta;

    int v;    // No. of vertices

    vector<KNode> k_node;
    int node_size;
    vector<int> node_degree;
    map<tuple<int,int>, set<int>> fre_edge;
public:
    vector<EdgeNode> *adjlist;
    Graph(int v, int k, int tau, int theta);  // Constructor
    // A recursive function to print DFS starting from v
    void DFSUtil(int, vector<bool> &, int k);
    int buildData(string fileName);
    void bianli();
    void printCore();
    int nodeSize();
    //bool NodeFrequent(int v);
    bool nodeFrequent(int v);
    map<int, set<int>> strongNeighbour();
    set<int> computeNeighbour();
    int fre2Item(map<tuple<int, int>, set<int>> item);
    set<int> itemIntersect(set<int> set1, set<int> set2);
    void greedyAlg();
    void maxKStar(map<tuple<int, int>, set<int>> );
    int combinationNums(int n, int r);
    int delNode(int m);
    int delEdge(map<int, set<int>> &del_set);
    void delSetNode(set<int> &m, int n);
    void verifyFre();
    int queueUniqueSize(queue<int>);
    void delNodeSet(set<int> &m);
    map<tuple<int, int>, set<int>> adjlistToFreItem();
    void cutEdge();

    bool HasSuperSet(map<tuple<int,int>, set<int>> item_set, map<tuple<int,int>, set<int>> max_fre_item);
};


#endif //K_STAR_GRAPH_H
