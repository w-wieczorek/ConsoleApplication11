#include <iostream>
#include <list>

using namespace std;

class Subject;

class Observer {
public:
    virtual ~Observer() = default;
    virtual void Update(Subject* theChangedSubject) = 0;
protected:
    Observer() = default;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void Attach(Observer*);
    virtual void Detach(Observer*);
    virtual void Notify();
protected:
    Subject() = default;
private:
    list<Observer*> _observers;
};

void Subject::Attach(Observer* o) {
    _observers.push_back(o);
}

void Subject::Detach(Observer* o) {
    _observers.remove(o);
}

void Subject::Notify() {
    list<Observer*>::iterator it = _observers.begin();
    while (it != _observers.end()) {
        (*it)->Update(this);
        ++it;
    }
}

class Int : public Subject {
public:
    Int(int _number) : number(_number) {}
    int getNumber() { return number; }
    void operator=(int rhsNumber) {
        number = rhsNumber;
        Notify();
    }
    pair<Int*, Int*> operator+(Int& rhsArg) {
        return make_pair(this, &rhsArg);
    }
private:
    int number;
};

class Expr : public Observer {
public:
    int value;
    Expr(pair<Int*, Int*> args) : lhsArg(args.first), rhsArg(args.second) {
        lhsArg->Attach(this);
        rhsArg->Attach(this);
        value = lhsArg->getNumber() + rhsArg->getNumber();
    }
    virtual void Update(Subject* theChangedSubject) {
        value = lhsArg->getNumber() + rhsArg->getNumber();
    }
private:
    Int* lhsArg;
    Int* rhsArg;
};

int main(int argc, char* argv[])
{
    Int x = 3; 
    Int y = 4; 
    Expr z = x + y; 
    cout << z.value << endl; // 7
    x = 5; 
    y = 6; 
    cout << z.value << endl; // 11
    return 0;
}
