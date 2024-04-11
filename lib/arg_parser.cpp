#include "arg_parser.h"
#include <string>

using namespace ArgumentParser;


bool ArgumentParser::isdigit(const std::string& string) {
    for (char i : string) {
        if (!std::isdigit(i))
            return false;
    }
    return true;
}

std::string Argument::GetLongFlag() {
    return long_flag;
}
char Argument::GetShortFlag() const {
    return short_flag;
}
std::string Argument::GetDescription() const {
    return description;
}
int Argument::GetMultiValue() const {
    return multivalue;
}
bool Argument::IsPositional() const {
    return is_positional;
}
bool Argument::IsDefaultValueProvided() const {
    return is_default_value_provided;
}

bool* BoolArgument::GetValueContainer() {
    return value_container;
}
std::vector<bool>* BoolArgument::GetValuesContainer() {
    return values_container;
}
BoolArgument::BoolArgument(const std::string& long_flag, char short_flag, const std::string& description) {
    this->long_flag = long_flag;
    this->short_flag = short_flag;
    this->description = description;
}
BoolArgument& BoolArgument::Default(bool value) {
    is_default_value_provided = true;
    default_value = value;
    return *this;
}
BoolArgument& BoolArgument::MultiValue(int amount) {
    multivalue = amount;
    return *this;
}
BoolArgument& BoolArgument::StoreValue(bool &container) {
    is_container_given = true;
    value_container = &container;
    return *this;
}
BoolArgument &BoolArgument::StoreValues(std::vector<bool>& container) {
    is_container_given = true;
    values_container = &container;
    return *this;
}
BoolArgument& BoolArgument::Positional() {
    is_positional = true;
    return *this;
}
void BoolArgument::SetValue(bool value) {
    if (multivalue < 0) {
        if (!value_container) {
            value_container = new bool;
        }
        *value_container = value;
        is_value_provided = true;
    } else {
        if (!values_container) {
            values_container = new std::vector<bool>;
        }
        values_container->push_back(value);
    }
}
bool BoolArgument::IsFull() {
    if (multivalue < 0) {
        return is_value_provided;
    } else {
        return values_container != nullptr && values_container->size() >= multivalue;
    }
}
bool BoolArgument::SetEmptyCellsToDefaultValue() {
    if (IsFull()) {
        return true;
    }
    if (multivalue < 0 && !is_value_provided) {
        SetValue(default_value);
    } else if (multivalue > 0) {
        while (values_container->size() < multivalue) {
            SetValue(default_value);
        }
    }
    return true;
}

bool BoolArgument::GetDefaultValue() const {
    return default_value;
}

BoolArgument::~BoolArgument() {
    if (!is_container_given) {
        delete value_container;
        delete values_container;
    }
}

int* IntArgument::GetValueContainer() {
    return value_container;
}
std::vector<int>* IntArgument::GetValuesContainer() {
    return values_container;
}
IntArgument::IntArgument(const std::string& long_flag, char short_flag, const std::string& description) {
    this->long_flag = long_flag;
    this->short_flag = short_flag;
    this->description = description;
}
IntArgument& IntArgument::Default(int value) {
    is_default_value_provided = true;
    default_value = value;
    return *this;
}
IntArgument& IntArgument::MultiValue(int amount) {
    multivalue = amount;
    return *this;
}
IntArgument& IntArgument::StoreValue(int& container) {
    is_container_given = true;
    value_container = &container;
    return *this;
}
IntArgument& IntArgument::StoreValues(std::vector<int>& container) {
    is_container_given = true;
    values_container = &container;
    return *this;
}
IntArgument& IntArgument::Positional() {
    is_positional = true;
    return *this;
}
void IntArgument::SetValue(int value) {
    if (multivalue < 0) {
        if (!value_container) {
            value_container = new int;
        }
        *value_container = value;
        is_value_provided = true;
    } else {
        if (!values_container) {
            values_container = new std::vector<int>;
        }
        values_container->push_back(value);
    }
}
bool IntArgument::IsFull() {
    if (multivalue < 0) {
        return is_value_provided;
    } else {
        return values_container != nullptr && values_container->size() >= multivalue;
    }
}
bool IntArgument::SetEmptyCellsToDefaultValue() {
    if (multivalue == SpecialMultiValue::NotLimited) {
        return true;
    }
    if (!is_default_value_provided) {
        return IsFull();
    }
    if (multivalue < 0 && !is_value_provided) {
        SetValue(default_value);
    } else if (multivalue > 0) {
        while (values_container->size() < multivalue) {
            SetValue(default_value);
        }
    }
    return true;
}

