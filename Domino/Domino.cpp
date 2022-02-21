// LeetCode 838
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class dominoesSolution {
public:
    // BFS方法
    string bfs_pushDominoes(string dominoes) {
        return dominoeBFS(dominoes);
    }
    // 双指针法
    string dptr_pushDominoes(string dominoes) {
        dominoeDptr(dominoes);
        return dominoes;
    }
    // 流泪tomato法
    string tomato_pushDominoes(string dominoes) {
        bool flag = true;
        while (flag) {
            flag = dominoeChangeOnce(dominoes, 0);
            // cout << dominoes << endl;
            if (flag)
                updateString(dominoes);
        }
        return dominoes;
    }

private:

    // 流泪tomato
    bool dominoeChangeOnce(string& dominoes, int);
    void updateString(string& dominoes);
    // BFS
    string dominoeBFS(string& dominoes);
    // 双指针
    void dominoeDptr(string& dominoes);


};




/*
    输入：dominoes = ".L.R...LR..L.."
    输出："LL.RR.LLRRLL.."
*/

int main() {
    dominoesSolution s1;
    string str = ".L.R...LR..L..";
    string ss1 = s1.bfs_pushDominoes(str);
    cout << "BFS:    " << ss1 << endl;
   
    string ss2 = s1.dptr_pushDominoes(str);
    cout << "DPtr:   " << ss2 << endl;
   
    string ss3 = s1.tomato_pushDominoes(str);
    cout << "Update: " << ss3 << endl;

	return 0;
}





/*
    流泪tomato法：遍历->更新->遍历->更新...
 */
bool dominoesSolution::dominoeChangeOnce(string& dominoes, int)
{
    bool hasChanged = false;
    for (int index = 0; index < dominoes.length(); index++) {
        if (dominoes[index] != '.') continue;
        if (index == 0 && dominoes[index + 1] != '.') {
            dominoes[index] = (dominoes[index + 1] == 'R' ? 'a' : 'A');
            hasChanged = true;
        }
        else if (index == dominoes.length() - 1 && dominoes[index - 1] != '.') {
            dominoes[index] = (dominoes[index - 1] == 'R' ? 'a' : 'A');
            hasChanged = true;
        }
        else {
            if (dominoes[index - 1] == 'R' && dominoes[index + 1] != 'L') {
                dominoes[index] = 'a'; hasChanged = true;
            }  // a 表示R
            else if (dominoes[index + 1] == 'L' && dominoes[index - 1] != 'R') {
                dominoes[index] = 'A'; hasChanged = true;
            }  // A 表示L
            else
                dominoes[index] = '.';
        }
        //printf("%d ",index);
    }
    return hasChanged;
}

void dominoesSolution::updateString(string& dominoes)
{
    for (int i = 0; i < dominoes.length(); i++) {
        if (dominoes[i] == 'a')
            dominoes[i] = 'R';
        else if (dominoes[i] == 'A')
            dominoes[i] = 'L';
    }
}

/*
    BFS Tn = O(n),Sn = O(n)
*/
string dominoesSolution::dominoeBFS(string& dominoes)
{
    int len = dominoes.size();
    queue<int> infectedDomino;      // 已经倾倒的骨牌
    vector<int> changeTime(len, -1);// 每块骨牌倾倒的时间
    vector<string> force(len);      // 每块骨牌的受力情况

    for (int i = 0; i < len; i++) { // 初始化状态数组
        if (dominoes[i] != '.') {
            infectedDomino.emplace(i);
            changeTime[i] = 0;
            force[i] = dominoes[i];
        }       
    }

    string ret(len, '.');      // 返回值
    while (!infectedDomino.empty()) {
        int index = infectedDomino.front();
        infectedDomino.pop();

        if (force[index].size() == 1) { // 只受一个力 ,若受两个力 则保持‘.’不变
            char dir = force[index][0];

            ret[index] = dir;

            int infectedNext = (dir == 'L') ? (index - 1) : (index + 1);
            if (infectedNext >= 0 && infectedNext < len) {
                int time = changeTime[index];
                if (changeTime[infectedNext] == -1) {
                    infectedDomino.emplace(infectedNext);
                    changeTime[infectedNext] = time + 1;
                    force[infectedNext].push_back(dir);
                }
                else if (changeTime[infectedNext] == time + 1) { // 
                    force[infectedNext].push_back(dir);
                }
                
            }
        }


    }
    return ret;
}

/*
    模拟+双指针 Tn = O(n),Sn = O(1)
*/
void dominoesSolution::dominoeDptr(string& dominoes)
{
    int len = dominoes.size(), index = 0;
    char leftborder = 'L', rightboder = 'R';    // 默认左边界和右边界为L、R 不影响原数组
    while (index < len) {
        int d_index = index;
        while (d_index < len && dominoes[d_index] == '.')    // 找到连续的未推倒骨牌
            d_index++;

        rightboder = d_index < len ? dominoes[d_index] : 'R';    // 如果到达右边界 取R

        if (leftborder == rightboder) {
            while (index < d_index)
                dominoes[index++] = rightboder;
        }
        else if (leftborder == 'R' && rightboder == 'L') {
            int d_index2 = d_index - 1;
            while (index < d_index2) {
                dominoes[index++] = leftborder;
                dominoes[d_index2--] = rightboder;
            }
        }

        leftborder = rightboder;    // 向后更新
        index = d_index + 1;
    }
}
