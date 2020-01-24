// Created by Jonathan Joseph on 10/01/2020

#include <iostream>
#include <cassert>
//#include "UniqueArray.h"
"../UniqueArray.h"

class TestElement{
    const unsigned int id;
public:
    TestElement(const unsigned int id): id(id){}
    TestElement(const TestElement& other) : id(other.id){}

    unsigned int getId() const {
        return id;
    }
};

class TestCompare{
public:
    TestCompare() = default;
    bool operator()(const TestElement& el1, const TestElement& el2) const{
        return el1.getId() == el2.getId();
    };
};

typedef UniqueArray<TestElement, TestCompare> UA;

class TestFilter: public UA::Filter{
public:
    bool operator()(const TestElement &element) const override {
        return element.getId()%2;
    }
};

UA createArr(unsigned int** indexArr = NULL){
    unsigned int size = 10;


    //Fill arr
    UA arr(size);
    assert(arr.getSize() == size);
    assert(arr.getCount() == 0);
    if(indexArr != NULL)
        *indexArr = new unsigned int[size];
    try{
        for (unsigned int i = 0; i < size; ++i){
            if(indexArr != NULL)
                (*indexArr)[i] = arr.insert(TestElement(i));
            else
                arr.insert((TestElement(i)));
            assert(arr.getCount() == i+1);
        }
    }catch (std::exception& e){
        std::cout << e.what() <<" in line "<< __LINE__ << std::endl;
        assert(false);
    }

    return arr;
}


//C'tors + D'tors + copy C'tor Test
void test1(){
    unsigned int size = 10;
    bool checkException = false;
    unsigned int* indexArr;

    //Fill arr
    UA arr = createArr(&indexArr);

    //Check if array is full
    try {
        arr.insert(TestElement(size+1));
    }catch (UA::UniqueArrayIsFullException& e) {
        checkException = true;
    }catch (std::exception& e){

    }
    assert(checkException);

    //Check for doubles
    try{
        for (unsigned int i = 0; i < size; ++i) {
            assert(indexArr[i] == arr.insert(TestElement(i)));
        }
    }catch (std::exception& e){
        std::cout << e.what() <<" in line "<< __LINE__ << std::endl;
        assert(false);
    }

    //Check Copy C'tor
    UA copyArr(arr);
    try{
        assert(copyArr.getSize() == arr.getSize());
        assert(copyArr.getCount() == arr.getCount());

        unsigned int index;
        for (unsigned int i = 0; i < 2*size; ++i) {
            if(i<size){
                assert(indexArr[i] == copyArr.insert(TestElement(i)));
                assert(copyArr.getIndex(TestElement(i),index));
                assert(indexArr[i] == index);
            }else{
                assert(!copyArr.getIndex(TestElement(i),index));
            };
        }
    }catch (std::exception& e){
        assert(false);
    }


    delete[] indexArr;
}

//operator[] Test
void test2(){
    UA arr = createArr();
    assert(arr[TestElement(30)] == NULL);
}

//Remove Test
void test3(){
    UA arr1 = createArr();
    UA arr2(arr1);

    for (unsigned int i = 0; i <arr1.getSize() ; i++) {
        assert(arr1.remove(TestElement(i)));
        assert(arr1.getCount() == arr2.getCount() - (i + 1));
    }
    assert(!arr1.remove(TestElement(0)));
    assert(!arr1.remove(TestElement(9)));
    assert(arr2.remove(TestElement(0)));
    assert(arr2.remove(TestElement(9)));
    assert(arr2.getCount() == 8);
}

//Filter Test
void test4(){
    unsigned int* indexArr;
    UA arr1 = createArr(&indexArr);
    unsigned int tempIndex;

    UA arr2 = arr1.filter(TestFilter());

    assert(arr1.getCount() == 10);
    assert(arr2.getCount() == 5);

    for (int i = 0; i <10 ; i++) {
        if(i%2){
            assert(arr2.getIndex(TestElement(i), tempIndex));
            assert(indexArr[i] == tempIndex);
        }
        else
            assert(!arr2.getIndex(TestElement(i), tempIndex));
    }
    delete [] indexArr;
}

int main(){
    test1();
    test2();
    test3();
    test4();

    std::cout << "Well done! Enjoy your Netflix" << std::endl;

    return 0;
}