int IntArgument::GetDefaultValue() const {
    return default_value;
}

IntArgument::~IntArgument() {
    if (!is_container_given) {
        delete value_container;
        delete values_container;
    }
}

std::string* StringArgument::GetValueContainer() {
    return value_container;
}
std::vector<std::string>* StringArgument::GetValuesContainer() {
    return values_container;
}
StringArgument::StringArgument(const std::string& long_flag, char short_flag, const std::string& description) {
    this->long_flag = long_flag;
    this->short_flag = short_flag;
    this->description = description;
}
StringArgument& StringArgument::Default(const std::string& value) {
    is_default_value_provided = true;
    default_value = value;
    return *this;
}
StringArgument& StringArgument::MultiValue(int amount) {
    multivalue = amount;
    return *this;
}
StringArgument& StringArgument::StoreValue(std::string& container) {
    is_container_given = true;
    value_container = &container;
    return *this;
}
StringArgument& StringArgument::StoreValues(std::vector<std::string>& container) {
    is_container_given = true;
    values_container = &container;
    return *this;
}
StringArgument& StringArgument::Positional() {
    is_positional = true;
    return *this;
}
void StringArgument::SetValue(std::string value) {
    if (multivalue < 0) {
        if (!value_container) {
            value_container = new std::string;
        }
        *value_container = value;
        is_value_provided = true;
    } else {
        if (!values_container) {
            values_container = new std::vector<std::string>;
        }
        values_container->push_back(value);
    }
}
bool StringArgument::IsFull() {
    if (multivalue < 0) {
        return is_value_provided;
    } else {
        return values_container != nullptr && values_container->size() >= multivalue;
    }
}
bool StringArgument::SetEmptyCellsToDefaultValue() {
    if (multivalue == SpecialMultiValue::NotLimited) {
        return true;
    }
    if (!is_default_value_provided) {
        return IsFull();
    }
    if (multivalue < 0 && !is_value_provided) {
        SetValue(default_value);
    } else if (multivalue > 0) {
        while (values_container->size() < multivalue) {
            SetValue(default_value);
        }
    }
    return true;
}
std::string StringArgument::GetDefaultValue() const {
    return default_value;
}

StringArgument::~StringArgument() {
    if (!is_container_given) {
        delete value_container;
        delete values_container;
    }
}

ArgParser::ArgParser(const std::string& name) {
    this->parser_name = name;
}

bool ArgParser::HandleHelpLongFlag(const std::string& long_flag) {
    for (int i = 0; i < help_arguments.size(); i++) {
        if (help_arguments[i]->GetLongFlag() == long_flag) {
            are_they_asking_for_help = true;
            help_argument_index = i;
            return true;
        }
    }
    return false;
}
bool ArgParser::HandleHelpShortFlag(char short_flag) {
    for (int i = 0; i < help_arguments.size(); i++) {
        if (help_arguments[i]->GetShortFlag() == short_flag) {
            are_they_asking_for_help = true;
            help_argument_index = i;
            return true;
        }
    }
    return false;
}

