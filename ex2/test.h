#ifndef _TEST_H_
#define _TEST_H_

#include <iostream>
#include <vector>
#include <chrono>
#include <typeinfo>
#include "transform.h"

#define DEBUG

template<typename TRANS, typename DataType>
void Trans( std::vector<DataType> data )
{
    // idata.clear();
    
    TRANS trans;
    std::cout << "-- " << typeid(trans).name() << " --" << std::endl;
    std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();

    trans.setData(data);
    trans.execute();

    std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>( t2-t1 );
    std::cout << "Time consumption: " << time_used.count()*1000 << " ms" << std::endl;

#ifdef DEBUG    
    trans.displayResult();
#endif

    // for(size_t i = 0; i < trans.outputSequence.size(); i++)
    // {
    //     // idata.push_back(trans.outputSequence[i]);
    // }
}

#endif