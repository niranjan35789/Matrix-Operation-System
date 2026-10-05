// MatrixLab - very small C++ web server demo.
// Calculations are performed in C++.
// Build on Linux/macOS: g++ server.cpp -std=c++17 -o matrixlab
// Run: ./matrixlab
// Then open: http://localhost:8080
//
// Windows users: this educational version uses Winsock.
// Build with MinGW: g++ server.cpp -std=c++17 -lws2_32 -o matrixlab.exe

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
using Socket = SOCKET;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
using Socket = int;
#endif

using namespace std;

struct Matrix {
    int r, c;
    vector<vector<double>> x;
    Matrix(int rows=2, int cols=2) : r(rows), c(cols), x(rows, vector<double>(cols,0)) {}
};

Matrix add(const Matrix&A,const Matrix&B){
    Matrix C(A.r,A.c);
    for(int i=0;i<A.r;i++) for(int j=0;j<A.c;j++) C.x[i][j]=A.x[i][j]+B.x[i][j];
    return C;
}
Matrix sub(const Matrix&A,const Matrix&B){
    Matrix C(A.r,A.c);
    for(int i=0;i<A.r;i++) for(int j=0;j<A.c;j++) C.x[i][j]=A.x[i][j]-B.x[i][j];
    return C;
}
Matrix mul(const Matrix&A,const Matrix&B){
    Matrix C(A.r,B.c);
    for(int i=0;i<A.r;i++)
        for(int j=0;j<B.c;j++)
            for(int k=0;k<A.c;k++)
                C.x[i][j]+=A.x[i][k]*B.x[k][j];
    return C;
}
Matrix scalar(const Matrix&A,double k){
    Matrix C(A.r,A.c);
    for(int i=0;i<A.r;i++) for(int j=0;j<A.c;j++) C.x[i][j]=A.x[i][j]*k;
    return C;
}
Matrix trans(const Matrix&A){
    Matrix C(A.c,A.r);
    for(int i=0;i<A.r;i++) for(int j=0;j<A.c;j++) C.x[j][i]=A.x[i][j];
    return C;
}
double trace(const Matrix&A){
    double t=0;
    for(int i=0;i<min(A.r,A.c);i++) t+=A.x[i][i];
    return t;
}
double det(const Matrix&A){
    if(A.r!=A.c) return 0;
    if(A.r==1) return A.x[0][0];
    if(A.r==2) return A.x[0][0]*A.x[1][1]-A.x[0][1]*A.x[1][0];
    if(A.r==3)
        return A.x[0][0]*(A.x[1][1]*A.x[2][2]-A.x[1][2]*A.x[2][1])
             -A.x[0][1]*(A.x[1][0]*A.x[2][2]-A.x[1][2]*A.x[2][0])
             +A.x[0][2]*(A.x[1][0]*A.x[2][1]-A.x[1][1]*A.x[2][0]);
    return 0;
}
Matrix power(Matrix A,int n){
    Matrix R(A.r,A.c);
    for(int i=0;i<A.r;i++) R.x[i][i]=1;
    for(int p=0;p<n;p++) R=mul(R,A);
    return R;
}

string matrixToJson(const Matrix&M){
    ostringstream out;
    out<<"{\"rows\":[";
    for(int i=0;i<M.r;i++){
        if(i) out<<",";
        out<<"[";
        for(int j=0;j<M.c;j++){
            if(j) out<<",";
            out<<M.x[i][j];
        }
        out<<"]";
    }
    out<<"]}";
    return out.str();
}

string valueJson(double v){
    ostringstream out; out<<"{\"value\":"<<v<<"}"; return out.str();
}

string urlDecode(string s){
    string r;
    for(size_t i=0;i<s.size();i++){
        if(s[i]=='+') r+=' ';
        else if(s[i]=='%' && i+2<s.size()){
            string h=s.substr(i+1,2);
            r+=(char)strtol(h.c_str(),nullptr,16); i+=2;
        } else r+=s[i];
    }
    return r;
}

