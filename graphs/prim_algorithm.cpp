#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <locale.h>
using namespace std;
struct edge
{
   unsigned int top1;
   unsigned int top2;
   float weight;
   edge(unsigned int m_top1, unsigned int m_top2, float m_weight) : top1(m_top1), top2(m_top2), weight(m_weight) {};
};

struct Compare
{
   bool operator () (edge &edge1, edge &edge2)
   {
      bool p{ edge1.weight > edge2.weight };
      return p;
   }
};

vector<vector<pair<unsigned int, float>>> convert_graph(unsigned int &count_top, unsigned int &count_edge)
{
   vector<vector<pair<unsigned int, float>>> list;
   unsigned int top_number{ 0 }, edge_number{ 0 };
   float weight{ 0 };
   ifstream in("input.txt");
   in >> count_top >> count_edge;
   list.resize(count_top);

   for ( size_t i = 0; i < count_edge; ++i )
   {
      in >> top_number >> edge_number >> weight;
      list[top_number - 1].push_back(make_pair(edge_number - 1, weight));
      list[edge_number - 1].push_back(make_pair(top_number - 1, weight));
   }
   in.close();

   return list;
}

void out_path(vector <edge> &path)
{
   ofstream out("output.txt");

   for ( auto &elem : path )
      out << elem.top1 + 1 << " - " << elem.top2 + 1 << " (" << elem.weight << ") " << endl;
   out.close();
}

vector <edge> algrotirhmPrima(vector<vector<pair<unsigned int, float>>> list, unsigned int &count_top, unsigned int &beginTop, vector<bool> crossed)
{
   vector <edge> tree;
   tree.reserve(count_top);
   priority_queue <edge, vector<edge>, Compare> p;
   
   unsigned int current_top = beginTop - 1;

   for ( auto &elem : list[current_top] )
   {
      edge e = { current_top, elem.first, elem.second };
      p.push(e);
   }
   crossed[current_top] = true;
   for ( ;; )
   {
      if ( p.empty() )
      {
         break;
      }
      edge v = p.top();
      p.pop();
      if ( !crossed[v.top2] )
      {
         tree.push_back(v);
      }
      for ( auto &elem : list[v.top2] )
      {
         edge e = { v.top2, elem.first, elem.second };
         if ( !crossed[e.top2] )
         {
            p.push(e);
         }
      }
      crossed[v.top2] = true;
   }
   return tree;
}

int main()
{
   setlocale(0, "Russian");
   unsigned int count_top{ 0 }, count_edge{ 0 }, beginTop{ 0 };
   vector<vector<pair<unsigned int, float>>> graph = convert_graph(count_top, count_edge);
   vector<bool> crossed;
   crossed.resize(count_top);

   for ( ;; )
   {
      cout << "Введите начальную вершину ( от 1 до " << count_top << " ) : ";
      cin >> beginTop;
      cout << endl;
      if ( beginTop <= count_top && beginTop > 0 )
      {
         break;
      }
      cout << "Введен неверный номер вершины" << endl;
   }

   vector <edge> tree = algrotirhmPrima(graph, count_top, beginTop, crossed);
   for ( bool elem : crossed )
   {
      if ( !elem )
      {
         cout << "Несвязный граф" << endl;
         return 0;
      }
   }
   out_path(tree);
}