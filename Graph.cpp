//
// Created by zph on 2017/5/21.
//

#include "Graph.h"



bool Graph::HasSuperSet(map<tuple<int,int>, set<int>> item_set, map<tuple<int,int>, set<int>> max_fre_item)
{
    set<tuple<int, int>> set1;
    //遍历item_set
    map<tuple<int,int>, set<int>>::iterator  iter1;
    for(iter1 = item_set.begin(); iter1 != item_set.end(); iter1++)
    {
        set1.insert(iter1->first);
    }

    set<tuple<int, int>> set2;
    map<tuple<int,int>, set<int> >::iterator  iter2;
    for(iter2 = max_fre_item.begin(); iter2 != max_fre_item.end(); iter2++)
    {
        set2.insert(iter2->first);
    }

    set<string> intersect;
    //set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),inserter(intersect, intersect.begin()));
    if(intersect.size() == item_set.size())
        return  true;
    else
        return  false;


}
//打开数据集，构建邻接表
int Graph::buildData(string dataFileName)
{
    ifstream inFile;
    inFile.open(dataFileName);
    if(!inFile){
        cerr << "open " <<dataFileName << " is failed!" << endl;
        return EXIT_FAILURE;
    }
    cout << "Loading Data... " << endl;
    EdgeNode e;
    int u ,v ,t;
    /*读取文本行*/
    string text_line;
    vector<string> lines_of_text;

    while(getline(inFile,text_line))
    {
        //lines_of_text.push_back(textline);

        istringstream line(text_line);
        line >> u >> v >> t;
        e.node = v;
        e.t = t;
        adjlist[u].push_back(e);
        if(u > node_size) node_size = u;
    }
    cout << "Loading Complete!" << endl;
    return EXIT_SUCCESS;
}

Graph::Graph(int v, int mk, int mtau, int mtheta)
{
    this->k = mk;
    this->tau = mtau;
    this->theta = mtheta;
    this->v = v;
    adjlist = new vector<EdgeNode>[v];
    //k_adjlist = new vector<EdgeNode>[V];
    node_size = 0;
}

int Graph::nodeSize()
{
    return node_size;
}
//遍历图，迭代删除不符合要求的点
void Graph::bianli()
{
    vector<int> del_node_list;
    //第一次迭代，找出不符合要求的点
    for(int i = 1; i <= node_size; ++i)
    {
        vector<EdgeNode>::iterator itr;
        if(adjlist[i].size() != 0 && !nodeFrequent(i))
        {
            int m  = i;
            //cout << i << endl;
            del_node_list.push_back(i);
            //i = DelNode(i);  //删除与t.node相邻的节点
            //adjlist[m].clear();

        }
        else
        {
            //++i;
        }

    }
    //开始第二次迭代，直到点集为空
    int recursive_count = 0;
    while(del_node_list.size() != 0)
    {
        recursive_count++;
        cout << "第" << recursive_count << "次迭代" <<endl;
        vector<int>::iterator itr;
        for(itr = del_node_list.begin(); itr != del_node_list.end();++itr)
        {
            cout << "del: " << *itr << endl;
            delNode(*itr);
            adjlist[*itr].clear();
        }
        del_node_list.clear();
        for(int i = 1; i <= node_size; ++i)
        {
            vector<EdgeNode>::iterator itr;
            if(adjlist[i].size() != 0 && !nodeFrequent(i))
            {
                int m  = i;
                //cout << i << endl;
                del_node_list.push_back(i);
                //i = DelNode(i);  //删除与t.node相邻的节点
                //adjlist[m].clear();

            }
            else
            {
                //++i;
            }

        }

    }

    cout << " 迭代完成！" <<endl;
}

