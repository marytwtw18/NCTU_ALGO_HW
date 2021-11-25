#include<iostream>
#include<string>
#include<climits>
#include<fstream>
#include<algorithm>
#include<vector>
#include<map>

using namespace std;

//---------------------2d interval tree------------------------------------------
struct node
{
    string rec_ID;
    int x_start,x_end;
    int y_start,y_end;
    int area;
};
typedef struct node Node;

void print_tree(map<string,Node> &interval_tree)
{
    for(const auto& m : interval_tree){
        cout << "ID:" << m.first << endl;
        cout << " X range:" <<  m.second.x_start<<"~"<<m.second.x_end<<endl;
        cout << " Y range:" <<  m.second.y_start<<"~"<<m.second.y_end<<endl;
        cout << " Area:" <<  m.second.area<<endl;
    }
}

//S mode
vector<string> search_tree(map<string,Node> &interval_tree,int x_start,int x_end,int y_start,int y_end)
{
    vector<string> return_id;
    for(const auto& m : interval_tree){
        if( (m.second.x_start == x_start) && (m.second.x_end == x_end) && (m.second.y_start == y_start) &&(m.second.y_end == y_end))
        {
            return_id.push_back(m.first);
        }
    }  
    return  return_id;
}
//D mode
void delete_node(map<string,Node> &interval_tree,string str)
{
    interval_tree.erase(str);
}
//A mode
vector<string> return_min_area_node(map<string,Node> &interval_tree,int x_start,int y_start)
{
    vector<string> id,return_id;
    int min_area = INT_MAX;
    //find the rectangle that is the same as the given coordinate
    for(const auto& m : interval_tree){
        if((m.second.x_start == x_start) && (m.second.y_start == y_start))
        {
            id.push_back(m.first);
        }
    }  
    for(int i=0;i<id.size();++i){
        if(interval_tree[id[i]].area < min_area)
        {
            min_area = interval_tree[id[i]].area;
        }
    }
    
    for(int i=0;i<id.size();++i){
        if(interval_tree[id[i]].area == min_area)
        {
            return_id.push_back(id[i]);
        }
    }
    
    return  return_id;
}

/*--------O mode--------*/
//----find overlap node----
vector<string> find_overlap_node(map<string,Node> &interval_tree,int x_start,int x_end,int y_start,int y_end)
{
    vector<string> return_id;
    for(const auto& m : interval_tree){
        if((m.second.x_start<=x_start) &&(m.second.y_start<=y_start) && (m.second.x_end>=x_end) && (m.second.y_end>=y_end))
        {
            return_id.push_back(m.first);
        }
    }
      
    return  return_id;
}

//Design rule check
void DRC(map<string,Node> &interval_tree,vector<string>& over_id,int x_start,int x_end,int y_start,int y_end)
{
    //if a.higher.x < b.lower.x && a.lower.y< b.higher.y design rule violate
    for(int i=0;i<over_id.size();++i){
        if( (x_start-interval_tree[over_id[i]].x_start) < 20){
            cout <<  over_id[i] <<" violate"<<endl;
        }
        else if( (y_start-interval_tree[over_id[i]].y_start) < 20){
            cout <<  over_id[i] <<" violate"<<endl;
        }
        else if( (interval_tree[over_id[i]].x_end - x_end) < 20){
            cout <<  over_id[i] <<" violate"<<endl;
        }
        else if( (interval_tree[over_id[i]].y_end - y_end) < 20){
            cout <<  over_id[i] <<" violate"<<endl;
        }
        else{
            cout <<  over_id[i] << endl;
        }
    }
}


/*----------------------other functions that used------------------------------*/
void splitStr2Vec(string str, vector<string>& buf);

int max_num = 0;

//sort the vector
void counting_sort(vector<int> &index)
{
    vector<int> sort(index.size());
    vector<int> count(max_num +1);
    for(int i=0;i<max_num ;++i){
        count[i] = 0;
    }
    for(int i=0;i<index.size();++i){
        ++count[index[i]];
    }
    for(int i=0;i<max_num ;++i){
        count[i+1] +=  count[i];
    }
    for(int i=0;i<index.size();++i){
        sort[--count[index[i]]] = index[i];
    }
    
    for(int i=0; i<index.size();++i){
        index[i] = sort[i];
    }
}


