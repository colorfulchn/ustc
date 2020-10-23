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
//保存所有的多项式：map保存数据，string提前存进数组里，以后每次query会比较快
vector<map<float, float>> multi_;
vector<string> str_multi_;
//创建一个多项式
void amulti(map<float, float>& oneofmulti) {
    string p;
    string e;
    cout << "请输入系数和幂";
    //当输入#的时候跳出
    //key为幂数，value为系数，采用map因为map自动排序，不采用ordered_map，因为遍历哈希表在显示多项式中不美观（次序没有排序），复杂度为O（logN）
    //当然哈希表的效率会高很多，这里采用map，如果想加快速度，可以采用哈希表，这样在合并多项式的时候，查找的时间复杂度为O(1)，
    while (1) {
        //atoi(input.c_str())
        cin >> p;
        if (p == "#" || e == "#") return;
        cin >> e;
        if (p == "#" || e == "#") return;
        //将p和e转换成int，这里没有考虑数据大小导致超过int的范围，可以使用long long扩大精度
        float p_ = atof(p.c_str());
        float e_ = atof(e.c_str());
        //if(oneofmulti.count(e_))
        //幂数相同，系数相加
        oneofmulti[e_] += p_;
    }
}

//显示多项式
string showmulti(map<float, float> oneofmulti) {
    string res = "";
    for (auto one : oneofmulti)
    {
        //系数为0直接跳过
        if (one.second == 0) continue;
        //运用输出流将int类型转换成string
        stringstream p;
        stringstream e;
        p << one.second;
        e << one.first;
        //如果res为空串或者系数小于0，直接将string相加

        if (res == "" || one.second < 0)
        {
            if (one.first != 0) {
                res += p.str() + "x^" + e.str();
            }
            else {
                res += p.str();
            }
        }
        //系数为正的话，需要加上加号
        else
        {
            if (one.first != 0) {
                res += "+" + p.str() + "x^" + e.str();
            }
            else {
                res += "+" + p.str();
            }
        }
        //res+=itoa(one.second)+"x^"+itoa(one.first);
    }
    //返回结果
    return res;
}

