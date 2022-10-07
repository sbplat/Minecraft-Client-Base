#ifndef MAPPER_H
#define MAPPER_H

#include <map>
#include <set>
#include <string>
#include <vector>

enum class CLIENT {
    VANILLA,
    FORGE,
    FORGE_ESSENTIAL,
    LUNAR,   // TODO: Implement
    BADLION  // TODO: Implement
};

class Mapper {
private:
    CLIENT Client;

    // File string buffers
    std::string joined, fields, methods, params, static_methods;

    // searge: obfuscated name (getMinecraft)
    // retroguard: mcp obfuscation (method_12345, field_12345)
    // proguard: vanilla obfuscation (ave)

    // joined.srg
    std::map<std::string, std::string> searge_to_proguard_class_name /*, proguard_to_searge_class_name*/;
    std::map<std::string, std::map<std::string, std::string>> /*proguard_to_retroguard_field_name,*/ searge_to_proguard_field_name,
        searge_to_retroguard_field_name;
    std::map<std::string, std::map<std::string, std::map<std::string, std::string>>> /*proguard_to_retroguard_method_name,*/
        searge_to_proguard_method_name, searge_to_retroguard_method_name;
    std::map<std::string, std::string> searge_to_proguard_method_descriptor;

    // fields.csv
    std::map<std::string, std::string> retroguard_field_to_searge /*, searge_to_retroguard_field*/;

    // methods.csv
    std::map<std::string, std::string> retroguard_method_to_searge /*, searge_to_retroguard_method*/;

    // static_methods.txt
    std::set<std::string> static_methods_searge /*, static_methods_retroguard*/;

public:
    Mapper();
    ~Mapper();

    // Set the Minecraft client for automatic mapping
    // client: The Minecraft client to set.
    void SetClient(CLIENT client);

    // Get the Minecraft client
    CLIENT GetClient();

    // Reads a file into a string
    // filePath: The path of the file to read.
    // out: The string to read the file into.
    // binaryFile: Whether or not to open the file using std::ios::binary.
    void Load(std::string filePath, std::string &out, bool binaryFile = false);

    // Splits a string into a vector of strings by a delimiter
    // str: The string to split.
    // delim: The delimiter to split by.
    std::vector<std::string> Split(std::string &str, char delim);

    // Parses the data from the file string buffers into maps
    void ParseData();

    std::string SeargeToProguardClass(std::string seargeClassName);

    std::string SeargeToProguardField(std::string seargeClassName, std::string seargeFieldName);
    std::string SeargeToRetroguardField(std::string seargeClassName, std::string seargeFieldName);

    std::string SeargeToProguardMethod(std::string seargeClassName, std::string seargeMethodName, std::string seargeMethodDescriptor);
    std::string SeargeToRetroguardMethod(std::string seargeClassName, std::string seargeMethodName, std::string seargeMethodDescriptor);

    bool IsSeargeStaticMethod(std::string seargeMethodName);

    std::string SeargeToFieldDescriptor(std::string seargeClassName);
    std::string SeargeToMethodDescriptor(std::string seargeMethodDescriptor);
    std::string SeargeToClientClass(std::string seargeClassName);
    std::string SeargeToClientField(std::string seargeClassName, std::string seargeFieldName);
    std::string SeargeToClientMethod(std::string seargeClassName, std::string seargeMethodName, std::string seargeMethodDescriptor);
};

extern Mapper mapper;

#endif  // MAPPER_H
