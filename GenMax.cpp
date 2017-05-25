//
// Created by zph on 2017/5/21.
//

#include "GenMax.h"

GenMax::GenMax(int sk, int stau, int stheta)
{
    this->mk = sk;
    this->min_sup = stau;
    this->mtheta = stheta;
}

void GenMax::genData(vector<EdgeNode> *adjlist, int node_size)
{
    //adjlist = my_adjlist;
    set<int> my_set;
    for(int i = 1;i <= node_size;i++)
    {
        if(adjlist[i].size() != 0)
        {

            sort(adjlist[i].begin(),adjlist[i].end());
            vector<EdgeNode>::iterator itr = adjlist[i].begin();
            EdgeNode t = *itr;
            int pre_node = t.node;
            for(itr = adjlist[i].begin();itr != adjlist[i].end();itr++)
            {
                t = *itr;
                if(t.node == pre_node)
                {
                    for(int m = t.t - mtheta; m <= t.t; m++)
                    {
                        //cout << t.node << endl;
                        my_set.insert(m);
                    }
                }
                else
                {
                    //cout << "compute fre edge : " <<i <<"-----" <<pre_node<< endl;
                    cout << i << "   " << pre_node << endl;
                    pair <tuple<int,int>, set<int>> my_pair = make_pair(make_tuple(i, pre_node), my_set);
                    text_data.insert(my_pair);
                    //cout << "fre edge size is : " << text_data.size() << endl;
                    my_set.clear();
                    for(int m = t.t - mtheta; m <= t.t; m++)
                    {
                        //cout << t.node << endl;
                        my_set.insert(m);
                    }
                    //my_set.insert(m);
                }
                pre_node = t.node;
            }
            cout << i << "   " << pre_node << endl;
            pair <tuple<int,int>, set<int>> my_pair = make_pair(make_tuple(i, pre_node), my_set);
            text_data.insert(my_pair);
            //cout << "fre edge size is : " << text_data.size() << endl;
            my_set.clear();

        }

    }
}
//读取数据，构建数据集
//int GenMax::BuildData(string file_name)
//{
//
//    /*打开文本文件*/
//    ifstream inFile;
//    inFile.open(file_name.c_str());
//    if(!inFile){
//        cerr << "open " <<file_name << "is failed!" << endl;
//        return EXIT_FAILURE;
//    }
//    /*读取文本行*/
//    string textline;
//    vector<string> lines_of_text;
//
//    while(getline(inFile,textline))
//        lines_of_text.push_back(textline);
//    /*产生事务数据库*/
//    int line_num ;
//    for(line_num = 0; line_num != lines_of_text.size(); ++line_num){
//        istringstream line(lines_of_text[line_num]);
//        string word;
//        string item_name;
//        pair <string, set<int>> my_pair;
//        set<int> tid;
//        int i = 0;
//        while(line >> word)
//        {
//            if(i == 0)
//            {
//                item_name = word;
//                cout << item_name << endl;
//                ++i;
//            }
//            else
//                tid.insert(stoi(word));
//        }
//        my_pair = make_pair(item_name, tid);
//        text_data.insert(my_pair);
//    }
//    text_data_count = text_data.size();
//    cout << "textDatabaseCount: " << text_data_count << " " << line_num<< endl;
//    return EXIT_SUCCESS;
//}

//计算频繁1项集
void GenMax::computeFre1()
{
    map<tuple<int,int>, set<int>>::iterator iter;
    for(iter = single_text_data.begin(); iter != single_text_data.end(); iter++)
    {
        if(iter->second.size() >= min_sup)
        {
            pair <tuple<int,int>, set<int>> my_pair;
            my_pair = make_pair(make_tuple(get<0>(iter->first), get<1>(iter->first)),iter->second);
            fre1_item.insert(my_pair);
        }
    }
}

//计算交集
set<int> GenMax::itemIntersect(set<int> set1, set<int> set2)
{

    set<int>::iterator set1It = set1.begin();
    set<int> retSet;

    while(set1It != set1.end()){
        set<int>::iterator set2It = set2.begin();
        while(set2It != set2.end()){
            if((*set1It) == (*set2It)){
                retSet.insert(*set1It);
                break;
            }
            ++set2It;
        }
        ++set1It;
    }

    return retSet;
}

