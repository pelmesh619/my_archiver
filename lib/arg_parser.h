#pragma once

#include <iostream>
#include <vector>
#include <string>


namespace ArgumentParser {
const std::string kTrueString = "true";
const std::string kFalseString = "false";

bool isdigit(const std::string& string);

enum SpecialMultiValue : int {
    NoMultiValue = -1,
    NotLimited = 0,
};

class ArgParser;

class Argument {
public:
    std::string description;
    std::string GetLongFlag();
    char GetShortFlag() const;
    std::string GetDescription() const;
    int GetMultiValue() const;
    bool IsPositional() const;
    bool IsDefaultValueProvided() const;
protected:
    bool is_default_value_provided = false;
    bool is_value_provided = false;
    bool is_positional = false;
    int multivalue = SpecialMultiValue::NoMultiValue;
    char short_flag = '\0';
    std::string long_flag;
    bool is_container_given = false;
};

class StringArgument : public Argument {
public:
    StringArgument(const std::string& long_flag, char short_flag = '\0', const std::string& description = "");
    ~StringArgument();
    StringArgument& Default(const std::string& value);
    StringArgument& StoreValue(std::string& container);
    StringArgument& StoreValues(std::vector<std::string>& container);
    StringArgument& MultiValue(int amount = SpecialMultiValue::NotLimited);
    StringArgument& Positional();

    std::string* GetValueContainer();
    std::vector<std::string>* GetValuesContainer();
    void SetValue(std::string value);
    bool IsFull();
    bool SetEmptyCellsToDefaultValue();
    std::string GetDefaultValue() const;
private:
    std::string default_value;
    std::string* value_container = nullptr;
    std::vector<std::string>* values_container = nullptr;
};

class IntArgument : public Argument {
public:
    IntArgument(const std::string& long_flag, char short_flag = '\0', const std::string& description = "");
    ~IntArgument();
    IntArgument& Default(int value);
    IntArgument& StoreValue(int& container);
    IntArgument& StoreValues(std::vector<int>& container);
    IntArgument& MultiValue(int amount = SpecialMultiValue::NotLimited);
    IntArgument& Positional();

    int* GetValueContainer();
    std::vector<int>* GetValuesContainer();
    void SetValue(int value);
    bool IsFull();
    bool SetEmptyCellsToDefaultValue();
    int GetDefaultValue() const;
private:
    int default_value;
    int* value_container = nullptr;
    std::vector<int>* values_container = nullptr;
};

class BoolArgument : public Argument {
public:
    BoolArgument(const std::string& long_flag, char short_flag = '\0', const std::string& description = "");
    ~BoolArgument();
    BoolArgument& Default(bool value);
    BoolArgument& StoreValue(bool& container);
    BoolArgument& StoreValues(std::vector<bool>& container);
    BoolArgument& MultiValue(int amount = SpecialMultiValue::NotLimited);
    BoolArgument& Positional();

    bool* GetValueContainer();
    std::vector<bool>* GetValuesContainer();
    void SetValue(bool value);
    bool IsFull();
    bool SetEmptyCellsToDefaultValue();
    bool GetDefaultValue() const;
private:
    bool default_value = false;
    bool* value_container = nullptr;
    std::vector<bool>* values_container = nullptr;
};

class HelpArgument : public Argument {
public:
    HelpArgument(char short_flag, const std::string& long_flag, const std::string& description);
};

enum ArgumentTypes {
    NotClassified,
    BoolArgumentType,
    IntArgumentType,
    StringArgumentType,
};

struct ArgumentPointer {
    void* argument = nullptr;
    ~ArgumentPointer();
    ArgumentTypes type = ArgumentTypes::NotClassified;
    bool IsPositional() const;
    int GetMultiValue() const;
    std::string GetLongFlag() const;
    char GetShortFlag() const;
    std::string GetDescription() const;
    bool IsFull() const;
    bool SetEmptyCellsToDefaultValue() const;
    bool IsDefaultValueProvided() const;
    std::string GetDefaultValue() const;
};

enum ArgParserError {
    NoError,
    UnexpectedArgument,
    IntArgumentIsNotDigit,
    NotEnoughArguments,
};

class ArgParser {
public:
    ArgParserError error = NoError;
    std::string parser_name;
    std::string application_name;

    ArgParser(const std::string& name);
    ~ArgParser();
    bool Parse(int argc, char** argv);
    bool Parse(const std::vector<std::string>& strings);
    void AddHelp(char short_flag, const std::string& long_flag, const std::string& description = "");
    void AddHelp(const std::string& long_flag, const std::string& description);
    bool Help() const;
    std::string HelpDescription();

    StringArgument& AddStringArgument(const std::string& long_flag, const std::string& description = "");
    StringArgument& AddStringArgument(char short_flag, const std::string& long_flag, const std::string& description = "");
    std::string GetStringValue(const std::string& long_flag, int index = 0);

    IntArgument& AddIntArgument(const std::string& long_flag, const std::string& description = "");
    IntArgument& AddIntArgument(char short_flag, const std::string& long_flag, const std::string& description = "");
    int GetIntValue(const std::string& long_flag, int index = 0);

    BoolArgument& AddFlag(const std::string& long_flag, const std::string& description = "");
    BoolArgument& AddFlag(char short_flag, const std::string& long_flag, const std::string& description = "");
    bool GetFlag(const std::string& long_flag, int index = 0);

private:
    bool are_they_asking_for_help = false;
    int help_argument_index = -1;
    std::vector<ArgumentPointer*> arguments;
    std::vector<ArgumentPointer*> positional_arguments;
    std::vector<HelpArgument*> help_arguments;

    bool HandleLongFlag(const std::vector<std::string>& strings, int& i);
    bool HandleShortFlag(const std::vector<std::string>& strings, int& i);
    bool HandlePositionalArgument(const std::vector<std::string>& strings, int i);

    BoolArgument* GetBoolArgument(const std::string& long_flag);
    IntArgument* GetIntArgument(const std::string& long_flag);
    StringArgument* GetStringArgument(const std::string& long_flag);
    ArgumentPointer* GetFirstNotFullArgument(const std::string& long_flag);
    ArgumentPointer* GetFirstNotFullArgument(char short_flag);
    ArgumentPointer* GetFirstNotFullArgument(char short_flag, const std::string& long_flag);

    bool HandleHelpLongFlag(const std::string& long_flag);
    bool HandleHelpShortFlag(char short_flag);

    void AddArgument(ArgumentPointer* ptr);
    bool CheckArguments();
};

}