bool ArgParser::HandleLongFlag(const std::vector<std::string>& strings, int& i) {
    std::string long_flag;
    std::string value;
    bool is_there_no_argument_next = false;
    size_t equal_sign_index;

    equal_sign_index = strings[i].find('=', 2);
    if (equal_sign_index == std::string::npos) {
        long_flag = strings[i].substr(2);
        is_there_no_argument_next = (i + 1 >= strings.size()) || (strings[i + 1][0] == '-');
    } else {
        long_flag = strings[i].substr(2, equal_sign_index - 2);
        value = strings[i].substr(equal_sign_index + 1);
    }
    if (HandleHelpLongFlag(long_flag)) {
        return true;
    }

    ArgumentPointer* argument = GetFirstNotFullArgument(long_flag);
    if (argument == nullptr) {
        error = ArgParserError::UnexpectedArgument;
        return false;
    }
    if (argument->type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument->argument);
        if (equal_sign_index == std::string::npos) {
            bool_argument->SetValue(true);
        } else {
            bool_argument->SetValue(value != kFalseString);
        }
        return true;
    }
    if (equal_sign_index == std::string::npos) {
        if (is_there_no_argument_next) {
            return false;
        }
        value = strings[++i];
    }
    if (argument->type == ArgumentTypes::IntArgumentType) {
        if (!isdigit(value)) {
            error = IntArgumentIsNotDigit;
            return false;
        }
        auto* int_argument = reinterpret_cast<IntArgument*>(argument->argument);
        int_argument->SetValue(std::stoi(value));
    } else if (argument->type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument->argument);
        string_argument->SetValue(value);
    }
    return true;
}

bool ArgParser::HandleShortFlag(const std::vector<std::string>& strings, int& i) {
    std::string flags;
    char short_flag;
    std::string value;
    bool is_there_no_argument_next = false;
    size_t equal_sign_index;

    equal_sign_index = strings[i].find('=', 1);
    if (equal_sign_index == std::string::npos) {
        flags = strings[i].substr(1);
        is_there_no_argument_next = (i + 1 >= strings.size()) || (strings[i + 1][0] == '-');
    } else {
        flags = strings[i].substr(1, equal_sign_index - 1);
        value = strings[i].substr(equal_sign_index + 1);
    }
    if (equal_sign_index == std::string::npos && !is_there_no_argument_next) {
        value = strings[++i];
    }
    for (int j = 0; j < flags.size(); j++) {
        short_flag = flags[j];
        if (HandleHelpShortFlag(short_flag)) {
            continue;
        }
        ArgumentPointer* argument = GetFirstNotFullArgument(short_flag);
        if (argument == nullptr) {
            error = ArgParserError::UnexpectedArgument;
            return false;
        }
        if (argument->type == ArgumentTypes::BoolArgumentType) {
            auto* bool_argument = reinterpret_cast<BoolArgument*>(argument->argument);
            if (equal_sign_index == std::string::npos) {
                bool_argument->SetValue(true);
            } else {
                bool_argument->SetValue(value != kFalseString);
            }
        }

        if (argument->type == ArgumentTypes::IntArgumentType) {
            if (!isdigit(value)) {
                error = IntArgumentIsNotDigit;
                return false;
            }
            auto* int_argument = reinterpret_cast<IntArgument*>(argument->argument);
            int_argument->SetValue(std::stoi(value));
        } else if (argument->type == ArgumentTypes::StringArgumentType) {
            auto* string_argument = reinterpret_cast<StringArgument*>(argument->argument);
            string_argument->SetValue(value);
        }
    }
    return true;
}

bool ArgParser::HandlePositionalArgument(const std::vector<std::string>& strings, int i) {
    for (ArgumentPointer* argument : positional_arguments) {
        if (!argument->IsFull() || argument->GetMultiValue() >= 0) {
            if (argument->type == ArgumentTypes::BoolArgumentType) {
                auto *bool_argument = reinterpret_cast<BoolArgument *>(argument->argument);
                bool_argument->SetValue(strings[i] != kFalseString);
            } else if (argument->type == ArgumentTypes::IntArgumentType) {
                if (!isdigit(strings[i])) {
                    error = IntArgumentIsNotDigit;
                    return false;
                }
                auto *int_argument = reinterpret_cast<IntArgument *>(argument->argument);
                int_argument->SetValue(std::stoi(strings[i]));
            } else if (argument->type == ArgumentTypes::StringArgumentType) {
                auto *string_argument = reinterpret_cast<StringArgument *>(argument->argument);
                string_argument->SetValue(strings[i]);
            }
            return true;
        }
    }
    error = ArgParserError::UnexpectedArgument;
    return false;
}

