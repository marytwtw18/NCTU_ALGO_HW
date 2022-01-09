#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

using namespace std;

class Dis{
public:
    int vertex1,vertex2; 
    int distance; 
    Dis(){
        vertex1 = 0;
        vertex2= 0;
        distance = 0;
    }
    Dis(int xin,int yin,int dis){
        vertex1 = xin;
        vertex2= yin;
        distance = dis;
    }
};

bool compare(const Dis dis1,const Dis dis2){
    return dis1.distance < dis2.distance;
}

void print_table(vector<Dis> &table){
    for(int i = 0;i< table.size(); ++i){
        cout <<"Distance of "<<table[i].vertex1<<" and "<<table[i].vertex2<<" is "<<table[i].distance<<endl;
    }
}

class Node{  
public:
    int x,y; 
    vector<int> link;   //link to index:adjacency list
    bool colar;         //traversal for dfs  
    
    Node(){
        x = 0;
        y = 0;
        colar = false;
    };
    Node(int xin,int yin){
        x = xin;
        y = yin;
    }
    int  return_x(int xin) {
        xin = x;  
        return xin;
    };
    int  return_y(int yin) {
        yin = y;  
        return yin;
    };
    void insert_link(int);
    void print_coordinate();
    void print_link();
};

void Node::insert_link(int input){
    link.push_back(input);
}
void Node::print_coordinate(){
    cout <<"(x , y) = (" << x <<", "<<  y << ")"<<endl;
}

void Node::print_link(){
    for(int i=0;i<link.size();++i){
        cout<<"link to:"<<link[i]<<endl;
    }
}

void splitStr2Vec(string str, vector<string>& buf)
{
	  int current = 0; //initial position
	  int next;
 
	  while (true)
	  {
	      next = str.find_first_of(" ", current);
		    if (next != current)
		    {
			      string tmp = str.substr(current, next - current);
			      if (tmp.size() != 0)   buf.push_back(tmp);
        }
		    if (next == string::npos) break;
		    current = next + 1; 
    }
}

int manhattan_distance(int x1,int y1,int x2 ,int y2){
    int distance = 0;
    if(x1>=x2){
        distance += (x1 - x2);
    }
    else{
        distance += (x2 - x1);
    }
    
    if(y1 >= y2){
        distance += (y1 - y2);
    }
    else{
        distance += (y2 - y1);
    }
    
    return distance;
}


void print_vec(vector<Node> &vec){
    for(int i=0 ;i < vec.size(); ++i){
        cout << "index "<< i<<": ";
        vec[i].print_coordinate();
        vec[i].print_link();
    }
}

void DFS(vector<Node> &routing,int index){
     routing[index].colar = true;
     for(int i=0;i<routing[index].link.size();++i){
         if(!routing[routing[index].link[i]].colar){ //if we do not go through yet
             DFS(routing,routing[index].link[i]);
         }
     }
     return;
}

int find_scc_count(vector<Node> &routing){
    int scc_count = 0;
    for(int i=0;i < routing.size();++i){
        if(!routing[i].colar){    //if we didn't go through
            DFS(routing,i);        
            ++scc_count;
        }
    }
    //reset
    for(int i=0;i < routing.size();++i){
        routing[i].colar = false;
    }
    return scc_count;
}

//determine two pins' connections
bool scc_connect(vector<Node> &routing,int index_x,int index_y){
        
    bool desicion = false;
    DFS(routing,index_x); 
    
    if(routing[index_x].colar == routing[ index_y].colar)
    {
        desicion = true;
    }
    
    //reset
    for(int i=0;i < routing.size();++i){
        routing[i].colar = false;
    }
    return desicion;
}

int main(int argc,char* argv[])
{
    ifstream infile;
    string instr; 
    infile.open(argv[1]);
    ofstream outfile;
    outfile.open(argv[2]);    
    
    vector<Node> routing;
    int pin_count = 0;
    int rank = 0;                //read file in which rank
    int scc_count = 0;
    
    int span_count = 0;
    int min_cost = 0;
    vector<Dis> table;
    vector<Dis> spanning;
    
    /*------------------------read file-------------------------------------*/
    
    if(!infile.is_open())
    {
        cout << "fail to open" << endl;
    }
    else
    {
        while(!infile.eof()){ //read in
            vector<string> new_vec;
            Node node;
            getline(infile,instr);
            splitStr2Vec(instr,new_vec);
                         
            ++rank;
            
            //check the number of vertex we need to store
            if((rank == 1) && (!new_vec.empty())){
                pin_count = stoi(new_vec[0]);
                //cout << pin_count << endl;
            }
            //store vertex
            else if( (rank <= pin_count+1 ) && (!new_vec.empty())){
                node = Node(stoi(new_vec[0]),stoi(new_vec[1]));
                routing.push_back(node);
            }
            //store the linking relationship
            else if(!new_vec.empty()){
                int link1,link2;
                link1 =  stoi(new_vec[0]);
                link2 =  stoi(new_vec[1]);
                
                //two point connected to each other
                routing[link1].insert_link(link2);
                routing[link2].insert_link(link1);
            }
        }
    }
        
    //cout << endl;
    //print_vec(routing);
    
/*------------------------strongly connected component---------------------*/
    scc_count = find_scc_count(routing);
    //cout << "Strongly connect component = " << scc_count << endl;
    

    for(int i=0;i<routing.size()-1;++i)
    {
        for(int j = i+1;j<routing.size();++j)
        {
            if(!scc_connect( routing,i,j)){
                Dis dis =  Dis(i,j,manhattan_distance(routing[i].x,routing[i].y,routing[j].x ,routing[j].y));
                table.push_back(dis);
            }
        }
    }
    
    sort(table.begin(),table.end(),compare);
    //cout << endl;
    //print_table(table);

    //cout << scc_connect( routing,3,7) <<endl;
    
    while(scc_count > 1) //break out condition
    {
        if(!scc_connect(routing,table[span_count].vertex1,table[span_count].vertex2)){
            routing[table[span_count].vertex1].insert_link(table[span_count].vertex2);
            routing[table[span_count].vertex2].insert_link(table[span_count].vertex1);
            min_cost += table[span_count].distance;
            spanning.push_back(table[span_count]);
        }
        
        ++span_count;
        scc_count = find_scc_count(routing);
    }
    
    //cout << endl;
    //cout <<"min cost = "<< min_cost<<endl;
    //print_table(spanning);
    outfile <<  min_cost << endl;
    
    for(int i=0;i<spanning.size();++i){
        outfile << spanning[i].vertex1<<" "<<spanning[i].vertex2<<endl;
    }
 
    infile.close();
    outfile.close();
    
    return 0;
}