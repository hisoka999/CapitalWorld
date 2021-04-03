#ifndef UTILS_INIBASE_H
#define UTILS_INIBASE_H

#include "engine/utils/exceptions.h"

#include <assert.h>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#include <fcntl.h>
#include <io.h>
#define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#define SET_BINARY_MODE(file)
#endif

namespace utils {

#define CHUNK 16384

typedef std::map<std::string, std::string> Attributes;
typedef std::map<std::string, Attributes>::iterator GroupIterator;

class IniBase {
public:
    /** Default constructor */
    IniBase();
    /** Default destructor */
    virtual ~IniBase();
    /** Access filename
         * \return The current value of filename
         */
    std::string Getfilename() { return filename; }
    /** Set filename
         * \param val New value to set
         */
    void Setfilename(std::string val) { filename = val; }
    void read();
    void write();
    void setAttr(const std::string& group, const std::string& name, std::string value);
    void setAttrB(const std::string& group, const std::string& name, bool value);
    void setAttrI(const std::string& group, const std::string& name, int value);
    std::string getValue(const std::string& group, const std::string& name);

    bool getValueB(const std::string& group, const std::string& name);
    int getValueI(const std::string& group, const std::string& name);

protected:
private:
    std::string filename; //!< Member variable "filename"
    std::map<std::string, Attributes> groups;
};

} // namespace utils

#endif // UTILS_INIBASE_H