//删除v的所有邻居节点
int Graph::delNode(int v)
{
    vector<EdgeNode>::iterator i;
    set<int> node_set;
    int min_value;
    //v的所有邻居放入node_set中
    for (i = adjlist[v].begin(); i != adjlist[v].end(); ++i)
    {
        EdgeNode t = *i;
        node_set.insert(t.node);
    }
    set <int> :: iterator it;
    it = node_set.begin();
    min_value = *it;
    //删除v的其他节点
    for(it = node_set.begin(); it != node_set.end();++it)
    {
        vector<EdgeNode>::iterator itr;
        for(itr = adjlist[(*it)].begin(); itr != adjlist[(*it)].end();)
        {

            EdgeNode t = *itr;
            if(t.node == v)
                itr = adjlist[(*it)].erase(itr);
            else
                ++itr;
        }
    }
    return min_value;

}
void Graph::delSetNode(set<int> &c, int n)
{
    for(auto it = c.begin(); it != c.end(); )
        if(*it == n)
            it = c.erase(it);
        else
            ++it;
}
//队列的大小
int Graph::queueUniqueSize(queue<int> my_queue)
{
    set<int> my_set;
    while (!my_queue.empty())
    {
        my_set.insert(my_queue.front());
        my_queue.pop();
    }
    return my_set.size();
}
bool Graph::nodeFrequent(int v)
{
    vector<EdgeNode>::iterator itr;
    queue<int> time_queue;
    queue<int> node_queue;
    //set<int> node_set;
    itr = adjlist[v].begin();
    EdgeNode node = *itr;
    EdgeNode next_ndoe = *itr;
    int start_time = node.t;
    int my_tau = 0;
    int du = 0;
    //int i = 0;
    for(itr = adjlist[v].begin();itr != adjlist[v].end();itr++)
    {
        //++i;
        node = *itr;
        next_ndoe = *(++itr);
        time_queue.push(node.t);
        node_queue.push(node.node);

        //node_set.insert(node.node);
        if(node.t - start_time == theta - 1 && node.t != next_ndoe.t)
        {
            du = queueUniqueSize(node_queue);
            if(du >= k)
            {
                ++my_tau;
                while(!time_queue.empty() && time_queue.front() == start_time)
                {
                    time_queue.pop();
                    //删除node_set中元素
                    //delSetNode(node_set, node_queue.front());
                    node_queue.pop();
                }
                start_time = time_queue.front();
            }
            else
            {
                while(!time_queue.empty() && time_queue.front() == start_time)
                {
                    time_queue.pop();
                    //删除node_set中元素
                    //delSetNode(node_set, node_queue.front());
                    node_queue.pop();
                }
                start_time = time_queue.front();
            }
        }
        else if(node.t - start_time < theta - 1 && next_ndoe.t - start_time > theta - 1)
        {
            du = queueUniqueSize(node_queue);
            if(du >= k)
            {
                ++my_tau;
                while(!time_queue.empty() && time_queue.front() == start_time)
                {
                    time_queue.pop();
                    //删除node_set中元素
                    //delSetNode(node_set, node_queue.front());
                    node_queue.pop();
                }
                start_time = time_queue.front();
            }
            else
            {
                while(!time_queue.empty() && time_queue.front() == start_time)
                {
                    time_queue.pop();
                    //删除node_set中元素
                    //delSetNode(node_set, node_queue.front());
                    node_queue.pop();
                }
                start_time = time_queue.front();
            }
        }
        --itr;

    }

    if( my_tau >= tau)
        return true;
    else
        return false;
}
//bool Graph::NodeFrequent(int v)
//{
//    vector<EdgeNode>::iterator itr_left;  //theta区间左端
//    vector<EdgeNode>::iterator itr_right;  //theta区间右端
//    itr_left = adjlist[v].begin();
//
//    vector<int> node_list;
//    vector<int>::iterator nodeitr = node_list.begin();
//    set<int> node_set;
//    set<int>::iterator set_itr = node_set.begin();
//
//    EdgeNode left_node = *itr_left;
//    int m = 0;
//    int start_time = left_node.t;  //开始时间
//    int step = 0;   //  左指针移动步数
//    if((adjlist[v].size() <= tau * k))
//    {
//        return  false;
//    }
//    else
//    {
//
//        for (itr_right = adjlist[v].begin(); itr_right != adjlist[v].end(); ++itr_right)
//        {
//            EdgeNode right_node = *itr_right;
//            node_list.push_back(right_node.node);
//            node_set.insert(right_node.node);
//            if(right_node.t - start_time == theta)
//            {
//                step = node_list.size();
//                if(node_set.size() >= k)
//                {
//
//                    node_list.clear();
//                    node_set.clear();
//                    ++m;
//                    //itr_right++;
//                    //right_node = *itr_right;
//                    itr_left += step;
//                    start_time = right_node.t;
//                }
//                else
//                {
//                    itr_left += 1;
//                    left_node = *itr_left;
//                    nodeitr = node_list.begin();
//                    node_list.erase(nodeitr);
//                    node_set.erase(*nodeitr);
//                    start_time = left_node.t;
//                }
//            }
//            else if(right_node.t - start_time > theta)
//            {
//                step = node_list.size();
//
//                if(node_set.size() - 1  >= k)
//                {
//
//                    node_list.clear();
//                    node_list.push_back(right_node.node);
//                    node_set.clear();
//                    node_set.insert(right_node.node);
//                    ++m;
//                    itr_left += step - 1;
//                    start_time = right_node.t;
//                }
//                else
//                {
//                    itr_left += 1;
//                    left_node = *itr_left;
//                    nodeitr = node_list.begin();
//                    node_list.erase(nodeitr);
//                    node_set.erase(*nodeitr);
//                    start_time = left_node.t;
//                }
//            }
//
//
//        }
//        return m >= tau ? true : false;
//    }
//
//}

