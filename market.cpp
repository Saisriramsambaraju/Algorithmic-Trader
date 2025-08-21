#include "market.h"
#include "priority.h"
#include "map.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

market::market(int argc, char **argv)
{
}


int safe_stoi(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() <<  std::endl;
    }
    return 0; // Or a default value suitable for your logic
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

string sort_string(string lin_comb){
    string outcome = "";
    int a = 0;
    vector<pair<string,int>>intermediate;
    while(lin_comb.find(' ',a) != lin_comb.find_last_of(' ')){
        int first = lin_comb.find(' ',a);
        string stock = lin_comb.substr(a,first-a);
        int second = lin_comb.find(' ',first+1);
        int quantity = safe_stoi(lin_comb.substr(first+1,second-first-1));
        a = second + 1;
        intermediate.push_back({stock,quantity});
    }
    int first = lin_comb.find(' ',a);
    string stock = lin_comb.substr(a,first-a);
    int quantity = safe_stoi(lin_comb.substr(first+1,lin_comb.size()-first-1));
    intermediate.push_back({stock,quantity});
    vector<pair<string,int>>result = merge_sort(intermediate);
    for(auto i : result){
        outcome = outcome + i.first + " " + to_string(i.second) + " ";
    }
    outcome.pop_back();
    return outcome;
}

std::vector<std::string> splitString(const std::string &inputString, char delimiter = ' ')
{
    std::vector<std::string> resultList;
    std::size_t start = 0;
    std::size_t end = inputString.find(delimiter);

    while (end != std::string::npos)
    {
        resultList.push_back(inputString.substr(start, end - start));
        start = end + 1;
        end = inputString.find(delimiter, start);
    }
    // Push the last substring into the vector
    resultList.push_back(inputString.substr(start, end));
    return resultList;
}
// string sort(string lin_comb);
std::string string_modifier(vector<string> line_vector)
{
    // string sorted_Company = line_vector[3];
    std::string sorted_Company="";
    if(line_vector.size()>7){
    for(int i=3;i<line_vector.size()-3;i++){
        sorted_Company =sorted_Company + line_vector[i] + " ";
        // sorted_Company+=" ";
    }
    sorted_Company.pop_back();
    sorted_Company = sort_string(sorted_Company);
    }
    else sorted_Company=line_vector[3];


    return sorted_Company;
}





