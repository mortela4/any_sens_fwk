/**
@file std_any_iterator_ex1.cpp

@brief Demo of iteration over 'std::any' collections.
*/

#include <iostream>
#include <string>
#include <type_traits>
#include <array>
#include <tuple>
#include <optional>


using opt_int_t = std::optional<int>;
using opt_float_t = std::optional<float>;
using opt_str_t = std::optional<std::string>;
 

class SensorBase
{
    private:
    std::string _sensorInfo;
    public:
    SensorBase(const std::string info) : _sensorInfo(info) {};
};


class IntSensor : public SensorBase
{
    private:
    opt_int_t _value = std::nullopt;
    public:
    IntSensor(const std::string info) : SensorBase(info) {};
    void Sample(void)
    {
        static int iVal = 77777;

        _value = iVal;
    
        iVal += 1234;
    };
    int GetValue(bool resetValueAfterRead=false)
    { 
        const auto maybeVal = _value;
        
        if (resetValueAfterRead)
        {
            _value = std::nullopt;
        }
        
        if ( maybeVal.has_value() )
        {
            return(maybeVal.value());
        }
        else
        {
            return(int(0xFFFFFFFF));
        }
    };
};


class StringSensor : public SensorBase
{
    private:
        opt_str_t _value = std::nullopt;
    public:
    StringSensor(const std::string info) : SensorBase(info) {};
    void Sample(void)
    {
        static int count = 1;
        
        const std::string sVal = "A";

        _value = sVal + std::to_string(count);
        
        ++count;
    };
    std::string GetValue(bool resetValueAfterRead=true)
    { 
        const auto maybeVal = _value;
    
        if (resetValueAfterRead)
        {
            _value = std::nullopt;
        }

        if ( maybeVal.has_value() )
        {
            return( maybeVal.value() );
        }
        else
        {
            return("<no value>");
        }
    }; 
};


class FloatSensor : public SensorBase
{
    private:
        opt_float_t _value = std::nullopt;
    public:
    FloatSensor(const std::string info) : SensorBase(info) {};
    void Sample(void)
    {
        static float fVal = 9.563F;
    
        _value = fVal;
        
        fVal += 1.234F;
    };
    float GetValue(bool resetValueAfterRead=false)
    { 
        const auto maybeVal = _value;
        
        if (resetValueAfterRead)
        {
            _value = std::nullopt;
        }

        if ( maybeVal.has_value() )
        {
            return( maybeVal.value() );
        }
        else
        {
            return( float(0xFFFFFFFF) );
        }    
    }; 
};


class UnknownSensor{};



/********************************* Functional Test ************************************/

int main()
{
    IntSensor f1{"fa"};
    StringSensor f2{"fb"};
    FloatSensor f3{"fc"};
    UnknownSensor f4;                   
         
    f1.Sample();
    f2.Sample();
    f3.Sample();
    
    std::cout << "\nRESULT: " << f1.GetValue() << std::endl;    
    std::cout << "\nRESULT: " << f2.GetValue() << std::endl;
    std::cout << "\nRESULT: " << f3.GetValue() << std::endl;
  
    //f1.Sample();
    //f2.Sample();
    //f3.Sample();
    
    std::cout << "\nRESULT: " << f1.GetValue() << std::endl;    
    std::cout << "\nRESULT: " << f2.GetValue() << std::endl;
    std::cout << "\nRESULT: " << f3.GetValue() << std::endl;
      
    return 0;
}


