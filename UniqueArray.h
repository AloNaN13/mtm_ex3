#ifndef MTMPARKINGLOT_UNIQUEARRAY_H
#define MTMPARKINGLOT_UNIQUEARRAY_H


template <class Element, class Compare = std::equal_to<Element>>
class UniqueArray {
private:
    const unsigned int max_size;
    //unsigned int max_size;
    Element** data;

public:

    explicit UniqueArray(unsigned int size);
    UniqueArray(const UniqueArray& other);
    ~UniqueArray();
    UniqueArray& operator=(const UniqueArray&) = delete;
    unsigned int insert(const Element& element);
    bool getIndex(const Element& element, unsigned int& index) const;
    const Element* operator[] (const Element& element) const;
    bool remove(const Element& element);
    unsigned int getCount() const;
    unsigned int getSize() const;


    /** getElement: returns a pointer to the element with the given index
     * in the uniqueArray. there are 2 versions, one for const and one without
     */
    Element* getElement(unsigned int index) {return data[index];}
    const Element* getElement(unsigned int index) const {return data[index];}

    class Filter {
    public:
        virtual bool operator() (const Element&) const = 0;
    };
    UniqueArray filter(const Filter& f) const;
    class UniqueArrayIsFullException{};
    
};





#include "UniqueArrayImp.h"


#endif //MTMPARKINGLOT_UNIQUEARRAY_H
