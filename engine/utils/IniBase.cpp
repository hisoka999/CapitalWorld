#include "engine/utils/IniBase.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace utils
{

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
    if (!file.is_open())
    {
        throw IOException(filename, "file does not exists");
    }
    while (getline(file, s))
    {

        if (s[0] == '[')
        {
            group = s.substr(1, s.length() - 2);
        }
        else if (!s.empty())
        {
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
/* Decompress from file source to file dest until stream ends or EOF.
 inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
 allocated for processing, Z_DATA_ERROR if the deflate data is
 invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
 the version of the library linked do not match, or Z_ERRNO if there
 is an error reading or writing the files. */
std::string IniBase::inf(FILE *source)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];
    std::stringstream buffer;

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        zerr(ret);

    /* decompress until deflate stream ends or end of file */
    do
    {
        strm.avail_in = fread(in, sizeof(char), sizeof(in), source);
        if (ferror(source))
        {
            (void) inflateEnd(&strm);
            return NULL;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do
        {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR); /* state not clobbered */
            switch (ret)
            {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;
                /* no break */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void) inflateEnd(&strm);
                zerr(ret);
                return NULL;
            }
            have = CHUNK - strm.avail_out;
            buffer << out; //buffer.sputn(out,have);// std::string(out);
            /*
             if (fwrite(out, 1, have, dest)  != have || ferror(dest)) {
             (void)inflateEnd(&strm);
             return Z_ERRNO;
             }*/
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void) inflateEnd(&strm);

    zerr(ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR);
    return buffer.str();
}

/* report a zlib or i/o error */
void IniBase::zerr(int ret)
{
    fputs("zlib: ", stderr);
    switch (ret)
    {
    case Z_ERRNO:
        if (ferror (stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror (stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
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
    for (GroupIterator it = groups.begin(); it != groups.end(); ++it)
    {
        file << '[' << it->first << "]\n";
        //if (it->second.begin() != it->second.end())
        //std::cout<<"TEST"<<it->first<<std::endl;

        for (Attributes::iterator a_it = it->second.begin();
                a_it != it->second.end(); ++a_it)
        {
            file << a_it->first << "=" << a_it->second << '\n';
        }

    }
    file.close();

}
void IniBase::setAttr(std::string group, std::string name, std::string value)
{
    groups[group][name] = value;
}
void IniBase::setAttrB(std::string group, std::string name, bool value)
{
    std::string valB = "FALSE";
    if (value)
        valB = "TRUE";
    groups[group][name] = valB;
}
void IniBase::setAttrI(std::string group, std::string name, int value)
{
    groups[group][name] = std::to_string(value);
}

std::string IniBase::getValue(std::string group, std::string name)
{
    return groups[group][name];
}

bool IniBase::getValueB(std::string group, std::string name)
{
    std::string val = getValue(group, name);
    if (val == "FALSE")
    {
        return false;
    }
    else
    {
        return true;
    }
}

int IniBase::getValueI(std::string group, std::string name)
{
    std::string val = getValue(group, name);
    if (val.length() > 0)
        return std::stoi(val);
    else
        return 0;
}
}
