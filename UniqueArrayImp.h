//
// Created by User on 1/7/2020.
//

#ifndef EX3_UNIQUEARRAYIMP_H
#define EX3_UNIQUEARRAYIMP_H



template <class Element, class Compare >
UniqueArray<Element,Compare>:: UniqueArray(unsigned int size) : max_size(size), data(new Element*[size]){
    for(int i=0;i<max_size;i++){
        data[i]=NULL;
    }

}

template <class Element, class Compare >
UniqueArray<Element,Compare>:: UniqueArray(const UniqueArray& other): max_size(other.max_size),
                                                                      data(new Element*[other.max_size]){
    for(unsigned int i=0;i<max_size;i++){
        data[i]=new Element(*other.data[i]);
    }


}

template <class Element, class Compare >
UniqueArray<Element,Compare>:: ~UniqueArray(){
    for(unsigned int  i=0;i<max_size;i++){
        //need to check if data[i]!=NUll?
        delete data[i];
    }
    delete [] data;
}

template <class Element, class Compare >
unsigned int UniqueArray<Element,Compare>::  insert(const Element& element){
    unsigned int index=0;
    if(getIndex(element,index)){
        return index;
    }
    // element doesn't exists in the UniqueArray
    for(unsigned int i=0;i<max_size;i++){
        if(data[i]==NULL) {    //ask
            data[i]=new Element(element);
            return i;
        }
    }
    //array is full
    UniqueArrayIsFullException fullException;
    throw (fullException);

    //exceptions!!!
}


template <class Element, class Compare >
bool UniqueArray<Element,Compare>::getIndex(const Element& element, unsigned int& index) const{
    Compare c;
    for(unsigned int i=0;i<max_size;i++){
        if(data[i]!=NULL) {
            if (c(*data[i], element)) {
                index = i;
                return true;
            }
        }
    }
    //element doesn't exists in the UniqueArray
    return false;
}

template <class Element, class Compare >
const Element* UniqueArray<Element,Compare>::operator[] (const Element& element) const{
    Compare c;
    for(unsigned int i=0;i<max_size;i++){
        if(data[i]!=NULL) {
            if (c(*data[i], element)) {//need to use the compare class and not sure if it will work
                return &element;
            }
        }
    }
    return NULL;
}


template <class Element, class Compare >
bool UniqueArray<Element,Compare>:: remove(const Element& element){
    unsigned int index=0;
    if(getIndex(element,index)){
        delete data[index];
        data[index]=NULL;
        return true;
    }
    return false;
}

template <class Element, class Compare >
unsigned int UniqueArray<Element,Compare>::  getCount() const{
    unsigned int count=0;
    for(unsigned int  i=0;i<max_size;i++){
        if(data[i]!=NULL){
            count++;
        }

    }
    return count;
}

template <class Element, class Compare >
unsigned int UniqueArray<Element,Compare>::  getSize() const{
    return max_size;
}


template <class Element, class Compare >
UniqueArray<Element,Compare> UniqueArray<Element,Compare>:: filter(const Filter& f) const{
    UniqueArray filterd_array(max_size);
    for(unsigned int  i=0;i<max_size;i++){
        if(f.operator()(*data[i])){
            filterd_array.data[i]=new Element(*data[i]);
        }
    }
    return filterd_array;

}





#endif //EX3_UNIQUEARRAYIMP_H




// change settings in clion for templates