//多项式求和
map<float, float> add2multi(map<float, float> thefirst, map<float, float> thesecond) {
    //分别遍历thefirst和thesecond，并且将其信息记录在ans_multiply
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

//微分
map<float, float> differential(map<float, float> oneofmulti) {
    map<float, float> differential_it;
    for (auto one : oneofmulti)
    {
        float e = one.first;
        float p = one.second;
        //由于在创建多项式的时候已经考虑了合并多项式的问题，这里仅仅考虑指数为1和0的情况
        if (e == 0) continue;
        differential_it[e - 1] = p * e;
        //这里采用在新的空间上保存微分的结果，如果原有结构上进行微分，最好按照指数的从小到大，以免发生数据的覆盖
    }
    return differential_it;
}

//不定积分
map<float, float> integral(map<float, float> oneofmulti) {
    map<float, float> integral_it;
    for (auto one : oneofmulti)
    {
        float e = one.first;
        float p = one.second;
        integral_it[e + 1] += p / (e + 1);//！！！！！这要考虑p/（e+1）为小数，所以采用float类型的map更加好
    }
    //这里采用在新的空间上保存积分的结果，如果原有结构上进行积分，最好按照指数的从大到小，以免发生数据的覆盖
    //return showmulti(integral_it)+"+contant";
    return integral_it;
}

//定积分
float definite_integral(map<float, float> oneofmulti, float low, float high) {
    //直接调用定积分的函数，并且将系数p和指数e分别用low和high求解
    float ans_high = 0;
    float ans_low = 0;
    float sum = 0;
    map<float, float> oneofmulti_;
    oneofmulti_ = integral(oneofmulti);
    for (auto one : oneofmulti_) {
        float e = one.first;
        float p = one.second;
        sum += p * (pow(high, e) - pow(low, e));
        //ans_high += p * pow(high, e);
        //cout << p*pow(high,e) << endl;
        //ans_low += p * pow(low, e);
    }
    return sum;
    //return ans_high - ans_low;
}

//乘法
map<float, float> multiply(map<float, float> thefirst, map<float, float> thesecond) {
    //用thefirst中的每一个元素遍历thesecond的中每一个元素，系数相乘，指数相加。
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

//乘方
//乘方可以用牛顿二项式来简化整个运算过程，具体过程为：先将组合数用杨辉三角（动态规划）做出来，然后每次调用的时候减少时间复杂度；
//下面的方法调用multiply简单写出该函数，但是时间复杂度较高，达到了n*n*m,哎.....；
map<float, float> Nth_power(int m, map<float, float>oneofmulti) {
    map<float, float> temp = oneofmulti;
    while (--m) {
        temp = multiply(temp, oneofmulti);
    }
    return temp;
}

//显示当前所有的多项式
void show_all_multi() {
    //直接遍历全局变量str_multi_，并且输出
    cout << "当前存储的所有多项式为：" << endl;
    int i = 0;
    for (auto s : str_multi_)
    {
        cout << i << ":   " << s << endl;
        ++i;
    }
}

//保存
void savethemulti(map<float, float> oneofmulti) {
    //将新的多项式直接加载str_multi_和multi_中
    multi_.push_back(oneofmulti);
    str_multi_.push_back(showmulti(oneofmulti));
    return;
}

//删除
void deletemulti() {
    int index = 1;
    //考虑性能，使用哈希set
    unordered_set <int> deletelist;
    //记录所有被删除多项式的index
    while (index >= 0) {
        cin >> index;
        //如果删除的index超过数组长度，则不做处理。
        if (index >= (int)multi_.size())
            continue;
        else
            deletelist.insert(index);
    }
    //创建新的vector保存未删除的多项式
    vector<map<float, float>> tmp;
    vector<string> tmp1;
    for (int i = 0; i < (int)multi_.size(); ++i)
    {
        if (deletelist.count(i))
            continue;
        else
        {
            tmp.push_back(multi_[i]);
            tmp1.push_back(str_multi_[i]);
        }
    }
    multi_.clear();
    multi_ = tmp;
    str_multi_.clear();
    str_multi_ = tmp1;
    return;
}

//展示主界面
void showthemeau() {
    system("clear");
    cout << "功能：" << endl;
    cout << "a：创建新的多项式：" << endl;
    cout << "b：显示当前所有的多项式" << endl;
    cout << "c：删除多项式" << endl;
    cout << "d：显示第n个多项式" << endl;

    cout << "e：微分" << endl;
    cout << "f：定积分" << endl;
    cout << "g：不定积分" << endl;
    cout << "h：乘方" << endl;

    cout << "i：求和" << endl;
    cout << "j：乘法" << endl;
    cout << "z：退出计算器" << endl;
}

//创建多项式的界面
void show_amulti() {
    system("clear");
    cout << "请输入输入多项式的个数（返回主界面按0以后敲回车）：" << endl;
    int num = 0;
    cin >> num;
    if (num <= 0)
        return showthemeau();

    cout << "请输入多项式（以'#'结束）：" << endl;
    cout << "示例：3x^4 - 0.5x^0.6 + 7" << endl;
    cout << "输入：3 4 -0.5 0.6 7 0 #" << endl;
    //调用创建多项式的函数
    for (int i = 0; i < num; ++i) {
        map<float, float> tmp;
        amulti(tmp);
        savethemulti(tmp);
    }
    return showthemeau();
}

//删除多项式的界面
void show_delete_multi() {
    system("clear");
    show_all_multi();
    cout << "请输入你要删除的多项式的序号(以-1结束，示例：0 -1敲回车)：" << endl;
    deletemulti();
    cout << "删除以后的多项式为：" << endl;
    show_all_multi();
    cout << "按任何键返回主界面" << endl;
    char keyboard_in;
    cin >> keyboard_in;
    return showthemeau();
}

//显示特定的多项式界面
void show_n_multi() {
    system("clear");
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

//显示微分界面
void show_diff() {
    system("clear");
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
    //else
    return showthemeau();
}

//显示不定积分界面
void show_inte() {
    system("clear");
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
    //else
    return showthemeau();
}

//显示定积分的界面
void show_definite_integral() {
    system("clear");
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

//显示乘方界面
void show_Nth_power() {
    system("clear");
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
    //else
    return showthemeau();
}

//显示乘法界面
void show_multiply() {
    system("clear");
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
    //else
    return showthemeau();
}

//显示求和界面add2multi
void show_add2multi() {
    system("clear");
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
    //else
    return showthemeau();

}

//控制菜单
void switchit(char keyboard_in) {
    switch (keyboard_in) {
    case 'a'://a：创建新的多项式
        show_amulti();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'b'://b：显示当前所有的多项式
        system("clear");
        show_all_multi();
        cout << "输入任意键敲回车返回主界面" << endl;
        cin >> keyboard_in;
        showthemeau();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'c'://c：删除多项式
        show_delete_multi();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'd'://d：显示第n个多项式
        show_n_multi();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'e'://e：微分
        show_diff();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'f'://f：定积分
        show_definite_integral();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'g'://g：不定积分
        show_inte();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'h'://h：乘方
        show_Nth_power();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'i'://i：求和
        show_add2multi();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'j'://j：乘法
        show_multiply();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    case 'z'://z：退出计算器
        cout << "谢谢使用" << endl;
        system("clear");
        return;
        break;
    default:
        system("clear");
        cout << "输入错误,并且输入任意键按回车返回主界面" << endl;
        cin >> keyboard_in;
        showthemeau();
        cin >> keyboard_in;
        switchit(keyboard_in);
        break;
    }
}
int main() {
    //调试
    multi_.clear();
    showthemeau();
    cout << "输入功能字母" << endl;
    char keyboard_in;
    cin >> keyboard_in;
    switchit(keyboard_in);

    //show_amulti();
    //show_delete_multi();
    //show_n_multi();
    //show_diff();
    //show_inte();
    //show_Nth_power();
    //show_add2multi();
    //show_all_multi();
    //definite_integral();
    /*
    vector<map<float,float>> multi_;
    map<float,float>tmp;
    amulti(tmp);
    map<float,float>tmp2;
    amulti(tmp2);
    multi_.push_back(tmp);
    multi_.push_back(tmp2);
    cout << "第一个多项式为"<<showmulti(multi_[0])<<endl;
    cout << "第二个多项式为"<<showmulti(multi_[1])<<endl;
    */
    //cout << "两个多项式的乘法为"<<showmulti(multiply(multi_[0],multi_[1]))<<endl;
    //cout << "乘方"<< showmulti(Nth_power(2,multi_[0]))<<endl;
    //cout << "微分"<< showmulti(differential(multi_[0]))<<endl;
    //cout << "不定积分"<< showmulti(integral(multi_[0]))+"+contant"<<endl;
    //cout << "定积分"<<definite_integral(multi_[0],1,2)<<endl;
}
