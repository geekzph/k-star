#include "Graph.h"
#include "GenMax.h"


int main() {

//========================test begin=============================================================
//    GenMax gentest(6, 9);
//    map<tuple<int, int>, set<int>> fre_edge;
//    set<int> my_set;
//    my_set.insert(1);
//    my_set.insert(2);
//    my_set.insert(3);
//    pair<tuple<int, int>, set<int>> my_pair = make_pair(make_tuple(111, 22), my_set);
//    fre_edge.insert(my_pair);
//
//    map<tuple<int, int>, set<int>> max_fre_edge;
//    my_set.clear();
//    my_set.insert(1);
//    my_set.insert(2);
//    my_set.insert(3);
//    pair<tuple<int, int>, set<int>> my_pair1 = make_pair(make_tuple(11, 22), my_set);
//    max_fre_edge.insert(my_pair1);
//
//    my_set.clear();
//    my_set.insert(1);
//    my_set.insert(2);
//    my_set.insert(3);
//    pair<tuple<int, int>, set<int>> my_pair2 = make_pair(make_tuple(55, 66), my_set);
//    max_fre_edge.insert(my_pair2);
//
//    if(gentest.HasSuperSet2(fre_edge, max_fre_edge))
//        cout << "dui le" << endl;
//
//    map<tuple<int, int>, set<int>>::iterator iter;
//    tuple<int, int> hh = make_tuple(11, 22);
//    iter = fre_edge.find(hh);
//    fre_edge.erase(iter);
//    if (iter == fre_edge.end())
//        cout << "we do not find 112" << endl;
//    else cout << "we find 112" << endl;
//    vector<int>::iterator vec_iter;
//
//    set<int> test;
//    for (iter = fre_edge.begin(); iter != fre_edge.end(); iter++) {
//        //vec_iter = iter->first.begin();
//        cout << get<0>(iter->first) << "  ";
//
//        test = iter->second;
//        set<int>::iterator set_iter;
//        for (set_iter = iter->second.begin(); set_iter != iter->second.end(); set_iter++) {
//            cout << *set_iter << "  ";
//        }
//        cout << endl;
//    }
//
//    for (auto i : test)
//        cout << i << endl;
////    pair<int, int> my_pair = make_pair(12, 14);
////    set<int> my_set;
////    my_set.insert(12);
////    my_set.insert(13);
////    pair<map<int, int>, set<int>> my_pair2 = make_pair(my_pair, my_set);
////    fre_edge.insert(my_pair2);
//
//    int e, n, m;
//    queue<int> q1;
//    q1.push(1);
//    q1.push(2);
//    q1.push(2);
//    q1.push(3);
//    q1.push(4);
//    q1.push(5);
//    q1.push(5);
////    cout << q1.front() << endl;
////    q1.pop();
////    cout << q1.front() << endl;
////    cout << q1.back() << endl;
//
//    //cout << queueUniqueSize(q1) << endl;
//    while (!q1.empty()) {
//        std::cout << ' ' << q1.front();
//        q1.pop();
//    }
//    std::cout << '\n';

//===========================test end==============================================================

    string file_path;
    std::cout << "please input num: " << std::endl;
    //cin >> m;
    //4种不同的剪枝方法
    int method = 1;
    //方法1：weak core 剪枝
    if(method == 1)
    {

        int k = 4;
        int theta = 3;
        int tau = 9;
        Graph g1(1314051,k, tau, theta);    //点的个数
        //int res = g1.combinationNums(7, 3);
        //g1.fre2Item(fre_item);
        g1.buildData("/Users/zph/ClionProjects/FrequentKCore/out.dblp.txt");
        //g1.bianli();
        cout << "g1.NodeSize():" << g1.nodeSize() << endl;
        //g1.printCore();
        map<int, set<int>> del_set_1 = g1.strongNeighbour();
//        for(map<int, set<int>>::iterator iter = del_set_1.begin(); iter != del_set_1.end(); iter++)
//        {
//            cout << iter->first << " ";
//            for(set<int>::iterator iter1 = iter->second.begin(); iter1 != iter->second.end(); iter1++)
//                cout << *iter1 << "  ";
//            cout << endl;
//        }
        g1.delEdge(del_set_1);
        //g1.printCore();
        //set<int> del_set_2 = g1.computeNeighbour();
        //g1.delNodeSet(del_set_2);
        //g1.verifyFre();
//        cout << del_set.size() << endl;
//        vector<int> del_set_2 = g1.computeNeighbour();
//        for(auto i : del_set_2)
//        {
//            cout << i << endl;
//            g1.delNode(i);
//        }
        //调用GenMax
        GenMax my_genmax(k,tau,theta);
        set<int> my_set;

        int iter_count = 0;
        my_genmax.runGenMax(g1.adjlist, g1.nodeSize());
        my_set = my_genmax.del_set;
        g1.delNodeSet(my_set);
        if(my_set.size() == 0)
            my_genmax.printFreItem();
        while(my_set.size() != 0)
        {
            iter_count++;
            cout << "===========" << iter_count <<"=================="<<  endl;
            GenMax my_genmax(k,tau,theta);
            my_genmax.runGenMax(g1.adjlist, g1.nodeSize());
            my_set.clear();
            my_set = my_genmax.del_set;
            g1.delNodeSet(my_set);
            if(my_set.size() == 0)
                my_genmax.printFreItem();
        }
        cout << "heelo" << endl;

    }
//    //方法2：方法1 + 强邻居
//    if(method == 2)
//    {
//        Graph g1(1314051);    //点的个数
//        int res = g1.combinationNums(7, 3);
//        g1.buildData("/Users/zph/ClionProjects/FrequentKCore/out.dblp.txt");
//        //g1.NodeFrequent(1);
//        g1.bianli();
//        cout << "g1.NodeSize():" << g1.nodeSize() << endl;
//        //调用GenMax
//        //GenMax my_genmax(6, 9);
//        g1.printCore();
//        g1.verifyFre();
//    }
//    //方法3：方法2 + 虚度core
//    if(method == 3)
//    {
//        Graph g1(1314051);    //点的个数
//        int res = g1.combinationNums(7, 3);
//        g1.buildData("/Users/zph/ClionProjects/FrequentKCore/out.dblp.txt");
//        //g1.NodeFrequent(1);
//        g1.bianli();
//        cout << "g1.NodeSize():" << g1.nodeSize() << endl;
//        //g1.strongNeighbour();
//        //调用GenMax
//        g1.printCore();
//        g1.verifyFre();
//    }
//    //方法4：方法3 + 贪心算法
//    if(method == 4)
//    {
//        Graph g1(1314051);    //点的个数
//        int res = g1.combinationNums(7, 3);
//        g1.buildData("/Users/zph/ClionProjects/FrequentKCore/out.dblp.txt");
//        //g1.NodeFrequent(1);
//        g1.bianli();
//        cout << "g1.NodeSize():" << g1.nodeSize() << endl;
//        //g1.strongNeighbour();
//        //调用GenMax
//        g1.printCore();
//        g1.verifyFre();
//    }
    return 0;
}