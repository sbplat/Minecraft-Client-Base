#include "mapper.h"

#include "utils.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>

Mapper::Mapper() {
    Load(getParentPath() + "/mappings/joined.srg", joined);
    Load(getParentPath() + "/mappings/fields.csv", fields);
    Load(getParentPath() + "/mappings/methods.csv", methods);
    // Load(getParentPath() + "/mappings/params.csv", params);
    Load(getParentPath() + "/mappings/static_methods.txt", static_methods);

    ParseData();

    // Free memory from the buffers
    std::string().swap(joined);
    std::string().swap(fields);
    std::string().swap(methods);
    // std::string().swap(params);
    std::string().swap(static_methods);
}

Mapper::~Mapper() {
}

void Mapper::SetClient(CLIENT client) {
    Client = client;
}

CLIENT Mapper::GetClient() {
    return Client;
}

void Mapper::Load(std::string filePath, std::string &out, bool binaryFile) {
    std::ifstream inFile(filePath, (binaryFile ? std::ios::in | std::ios::binary : std::ios::in));
    inFile.seekg(0, std::ios::end);
    out.resize(inFile.tellg());
    inFile.seekg(0, std::ios::beg);
    inFile.read(&out[0], out.size());
    inFile.close();
}

std::vector<std::string> Mapper::Split(std::string &str, char delim) {
    std::istringstream iss(str);
    std::string element;
    std::vector<std::string> elements;

    while (std::getline(iss, element, delim)) {
        elements.push_back(element);
    }

    return elements;
}

void Mapper::ParseData() {
    // fields.csv
    std::istringstream iss(fields);
    std::string line;

    iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // ignore first line (searge,name,side,desc)

    while (std::getline(iss, line)) {
        std::vector<std::string> splitted = Split(line, ',');

        retroguard_field_to_searge[splitted[0]] = splitted[1];
        // searge_to_retroguard_field[splitted[1]] = splitted[0];
    }

    iss.clear();

    // methods.csv
    iss.str(methods);

    iss.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // ignore first line (searge,name,side,desc)

    while (std::getline(iss, line)) {
        std::vector<std::string> splitted = Split(line, ',');

        retroguard_method_to_searge[splitted[0]] = splitted[1];
        // searge_to_retroguard_method[splitted[1]] = splitted[0];
    }

    iss.clear();

    // static_methods.txt
    iss.str(static_methods);

    while (std::getline(iss, line)) {
        // static_methods_retroguard.insert(line);
        static_methods_searge.insert(retroguard_method_to_searge[line]);
    }

    iss.clear();

    // joined.retroguard
    iss.str(joined);

    while (std::getline(iss, line)) {
        std::vector<std::string> splitted = Split(line, ' ');

        if (splitted[0] == "PK:") {
            // Package
            continue;

        } else if (splitted[0] == "CL:") {
            // Class
            // splitted[1] = proguard class name
            // splitted[2] = searge class name
            // CL: ave net/minecraft/client/Minecraft
            // ave = net/minecraft/client/Minecraft
            std::string proguard_class_name = splitted[1], searge_class_name = splitted[2];
            searge_to_proguard_class_name[searge_class_name] = proguard_class_name;
            // proguard_to_searge_class_name[splitted[1]] = splitted[2];

        } else if (splitted[0] == "FD:") {
            // Field
            // splitted[1] = proguard class name/proguard field name
            // splitted[2] = searge class name/retroguard field name
            // FD: ave/f net/minecraft/client/Minecraft/field_71441_e
            // ave = net/minecraft/client/Minecraft, f = theWorld, field_71441_e = theWorld
            size_t lastSlash = splitted[1].find_last_of('/');
            std::string proguard_class_name = splitted[1].substr(0, lastSlash), proguard_field_name = splitted[1].substr(lastSlash + 1);

            lastSlash = splitted[2].find_last_of('/');
            std::string searge_class_name = splitted[2].substr(0, lastSlash), retroguard_field_name = splitted[2].substr(lastSlash + 1);

            // proguard_to_retroguard_field_name[proguard_class_name][proguard_field_name] = retroguard_field_name;

            std::string searge_field_name = retroguard_field_to_searge[retroguard_field_name];

            searge_to_proguard_field_name[searge_class_name][searge_field_name] = proguard_field_name;
            searge_to_retroguard_field_name[searge_class_name][searge_field_name] = retroguard_field_name;

        } else if (splitted[0] == "MD:") {
            // Method
            // splitted[1] = proguard class name/proguard method name
            // splitted[2] = proguard method descriptor
            // splitted[3] = searge class name/retroguard method name
            // splitted[4] = searge method descriptor
            // MD: ave/A ()Lave; net/minecraft/client/Minecraft/func_71410_x ()Lnet/minecraft/client/Minecraft;
            // ave = net/minecraft/client/Minecraft, A = getMinecraft(), ()Lave; = descriptor, func_71410_x = getMinecraft()
            size_t lastSlash = splitted[1].find_last_of('/');
            std::string proguard_class_name = splitted[1].substr(0, lastSlash), proguard_method_name = splitted[1].substr(lastSlash + 1);

            lastSlash = splitted[3].find_last_of('/');
            std::string searge_class_name = splitted[3].substr(0, lastSlash), retroguard_method_name = splitted[3].substr(lastSlash + 1);

            std::string proguard_method_descriptor = splitted[2];
            // proguard_to_retroguard_method_name[proguard_class_name][proguard_method_name][proguard_method_descriptor] = retroguard_method_name;

            std::string searge_method_descriptor = splitted[4];
            searge_to_proguard_method_descriptor[searge_method_descriptor] = proguard_method_descriptor;

            std::string searge_method_name = retroguard_method_to_searge[retroguard_method_name];
            searge_to_proguard_method_name[searge_class_name][searge_method_name][searge_method_descriptor] = proguard_method_name;
            searge_to_retroguard_method_name[searge_class_name][searge_method_name][searge_method_descriptor] = retroguard_method_name;
        }
    }
}

