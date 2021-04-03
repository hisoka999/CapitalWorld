#include "engine/utils/IniBase.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace utils {

IniBase::IniBase()
{
    //ctor
}

IniBase::~IniBase()
{
    groups.clear();
}

void IniBase::read()
{
    std::fstream file;
    std::istringstream is;
    std::string s;
    std::string group;
    std::cout << filename << std::endl;

    file.open(filename.c_str(), std::ios::in);
    if (!file.is_open()) {
        throw IOException(filename, "file does not exists");
    }
    while (getline(file, s)) {

        if (s[0] == '[') {
            group = s.substr(1, s.length() - 2);
        } else if (!s.empty()) {
            std::string attr = s.substr(0, s.find("="));
            std::string value = s.substr(s.find("=") + 1, s.length() - 1);
            this->groups[group][attr] = value;
        }
    }
    //while(!s.empty());
    file.close();

    // demo compression
    /*
     FILE *in = fopen("/home/stefan/codeblocks/Dusk2d/test.map.gz","rb");
     if (!in )
     {
     fprintf (stderr, "%s:%d: open"            \
                     " file '%s' failed: %s\n",          \
                     __FILE__, __LINE__, "/home/stefan/codeblocks/Dusk2D/test.map.gz",      \
                     strerror (errno));                  \
            exit (EXIT_FAILURE);
     }
     SET_BINARY_MODE(in);
     //SET_BINARY_MODE(stdout);

     std::string buffer = inf(in);
     std::cout<<"buffer: "<<buffer.substr(0,100)<<std::endl;
     */
}


void IniBase::write()
{
    GroupIterator it;
    std::ofstream file;
    std::ostringstream is;
    std::string s;
    std::string group;
    std::cout << filename << std::endl;

    file.open(filename, std::ios::out);

    // show content:
    for (GroupIterator it = groups.begin(); it != groups.end(); ++it) {
        file << '[' << it->first << "]\n";
        //if (it->second.begin() != it->second.end())
        //std::cout<<"TEST"<<it->first<<std::endl;

        for (Attributes::iterator a_it = it->second.begin();
             a_it != it->second.end(); ++a_it) {
            file << a_it->first << "=" << a_it->second << '\n';
        }
    }
    file.close();
}
void IniBase::setAttr(const std::string& group, const std::string& name, std::string value)
{
    groups[group][name] = value;
}
void IniBase::setAttrB(const std::string& group, const std::string& name, bool value)
{
    std::string valB = "FALSE";
    if (value)
        valB = "TRUE";
    //    if (groups.count(group) == 0) {
    //        Attributes attribs;
    //        groups[group] = attribs;
    //    }
    groups[group][name] = valB;
}
void IniBase::setAttrI(const std::string& group, const std::string& name, int value)
{
    groups[group][name] = std::to_string(value);
}

std::string IniBase::getValue(const std::string& group, const std::string& name)
{
    return groups[group][name];
}

bool IniBase::getValueB(const std::string& group, const std::string& name)
{
    std::string val = getValue(group, name);
    if (val == "FALSE") {
        return false;
    } else {
        return true;
    }
}

int IniBase::getValueI(const std::string& group, const std::string& name)
{
    std::string val = getValue(group, name);
    if (val.length() > 0)
        return std::stoi(val);
    else
        return 0;
}
}
