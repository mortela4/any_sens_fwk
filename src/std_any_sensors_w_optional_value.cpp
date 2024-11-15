/**
@file std_any_iterator_ex1.cpp

@brief Demo of iteration over 'std::any' collections.
*/

#include <iostream>
#include <string>
#include <type_traits>
#include <array>
#include <any>
#include <tuple>
#include <variant>
#include <optional>


using sensor_data_t = std::optional<std::variant<int, float, std::string>>;


class SensorBase
{
    private:
    std::string _sensorInfo;
    sensor_data_t _sensorVal;
    public:
    SensorBase(const std::string info, sensor_data_t initVal) : _sensorInfo(info), _sensorVal(initVal) {};
    sensor_data_t GetVal(void) { return(_sensorVal); };
    void SetData(sensor_data_t val) { _sensorVal = val; };
};


class IntSensor : public SensorBase
{
    public:
    IntSensor(const std::string info, int ival) : SensorBase(info, static_cast<sensor_data_t>(ival)) {};
    void Sample(void)
    {
        static int iVal = 77777;

        SetData(iVal);
    
        iVal += 1234;
    };
    int GetValue(bool resetValueAfterRead=false)
    { 
        const auto maybeVal = GetVal();
        
        if (resetValueAfterRead)
        {
            SetData(std::nullopt);
        }
        
        if ( maybeVal.has_value() )
        {
            return(std::get<int>(maybeVal.value()));
        }
        else
        {
            return(int(0xFFFFFFFF));
        }
    };
};


class StringSensor : public SensorBase
{
    public:
    StringSensor(const std::string info, std::string sval) : SensorBase(info, static_cast<sensor_data_t>(sval)) {};
    void Sample(void)
    {
        static int count = 1;
        
        const std::string sVal = "A";

        SetData(sVal + std::to_string(count));
        
        ++count;
    };
    std::string GetValue(bool resetValueAfterRead=true)
    { 
        const auto maybeVal = GetVal();
    
        if (resetValueAfterRead)
        {
            SetData(std::nullopt);
        }

        if ( maybeVal.has_value() )
        {
            return( std::get<std::string>(maybeVal.value()) );
        }
        else
        {
            return("<no value>");
        }
    }; 
};


class FloatSensor : public SensorBase
{
    public:
    FloatSensor(const std::string info, float fval) : SensorBase(info, static_cast<sensor_data_t>(fval)) {};
    void Sample(void)
    {
        static float fVal = 9.563F;
    
        SetData( fVal );
        
        fVal += 1.234F;
    };
    float GetValue(bool resetValueAfterRead=false)
    { 
        const auto maybeVal = GetVal();
        
        if (resetValueAfterRead)
        {
            SetData(std::nullopt);
        }

        if ( maybeVal.has_value() )
        {
            return( std::get<float>(maybeVal.value()) );
        }
        else
        {
            return( float(0xFFFFFFFF) );
        }    
    }; 
};


class UnknownSensor{};


// Handlers:

std::string AddFloatVal(class FloatSensor& fsens)
{
    const auto val = fsens.GetValue();
    
    std::cout << "FLOAT. Value = " << val << std::endl;
    
    return(std::to_string(val));
}

std::string AddIntVal(class IntSensor& isens)
{
    const auto val = isens.GetValue();
    
    std::cout << "INT. Value = " << val << std::endl;
    
    return(std::to_string(val));
}


std::string AddStrVal(class StringSensor& ssens)
{
    const auto val = ssens.GetValue();
    
    std::cout << "STRING. Value = " << val << std::endl;
    
    return(val);
}

template <typename T>
std::string AddVal(T& sensor)
{
    if constexpr (std::is_same<T, class IntSensor>()) 
    //if ( std::is_same<T, class IntSensor>() )
    {
        std::cout << "IntSensor" << std::endl;

        return(std::to_string(sensor.GetValue()));
    }
    if constexpr (std::is_same<T, class FloatSensor>()) 
    //else if ( std::is_same<T, class FloatSensor>() )
    {
        std::cout << "FloatSensor" << std::endl;

        return(std::to_string(sensor.GetValue()));
    }
    if constexpr (std::is_same<T, class StringSensor>()) 
    //else if ( std::is_same<T, class StringSensor>() )
    {
        std::cout << "StringSensor" << std::endl;

        return(sensor.GetValue());
    }
    else
    {
        std::cout << "ERROR: unknown sensor!" << std::endl;

        return("<unknown>");    // Or, return EMPTY string??? (but, we should NEVER get here!!!)
    }
}


/**********************************************************************************/

void sample_data(std::any& sensor)
{
    if (sensor.has_value())
    {
        if ( sensor.type() == typeid(class IntSensor) )
        {
            std::any_cast<struct IntSensor&>(sensor).Sample();
        }
        else if ( sensor.type() == typeid(class FloatSensor) )
        {
            std::any_cast<struct FloatSensor&>(sensor).Sample();
        }
        else if ( sensor.type() == typeid(class StringSensor) )
        {
            std::any_cast<struct StringSensor&>(sensor).Sample();
        }        
        else
        {
    	    std::cout << "ERROR: unknown sensor!" << std::endl;
        }        
    }
    else
    {
        std::cout << "WARN: empty data!" << std::endl;
    }
}


std::string process_any(std::any& sensor)
{
    std::string result = "";
    
    if (sensor.has_value())
    {
        if ( sensor.type() == typeid(class IntSensor) )
        {
            //result = AddIntVal( std::any_cast<struct IntSensor&>(sensor) );
            result = AddVal( std::any_cast<struct IntSensor&>(sensor) );
        }
        else if ( sensor.type() == typeid(class FloatSensor) )
        {
            //result = AddFloatVal( std::any_cast<struct FloatSensor&>(sensor) );
            result = AddVal( std::any_cast<struct FloatSensor&>(sensor) );
        }
        else if ( sensor.type() == typeid(class StringSensor) )
        {
            //result = AddStrVal( std::any_cast<struct StringSensor&>(sensor) );
            result = AddVal( std::any_cast<struct StringSensor&>(sensor) );
        }  
        else
        {
    	    std::cout << "ERROR: unknown sensor!" << std::endl;
        }        
    }
    else
    {
        std::cout << "WARN: empty data!" << std::endl;
    }
    
    return(result);
}


std::string collect_data(auto& sensors, bool resample=true)
{
    std::string result;
    
    for (auto& sensor : sensors)
    {
        if (resample)
        {
            sample_data(sensor);   
        }

        result += process_any(sensor) + ":";  
    } 
    
    return(result);
}


/********************************* Functional Test ************************************/

int main()
{
    IntSensor f1{"fa", 123};
    StringSensor f2{"fb", "Jadda"};
    FloatSensor f3{"fc", 3.579F};
    UnknownSensor f4;                   // Empty class - but still has VALUE! produces "Value = 0".
    std::any f5;                        // Really empty - produces warning.
    
    std::array<std::any,5> mySensors{f1, f2, f3, f4, f5};
    
    std::string result{collect_data(mySensors)};
    
    std::cout << "\nRESULT: " << result << std::endl << std::endl;  
    
    result = collect_data(mySensors, false);
    
    std::cout << "\nRESULT: " << result << std::endl;
    
    return 0;
}


