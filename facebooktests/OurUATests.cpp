//made by yuval and tuval

#include <iostream>
#include <cmath>
#include "../UniqueArray.h"

class TestClass{
private:
    double number;
    char letter;
public:
    TestClass(double number, char letter) : number(number), letter(letter){}
    void ChangeNumber(double& num){
        number = num;
    }
    char GetLetter() const {return letter;}
    double GetNumber() const {return number;}
};

class TestCompare{
public:
    bool operator()(TestClass a, TestClass b){
        return a.GetNumber() == b.GetNumber();
        //"TestClass" objects are compared only by number,
        //two objects with diffrent letters might be the "same"
    }
};

void TestInsertion(){
    std::cout << "-------testing of insertion:-------"<< std::endl;
    std::cout << ">> 3 succeses are expected (if there are less somthing is worng)"<< std::endl;
    TestClass a(1,'a'), b(1,'b'), c(2,'c'), d(3, 'a');
    //a and b has the same number, they are similar in compair
    UniqueArray<TestClass,TestCompare> ua(3);
    int a_index = ua.insert(a);
    int a_index2 = ua.insert(a);
    int b_index = ua.insert(b);//sholdn't be inserted, same as 'a'
    if(a_index == a_index2 && a_index == b_index)
        std::cout << ">> success! (inserting similar values)"<< std::endl;
    else
        std::cout << ">> failure ): (inserting similar values) tip: check if you used operator== instad of Compare "<< std::endl;
    ua.insert(c);
    ua.insert(d);
    //ua is full now
    if(ua.getCount() == 3)
        std::cout << ">> success! (getCount)"<< std::endl;
    else
        std::cout << ">> failure ): (getCount)"<< std::endl;

    try{
        ua.insert(TestClass(9,'9'));
    }
    catch (UniqueArray<TestClass,TestCompare>::UniqueArrayIsFullException e){
        std::cout << ">> success! (full UniqueArray exeption)"<< std::endl;
    }
    std::cout << "-------succes in testing of insertion-------"<< std::endl;
    std::cout << ""<< std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    std::cout << ""<< std::endl;
}

void TestRemoving(){
    std::cout << "-------testing of removing:-------"<< std::endl;
    std::cout << ">> 2 succeses are expected (if there are less somthing is worng)"<< std::endl;
    TestClass a(1,'1'), b(2,'2'), c(3,'3');
    UniqueArray<TestClass,TestCompare> ua(2);
    ua.insert(a);
    ua.insert(b);
    ua.remove(a);
    if(ua.getCount() == 1)
        std::cout << ">>success! (removing element)"<< std::endl ;
    else
        std::cout << ">>failure ): (removing element or getCount)"<< std::endl;
    ua.insert(c);

    unsigned int i =99999999;
    if(ua.getIndex(c,i))
        std::cout << ">>success! (adding new element in removed spot)"<< std::endl ;
    else
        std::cout << ">>failure ): (adding new element in removed spot)"<< std::endl ;
    std::cout << "-------succes in testing of removing-------"<< std::endl ;
    std::cout << ""<< std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    std::cout << ""<< std::endl;
}

void TestCopyCtor(){
    std::cout << "-------testing of Copy Constructor:-------"<< std::endl;
    std::cout << ">> 2 succeses are expected (if there are less somthing is worng)"<< std::endl;
    TestClass a(1,'1'), b(2,'2'), c(3,'3'), d(4,'4');
    UniqueArray<TestClass,TestCompare> ua1(10);
    ua1.insert(a);
    ua1.insert(b);
    ua1.insert(c);
    ua1.remove(a);
    ua1.insert(d);
    ua1.insert(a);
    ua1.remove(b);

    UniqueArray<TestClass,TestCompare> ua2 = ua1;

    unsigned int ai1, ai2, ci1, ci2, di1, di2;
    ua1.getIndex(a,ai1); ua2.getIndex(a,ai2);
    ua1.getIndex(c,ci1); ua2.getIndex(c,ci2);
    ua1.getIndex(d,di1); ua2.getIndex(d,di2);
    if(ai1==ai2 && ci1 == ci2 && di1==di2)
        std::cout << ">>success! (copying Unique array)"<< std::endl ;
    else
        std::cout << ">>failure ): (copying Unique array) (probably not same indexes)"<< std::endl ;

    ua1.remove(a);
    unsigned int new_ai2;
    if(!ua2.getIndex(a, new_ai2))
        std::cout << ">>failure ): (an element that was deleted from original UA was also removed from the copy)"<< std::endl ;
    if(new_ai2 == ai2)
        std::cout << ">>success! (change of original UA does not affect copy)"<< std::endl ;


    std::cout << "-------succes in testing of Copy Constructor -------"<< std::endl ;
    std::cout << ""<< std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<< std::endl;
    std::cout << ""<< std::endl;
}

int main() {
    TestInsertion();
    TestRemoving();
    TestCopyCtor();
    return 0;
}