int Graph::combinationNums(int n, int r)
{
    int i;
    int r1 = 1;
    int r2 = 1;
    int r3 = 1;
    for(i = 1;i <= n;i++)
        r1 *= i;
    for(i = 1;i <= r;i++)
        r2 *= i;
    for(i = 1;i <= (n - r);i++)
        r3 *= i;
    return r1/r2/r3;
}

//计算交集
set<int> Graph::itemIntersect(set<int> set1, set<int> set2)
{

//    set<int>::iterator set1It = set1.begin();
//    set<int> retSet;
//
//    while(set1It != set1.end()){
//        set<int>::iterator set2It = set2.begin();
//        while(set2It != set2.end()){
//            if((*set1It) == (*set2It)){
//                retSet.insert(*set1It);
//                break;
//            }
//            ++set2It;
//        }
//        ++set1It;
//    }

    set<int> intersect;
    set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),inserter(intersect, intersect.begin()));

    return intersect;
}

//频繁2项集的个数
int Graph::fre2Item(map<tuple<int, int>, set<int>> item)
{

    int num = 0;
    //找出所有2组合，计算频繁2项集
    map<tuple<int, int>, set<int>>::iterator  iter;
    map<tuple<int, int>, set<int>>::iterator  iter2;
    for(iter = item.begin(); iter != item.end(); iter++)
    {
        set<int> set1 = iter->second;
        //cout << set1.size() << endl;
        iter2 = ++iter;
        while(iter2 != item.end())
        {
            set<int> set2 = iter2->second;
            if((itemIntersect(set1, set2)).size() > tau)
            {num++;}
            iter2++;
        }
        iter--;
    }
    return  num;

}
//计算节点u的邻居的虚度
set<int> Graph::computeNeighbour()
{
    int du = combinationNums(k, 2);
    set<int> del_node;
    map<tuple<int, int>, set<int>> text_data;
    text_data = adjlistToFreItem();
    map<tuple<int, int>, set<int>>::iterator  iter = text_data.begin();
    int pre_node = get<0>(iter->first);
    map<tuple<int, int>, set<int>> my_map;
    for(iter = text_data.begin(); iter != text_data.end(); iter++)
    {
        if(get<0>(iter->first) == pre_node)
        {
            //节点u的邻居放人my_map中
            my_map.insert(*iter);
        }
        else
        {

            //计算my_map中的虚度，即频繁2项集
            int m = fre2Item(my_map);
            if(m < du)
            {
                cout << "put node into del_node " << pre_node << endl;
                del_node.insert(pre_node);
            }
            my_map.clear();
            my_map.insert(*iter);
        }
        pre_node = get<0>(iter->first);
    }
    return del_node;

}
//贪心算法，找出最大k-star
void Graph::maxKStar(map<tuple<int, int>, set<int>> fre_edge)
{
    map<tuple<int, int>, set<int>> k_star;
    map<tuple<int, int>, set<int>>::iterator  iter;
    int u, v;
    tuple<int,int> f1_tuple = make_tuple(1,22);
    set<int> f1_set;

    int max_fre_stru = 0;
    //找出最频繁的一条边，第一次迭代
    for(iter = fre_edge.begin(); iter != fre_edge.end(); iter++)
    {
        if(iter->second.size() > max_fre_stru)
        {
            max_fre_stru = iter->second.size();
            u = get<0>(iter->first);
            v = get<1>(iter->first);
            f1_set = iter->second;
        }


    }
    f1_tuple = make_tuple(u, v);
    //频繁边放入k_star中
    pair <tuple<int,int>, set<int>> my_pair = make_pair(f1_tuple, f1_set);
    k_star.insert(my_pair);
    //在fre_edge中删除此频繁边
    iter = fre_edge.find(f1_tuple);
    fre_edge.erase(iter);
    // 找出一条边使其与当前形状的频率最大
    //继续迭代
    max_fre_stru = 0;
    //m用来判断一次迭代是否完成
    int m = 0;
    for(iter = fre_edge.begin(); iter != fre_edge.end(); iter++)
    {
        if(k_star.size() >= k && max_fre_stru >= tau)
            ++m;
        int fre_stru = (itemIntersect(f1_set,iter->second)).size();
        if(fre_stru > max_fre_stru && fre_stru > tau)
        {
            max_fre_stru = fre_stru;
            u = get<0>(iter->first);
            v = get<1>(iter->first);
            f1_set = iter->second;
        }
        if(fre_edge.size() == m && max_fre_stru >= tau)
        {
            f1_tuple = make_tuple(u, v);
            //频繁边放入k_star中
            pair <tuple<int,int>, set<int>> my_pair = make_pair(f1_tuple, f1_set);
            k_star.insert(my_pair);
            //在fre_edge中删除此频繁边
            iter = fre_edge.find(f1_tuple);
            fre_edge.erase(iter);
            iter = fre_edge.begin();
            m = 0;
        }


    }
}
//贪心算法
void Graph::greedyAlg()
{

    map<tuple<int, int>, set<int>>::iterator  iter = fre_edge.begin();
    int pre_node = get<0>(iter->first);
    map<tuple<int, int>, set<int>> my_map;
    for(iter = fre_edge.begin(); iter != fre_edge.end(); iter++)
    {
        if(get<0>(iter->first) == pre_node)
        {
            //节点u的邻居放人my_map中
            my_map.insert(*iter);
        }
        else
        {

            //计算my_map中的最大项集
            maxKStar(my_map);
            my_map.clear();
            my_map.insert(*iter);
        }
        pre_node = get<0>(iter->first);
    }
}

