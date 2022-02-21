// LeetCode 838
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class dominoesSolution {
public:
    // BFS����
    string bfs_pushDominoes(string dominoes) {
        return dominoeBFS(dominoes);
    }
    // ˫ָ�뷨
    string dptr_pushDominoes(string dominoes) {
        dominoeDptr(dominoes);
        return dominoes;
    }
    // ����tomato��
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

    // ����tomato
    bool dominoeChangeOnce(string& dominoes, int);
    void updateString(string& dominoes);
    // BFS
    string dominoeBFS(string& dominoes);
    // ˫ָ��
    void dominoeDptr(string& dominoes);


};




/*
    ���룺dominoes = ".L.R...LR..L.."
    �����"LL.RR.LLRRLL.."
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
    ����tomato��������->����->����->����...
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
            }  // a ��ʾR
            else if (dominoes[index + 1] == 'L' && dominoes[index - 1] != 'R') {
                dominoes[index] = 'A'; hasChanged = true;
            }  // A ��ʾL
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
    queue<int> infectedDomino;      // �Ѿ��㵹�Ĺ���
    vector<int> changeTime(len, -1);// ÿ������㵹��ʱ��
    vector<string> force(len);      // ÿ����Ƶ��������

    for (int i = 0; i < len; i++) { // ��ʼ��״̬����
        if (dominoes[i] != '.') {
            infectedDomino.emplace(i);
            changeTime[i] = 0;
            force[i] = dominoes[i];
        }       
    }

    string ret(len, '.');      // ����ֵ
    while (!infectedDomino.empty()) {
        int index = infectedDomino.front();
        infectedDomino.pop();

        if (force[index].size() == 1) { // ֻ��һ���� ,���������� �򱣳֡�.������
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
    ģ��+˫ָ�� Tn = O(n),Sn = O(1)
*/
void dominoesSolution::dominoeDptr(string& dominoes)
{
    int len = dominoes.size(), index = 0;
    char leftborder = 'L', rightboder = 'R';    // Ĭ����߽���ұ߽�ΪL��R ��Ӱ��ԭ����
    while (index < len) {
        int d_index = index;
        while (d_index < len && dominoes[d_index] == '.')    // �ҵ�������δ�Ƶ�����
            d_index++;

        rightboder = d_index < len ? dominoes[d_index] : 'R';    // ��������ұ߽� ȡR

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

        leftborder = rightboder;    // ������
        index = d_index + 1;
    }
}