bool ArgParser::Parse(int argc, char** argv) {
    std::vector<std::string> strings;
    for (int i = 0; i < argc; i++) {
        strings.push_back(argv[i]);
    }
    return Parse(strings);
}

bool ArgParser::Parse(const std::vector<std::string>& strings) {
    for (ArgumentPointer* i : arguments) {
        if (i->IsPositional())
            positional_arguments.push_back(i);
    }

    application_name = strings[0];

    for (int i = 1; i < strings.size(); i++) {
        if (strings[i].empty()) {
            continue;
        }

        if (strings[i].find("--", 0) == 0) {
            if (!HandleLongFlag(strings, i)) {
                return false;
            }
        } else if (strings[i][0] == '-') {
            if (!HandleShortFlag(strings, i))
                return false;
        } else {
            if (!HandlePositionalArgument(strings, i))
                return false;
        }
    }
    return are_they_asking_for_help || CheckArguments();
}
void ArgParser::AddHelp(const std::string& long_flag, const std::string& description) {
    AddHelp('\0', long_flag, description);
}
void ArgParser::AddHelp(const char short_flag, const std::string& long_flag, const std::string& description) {
    help_arguments.push_back(new HelpArgument(short_flag, long_flag, description));
}
bool ArgParser::Help() const {
    return are_they_asking_for_help;
}
std::string ArgParser::HelpDescription() {
    if (help_argument_index == -1) {
        help_argument_index = 0;
    }
    HelpArgument* help_argument = help_arguments[help_argument_index];
    std::string help_description = parser_name + "\n" +
            help_argument->description + "\n\n";

    if (help_argument->GetShortFlag() != '\0')
        help_description += std::string("-") + help_argument->GetShortFlag() + ",";
    help_description += "\t--" + help_argument->GetLongFlag() + "\t\tShow this message\n";

    for (int i = 0; i < arguments.size(); i++) {
        if (arguments[i]->GetShortFlag() != '\0')
            help_description += std::string("-") + arguments[i]->GetShortFlag() + ",";
        help_description += "\t--" + arguments[i]->GetLongFlag();

        if (arguments[i]->type == ArgumentTypes::IntArgumentType) {
            help_description += "=<int>";
        } else if (arguments[i]->type == ArgumentTypes::StringArgumentType) {
            help_description += "=<string>";
        }


        if (!arguments[i]->GetDescription().empty()) {
            help_description += "\t\t" + arguments[i]->GetDescription();
        }
        if (arguments[i]->GetMultiValue() != SpecialMultiValue::NoMultiValue || arguments[i]->IsPositional() ||
        arguments[i]->IsDefaultValueProvided()) {
            help_description += " [";
            if (arguments[i]->GetMultiValue() != SpecialMultiValue::NoMultiValue) {
                help_description += "multivalue";
                if (arguments[i]->GetMultiValue() != SpecialMultiValue::NotLimited) {
                    help_description += ", minimum " + std::to_string(arguments[i]->GetMultiValue()) + " argument";
                    if (arguments[i]->GetMultiValue() > 1) {
                        help_description += "s";
                    }
                }
                help_description += "; ";
            }
            if (arguments[i]->IsPositional()) {
                help_description += "positional; ";
            }
            if (arguments[i]->IsDefaultValueProvided()) {
                help_description += "by default = " + arguments[i]->GetDefaultValue() + "; ";
            }
            help_description.pop_back();
            help_description.pop_back();
            help_description += "]";
        }
        help_description += "\n";
    }
    return help_description;
}

