#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<cstring>
#include<algorithm>
#include <queue>
#include<stack>
#include <list>
#include <limits.h>

#define INFINITE 2147483647


//network array size
int Vec_size = 0;

using namespace std;

typedef pair<int, int> iPair; 

/* Returns true if there is a path from source 's' to sink 't' in
  residual graph. Also fills parent[] to store the path */
bool bfs(vector<vector<int>> rGraph, int s, int t, int parent[],int V)
{
    // Create a visited array and mark all vertices as not visited
    bool visited[V];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source vertex as visited
    queue<int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    int u;
    while (!q.empty())
    {
        // edge: u -> v
        u = q.front();  // head point u
        q.pop();
        for (int v = 0; v < V; ++v)  // tail point v
        {
            if (!visited[v] && rGraph[u][v] > 0)  // find one linked vertex
            {
                q.push(v);
                parent[v] = u;  // find pre point
                visited[v] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return true, else false
    return visited[t] == true;
}

// Returns the maximum flow from s to t in the given graph
int fordFulkerson(vector<vector<int>> graph, int s, int t,int V)
{
    int u, v;
    vector<vector<int>> rGraph;
    rGraph.resize(V);
    for(int i=0;i<V;++i){
        rGraph[i].resize(V);
    } 

    for (u = 0; u < V; ++u)
    {
        for (v = 0; v < V; ++v)
        {
            rGraph[u][v] = graph[u][v];
        }
    }

    int parent[V];
    int max_flow = 0;

    // Augment the flow while tere is path from source to sink
    while (bfs(rGraph, s, t, parent,V))
    {
        // edge: u -> v
        int path_flow = INT_MAX;
        for (v = t; v != s; v = parent[v])
        {
            // find the minimum flow
            u = parent[v];
            path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges along the path
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;  // assuming v->u weight is add path_flow
        }

        // Add path flow to overall flow
        max_flow += path_flow;
    }

    return max_flow;
}

void addEdge(vector <pair<int, int>> adj[], int u, 
                                     int v, int wt) 
{ 
    //adj[u].push_back(make_pair(v, wt)); 
    adj[v].push_back(make_pair(u, wt));  //v to u
} 

void splitStr2Vec(string str, vector<string>& buf)
{
	  int current = 0; //initial position
	  int next;
 
	  while (true)
	  {
	      next = str.find_first_of(" :", current);
		    if (next != current)
		    {
			      string tmp = str.substr(current, next - current);
			      if (tmp.size() != 0)   buf.push_back(tmp);
        }
		    if (next == string::npos) break;
		    current = next + 1; 
    }
}

//O((V + E)log E)
ofstream& shortestPath(vector<pair<int,int> > adj[], int V, int src,ofstream &outfile,vector<string> &karan_super_path,string start,string end,string index) 
{ 
    //small to big
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq; 
  
    vector<int> dist(V, INFINITE); 
    vector<int> from(V, 0); 
    stack<int> path;
    vector<bool> visited(V, false);
   

    pq.push(make_pair(0, src)); 
    dist[src] = 0; 
  
    while (!pq.empty()) 
    { 
        int u = pq.top().second; 
        pq.pop(); 
        if (visited[u]) {
            continue;
        }
        visited[u] = true;
      
        for (auto x : adj[u]) 
        { 
            int v = x.first; 
            int weight = x.second; 
  
            if (dist[v] > dist[u] + weight) 
            { 
                from[v] = u;
                dist[v] = dist[u] + weight; 
                pq.push(make_pair(dist[v], v)); 
            } 
        } 
    } 
    
    int k = 1;
    path.push(k);
    while(from[k] != 0){
        path.push(from[k]);
        //cout <<from[k]<<" ";
        k = from[k];
    }
    path.push(0);
    //cout << endl;
    
    while(!path.empty()){
        if(path.top() == 0){
            //cout <<"s ";
        }
        else if(path.top() == 1){
            //cout<<"t ";
        }
        else{
            //cout<<"a"<<path.top()-1<<" ";
            string super;
            super = "a"+to_string(path.top()-1);
            karan_super_path.push_back(super);
        }
        path.pop();
    }
    //cout << endl;
  
    //cout << "Vertex Distance from Source" << endl; 
    for (int i = 0; i < V; ++i){
        if(i == 0){
            outfile<< start <<": "<<dist[i]<<endl;
        }
        else if(i == 1){
            outfile<< end <<": "<<dist[i]<<endl;
        }
        else{
            if(dist[i] == INFINITE){
                outfile<< index <<i-1<<": "<<-1<<endl;
            }
            else{
                outfile<< index <<i-1<<": "<<dist[i]<<endl;
            }
        }
    } 
} 

int count_tangyuan(vector<int> peo,int ty_type){
    
    int count = 0;
    
    for(int i=0;i<peo.size();++i){
        if(peo[i] == ty_type) count++;
    }
    
    return count;
}
int exchange_cap(vector<int> peo){
    int count = 0;
    for(int i=0;i<peo.size()-1;++i){
        if(peo[i] == peo[i+1]) count++;
    }
    return count;
}

void print_network(vector<vector<int>> network){
    for(int i=0;i<network.size();++i){
        for(int j=0;j<network[i].size();++j){
            cout << network[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc,char* argv[])
{
    ifstream infile;
    string instr; 
    infile.open(argv[1]);
    ofstream outfile,outfile2;
    
    outfile.open(argv[2]);    
    outfile2.open(argv[3]);
    
    int rank = 1000000; 
    int rank2 = 1000000;
    map<string,int> supermarket;
    vector<string> karan_super_path;
    vector<int> karan_tangyuan;
    vector<vector<int>> people;
    string starts,ends,index;
    int tangyuan_type;
    int people_num =0;
    int supermarket_num = 0;
    
    vector<iPair > adj[500];
    
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
                         
            //++rank;
            
            //check the number of vertex we need to store
            if(new_vec.empty()){
                break;
            }
            else if(new_vec[0] == "home"){
                starts = new_vec[1];
                //cout <<starts<<endl;
            }
            else if(new_vec[0] == "party"){
                ends = new_vec[1];
                //cout <<ends<<endl;
            }
            else if(new_vec[0] == "tangyuan"){
                tangyuan_type = stoi(new_vec[1]);
            }
            else if(new_vec[0] == "people"){
                people_num = stoi(new_vec[1]);
                //cout <<people_num<<endl;
                rank = 1;
            }
            else if(rank < people_num ){
                int num_ty;
                vector<int> in_people;
                num_ty = stoi(new_vec[0]);
                //cout << num_ty << endl;
                for(int i=1;i <= num_ty;++i){
                    in_people.push_back(stoi(new_vec[i]));
                }
                people.push_back(in_people);
                ++rank;
            }
            else if(new_vec[0] == "supermarket"){
                supermarket_num = stoi(new_vec[1]);
                //cout << supermarket_num  << endl;
                rank2 = 0;
            }
            else if(rank2 <supermarket_num){
                string str;
                index = str.assign(new_vec[0].begin(),new_vec[0].begin()+1);
                supermarket[new_vec[0]] = stoi(new_vec[1]);
                //cout << new_vec[0] <<" "<< new_vec[1] << endl;
                //cout << index;
                ++rank2;
            }
            //build the directed graph
            else if(!new_vec.empty()){
                //cout << new_vec[1]  << endl;
                int start,end;
                string str;
                if(new_vec[1] == starts){
                    start = 0;
                }
                else if(new_vec[1] == ends){
                    start = 1;
                }
                else{
                    start = stoi(str.assign(new_vec[1].begin()+1,new_vec[1].end()))+1;
                }
                if(new_vec[0] == ends){
                    end = 1;
                }
                else if(new_vec[0] == starts){
                    end = 0;
                }
                else{
                    end = stoi(str.assign(new_vec[0].begin()+1,new_vec[0].end()))+1;
                }
                addEdge(adj, end, start, stoi(new_vec[2]));
            }
        }
    }
        
    shortestPath(adj, 2+supermarket_num, 0,outfile,karan_super_path,starts,ends,index); 
    
    for(int i=0;i<karan_super_path.size();++i){
        karan_tangyuan.push_back(supermarket[karan_super_path[i]]);
    }
    sort(karan_tangyuan.begin(),karan_tangyuan.end());
    
//print out the number of tangtuan

/*
    cout<<"karon's tangtuan:"<<endl;
    for(int i=0;i<karan_tangyuan.size();++i){
        cout << karan_tangyuan[i] <<" ";
    }  
    cout << endl;
    
    cout <<"people's tangyuan:"<<endl;
    for(int i=0;i<people.size();++i){
        for(int j=0;j<people[i].size();++j){
            cout <<people[i][j]<<" ";
        }
        cout << endl;
    }
*/

    //cout << endl;
    //cout << count_tangyuan(karan_tangyuan) << endl;
    
    //network flow calculation
    Vec_size = tangyuan_type + people_num + 1;
    //cout <<"flow size:"<< Vec_size << endl;
    
    vector<vector<int>> network;
    network.resize(Vec_size);
    for(int i=0;i<Vec_size;++i){
       network[i].resize(Vec_size);
       for(int j=0;j<Vec_size;++j){
           network[i][j] = 0;
       }
    } 
    
    for(int i=1;i<=tangyuan_type;++i){
        //cout <<i<<":"<< count_tangyuan(karan_tangyuan,i)<< endl;
        network[0][i] = count_tangyuan(karan_tangyuan,i);
        network[i][Vec_size-1] = 1;
    }
    //cout << endl;
    for(int i=0;i<people_num-1;++i){
        for(int j=1;j<=tangyuan_type;++j){
            int count = count_tangyuan(people[i],j);
            int ex = exchange_cap(people[i]);
            //cout <<j<<":"<< count << endl;
            if(count>=1){
                network[i+1+tangyuan_type][j] = count-1; 
                network[j][i+1+tangyuan_type] = 0;
            }
            if(count == 0){ //can exchange
                network[j][i+1+tangyuan_type] = 1;
                
                if(network[0][j] > 0){
                    //network[j][i+1+tangyuan_type] = network[0][j]-1;
                }
                else{
                    network[j][i+1+tangyuan_type] = ex;
                }
                
            }
        }      
    }
    
    //print_network(network);
    //cout << "the maximum flow from s to t is:" <<fordFulkerson(network, 0, Vec_size-1,Vec_size)<<endl;
    outfile2 << fordFulkerson(network, 0, Vec_size-1,Vec_size)<<endl;

    
    return 0;
}