int main(int argc,char* argv[])
{
/*---------------varaiable declaration----------------------------------*/
    
    //read write file
    ifstream infile;
    string instr; 
    infile.open(argv[1]);
    ofstream outfile;
    outfile.open(argv[2]);
    
    //map structure
    map<string,Node> interval_tree;
    int x_start,x_end;
    int y_start,y_end;
    
/*------------------------read file-------------------------------------*/
    
    if(!infile.is_open())
    {
        cout << "fail to open" << endl;
    }
    else
    {
        while(!infile.eof()){ //read in
            vector<string> new_vec;
            getline(infile,instr);
            splitStr2Vec(instr,new_vec);
            
            if(new_vec.empty()){
                break;
            }
            else if( new_vec[0] == "I"){
                Node new_node;
                new_node.rec_ID = new_vec[1];
                new_node.x_start = stoi(new_vec[2]);
                new_node.y_start = stoi(new_vec[3]);
                new_node.x_end = stoi(new_vec[4]);
                new_node.y_end = stoi(new_vec[5]);
                new_node.area = (new_node.x_end-new_node.x_start)*(new_node.y_end-new_node.y_start);
                interval_tree[new_node.rec_ID] = new_node;
            }
            else if(new_vec[0] == "D"){
                //find the same node and delete
                vector<string> str;
                x_start =  stoi(new_vec[1]);
                y_start =  stoi(new_vec[2]);
                x_end =  stoi(new_vec[3]);
                y_end = stoi(new_vec[4]); 
                str = search_tree(interval_tree,x_start,x_end,y_start,y_end); //find the node to delete
                
                string new_str;int item;
                vector<int> index;
                for(int i=0;i<str.size();++i){
                    new_str = new_str.assign(str[i],1,str[i].size()-1);
                    item = stoi(new_str);
                    index.push_back(item);
                }
                sort(index.begin(),index.end());
                new_str = "r" + to_string(index[0]);
                
                delete_node(interval_tree,new_str);
            }
            else if(new_vec[0] == "S"){
                //find and return the rectangle id
                vector<string> str,str2;
                x_start =  stoi(new_vec[1]);
                y_start =  stoi(new_vec[2]);
                x_end =  stoi(new_vec[3]);
                y_end = stoi(new_vec[4]);
                str = search_tree(interval_tree,x_start,x_end,y_start,y_end);
                
                string new_str;int item;
                vector<int> index;
                for(int i=0;i<str.size();++i){
                    new_str = new_str.assign(str[i],1,str[i].size()-1);
                    item = stoi(new_str);
                    index.push_back(item);
                }
                sort(index.begin(),index.end());
                for(int i=0;i<str.size();++i){
                    new_str = "r" + to_string(index[i]);
                    str2.push_back(new_str);
                }
                
                //print the data
                outfile <<"S"<<endl;
                for(int i=0;i<str2.size();++i){
                    outfile << str2[i]<<endl;
                }
                
            }
            else if(new_vec[0] == "O"){
                //find the overlap rectangle and check the design rule
                vector<string> overlap_id,sort_overlap_id;
                vector<int> index;int new_num;
                string new_str;
                x_start =  stoi(new_vec[1]);
                y_start =  stoi(new_vec[2]);
                x_end =  stoi(new_vec[3]);
                y_end = stoi(new_vec[4]);
                overlap_id = find_overlap_node(interval_tree,x_start,x_end,y_start,y_end);
                
                for(int i=0;i<overlap_id.size();++i){    
                    new_str = new_str.assign(overlap_id[i],1,overlap_id[i].size()-1);
                    new_num = stoi(new_str);
                    index.push_back(new_num);
                    if(new_num > max_num ){
                        max_num  = new_num;
                    }
                }
                
                vector<int> sort(index.size());
                counting_sort(index);
                for(int i=0;i<index.size();++i){
                    overlap_id[i] = "r" + to_string(index[i]);
                }
                outfile << "O"<<endl;
                //DRC(interval_tree,overlap_id,x_start,x_end,y_start,y_end);
                for(int i=0;i<overlap_id.size();++i){
                    if( (x_start-interval_tree[overlap_id[i]].x_start) < 20){
                        outfile <<  overlap_id[i] <<" violate"<<endl;
                    }
                    else if( (y_start-interval_tree[overlap_id[i]].y_start) < 20){
                        outfile <<  overlap_id[i] <<" violate"<<endl;
                    }
                    else if( (interval_tree[overlap_id[i]].x_end - x_end) < 20){
                        outfile <<  overlap_id[i] <<" violate"<<endl;
                    }
                    else if( (interval_tree[overlap_id[i]].y_end - y_end) < 20){
                        outfile <<  overlap_id[i] <<" violate"<<endl;
                    }
                    else{
                        outfile <<  overlap_id[i] << endl;
                    }
                }
            }
            else if(new_vec[0] == "A"){
                //return the min area with lower left corner
                vector<string> min_id,str;
                vector<int> index;int item;
                string new_str;
                x_start =  stoi(new_vec[1]);
                y_start =  stoi(new_vec[2]);
                min_id = return_min_area_node(interval_tree,x_start,y_start);
                
                for(int i=0;i<min_id.size();++i){
                    new_str = new_str.assign( min_id[i],1, min_id[i].size()-1);
                    item = stoi(new_str);
                    index.push_back(item);
                }
                sort(index.begin(),index.end());
                for(int i=0;i<index.size();++i){
                    new_str = "r" + to_string(index[i]);
                    str.push_back(new_str);
                }
                
                outfile << "A" <<endl;
                for(int i=0;i<str.size();++i){
                    outfile << str[i]<<endl;
                }
            }
        }
    }
    
    //cout<<endl;
    //print_tree(interval_tree);
    //cout<<endl;

    infile.close();
    outfile.close();
    return 0;
}

void splitStr2Vec(string str, vector<string>& buf)
{
	int current = 0; //initial position
	int next;
 
	while (1)
	{
		next = str.find_first_of(" ", current);
		if (next != current)
		{
			string tmp = str.substr(current, next - current);
			if (tmp.size() != 0) //忽略空字串
				buf.push_back(tmp);
		}
		if (next == string::npos) break;
		current = next + 1; //下次由 next + 1 的位置開始找起。
	}
}