StringArgument& ArgParser::AddStringArgument(const std::string& long_flag, const std::string& description) {
    return AddStringArgument('\0', long_flag, description);
}
StringArgument& ArgParser::AddStringArgument(const char short_flag, const std::string& long_flag, const std::string& description) {
    auto* new_argument = new StringArgument(long_flag, short_flag, description);
    auto* ptr = new ArgumentPointer{new_argument, ArgumentTypes::StringArgumentType};
    AddArgument(ptr);
    return *new_argument;
}
std::string ArgParser::GetStringValue(const std::string& long_flag, int index) {
    StringArgument* found_argument = GetStringArgument(long_flag);
    if (!found_argument) {
        return "";
    }
    if (found_argument->GetMultiValue() < 0) {
        return *found_argument->GetValueContainer();
    } else if (index >= found_argument->GetValuesContainer()->size()) {
        return "";
    } else {
        return (*found_argument->GetValuesContainer())[index];
    }
}

IntArgument& ArgParser::AddIntArgument(const std::string& long_flag, const std::string& description) {
    return AddIntArgument('\0', long_flag, description);
}
IntArgument& ArgParser::AddIntArgument(const char short_flag, const std::string& long_flag, const std::string& description) {
    auto* new_argument = new IntArgument(long_flag, short_flag, description);
    auto* ptr = new ArgumentPointer{new_argument, ArgumentTypes::IntArgumentType};
    AddArgument(ptr);
    return *new_argument;
}
int ArgParser::GetIntValue(const std::string& long_flag, int index) {
    IntArgument* found_argument = GetIntArgument(long_flag);
    if (!found_argument) {
        return 0;
    }
    if (found_argument->GetMultiValue() < 0) {
        return *found_argument->GetValueContainer();
    } else if (index >= found_argument->GetValuesContainer()->size()) {
        return 0;
    } else {
        return (*found_argument->GetValuesContainer())[index];
    }
}

BoolArgument& ArgParser::AddFlag(const std::string& long_flag, const std::string& description) {
    return AddFlag('\0', long_flag, description);

}
BoolArgument& ArgParser::AddFlag(const char short_flag, const std::string& long_flag, const std::string& description) {
    auto* new_argument = new BoolArgument(long_flag, short_flag, description);
    auto* ptr = new ArgumentPointer{new_argument, ArgumentTypes::BoolArgumentType};
    AddArgument(ptr);
    return *new_argument;
}
bool ArgParser::GetFlag(const std::string& long_flag, int index) {
    BoolArgument* found_argument = GetBoolArgument(long_flag);
    if (!found_argument) {
        return false;
    }
    if (found_argument->GetMultiValue() < 0) {
        return *found_argument->GetValueContainer();
    } else if (index >= found_argument->GetValuesContainer()->size()) {
        return false;
    } else {
        return (*found_argument->GetValuesContainer())[index];
    }
}

BoolArgument* ArgParser::GetBoolArgument(const std::string& long_flag) {
    for (ArgumentPointer* i : arguments) {
        if (i->type != ArgumentTypes::BoolArgumentType) {
            continue;
        }
        auto* argument = reinterpret_cast<BoolArgument*>(i->argument);
        if (argument->GetLongFlag() == long_flag) {
            return argument;
        }
    }
    return nullptr;
}
IntArgument* ArgParser::GetIntArgument(const std::string& long_flag) {
    for (ArgumentPointer* i : arguments) {
        if (i->type != ArgumentTypes::IntArgumentType) {
            continue;
        }
        auto* argument = reinterpret_cast<IntArgument*>(i->argument);
        if (argument->GetLongFlag() == long_flag) {
            return argument;
        }
    }
    return nullptr;
}
StringArgument* ArgParser::GetStringArgument(const std::string& long_flag) {
    for (ArgumentPointer* i : arguments) {
        if (i->type != ArgumentTypes::StringArgumentType) {
            continue;
        }
        auto* argument = reinterpret_cast<StringArgument*>(i->argument);
        if (argument->GetLongFlag() == long_flag) {
            return argument;
        }
    }
    return nullptr;
}

