#include "receiver.h"
using namespace std;
#include <vector>
#include <string>
#include "RBT.h"
// #include "RBT.h"
#include "RBT2.h"


char change(char direction){
    if(direction == 'b'){return 's';}
    else{return 'b';}
}
int dir(char direction){
    if(direction == 'b'){return 1;}
    else{return -1;}
}
void recursive(vector<int>a,int &sum,int index,vector<int>&path,vector<vector<int>>&result){
    if(sum == 0){
        bool check = 0;
        for(auto it = result.begin();it != result.end();it++){
            if(*it == path){
                check = 1;
                break;
            }
        }
        if(check == 0){
            result.push_back(path);
        }
    }
    if(index == -1){
        return;
    }
    path.push_back(index);
    sum+=a[index];
    recursive(a,sum,index - 1,path,result);
    path.pop_back();
    sum-=a[index];
    recursive(a,sum,index - 1,path,result);
}
vector<int>Arbitrage(RBT2 tree,vector<int>prices){
    //cout << "Hello" << endl;
    stock* root = tree.root;
    vector<int>a = root->quantity;
    int n = a.size();
    vector<vector<int>>result;
    int sum = a[n-1];
    vector<int>path = {n-1};
    recursive(a,sum,n-2,path,result);
    vector<vector<int>>true_result;
    int max = 0;
    for(auto j : result){
        bool check1 = 0;
        for(auto k : tree.preorder()){
            int sum = 0; 
            for(auto i : j){
                sum += k->quantity[i];
            }
            if(sum != 0){
                check1 = 1;
            }
        }
        if(check1 == 0){
            true_result.push_back(j);
        }
    }
    if(true_result.size() == 0){
        return {};
    }
    int index = 0;
    for(int j = 0;j < true_result.size();j++){
        int price_sum = 0;
        for(int i : true_result[j]){
            price_sum += prices[i];
        }
        if(max < price_sum){
            
            max = price_sum;
            index = j;
        }
    }
    if(max == 0){
        return {};
    }
    //cout << max;
    return true_result[index];
}
vector<pair<string,int>>merge_sort(vector<pair<string,int>>arr){
    if(arr.size() == 1){return arr;}
    vector<pair<string,int>>first,second,outcome1,outcome2,outcome;
    for(int i = 0;i <arr.size();i++){
        if(i <arr.size()/2){
            first.push_back(arr[i]);
        }
        else{
            second.push_back(arr[i]);
        }
    }
    outcome1 = merge_sort(first);
    outcome2 = merge_sort(second);
    int i = 0;int j = 0;
    while(i < outcome1.size() && j < outcome2.size()){
        if(outcome1[i].first < outcome2[j].first){
            outcome.push_back(outcome1[i]);
            i++;
        }
        else{
            outcome.push_back(outcome2[j]);
            j++;
        }
    }
    while(i < outcome1.size()){
        outcome.push_back(outcome1[i]);
        i++;
    }
    while(j < outcome2.size()){
        outcome.push_back(outcome2[j]);
        j++;
    }
    return outcome;
}
string sort(string lin_comb){
    string outcome = "";
    int a = 0;
    vector<pair<string,int>>intermediate;
    while(lin_comb.find(' ',a) != lin_comb.find_last_of(' ')){
        int first = lin_comb.find(' ',a);
        string stock = lin_comb.substr(a,first-a);
        int second = lin_comb.find(' ',first+1);
        int quantity = stoi(lin_comb.substr(first+1,second-first-1));
        a = second + 1;
        intermediate.push_back({stock,quantity});
    }
    int first = lin_comb.find(' ',a);
    string stock = lin_comb.substr(a,first-a);
    int quantity = stoi(lin_comb.substr(first+1,lin_comb.size()-first-1));
    intermediate.push_back({stock,quantity});
    vector<pair<string,int>>result = merge_sort(intermediate);
    for(auto i : result){
        outcome = outcome + i.first + " " + to_string(i.second) + " ";
    }
    outcome.pop_back();
    return outcome;
}
int main (int argc,char **argv ){
    int input = stoi(argv[1]);
   if( input == 1){
    Receiver rcv;
    //sleep(5);//Broken pipe error dealing. There will be lag if we decomment this.
    string company;
    int price;
    char direction;
    RBT mytree;
    string line = "";
    bool foundDollar = false;
    while (!foundDollar)
    {
        string message = rcv.readIML();
        int i = 0;
        while(i < message.size()){
            if (message[i] == '#'){ //
                direction = line[line.size() - 1];
                int last1 = line.size() - 2;
                string medival = line.substr(0,last1);
                int last2 = medival.find_last_of(' ');
                company = medival.substr(0,last2);
                price = stoi(medival.substr(last2+1,last1-last2-1));
                Node *q = mytree.find(company);
                if(q == NULL){
                    mytree.insert(company,price);
                    cout << company << " " << price << " " << change(direction) << endl;
                }
                else{
                    if(direction == 'b'){
                        if(q->buy_price.size() != 0 && q->buy_price[0] >= price){
                            cout << "No Trade" << endl;
                        }
                        else if(q->sell_price.size() != 0 && q->sell_price[0] == price){
                        if(q->buy_price.size() != 0 && q->buy_price[0] < price){q->buy_price.clear();}
                            cout << "No Trade" << endl;
                            q->sell_price.clear();
                        }
                        else if(price > q->Stockprice){
                            q->Stockprice = price;
                            cout << company << " " << price << " s" << endl;
                            q->buy_price.clear();
                        }
                        else {
                            cout << "No Trade" << endl;
                            q->buy_price.clear();
                            q->buy_price.push_back(price);
                        }
                    }
                    else{
                        if(q->sell_price.size() != 0 && q->sell_price[0] <= price){
                            cout << "No Trade" << endl;
                        }
                        else if(q->buy_price.size() != 0 && q->buy_price[0] == price){
                        if(q->sell_price.size() != 0 && q->sell_price[0] > price){q->sell_price.clear();}
                            cout << "No Trade" << endl;
                            q->buy_price.clear();
                        }
                        else if(price < q->Stockprice){
                            q->Stockprice = price;
                            cout << company << " " << price << " b" << endl;
                            q->sell_price.clear();
                        }
                        else{
                            cout << "No Trade" << endl;
                            q->sell_price.clear();
                            q->sell_price.push_back(price);
                        }
                    }
                }
                line = "";
                i += 2;
            }
            else{
                line.push_back(message[i]);
                i++;
            }
        }
        //parse1(message,0);
        if(message.find('$') != string::npos){
            rcv.terminate();
            foundDollar = true;
        }
    }
   }

    else if(input == 2 ){
         Receiver rcv;
    //sleep(5);
    int line_number = 0;
    string line = "";
    string lin_comb;
    int price;
    char direction;
    int profit = 0;
    bool foundDollar = false;
    vector<int>prices;          //prices wrt b
    vector<string>lines;
    RBT tree1;
    RBT2 tree2;
    while(!foundDollar){
        string message = rcv.readIML();
        int i = 0;
        while(i < message.size()){
            if(message[i] == '#'){
                direction = line[line.size() - 1];
                int last1 = line.find_last_of(' ');
                string medival = line.substr(0,last1);
                int last2 = medival.find_last_of(' ');
                lin_comb = sort(medival.substr(0,last2));
                price = stoi(medival.substr(last2+1,last1-last2-1));
                //cout << lin_comb << endl;
                if(tree1.find(lin_comb) == NULL || tree1.find(lin_comb)->buy_price.size() + tree1.find(lin_comb)->sell_price.size() == 0){
                    if(tree1.find(lin_comb) == NULL){tree1.insert(lin_comb,price);}
                    lines.push_back(line);
                    line_number++;
                    if(direction == 'b'){prices.push_back(price);}
                    else{prices.push_back(-1*price);}
                    int q = 0;
                    while(line.find(' ',q) != line.size() - 2){
                        int j = line.find(' ',q);
                        string a = line.substr(q,j - q);
                        int k = line.find(' ',j+1);
                        int b = stoi(line.substr(j+1,k-j-1));
                        if(direction == 's'){b = -1*b;}
                        if(tree2.find(a) != NULL){
                            tree2.find(a)->quantity.push_back(b);
                        }
                        else{
                            tree2.insert(a,b,line_number); 
                        }
                        q = k+1;
                    }
                    for(stock* p : tree2.preorder()){
                        p->quantity.resize(line_number,0);
                    }
                    //cout << line_number << endl;
                    if(Arbitrage(tree2,prices).size() > 0){
                        vector<int>removal = Arbitrage(tree2,prices);
                        
                        for(auto p : removal){
                            //cout << p << endl;
                            profit += prices[p];
                            char direction2 = lines[p][lines[p].size() - 1];
                            int last1 = lines[p].find_last_of(' ');
                            string medival = lines[p].substr(0,last1);
                            int last2 = medival.find_last_of(' ');
                            string test = sort(medival.substr(0,last2));
                            Node*w = tree1.find(test);
                            if(direction2 == 'b'){
                                cout << test << " " << prices[p] << " s" << endl;
                                w->buy_price.clear();
                            }
                            if(direction2 == 's'){
                                cout << test << " " << -prices[p] << " b" << endl;
                                w->sell_price.clear();
                            }
                            line_number--;
                            for(Node* e : tree1.preorder()){
                                if(e->buy_price.size() != 0 && e->buy_num > p + 1){
                                    e->buy_num--;
                                }
                                if(e->sell_price.size() != 0 && e->sell_num > p + 1){
                                    e->sell_num--;
                                }
                            }
                            lines.erase(lines.begin() + p);
                            prices.erase(prices.begin()+p);
                        }
                        for(stock* p : tree2.preorder()){
                            for(auto i : removal){
                                p->quantity.erase(p->quantity.begin() + i);
                            }
                        }
                    }
                    else{
                        cout << "No Trade" << endl;
                        if(direction == 'b'){tree1.find(lin_comb)->buy_price.push_back(price);
                        tree1.find(lin_comb)->buy_num = line_number;}
                        else{tree1.find(lin_comb)->sell_price.push_back(price);
                        tree1.find(lin_comb)->sell_num = line_number;}
                    }
                }
                else{
                    Node*q = tree1.find(lin_comb);
                    if(direction == 's'){
                        if(q->sell_price.size() != 0 && q->sell_price[0] <= price){
                            cout << "No Trade" << endl;
                        }    
                        else if(q->buy_price.size() != 0 && q->buy_price[0] == price){
                            cout << "No Trade" << endl;
                            if(q->sell_price.size() != 0){
                                    line_number--;
                                    lines.erase(lines.begin() + q->sell_num-1);
                                    prices.erase(prices.begin() + q->sell_num - 1);
                                    for(stock* p : tree2.preorder()){
                                        p->quantity.erase(p->quantity.begin() + q->sell_num - 1);
                                    }
                                    for(Node* e : tree1.preorder()){
                                        if(e->buy_price.size() != 0 && e->buy_num > q->sell_num){
                                            e->buy_num--;
                                        }
                                        if(e->sell_price.size() != 0 && e->sell_num > q->sell_num){
                                            e->sell_num--;
                                        }
                                    }
                            }
                            for(stock* p : tree2.preorder()){
                                p->quantity.erase(p->quantity.begin() + q->buy_num - 1);
                            }
                            for(Node* e : tree1.preorder()){
                                if(e->buy_price.size() != 0 && e->buy_num > q->buy_num){
                                    e->buy_num--;
                                }
                                if(e->sell_price.size() != 0 && e->sell_num > q->buy_num){
                                    e->sell_num--;
                                }
                            }
                            q->buy_price.clear();
                            line_number--;
                            prices.erase(prices.begin() + q->buy_num - 1);
                            lines.erase(lines.begin() + q->buy_num - 1);
                        }
                        else{
                            lines.push_back(line);
                            line_number++;
                            if(direction == 'b'){
                                prices.push_back(price);
                            }
                            else{prices.push_back(-1*price);}
                            int iterate = 0;
                            while(line.find(' ',iterate) != line.size() - 2){
                                int j = line.find(' ',iterate);
                                string a = line.substr(iterate,j - iterate);
                                int k = line.find(' ',j+1);
                                int b = stoi(line.substr(j+1,k-j-1));
                                if(direction == 's'){b = -b;}
                                if(tree2.find(a) != NULL){
                                    tree2.find(a)->quantity.push_back(b);  
                                }
                                else{
                                    tree2.insert(a,b,line_number); 
                                }
                                iterate = k+1;
                            }
                            for(stock* p : tree2.preorder()){
                                p->quantity.resize(line_number,0);
                            }
                            if(Arbitrage(tree2,prices).size() > 0){
                                vector<int>removal = Arbitrage(tree2,prices);
                                for(auto i : removal){
                                    profit += prices[i];
                                    //cout << lines[i] << endl;
                                    char direction2 = lines[i][lines[i].size() - 1];
                                    int last1 = lines[i].find_last_of(' ');
                                    string medival = lines[i].substr(0,last1);
                                    int last2 = medival.find_last_of(' ');
                                    string test = sort(medival.substr(0,last2));
                                    Node*w = tree1.find(test);
                                    if(direction2 == 'b'){
                                        cout << test << " " << prices[i] << " s" << endl;
                                        w->buy_price.clear();
                                    }
                                    if(direction2 == 's'){
                                        cout << test << " " << -prices[i] << " b" << endl;
                                        w->sell_price.clear();
                                    }
                                    lines.erase(lines.begin() + i);
                                    prices.erase(prices.begin()+i);
                                    line_number--;
                                    for(stock* p : tree2.preorder()){
                                        p->quantity.erase(p->quantity.begin() + i);
                                    }
                                    for(Node* e : tree1.preorder()){
                                        if(e->buy_price.size() != 0 && e->buy_num > i + 1){
                                            e->buy_num--;
                                        }
                                        if(e->sell_price.size() != 0 && e->sell_num > i + 1){
                                            e->sell_num--;
                                        }
                                    }
                                }
                            }
                            else{
                                cout << "No Trade" << endl;
                                
                                if(q->sell_price.size() != 0){
                                    line_number--;
                                    lines.erase(lines.begin() + q->sell_num-1);
                                    prices.erase(prices.begin() + q->sell_num - 1);
                                    for(stock* p : tree2.preorder()){
                                        p->quantity.erase(p->quantity.begin() + q->sell_num - 1);
                                    }
                                    for(Node* e : tree1.preorder()){
                                        if(e->buy_price.size() != 0 && e->buy_num > q->sell_num){
                                            e->buy_num--;
                                        }
                                        if(e->sell_price.size() != 0 && e->sell_num > q->sell_num){
                                            e->sell_num--;
                                        }
                                    }
                                    q->sell_price.clear();
                                }
                                q->sell_price.push_back(price);
                                tree1.find(lin_comb)->sell_num = line_number;
                            }
                        }
                    }
                    else{
                        if(q->buy_price.size() != 0 && q->buy_price[0] >= price){
                            cout << "No Trade" << endl;
                        }
                        else if(q->sell_price.size() != 0 && q->sell_price[0] == price){
                            cout << "No Trade" << endl;
                            if(q->buy_price.size() != 0){
                                    line_number--;
                                    lines.erase(lines.begin() + q->buy_num-1);
                                    prices.erase(prices.begin() + q->buy_num - 1);
                                    for(stock* p : tree2.preorder()){
                                        p->quantity.erase(p->quantity.begin() + q->buy_num - 1);
                                    }
                                    for(Node* e : tree1.preorder()){
                                    if(e->buy_price.size() != 0 && e->buy_num > q->buy_num){
                                        e->buy_num--;
                                    }
                                    if(e->sell_price.size() != 0 && e->sell_num > q->buy_num){
                                        e->sell_num--;
                                    }
                                    }
                                    q->buy_price.clear();
                            }
                            for(stock* p : tree2.preorder()){
                                p->quantity.erase(p->quantity.begin() + q->sell_num - 1);
                            }
                            for(Node* e : tree1.preorder()){
                                if(e->buy_price.size() != 0 && e->buy_num > q->sell_num){
                                    e->buy_num--;
                                }
                                if(e->sell_price.size() != 0 && e->sell_num > q->sell_num){
                                    e->sell_num--;
                                }
                            }
                            q->sell_price.clear();
                            line_number--;
                            prices.erase(prices.begin() + q->sell_num - 1);
                            lines.erase(lines.begin() + q->sell_num - 1);
                        }
                        else{
                            lines.push_back(line);
                            line_number++;
                            if(direction == 'b'){
                                prices.push_back(price);
                            }
                            else{prices.push_back(-1*price);}
                            int iterate = 0;
                            while(line.find(' ',iterate) != line.size() - 2){
                                int j = line.find(' ',iterate);
                                string a = line.substr(iterate,j - iterate);
                                int k = line.find(' ',j+1);
                                int b = stoi(line.substr(j+1,k-j-1));
                                if(direction == 's'){b = -b;}
                                if(tree2.find(a) != NULL){
                                    tree2.find(a)->quantity.push_back(b);  
                                }
                                else{
                                    tree2.insert(a,b,line_number); 
                                }
                                iterate = k+1;
                            }
                            for(stock* p : tree2.preorder()){
                                p->quantity.resize(line_number,0);
                            }
                            if(Arbitrage(tree2,prices).size() > 0){
                                vector<int>removal = Arbitrage(tree2,prices);
                                for(auto i : removal){
                                    profit += prices[i];
                                    //cout << lines[i] << endl;
                                    char direction2 = lines[i][lines[i].size() - 1];
                                    int last1 = lines[i].find_last_of(' ');
                                    string medival = lines[i].substr(0,last1);
                                    int last2 = medival.find_last_of(' ');
                                    string test = sort(medival.substr(0,last2));
                                    Node*w = tree1.find(test);
                                    cout << test << " " << prices[i]*dir(direction2) << " " << change(direction2) << endl;
                                    if(direction2 == 'b'){
                                        w->buy_price.clear();
                                    }
                                    if(direction2 == 's'){
                                        w->sell_price.clear();
                                    }
                                    lines.erase(lines.begin() + i);
                                    prices.erase(prices.begin()+i);
                                    line_number--;
                                    for(stock* p : tree2.preorder()){
                                        p->quantity.erase(p->quantity.begin() + i);
                                    }
                                    for(Node* e : tree1.preorder()){
                                        if(e->buy_price.size() != 0 && e->buy_num > i + 1){
                                            e->buy_num--;
                                        }
                                        if(e->sell_price.size() != 0 && e->sell_num > i + 1){
                                            e->sell_num--;
                                        }
                                    }
                                }
                            }
                            else{
                                cout << "No Trade" << endl;
                                
                                if(q->buy_price.size() != 0){
                                    line_number--;
                                    lines.erase(lines.begin() + q->buy_num-1);
                                    prices.erase(prices.begin() + q->buy_num - 1);
                                    for(stock* p : tree2.preorder()){
                                        p->quantity.erase(p->quantity.begin() + q->buy_num - 1);
                                    }
                                    for(Node* e : tree1.preorder()){
                                    if(e->buy_price.size() != 0 && e->buy_num > q->buy_num){
                                        e->buy_num--;
                                    }
                                    if(e->sell_price.size() != 0 && e->sell_num > q->buy_num){
                                        e->sell_num--;
                                    }
                                    }
                                    q->buy_price.clear();
                                }
                                q->buy_price.push_back(price);
                                tree1.find(lin_comb)->buy_num = line_number;
                            }
                        }   
                    }
                }
                line = "";
                i+=2;
            }
            else if(message[i] != '$'){
                line += message[i];
                i++;
            }
            // else{
            //     cout << profit << endl;
            // }
        }
        if(message.find('$') != string::npos){
            cout << profit << endl;
            rcv.terminate();
            foundDollar = true;
        }
    }
    }


    return 0;
}
