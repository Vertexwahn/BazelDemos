#pragma once

template<typename T>
class Singleton {
public:
    static T& instance() {   
        static T instance; 
        return instance;           
    }
private:              
    Singleton(const Singleton&) = delete;            
    void operator=(const Singleton&) = delete;       
};