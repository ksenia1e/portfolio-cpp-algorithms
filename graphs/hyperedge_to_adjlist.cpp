#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

void convert_graph()
{
    std::ifstream inn("n.txt");
    if (!inn.is_open()) {
        std::cerr << "Error open n.txt file";
        return;
    }

    int n{ 0 };
    inn >> n;
    inn.close();
    if (n <= 0) {
        std::cerr << "Error invalid n in n.txt file";
        return;
    }
    std::vector<std::vector<int>>list;


    for (size_t i = 1; i <= n; ++i)
    {
        std::string path(std::to_string(i));
        path.insert(1, ".txt");
        std::ifstream in(path);
        int count_top{ 0 };

        if (!in.is_open()) {
            std::cerr << "Error open " << i << ".txt file";
            return;
        }

        in >> count_top;
        if (count_top <= 0) {
            std::cerr << "Error invalid count_top in " << i << ".txt file";
            in.close();
            return;
        }
        std::vector<int> graph_tops;

        for (int j = 0; j < count_top; ++j)
        {
            int elem{ 0 };
            in >> elem;
            graph_tops.push_back(elem);
        }

        for (int top : graph_tops)
        {
            int count_new_top = top - list.size();
            if (count_new_top > 0)
            {
                std::vector<int> new_top;
                list.reserve(count_new_top);
                for (; count_new_top > 0; --count_new_top)
                {
                    list.push_back(new_top);
                }
            }
            for (size_t j = 0; j < count_top; ++j)
            {
                if (graph_tops[j] == top) continue;
                bool count{ false };
                for (int elem : list[top - 1])
                {
                    if (graph_tops[j] - 1 == elem) count = true;
                }
                if (count == false)
                {
                    list[top - 1].push_back(graph_tops[j] - 1);
                }
            }
        }
        in.close();
    }

    std::ofstream out("output.txt");

    for (size_t i = 0; i < list.size(); ++i)
    {
        std::sort(list[i].begin(), list[i].end());
        out << i + 1 << ": ";
        for (size_t j = 0; j < list[i].size(); j++)
        {
            out << list[i][j] + 1 << " ";
        }
        out << std::endl;
    }
    out.close();

}

int main()
{
    convert_graph();
}