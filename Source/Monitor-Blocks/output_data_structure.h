#pragma once

template<typename T>
struct MonitorBlockOutput
{
    int return_code {};
    T *data = new T;

    ~MonitorBlockOutput() {
        delete data;
    }
};

