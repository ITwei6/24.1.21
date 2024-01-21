#pragma once
#include <iostream>
#include <string>

std::string opera="+-*/%";
class TASK
{

public:
    TASK()
    {}
    TASK(int data1, int data2, char op) : _data1(data1), _data2(data2), _oper(op)
    {
    }
    void run()
    {
        switch (_oper)
        {
        case '+':
            _result = _data1 + _data2;
            break;
        case '-':
            _result = _data1 - _data2;
            break;
        case '*':
            _result = _data1 * _data2;
            break;
        case '/':
        {
            if (_data2 == 0)
                _exitcode = 1;
            else
                _result = _data1 / _data2;
        }
        break;
        case '%':
        {
            if (_data2 == 0)
                _exitcode = 2;
            else
                _result = _data1 % _data2;
        }
        break;

        default:
        _exitcode=3;
            break;
        }
    }

    std::string GetTASK()
    {
       std::string r=std::to_string(_data1);
       r+=_oper;
       r+=std::to_string(_data2);
       r+="=?";
       return r;
    }
    std::string Getresult()
    {
        std::string result=std::to_string(_data1);
        result+=_oper;
        result+=std::to_string(_data2);
        result+='=';
        result+=std::to_string(_result);
        result+="[code:";
        result+=std::to_string(_exitcode);
        result+=']';
        return result;
    }
    void operator()()
    {
        run();
    }
private:
    int _data1;
    int _data2;
    char _oper;

    int _result;
    int _exitcode=0;
};
