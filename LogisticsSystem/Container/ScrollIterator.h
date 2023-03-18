#pragma once
#include <type_traits>

template<typename Iter_t>
class ScrollIterator 
{
private:
	Iter_t topIter;
    Iter_t focusedIter;
    Iter_t bottomIter;
public:
    ScrollIterator() {}
    ScrollIterator(Iter_t topIter, Iter_t currIter,Iter_t bottomIter)
        : topIter(topIter), focusedIter(currIter), bottomIter(bottomIter) {
    }
    ScrollIterator(ScrollIterator& val) noexcept
        : topIter(val.topIter),focusedIter(val.focusedIter),bottomIter(val.bottomIter){}
    ScrollIterator(ScrollIterator&& val) noexcept
        : topIter(std::move(val.topIter)), focusedIter(std::move(val.focusedIter)), bottomIter(std::move(val.bottomIter)) {}

	Iter_t getTopIterator() {
		return topIter;
	}

	Iter_t getBottomIterator() {
		return bottomIter;
	}


    Iter_t getFocusedIterator() {
        return focusedIter;
    }

    bool hasNext() {
        return focusedIter!=bottomIter || bottomIter.hasNext();
    }

    bool hasPrevious() {
        return focusedIter!=topIter || topIter.hasPrevious();
    }

    void scrollDown() {
        bottomIter._setSelfNext();
        focusedIter._setSelfNext();
        topIter._setSelfNext();
    }

    void scrollUp() {
        topIter._setSelfPrevious();
        focusedIter._setSelfPrevious();
        bottomIter._setSelfPrevious();
    }


    void increaseBottomIterator() {
        bottomIter._setSelfNext();
    }

    bool isBottomIteratorIncreasable() {
        return bottomIter.hasNext();
    }

    void decreaseBottomIterator() {
        bottomIter._setSelfPrevious();
    }

    void _setSelfNext() {
        if (focusedIter != bottomIter) {
            focusedIter._setSelfNext();
        } else {
            scrollDown();
        }
    }

    void _setSelfPrevious() {
        if (focusedIter != topIter) {
            focusedIter._setSelfPrevious();
        } else {
            scrollUp();
        }
    }

    ScrollIterator& operator++() {
        _setSelfNext();
        return *this;
    }

    void operator=(const ScrollIterator& rgt){
        topIter = rgt.topIter;
        focusedIter = rgt.focusedIter;
        bottomIter = rgt.bottomIter;
    }

    ScrollIterator& operator++(int) {
        ScrollIterator oldIter = *this;
        _setSelfNext();
        return oldIter;
    }

};