//计算并集
map<tuple<int,int>, set<int>> GenMax::itemUnion(map<tuple<int,int>, set<int>> set1, map<tuple<int,int>, set<int>> set2)
{
    map<tuple<int,int>, set<int>>::iterator  set2_iter = set2.begin();
    map<tuple<int,int>, set<int>> ret_set(set1.begin(),set1.end());
    while(set2_iter != set2.end()){
        ret_set.insert(*set2_iter);
        ++set2_iter;
    }
    return ret_set;
}
//生成TidSet
set<int> GenMax::tidSet(map<tuple<int,int>, set<int>> item)
{
    map<tuple<int,int>, set<int>>::iterator  iter1 = item.begin();
    map<tuple<int,int>, set<int>>::iterator  iter2 = item.begin();
    iter2++;
    set<int> item_set;
    if(item.size() == 1)
        item_set = iter1->second;
    for(iter1 = item.begin(); iter1 != item.end() && iter2 != item.end(); iter1++,iter2++)
    {
        item_set = iter1->second;
        item_set = itemIntersect(item_set, iter2->second);

    }
    return item_set;
}

//合并项集
map<tuple<int,int>, set<int>> GenMax::freCombine(map<tuple<int,int>, set<int>> item_set, map<tuple<int,int>, set<int>> possible_set)
{
    map<tuple<int,int>, set<int>>::iterator  iter;
    set<int> ret_set;
    map<tuple<int,int>, set<int>> combine_set;
    for(iter = possible_set.begin(); iter != possible_set.end(); iter++)
    {
        set<int> test = tidSet(item_set);
        //cout << test.size()<< endl;
        ret_set = itemIntersect(tidSet(item_set), iter->second);
        if(ret_set.size() >= min_sup)
        {
            pair <tuple<int, int>, set<int>> my_pair;
            my_pair = make_pair(iter->first,iter->second);
            combine_set.insert(my_pair);
        }
    }
    return combine_set;
}


void GenMax::runGenMax(vector<EdgeNode> *adjlist, int node_size)
{

    genData(adjlist, node_size);
    //分别去找每个点的max k-star
    map<tuple<int,int>, set<int>>::iterator iter = text_data.begin();
    int pre_node = get<0>(iter->first);
    for(iter = text_data.begin(); iter != text_data.end(); iter++)
    {
        if(get<0>(iter->first) == pre_node)
        {
            //cout << get<1>(iter->first) << endl;
            //节点u的邻居放人my_map中
            single_text_data.insert(*iter);
        }
        else
        {
            //计算k-star;
            //把single_text_data传入ComputeFre1中进行计算

            //================test begin========
//            single_text_data.clear();
//
//            set<int> my_set;
//            my_set.insert(1);
//            my_set.insert(2);
//            my_set.insert(3);
//            my_set.insert(5);
//            pair<tuple<int, int>, set<int>> my_pair1 = make_pair(make_tuple(1, 2), my_set);
//            single_text_data.insert(my_pair1);
//
//            my_set.clear();
//            my_set.insert(2);
//            my_set.insert(3);
//            my_set.insert(4);
//            pair<tuple<int, int>, set<int>> my_pair2 = make_pair(make_tuple(1, 3), my_set);
//            single_text_data.insert(my_pair2);
//
//            my_set.clear();
//            my_set.insert(2);
//            my_set.insert(3);
//            my_set.insert(5);
//            pair<tuple<int, int>, set<int>> my_pair3 = make_pair(make_tuple(1, 4), my_set);
//            single_text_data.insert(my_pair3);
//
//            my_set.clear();
//            my_set.insert(2);
//            my_set.insert(3);
//            my_set.insert(5);
//            pair<tuple<int, int>, set<int>> my_pair4 = make_pair(make_tuple(1, 5), my_set);
//            single_text_data.insert(my_pair4);
            //=================test end========
            computeFre1();
            map<tuple<int,int>, set<int>> i_set;
            maxFreItem(i_set, fre1_item, 1);
            //==================test begin===================
//            cout << "print fre item" << endl;
//            map<tuple<int, int>, set<int>>::iterator iter11;
//            for (iter11 = max_fre_item.begin(); iter11 != max_fre_item.end(); iter11++)
//            {
//            //vec_iter = iter->first.begin();
//                cout << get<0>(iter11->first) << "  " << get<1>(iter11->first) << endl;
//            }
            //==================test end===================
            if(max_fre_item.size() == 0)
            {
                del_set.insert(pre_node);
            }
            //将点u的频繁项集插入到all_max_fre_item中
            for(auto i : max_fre_item)
            {
                all_max_fre_item.insert(i);
            }
            single_text_data.clear();
            single_text_data.insert(*iter);
            max_fre_item.clear();
            fre1_item.clear();
            //possible_set.clear();
            //combine_set.clear();
            //fre2Item(my_map);
            //my_map.clear();
            //my_map.insert(*iter);
        }
        pre_node = get<0>(iter->first);
    }


    all_size = all_max_fre_item.size();
}

