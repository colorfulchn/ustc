#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <math.h>
#include <unordered_set>
using namespace std;
 vector<map<float, float>> multi_;
vector<string> str_multi_;
 void amulti(map<float, float>& oneofmulti) {
    string p;
    string e;
    cout << "请输入系数和幂";
                   while (1) {
                 cin >> p;
        if (p == "#" || e == "#") return;
        cin >> e;
        if (p == "#" || e == "#") return;
                 float p_ = atof(p.c_str());
        float e_ = atof(e.c_str());
                          oneofmulti[e_] += p_;
    }
}

 string showmulti(map<float, float> oneofmulti) {
    string res = "";
    for (auto one : oneofmulti)
    {
                 if (one.second == 0) continue;
                 stringstream p;
        stringstream e;
        p << one.second;
        e << one.first;
         
        if (res == "" || one.second < 0)
        {
            if (one.first != 0) {
                res += p.str() + "x^" + e.str();
            }
            else {
                res += p.str();
            }
        }
                 else
        {
            if (one.first != 0) {
                res += "+" + p.str() + "x^" + e.str();
            }
            else {
                res += "+" + p.str();
            }
        }
             }
         return res;
}

 map<float, float> add2multi(map<float, float> thefirst, map<float, float> thesecond) {
         map<float, float> ans_multiply;
    for (auto one : thefirst)
    {
        const float e1 = one.first;
        const float p1 = one.second;
        ans_multiply[e1] += p1;
    }
    for (auto two : thesecond)
    {
        const float e2 = two.first;
        const float p2 = two.second;
        ans_multiply[e2] += p2;
    }
    return ans_multiply;
}

 map<float, float> differential(map<float, float> oneofmulti) {
    map<float, float> differential_it;
    for (auto one : oneofmulti)
    {
        float e = one.first;
        float p = one.second;
                 if (e == 0) continue;
        differential_it[e - 1] = p * e;
             }
    return differential_it;
}

 map<float, float> integral(map<float, float> oneofmulti) {
    map<float, float> integral_it;
    for (auto one : oneofmulti)
    {
        float e = one.first;
        float p = one.second;
        integral_it[e + 1] += p / (e + 1);     }
              return integral_it;
}

 float definite_integral(map<float, float> oneofmulti, float low, float high) {
         float ans_high = 0;
    float ans_low = 0;
    float sum = 0;
    map<float, float> oneofmulti_;
    oneofmulti_ = integral(oneofmulti);
    for (auto one : oneofmulti_) {
        float e = one.first;
        float p = one.second;
        sum += p * (pow(high, e) - pow(low, e));
                               }
    return sum;
     }

 map<float, float> multiply(map<float, float> thefirst, map<float, float> thesecond) {
         map<float, float> ans_multiply;
    for (auto one : thefirst)
    {
        const float e1 = one.first;
        const float p1 = one.second;
        for (auto two : thesecond)
        {
            const float e2 = two.first;
            const float p2 = two.second;
            ans_multiply[e1 + e2] += p1 * p2;
        }
    }
    return ans_multiply;
}

   map<float, float> Nth_power(int m, map<float, float>oneofmulti) {
    map<float, float> temp = oneofmulti;
    while (--m) {
        temp = multiply(temp, oneofmulti);
    }
    return temp;
}

 void show_all_multi() {
         cout << "当前存储的所有多项式为：" << endl;
    int i = 0;
    for (auto s : str_multi_)
    {
        cout << i << ":   " << s << endl;
        ++i;
    }
}

 void savethemulti(map<float, float> oneofmulti) {
         multi_.push_back(oneofmulti);
    str_multi_.push_back(showmulti(oneofmulti));
    return;
}


 void showthemeau() {
    system("clr");
    cout << "功能：" << endl;
    cout << "a：创建新的多项式：" << endl;
    cout << "b：显示当前所有的多项式" << endl;
    cout << "d：显示第n个多项式" << endl;

    cout << "e：微分" << endl;
    cout << "f：定积分" << endl;
    cout << "g：不定积分" << endl;
    cout << "h：乘方" << endl;

    cout << "i：求和" << endl;
    cout << "j：乘法" << endl;
    cout << "z：退出计算器" << endl;
}

 void show_amulti() {
    system("clr");
    cout << "请输入输入多项式的个数：" << endl;
    int num = 0;
    cin >> num;
    if (num <= 0)
        return showthemeau();

    cout << "请输入多项式（以'#'结束）：" << endl;
    cout << "示例：3x^4 - 0.5x^0.6 + 7" << endl;
    cout << "输入：3 4 -0.5 0.6 7 0 #" << endl;
         for (int i = 0; i < num; ++i) {
        map<float, float> tmp;
        amulti(tmp);
        savethemulti(tmp);
    }
    return showthemeau();
}
 void show_n_multi() {
    system("clr");
    cout << "请输入显示多项式的index：" << endl;
    int len = str_multi_.size() - 1;
    cout << "index的范围为0～" << len << endl;
    int n = -1;
    cin >> n;
    cout << str_multi_[n] << endl;
    cout << "输入任何键敲回车返回主界面" << endl;
    char ss;
    cin >> ss;
    return showthemeau();
}

 void show_diff() {
    system("clr");
    int len = str_multi_.size() - 1;
    cout << "请输入你要进行微分的多项式的index(0~" << len << "):" << endl;
    int index = -1;
    cin >> index;
    cout << "微分结果为：" << endl;
    map<float, float> diffone = differential(multi_[index]);
    cout << showmulti(diffone) << endl;
    cout << "请问您需要该结果么？" << endl;
    cout << "保存按1，不保存按0" << endl;
    int keyborad_in = -1;
    cin >> keyborad_in;
    if (keyborad_in == 1)
        savethemulti(diffone);
         return showthemeau();
}

 void show_inte() {
    system("clr");
    int len = str_multi_.size() - 1;
    cout << "请输入你要进行不定积分的多项式的index(0~" << len << "):" << endl;
    int index = -1;
    cin >> index;
    cout << "积分结果为：" << endl;
    map<float, float> inteone = integral(multi_[index]);
    cout << showmulti(inteone) + "+Constant" << endl;
    cout << "请问您需要该结果么？" << endl;
    cout << "保存按1，不保存按0" << endl;
    int keyborad_in = -1;
    cin >> keyborad_in;
    if (keyborad_in == 1)
        savethemulti(inteone);
         return showthemeau();
}

 void show_definite_integral() {
    system("clr");
    int len = str_multi_.size() - 1;
    cout << "请输入你要进行定积分的多项式的index(0~" << len << "):" << endl;
    int index = -1;
    cin >> index;
    float up, down;
    cout << "输入上限、下限：" << endl;
    cin >> up;
    cin >> down;
    cout << "定积分结果为：" << endl;
    cout << definite_integral(multi_[index], down, up) << endl;

    cout << "输入任意键敲回车返回主界面" << endl;
    char keyborad_in = -1;
    cin >> keyborad_in;
    return showthemeau();
}

 void show_Nth_power() {
    system("clr");
    int len = str_multi_.size() - 1;
    cout << "请输入你要进行乘方的多项式的index(0~" << len << "):" << endl;
    int index = -1;
    cin >> index;
    cout << "输入乘方数：" << endl;
    int N;
    cin >> N;
    if (N==0)
    {
        cout << "结果为：1" << endl;
        cout << "输入任意键按回车返回主菜单" << endl;
        char ss;
        cin >> ss;
        return showthemeau();
    }
    map<float, float> powone = Nth_power(N, multi_[index]);
    cout << showmulti(powone) << endl;
    cout << "请问您需要该结果么？" << endl;
    cout << "保存按1，不保存按0" << endl;
    int keyborad_in = -1;
    cin >> keyborad_in;
    if (keyborad_in == 1)
        savethemulti(powone);
         return showthemeau();
}

 void show_multiply() {
    system("clr");
    vector<int> index;
    int len = str_multi_.size() - 1;
    cout << "示例：0 1 0 -1回车" << endl;
    cout << "请选择进行乘法的多项式的index(以-1结尾，0~" << len << "):" << endl;
    int idx;
    while (1)
    {
        cin >> idx;
        if (idx == -1) break;
        index.push_back(idx);
    }
    map<float, float> res = multi_[index[0]];
    for (int i = 1; i < (int)index.size(); ++i)
    {
        res = multiply(res, multi_[index[i]]);
    }
    cout << "结果为：" << endl;
    cout << showmulti(res) << endl;
    cout << "请问您需要该结果么？" << endl;
    cout << "保存按1，不保存按0" << endl;
    int keyborad_in = -1;
    cin >> keyborad_in;
    if (keyborad_in == 1)
        savethemulti(res);
         return showthemeau();
}

 void show_add2multi() {
    system("clr");
    vector<int> index;
    int len = str_multi_.size() - 1;
    cout << "示例：0 1 0 -1回车" << endl;
    cout << "请选择进行求和的多项式的index(以-1结尾，0~" << len << "):" << endl;
    int idx;
    while (1)
    {
        cin >> idx;
        if (idx == -1) break;
        index.push_back(idx);
    }
    map<float, float> res = multi_[index[0]];
    for (int i = 1; i < (int)index.size(); ++i)
    {
        res = add2multi(res, multi_[index[i]]);
    }
    cout << "结果为：" << endl;
    cout << showmulti(res) << endl;
    cout << "请问您需要该结果么？" << endl;
    cout << "保存按1，不保存按0" << endl;
    int keyborad_in = -1;
    cin >> keyborad_in;
    if (keyborad_in == 1)
        savethemulti(res);
         return showthemeau();

}

 void switchit(char keyboard_in) {
    switch (keyboard_in) {
    case 'a':         show_amulti();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'b':         system("clr");
        show_all_multi();
        cout << "输入任意键敲回车返回主界面" << endl;
        cin >> keyboard_in;
        showthemeau();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'd':         show_n_multi();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'e':         show_diff();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'f':         show_definite_integral();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'g':         show_inte();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'h':         show_Nth_power();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'i':         show_add2multi();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'j':         show_multiply();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'z':         cout << "谢谢使用" << endl;
        system("clr");
        return;
        break;
    default:
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    }
}
int main() {
         multi_.clear();
    showthemeau();
    cout << "输入功能字母" << endl;
    char keyboard_in;
    cin >> keyboard_in;
    switchit(keyboard_in);

        }