//删除边
int Graph::delEdge(map<int, set<int>> &del_set)
{
    map<int, set<int>>::iterator iter;
    for(iter = del_set.begin(); iter != del_set.end(); iter++)
    {
        int i = iter->first;

        cout << "delelte   " << i << endl;
        set<int>::iterator set_iter;
        for(set_iter = iter->second.begin(); set_iter != iter->second.end(); set_iter++)
        {
            vector<EdgeNode>::iterator itr;
            for(itr = adjlist[i].begin(); itr != adjlist[i].end();)
            {

                EdgeNode t = *itr;

                if(t.node == *set_iter)
                {
                    cout << t.node << endl;
                    itr = adjlist[i].erase(itr);
                }
                else
                    ++itr;
            }
            //继续删除与之相邻的点
            vector<EdgeNode>::iterator itr2;
            for(itr2 = adjlist[(*set_iter)].begin(); itr2 != adjlist[(*set_iter)].end();)
            {

                EdgeNode t = *itr2;
                //cout << t.node << endl;
                if(t.node == i)
                    itr2 = adjlist[(*set_iter)].erase(itr2);
                else
                    ++itr2;
            }
        }


    }
}
//计算节点U的强邻居
map<int, set<int>> Graph::strongNeighbour()
{
    //============结点u的k个强邻居===========================================
//    cout << tau << endl;
//    set<int> del_node;
//    map<tuple<int, int>, set<int>> text_data;
//    text_data = adjlistToFreItem();
//    map<tuple<int, int>, set<int>>::iterator iter = text_data.begin();
//    int pre_node = get<0>(iter->first);
//    int k_count = 0;
//    for(iter = text_data.begin(); iter != text_data.end(); iter++)
//    {
//        if(get<0>(iter->first) == pre_node)
//        {
//            if(iter->second.size() >= tau)
//                k_count++;
//        }
//        else
//        {
//            if(k_count < k)
//            {
//                cout << pre_node << endl;
//                del_node.insert(pre_node);
//                k_count = 0;
//            }
//            if(iter->second.size() >= tau)
//                k_count++;
//
//        }
//        pre_node = get<0>(iter->first);
//    }
//    return del_node;
    //============结点u的k个强邻居================================================

    //=============计算频繁边====================================================
    map<int, set<int>> del_set;
    set<int> edge_set;
    map<tuple<int, int>, set<int>> text_data;
    text_data = adjlistToFreItem();
    map<tuple<int, int>, set<int>>::iterator iter = text_data.begin();
    int pre_node = get<0>(iter->first);
    for(iter = text_data.begin(); iter != text_data.end(); iter++)
    {   cout << get<0>(iter->first) << "----" << get<1>(iter->first) << endl;
        if(pre_node == get<0>(iter->first))
        {
            if(iter->second.size() < tau)
            {
                edge_set.insert(get<1>(iter->first));
            }
        }
        else
        {   cout << "删除" << pre_node << "  " << edge_set.size() << endl;
            pair<int, set<int>> pair = make_pair(pre_node,edge_set);
            del_set.insert(pair);
            edge_set.clear();
            if(iter->second.size() < tau)
            {
                edge_set.insert(get<1>(iter->first));
            }

        }
        pre_node = get<0>(iter->first);

    }
    cout << "删除" << pre_node << "  " << edge_set.size() << endl;
    pair<int, set<int>> pair = make_pair(pre_node,edge_set);
    del_set.insert(pair);
    edge_set.clear();
    return del_set;
    //==============计算频繁边===================================================
//    map<int, set<int>> del_set;
//    set<int> edge_set;
//    set<int> my_set;
//    for(int i = 1;i <= nodeSize();i++)
//    {
//        if(adjlist[i].size() != 0)
//        {
//
//            sort(adjlist[i].begin(),adjlist[i].end());
//            vector<EdgeNode>::iterator itr = adjlist[i].begin();
//            EdgeNode t = *itr;
//            int pre_node = t.node;
//            for(itr = adjlist[i].begin();itr != adjlist[i].end();itr++)
//            {
//                t = *itr;
//                //pre_node = t.node;
//                if(t.node == pre_node)
//                {
//                    for(int m = t.t - theta; m <= t.t; m++)
//                    {
//                        //cout << t.node << endl;
//                        my_set.insert(m);
//                    }
//
//                }
//                else
//                {
//                    cout << "compute fre edge : " <<i <<"-----" <<pre_node<< endl;
//                    if(my_set.size() < tau)
//                    {
//                        edge_set.insert(pre_node);
//                    }
//                    pair <tuple<int,int>, set<int>> my_pair = make_pair(make_tuple(i, pre_node), my_set);
//                    fre_edge.insert(my_pair);
//                    cout << "fre edge size is : " << fre_edge.size() << endl;
//                    my_set.clear();
//                    for(int m = t.t - theta; m <= t.t; m++)
//                    {
//                        //cout << t.node << endl;
//                        my_set.insert(m);
//                    }
//                    //my_set.insert(m);
//                }
//                pre_node = t.node;
//            }
//            cout << "compute fre edge : " <<i <<"-----" <<pre_node<< endl;
//            if(my_set.size() < tau)
//            {
//                edge_set.insert(pre_node);
//            }
//            pair <tuple<int,int>, set<int>> my_pair = make_pair(make_tuple(i, pre_node), my_set);
//            fre_edge.insert(my_pair);
//            cout << "fre edge size is : " << fre_edge.size() << endl;
//            my_set.clear();
//
//            pair<int, set<int>> pair = make_pair(i,edge_set);
//            del_set.insert(pair);
//        }
//
//    }
//    return  del_set;
}
//将adjlist转化为事务
map<tuple<int, int>, set<int>> Graph::adjlistToFreItem()
{
    map<tuple<int, int>, set<int>> text_data;
    set<int> my_set;
    for (int i = 1; i <= node_size; i++)
    {
        if (adjlist[i].size() != 0)
        {
            //对node排序，使得相邻的点在一起
            sort(adjlist[i].begin(),adjlist[i].end());
            vector<EdgeNode>::iterator itr = adjlist[i].begin();
            EdgeNode t = *itr;
            int pre_node = t.node;
            for (itr = adjlist[i].begin(); itr != adjlist[i].end(); itr++)
            {
                t = *itr;
                //cout << i << "   " << t.node << endl;
                if (t.node == pre_node)
                {
                    for (int m = t.t - theta; m <= t.t; m++)
                    {
                        my_set.insert(m);
                    }
                }
                else
                {
                    //cout << i << "   " << pre_node << endl;
                    pair<tuple<int, int>, set<int>> my_pair = make_pair(make_tuple(i, pre_node), my_set);
                    text_data.insert(my_pair);
                    my_set.clear();
                    for (int m = t.t - theta; m <= t.t; m++)
                    {
                        my_set.insert(m);
                    }
                }
                pre_node = t.node;
            }
            //adjlist[i]的最后一项加入text_data中
            pair<tuple<int, int>, set<int>> my_pair = make_pair(make_tuple(i, pre_node), my_set);
            text_data.insert(my_pair);
            my_set.clear();

        }
    }
    return text_data;
}
void Graph::DFSUtil(int v, vector<bool> &visited, int tau)
{
    if(v == 129303)
        cout << "bingo";
    visited[v] = true;
    cout << v << endl;


    vector<EdgeNode>::iterator i;
    for (i = adjlist[v].begin(); i != adjlist[v].end(); ++i)
    {

        EdgeNode t = *i;
        if ((adjlist[v].size() / theta) < tau)   //如果不符合条件
            //更新节点i的度
        {
            //NodeDel(t.node, v);  //删除与v相邻的节点
        }


        if (!visited[t.node])
        {
            DFSUtil(t.node, visited, 5);
            //if(DFSUtil(t.node, visited, 5))
            //NodeDel(t.node, v);  //删除与v相邻的节点

        }
    }

    //return ((adjlist[v].size() / theta) < tau);

}