//最大频繁项集
void GenMax::maxFreItem(map<tuple<int,int>, set<int>> i_set, map<tuple<int,int>, set<int>> combine_set, int l)
{
    map<tuple<int,int>, set<int>> possible_set;
    map<tuple<int,int>, set<int>> my_i_set;
    map<tuple<int,int>, set<int>> temp;
    //遍历combine_set
    map<tuple<int,int>, set<int>>::iterator  iter;
    for(iter = combine_set.begin(); iter != combine_set.end(); iter++)
    {
        temp.clear();
        pair <tuple<int, int>, set<int>> my_pair;
        my_pair = make_pair(iter->first,iter->second);
        //插入x
        temp.insert(my_pair);
        my_i_set = itemUnion(i_set, temp);
        //插入x之后的值
        for (map<tuple<int,int>, set<int>>::iterator  iter1 = ++iter; iter1 != combine_set.end(); iter1++)
        {
            pair <tuple<int, int>, set<int>> my_pair1;
            //cout << get<1>(iter1->first) << endl;
            my_pair1 = make_pair(iter1->first,iter1->second);
            possible_set.insert(my_pair1);
        }
        iter--;
        //检测I与P的并集在MFI中是否有超集
        if(hasSuperSet(itemUnion(my_i_set, possible_set)) == true)
            return;
        //F1-combine
        map<tuple<int,int>, set<int>> my_combine_set = freCombine(my_i_set, possible_set);
        if((my_combine_set.size() == 0) && (hasSuperSet(my_i_set) == false))
        {
            if(my_i_set.size() >= mk)
                //MFI = MFI 并 combine_set
                max_fre_item = itemUnion(max_fre_item, my_i_set);
        }
        else
        {
            maxFreItem(my_i_set, my_combine_set, 1);
        }

    }
}

//判断是否有超集
bool GenMax::hasSuperSet(map<tuple<int,int>, set<int>> item_set)
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

    int num = 0;
    for(auto i : set1)
    {
        for(auto m : set2)
        {
            if((get<0>(i) == get<0>(m)) && (get<1>(i) == get<1>(m)))
                num++;
        }
    }
    //set<string> intersect;
    //set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),inserter(intersect, intersect.begin()));
    if(set1.size() == num)
        return  true;
    else
        return  false;


}

void GenMax::printFreItem()
{
    ofstream file("/Users/zph/ClionProjects/FrequentKCore/k_star.txt", ios::app);
    map<tuple<int,int>, set<int>>::iterator iter;
    for(iter = all_max_fre_item.begin(); iter != all_max_fre_item.end(); iter++)
    {
        file << get<0>(iter->first) << " " << get<1>(iter->first) << "\n";
        cout << get<0>(iter->first) <<"----" << get<1>(iter->first) << endl;
    }
    file.close();
}

//判断是否有超集
//bool GenMax::HasSuperSet2(map<tuple<int,int>, set<int>> item_set, map<tuple<int,int>, set<int>> max_fre_item)
//{
//    set<tuple<int, int>> set1;
//    //遍历item_set
//    map<tuple<int,int>, set<int>>::iterator  iter1;
//    for(iter1 = item_set.begin(); iter1 != item_set.end(); iter1++)
//    {
//        set1.insert(iter1->first);
//    }
//
//    set<tuple<int, int>> set2;
//    map<tuple<int,int>, set<int> >::iterator  iter2;
//    for(iter2 = max_fre_item.begin(); iter2 != max_fre_item.end(); iter2++)
//    {
//        set2.insert(iter2->first);
//    }
//
//    int num = 0;
//    for(auto i : set1)
//    {
//        for(auto m : set2)
//        {
//            if((get<0>(i) == get<0>(m)) && (get<1>(i) == get<1>(m)))
//                num++;
//        }
//    }
//    //set<string> intersect;
//    //set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(),inserter(intersect, intersect.begin()));
//    if(set1.size() == num)
//        return  true;
//    else
//        return  false;
//
//
//}


//打印
//void GenMax::PrintMap(map<string, set<int> > item)
//{
//    map<string, set<int> >::iterator  iter;
//    for(iter = item.begin(); iter != item.end(); iter++)
//    {
//        cout << iter->first << "  ";
//        set<int>::iterator set_iter;
//        for(set_iter = iter->second.begin(); set_iter != iter->second.end(); set_iter++)
//        {
//            cout << *set_iter << "  ";
//        }
//        cout << endl;
//    }
//}

