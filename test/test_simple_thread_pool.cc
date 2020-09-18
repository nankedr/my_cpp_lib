//#include "../thread_pool/simple_thread_pool.h"
#include <iostream>
#include <queue>
using namespace std;

class A {
public:
    A(int i) {
        this->i = i;
        cout<<"init "<<i<<endl;
    }
    ~A() {
        cout<<"destory"<<endl;
    }
    int i;
};

int main() {
    queue<A> q;
    q.emplace(1);
    q.emplace(2);
    auto a = q.front();
    q.pop();
    cout << a.i << endl;
    while(true);
    
    return 0;
}