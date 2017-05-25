//
// Created by zph on 2017/5/21.
//

#ifndef K_STAR_GENMAX_H
#define K_STAR_GENMAX_H


#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include <sstream>
#include "Graph.h"

using namespace std;

struct comparator
{
    template<typename T, typename U>
    bool operator()(T const& lhs, U const& rhs) const {
        return lhs.mCommonField < rhs.mCommonField;
    }
};

class GenMax
{
private:

    //int text_data_count;
    int mk;
    int min_sup;
    int mtheta;
    vector<EdgeNode> *my_adjlist;
    map<tuple<int,int>, set<int>> text_data;   //所有事务
    map<tuple<int,int>, set<int>> single_text_data;   // 单个结点事务
    map<tuple<int,int>, set<int>> max_fre_item;   //最大频繁项集

    //map<tuple<int,int>, set<int>> possible_set;
    //map<tuple<int,int>, set<int>> combine_set;

public:
    int all_size;
    set<int> del_set;
    map<tuple<int,int>, set<int>> all_max_fre_item;  //所有最大频繁项集
    map<tuple<int,int>, set<int>> fre1_item;   //频繁1项集
    GenMax(int sk, int stau, int stheta);
    void runGenMax(vector<EdgeNode> *adjlist, int node_size);
    void genData(vector<EdgeNode> *adjlist, int node_size);
    //int buildData(string file_name);
    void computeFre1();
    void maxFreItem(map<tuple<int,int>, set<int>> possible_set, map<tuple<int,int>, set<int>> combine_set, int l);
    map<tuple<int,int>, set<int>> freCombine(map<tuple<int,int>, set<int>> possible_set, map<tuple<int,int>, set<int>> combine_set);
    set<int> itemIntersect(set<int> set1, set<int> set2);
    set<int> tidSet(map<tuple<int,int>, set<int>> item_set);
    bool hasSuperSet(map<tuple<int,int>, set<int>> item_set);
    //bool HasSuperSet2(map<tuple<int,int>, set<int>> item_set, map<tuple<int,int>, set<int>> item);
    map<tuple<int,int>, set<int>> itemUnion(map<tuple<int,int>, set<int>> set1, map<tuple<int,int>, set<int>> set2);
    void printFreItem();
};


#endif //K_STAR_GENMAX_H