void market::start()
{
    std::ifstream outputFile("output.txt");
    std::string line;
    Map<string, pq> buy_map;
    Map<string, pq> sell_map;
    // std::cout<<"SI!";/
    std::getline(outputFile, line);
    int i = 1;
    int num_trades = 0;
    int money_transfered = 0;
    int num_shares_traded = 0;
    Map<string,broker_details> Bdetails;
    while (std::getline(outputFile, line))
    {
        i++;
        // if(!std::getline(outputFile, line)) break;
        //  std::cout<<line<<endl;
        if (line == "!@")
            break;
        // if (step != 0)

        {
            // std::cout<<line<<endl;
            vector<string> line_vector = splitString(line); // This is used to split the string into its contents;
            int size_of_line = line_vector.size();
            // std::cout<<size_of_line<<endl;
            //  std::cout<<line_vector[2]<<endl;
            // }
            broker nn;
            string company = string_modifier(line_vector);
            nn.Broker_name = line_vector[1];      // Broker_name
            nn.start_time = safe_stoi(line_vector[0]); // start_time<
            // cout<<nn.start_time<<endl;
            //   std::cout<<line_vector[size_of_line-1]<<" ";
            // std::cout<<line_vector[size_of_line-1].size()<<" ";
            bool a = (safe_stoi(line_vector[size_of_line - 1]) == -1);
            // bool a=1;
            // std::cout<<a;
            if (a)
                nn.end_time = -1;
            else
                nn.end_time = safe_stoi(line_vector[size_of_line - 1]) + safe_stoi(line_vector[0]);
            // the above is for end_time.
            nn.Quantity = safe_stoi(line_vector[size_of_line - 2].substr(1)); // Quantity
            nn.Price = safe_stoi(line_vector[size_of_line - 3].substr(1));    // Price
            bool buy = line_vector[2] == "BUY" ? true : false;           // Bool for deciding buy or sell.
            //    std::cout<<i<<" endtime:"<<nn.end_time<<" buy:"<<buy<<endl;
            //  std::cout<<line_vector[2]<<" ";

            //    std::cout<<buy<<endl;
            // std::cout<<"Hi";
            if (buy) // if the order is a buy order
            {
                if (sell_map.empty()) // if the sell_map is empty then just add the order into
                {
                    // std::cout<<nn.Broker_name<<" and "<<company<<" is inserted in buy_map when sell_map is empty"<<endl;
                    buy_map[company].insertNode(nn);
                    buy_map[company].isMinHeap = false; // Buy orders are kept in max heap so that the seller gets the maximum profit.
                }
                else
                {
                    if (sell_map.find(company) && sell_map[company].size != 0) // This if statement checks wheather the company is present in the sell_map , if it is present then is the priority queue not empty.
                    {
                        while (sell_map[company].size != 0 && nn.Quantity != 0 && sell_map[company].get_top().Price <= nn.Price) // This condition evaluates wheather the pq is not empty and the buy quantity is not 0 and the sell price is less than buyer's limit.
                        {
                            broker b = sell_map[company].get_top();
                            if (b.end_time != -1 && b.end_time < nn.start_time)
                            {
                                sell_map[company].pop_max();
                            }
                            else
                            {
                                if (b.Quantity > nn.Quantity)
                                {
                                    b.Quantity -= nn.Quantity;
                                    std::cout << nn.Broker_name << " purchased " << nn.Quantity << " share of " << company << " from " << b.Broker_name << " for $" << b.Price << "/share" << endl;
                                    // if(Bdetails.find(nn.Broker_name)!=Bdetails.end()){
                                        // if(Bdetails.find(nn.Broker_name)!=Bdetails.end()){Bdetails.insert(nn.Broker_name);}
                                        Bdetails[nn.Broker_name].bought+=nn.Quantity;
                                        Bdetails[nn.Broker_name].net_transfer+= -1*nn.Quantity*b.Price;
                                        // if(Bdetails.find(b.Broker_name)!=Bdetails.end()){Bdetails.insert(b.Broker_name);}
                                        Bdetails[b.Broker_name].sold+=nn.Quantity;
                                        Bdetails[b.Broker_name].net_transfer+=nn.Quantity*b.Price;
                                    sell_map[company].arr[0].Quantity = b.Quantity;
                                    num_shares_traded += nn.Quantity;
                                    num_trades++;
                                    money_transfered += nn.Quantity * b.Price;
                                    nn.Quantity = 0;
                                    break;
                                }
                                if (b.Quantity <= nn.Quantity)
                                {
                                    nn.Quantity -= b.Quantity;
                                    sell_map[company].pop_max();
                                    std::cout << nn.Broker_name << " purchased " << b.Quantity << " share of " << company << " from " << b.Broker_name << " for $" << b.Price << "/share" << endl;
                                        // if(Bdetails.find(nn.Broker_name)!=Bdetails.end()){Bdetails.insert(nn.Broker_name);}
                                        Bdetails[nn.Broker_name].bought+=b.Quantity;
                                        Bdetails[nn.Broker_name].net_transfer+= -1*b.Quantity*b.Price;
                                        // if(Bdetails.find(b.Broker_name)!=Bdetails.end()){Bdetails.insert(b.Broker_name);}
                                        Bdetails[b.Broker_name].sold+=b.Quantity;
                                        Bdetails[b.Broker_name].net_transfer+=b.Quantity*b.Price;

                                    num_shares_traded += b.Quantity;
                                    num_trades++;
                                    money_transfered += b.Quantity * b.Price;
                                    if (nn.Quantity != 0)
                                    {
                                        continue;
                                    }
                                    else
                                        break;
                                }
                            }
                        }
                    }
                    if (nn.Quantity != 0)
                    {
                        // std::cout<<nn.Broker_name<<" and "<<company<<" is inserted in buy_map"<<endl;
                        buy_map[company].insertNode(nn);
                        if (buy_map[company].size == 1)
                            buy_map[company].isMinHeap = false;
                    }
                }
            }
            // //Buy case covered !!
            if (!buy)
            {
                if (buy_map.empty())
                {
                    // std::cout<<nn.Broker_name<<" and "<<company<<" is inserted in sell_map when buy_map is empty"<<endl;
                    sell_map[company].insertNode(nn);
                    sell_map[company].isMinHeap = true;
                }
                else
                {
                    if (buy_map.find(company) && buy_map[company].size != 0) // This if statement checks wheather the company is present in the buy_map , if it is present then is the priority queue not empty.
                    {
                        while (buy_map[company].size != 0 && nn.Quantity != 0) // This condition evaluates wheather the pq is not empty and the buy quantity is not 0 and the sell price is less than buyer's limit.
                        {
                            broker b = buy_map[company].get_top();
                            if (b.Price < nn.Price)
                                break;
                            if (((b.end_time != -1) && (b.end_time < nn.start_time)))
                            {
                                buy_map[company].pop_max();
                                continue;
                            }
                            else
                            {
                                if (b.Quantity > nn.Quantity)
                                {
                                    b.Quantity -= nn.Quantity;
                                    std::cout << b.Broker_name << " purchased " << nn.Quantity << " share of " << company << " from " << nn.Broker_name << " for $" << b.Price << "/share" << endl;
                                        // if(Bdetails.find(nn.Broker_name)!=Bdetails.end()){Bdetails.insert(nn.Broker_name);}
                                        Bdetails[nn.Broker_name].sold+=nn.Quantity;
                                        Bdetails[nn.Broker_name].net_transfer+= nn.Quantity*b.Price;
                                        // if(Bdetails.find(b.Broker_name)!=Bdetails.end()){Bdetails.insert(b.Broker_name);}
                                        Bdetails[b.Broker_name].bought+=nn.Quantity;
                                        Bdetails[b.Broker_name].net_transfer+=-1*nn.Quantity*b.Price;
                                    buy_map[company].arr[0].Quantity = b.Quantity;    
                                    num_shares_traded += nn.Quantity;
                                    num_trades++;
                                    money_transfered += nn.Quantity * b.Price;
                                    nn.Quantity = 0;
                                    break;
                                }
                                if (b.Quantity <= nn.Quantity)
                                {
                                    nn.Quantity -= b.Quantity;
                                    buy_map[company].pop_max();
                                    std::cout << b.Broker_name << " purchased " << b.Quantity << " share of " << company << " from " << nn.Broker_name << " for $" << b.Price << "/share" << endl;
                                        // if(Bdetails.find(nn.Broker_name)!=Bdetails.end()){Bdetails.insert(nn.Broker_name);}
                                        Bdetails[nn.Broker_name].sold+=b.Quantity;
                                        Bdetails[nn.Broker_name].net_transfer+= b.Quantity*b.Price;
                                        // if(Bdetails.find(b.Broker_name)!=Bdetails.end()){Bdetails.insert(b.Broker_name);}
                                        Bdetails[b.Broker_name].bought+=b.Quantity;
                                        Bdetails[b.Broker_name].net_transfer+=-1*b.Quantity*b.Price;

                                    num_shares_traded += b.Quantity;
                                    num_trades++;
                                    money_transfered += b.Quantity * b.Price;
                                    if (nn.Quantity != 0)
                                    {

                                        continue;
                                    }
                                    else
                                        break;
                                }
                            }
                        }
                    }
                    if (nn.Quantity != 0)
                    {
                        // std::cout<<nn.Broker_name<<" and "<<company<<" is inserted in sell_map"<<endl;
                        sell_map[company].insertNode(nn);
                        if (sell_map[company].size == 1)
                            sell_map[company].isMinHeap = true;
                    }
                }
            }
            // Sell case covered !!
        }
    }
    std::cout << endl;
    std::cout << "---End of Day---" << endl;
    std::cout << "Total Amount of Money Transferred: $" << money_transfered << endl;
    std::cout << "Number of Completed Trades: " << num_trades << endl;
    std::cout << "Number of Shares Traded: " << num_shares_traded << endl;
    
//The below code is to print the trades done by each broker.
    for (auto it : Bdetails) {
    std::cout << it.first << " bought " << it.second.bought << " and sold " << it.second.sold << " for a net transfer of $" << it.second.net_transfer << std::endl;
    }
}