void ArgParser::AddArgument(ArgumentPointer* ptr) {
    arguments.push_back(ptr);
}

ArgumentPointer* ArgParser::GetFirstNotFullArgument(const std::string& long_flag) {
    return GetFirstNotFullArgument('\0', long_flag);
}
ArgumentPointer* ArgParser::GetFirstNotFullArgument(char short_flag) {
    return GetFirstNotFullArgument(short_flag, "");
}
ArgumentPointer* ArgParser::GetFirstNotFullArgument(char short_flag, const std::string& long_flag) {
    ArgumentPointer* argument = nullptr;
    for (size_t i = 0; i < arguments.size(); i++) {
        if (arguments[i]->GetShortFlag() != short_flag && short_flag != '\0') {
            continue;
        }
        if (arguments[i]->GetLongFlag() != long_flag && !long_flag.empty()) {
            continue;
        }
        argument = arguments[i];
        if (!argument->IsFull()) {
            return argument;
        }
    }
    return argument;
}
bool ArgParser::CheckArguments() {
    for (ArgumentPointer* i : arguments) {
        if (!i->SetEmptyCellsToDefaultValue()) {
            error = ArgParserError::NotEnoughArguments;
            return false;
        }
    }
    return true;
}

ArgParser::~ArgParser() {
    for (ArgumentPointer* i : arguments) {
        delete i;
    }
    for (HelpArgument* i : help_arguments) {
        delete i;
    }
}

bool ArgumentPointer::IsPositional() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->IsPositional();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->IsPositional();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->IsPositional();
    }
    return false;
}
int ArgumentPointer::GetMultiValue() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->GetMultiValue();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->GetMultiValue();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->GetMultiValue();
    }
    return -1;
}
std::string ArgumentPointer::GetLongFlag() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->GetLongFlag();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->GetLongFlag();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->GetLongFlag();
    }
    return "";
}
char ArgumentPointer::GetShortFlag() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->GetShortFlag();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->GetShortFlag();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->GetShortFlag();
    }
    return '\0';
}
bool ArgumentPointer::IsFull() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->IsFull();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->IsFull();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->IsFull();
    }
    return "";
}
bool ArgumentPointer::SetEmptyCellsToDefaultValue() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->SetEmptyCellsToDefaultValue();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->SetEmptyCellsToDefaultValue();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->SetEmptyCellsToDefaultValue();
    }
    return false;
}
bool ArgumentPointer::IsDefaultValueProvided() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->IsDefaultValueProvided();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->IsDefaultValueProvided();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->IsDefaultValueProvided();
    }
    return false;
}
std::string ArgumentPointer::GetDefaultValue() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->GetDefaultValue() ? kTrueString : kFalseString;
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return std::to_string(int_argument->GetDefaultValue());
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return "\"" + string_argument->GetDefaultValue() + "\"";
    }
    return "";
}
std::string ArgumentPointer::GetDescription() const {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        return bool_argument->GetDescription();
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        return int_argument->GetDescription();
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        return string_argument->GetDescription();
    }
    return "";
}

ArgumentPointer::~ArgumentPointer() {
    if (type == ArgumentTypes::BoolArgumentType) {
        auto* bool_argument = reinterpret_cast<BoolArgument*>(argument);
        delete bool_argument;
    } else if (type == ArgumentTypes::IntArgumentType) {
        auto* int_argument = reinterpret_cast<IntArgument*>(argument);
        delete int_argument;
    } else if (type == ArgumentTypes::StringArgumentType) {
        auto* string_argument = reinterpret_cast<StringArgument*>(argument);
        delete string_argument;
    }
}

HelpArgument::HelpArgument(char short_flag, const std::string& long_flag, const std::string& description) {
    this->short_flag = short_flag;
    this->long_flag = long_flag;
    this->description = description;
}