std::string Mapper::SeargeToProguardClass(std::string seargeClassName) {
    return searge_to_proguard_class_name.at(seargeClassName);
}


std::string Mapper::SeargeToProguardField(std::string seargeClassName, std::string seargeFieldName) {
    return searge_to_proguard_field_name.at(seargeClassName).at(seargeFieldName);
}

std::string Mapper::SeargeToRetroguardField(std::string seargeClassName, std::string seargeFieldName) {
    return searge_to_retroguard_field_name.at(seargeClassName).at(seargeFieldName);
}


std::string Mapper::SeargeToProguardMethod(std::string seargeClassName, std::string seargeMethodName, std::string seargeMethodDescriptor) {
    return searge_to_proguard_method_name.at(seargeClassName).at(seargeMethodName).at(seargeMethodDescriptor);
}

std::string Mapper::SeargeToRetroguardMethod(std::string seargeClassName, std::string seargeMethodName, std::string seargeMethodDescriptor) {
    return searge_to_retroguard_method_name.at(seargeClassName).at(seargeMethodName).at(seargeMethodDescriptor);
}


bool Mapper::IsSeargeStaticMethod(std::string seargeMethodName) {
    return static_methods_searge.count(seargeMethodName);
}


std::string Mapper::SeargeToFieldDescriptor(std::string seargeClassName) {
    return "L" + SeargeToClientClass(seargeClassName) + ";";
}

std::string Mapper::SeargeToMethodDescriptor(std::string seargeMethodDescriptor) {
    if (Client == CLIENT::VANILLA) {
        return searge_to_proguard_method_descriptor.at(seargeMethodDescriptor);

    } else if (Client == CLIENT::FORGE || Client == CLIENT::FORGE_ESSENTIAL) {
        return seargeMethodDescriptor;

    } else {
        return std::string();
    }
}

std::string Mapper::SeargeToClientClass(std::string seargeClassName) {
    if (Client == CLIENT::VANILLA) {
        return SeargeToProguardClass(seargeClassName);

    } else if (Client == CLIENT::FORGE || Client == CLIENT::FORGE_ESSENTIAL) {
        return seargeClassName;

    } else {
        return std::string();
    }
}

std::string Mapper::SeargeToClientField(std::string seargeClassName, std::string seargeFieldName) {
    if (Client == CLIENT::VANILLA) {
        return SeargeToProguardField(seargeClassName, seargeFieldName);

    } else if (Client == CLIENT::FORGE || Client == CLIENT::FORGE_ESSENTIAL) {
        return SeargeToRetroguardField(seargeClassName, seargeFieldName);

    } else {
        return std::string();
    }
}

std::string Mapper::SeargeToClientMethod(std::string seargeClassName, std::string seargeMethodName, std::string seargeMethodDescriptor) {
    if (Client == CLIENT::VANILLA) {
        return SeargeToProguardMethod(seargeClassName, seargeMethodName, seargeMethodDescriptor);

    } else if (Client == CLIENT::FORGE || Client == CLIENT::FORGE_ESSENTIAL) {
        return SeargeToRetroguardMethod(seargeClassName, seargeMethodName, seargeMethodDescriptor);

    } else {
        return std::string();
    }
}

Mapper mapper;
