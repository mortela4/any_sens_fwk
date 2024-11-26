/**
@file std_any_iterator_ex1.cpp

@brief Demo of iteration over 'std::any' collections.
*/

#include <iostream>
#include <string>
#include <type_traits>
#include <array>
#include <tuple>
#include <variant>
#include <optional>
#include <functional>   // NOTE: for 'std::reference_wrapper<T>' class template.


using sensor_data_t = std::optional<std::variant<int, float, std::string>>;

class SensorBase
{
    private:
    std::string _sensorInfo;
    sensor_data_t _sensorVal;
    public:
    std::string GetInfo(void) { return(_sensorInfo); };
    SensorBase(const std::string info, sensor_data_t initVal) : _sensorInfo(info), _sensorVal(initVal) {};
    sensor_data_t GetVal(void) { return(_sensorVal); };
    void SetData(sensor_data_t val) 
    {
        _sensorVal = val; 
        //std::cout << _sensorInfo << ": value = " << _sensorVal.value() << std::endl;
    };
};


class IntSensor : public SensorBase
{
    public:
    IntSensor(const std::string info, int ival) : SensorBase(info, static_cast<sensor_data_t>(ival)) {};
    void Sample(void)
    {
        static int iVal = 77777;

        SetData(iVal);
        std::cout << GetInfo() << ": value = " << GetValue() << std::endl;
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
        std::cout << GetInfo() << ": value = " << GetValue() << std::endl;
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
        std::cout << GetInfo() << ": value = " << GetValue() << std::endl;
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


using sensor_types_t = std::variant<IntSensor, FloatSensor, StringSensor>;


// Handlers:
/*
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
*/

// Or - just a single, unified handler:
template <typename T>
std::string AddVal(T& sensor)
{
    if constexpr (std::is_same<T, class IntSensor>()) 
    {
        //std::cout << "IntSensor" << std::endl;

        return(std::to_string(sensor.GetValue()));
    }
    else if constexpr (std::is_same<T, class FloatSensor>()) 
    {
        //std::cout << "FloatSensor" << std::endl;

        return(std::to_string(sensor.GetValue()));
    }
    else if constexpr (std::is_same<T, class StringSensor>()) 
    {
        //std::cout << "StringSensor" << std::endl;

        return(sensor.GetValue());
    }
    else
    {
        std::cout << "ERROR: unknown sensor!" << std::endl;

        return("<unknown>");    // Or, return EMPTY string??? (but, we should NEVER get here!!!)
    }
}


/**********************************************************************************/

void sample_sensor(sensor_types_t& sensor) 
{
    std::visit([](auto&& arg) { arg.Sample(); }, sensor);
}

void read_value(sensor_types_t& sensor, const bool resetValueAfterRead=false) 
{ 
    const bool resetAfterRead = resetValueAfterRead;

    std::visit([&resetAfterRead](auto&& arg) { std::cout << "\nRESULT: " << arg.GetValue(resetAfterRead) << std::endl; }, sensor);
}


/********************************* Functional Test ************************************/

int main()
{
    sensor_types_t f1 = IntSensor("fa", 123);
    sensor_types_t f2 = StringSensor("fb", "Jadda");
    sensor_types_t f3 = FloatSensor("fc", 3.579F);
    UnknownSensor f4;                                   // Empty class - but still has VALUE! produces "Value = 0".
    
    std::array<std::reference_wrapper<sensor_types_t>,3> mySensors{std::ref(f1), std::ref(f2), std::ref(f3)};
    
    for (auto& sensor : mySensors)
    {
        sample_sensor(sensor);
        read_value(sensor, false);
    }

    for (auto& sensor : mySensors)
    {
        sample_sensor(sensor);
        read_value(sensor, true);
    }

    for (auto& sensor : mySensors)
    {
        sample_sensor(sensor);
        read_value(sensor);
    }
    
    return 0;
}