void Graph::cutEdge()
{
    vector<int> del_line;
    for(int i = 1;i <= nodeSize();i++)
    {
        cout << i << " ";
        if(adjlist[i].size() != 0)
        {
            sort(adjlist[i].begin(),adjlist[i].end());
            vector<EdgeNode>::iterator itr = adjlist[i].begin();
            int count = 0;
            int pre_line = 0;
            EdgeNode t = *itr;
            pre_line = t.node;
            KNode my_node;
            for(itr = adjlist[i].begin();itr != adjlist[i].end();itr++)
            {
                t = *itr;
                my_node.u = i;
                my_node.v = t.node;
                my_node.t = t.t;
                k_node.push_back(my_node);
                if(t.node != pre_line && count < k)
                {
                    k_node.clear();
                }
                pre_line = t.node;
                if(count >= k)
                {

                }

            }
        }

    }
}

//打印节点
void Graph::printCore()
{
    string path = "/Users/zph/ClionProjects/k_star/";
    string file_path = path+to_string(k)+"_"+to_string(theta)+"_"+to_string(tau)+".txt";
    ofstream file(file_path, ios::trunc);
    for(int i = 1;i <= nodeSize();i++)
    {
        cout << i << " ";
        if(adjlist[i].size() != 0)
        {
            sort(adjlist[i].begin(),adjlist[i].end());
            vector<EdgeNode>::iterator itr;
            for(itr = adjlist[i].begin();itr != adjlist[i].end();itr++)
            {
                EdgeNode t = *itr;
                file << i << " " << t.node << " " << t.t << "\n";
                cout << "("<<t.node << " " << t.t << ")";
            }
            cout << endl;
            cout << "==============================================================" << endl;
        }

    }
    file.close();
}

void Graph::verifyFre()
{
    for(int i = 1; i <= node_size; ++i)
    {
        if(nodeFrequent(i))
        {
            cout << "Node" << i << "Frequent" << endl;
        }
    }

}

void Graph::delNodeSet(set<int> &del_set)
{
    set<int>::iterator iter;
    for(iter = del_set.begin(); iter != del_set.end(); iter++)
    {
        cout << "删除" << *iter << endl;
        delNode(*iter);
        adjlist[*iter].clear();
    }
}