string getParam(const string&q,const string&name){
    string key=name+"=";
    size_t p=q.find(key);
    if(p==string::npos) return "";
    p+=key.size();
    size_t e=q.find('&',p);
    return urlDecode(q.substr(p,e==string::npos?string::npos:e-p));
}

vector<double> nums(string s){
    vector<double> v; stringstream ss(s); double n;
    while(ss>>n) v.push_back(n);
    return v;
}

Matrix readMatrix(string s,int r,int c){
    Matrix M(r,c); vector<double> v=nums(s);
    for(int i=0;i<r;i++) for(int j=0;j<c;j++){
        int k=i*c+j; if(k<(int)v.size()) M.x[i][j]=v[k];
    }
    return M;
}

string api(const string&q){
    string op=getParam(q,"op");
    int r=atoi(getParam(q,"r").c_str());
    int c=atoi(getParam(q,"c").c_str());
    if(r<1||r>4||c<1||c>4) return "{\"error\":\"Rows and columns must be 1 to 4.\"}";
    Matrix A=readMatrix(getParam(q,"a"),r,c);
    string bs=getParam(q,"b");
    int br=atoi(getParam(q,"br").c_str()); int bc=atoi(getParam(q,"bc").c_str());
    Matrix B=readMatrix(bs,br,bc);
    Matrix C;
    if(op=="add" && r==br && c==bc) C=add(A,B);
    else if(op=="sub" && r==br && c==bc) C=sub(A,B);
    else if(op=="mul" && c==br) C=mul(A,B);
    else if(op=="scalar") C=scalar(A,atof(getParam(q,"k").c_str()));
    else if(op=="transpose") C=trans(A);
    else if(op=="trace") return valueJson(trace(A));
    else if(op=="det" && r==c && r<=3) return valueJson(det(A));
    else if(op=="power" && r==c){
        int n=atoi(getParam(q,"n").c_str());
        C=power(A,n);
    } else return "{\"error\":\"Invalid sizes or operation.\"}";
    return matrixToJson(C);
}

string readFile(const string&path){
    ifstream f(path); stringstream ss; ss<<f.rdbuf(); return ss.str();
}

void send(Socket client,const string&body,const string&type="text/html"){
    string header="HTTP/1.1 200 OK\r\nContent-Type: "+type+"; charset=UTF-8\r\nContent-Length: "+to_string(body.size())+"\r\nConnection: close\r\n\r\n";
#ifdef _WIN32
    ::send(client,header.c_str(),(int)header.size(),0);
    ::send(client,body.c_str(),(int)body.size(),0);
    closesocket(client);
#else
    ::send(client,header.c_str(),header.size(),0);
    ::send(client,body.c_str(),body.size(),0);
    close(client);
#endif
}

int main(){
#ifdef _WIN32
    WSADATA wsa; WSAStartup(MAKEWORD(2,2),&wsa);
#endif
    Socket serverSock=socket(AF_INET,SOCK_STREAM,0);
    sockaddr_in addr{}; addr.sin_family=AF_INET; addr.sin_port=htons(8080); addr.sin_addr.s_addr=INADDR_ANY;
    bind(serverSock,(sockaddr*)&addr,sizeof(addr));
    listen(serverSock,10);

    cout<<"MatrixLab running at http://localhost:8080\n";

    while(true){
        sockaddr_in clientAddr{};
#ifdef _WIN32
        int len=sizeof(clientAddr);
#else
        socklen_t len=sizeof(clientAddr);
#endif
        Socket client=accept(serverSock,(sockaddr*)&clientAddr,&len);
        char buffer[8192]={0};
#ifdef _WIN32
        recv(client,buffer,sizeof(buffer)-1,0);
#else
        read(client,buffer,sizeof(buffer)-1);
#endif
        string req(buffer);
        size_t p=req.find(" ");
        size_t e=req.find(" ",p+1);
        string target=req.substr(p+1,e-p-1);

        if(target.rfind("/api?",0)==0){
            send(client,api(target.substr(5)),"application/json");
        }else{
            string page=readFile("web/index.html");
            if(page.empty()) page="MatrixLab: put web/index.html beside the executable.";
            send(client,page);
        }
    }
    return 0;
}
