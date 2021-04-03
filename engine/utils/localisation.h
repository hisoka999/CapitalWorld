#ifndef LOCALISATION_H
#define LOCALISATION_H

#include <map>
#include <memory>
#include <mutex>

enum class Language : long
{
    de,
    en
};

class Localisation
{
public:
    static Localisation &Instance()
    {
        static Localisation instance;
        return instance;
    }

    void loadLanguage(const Language lang);
    void detectLanguage();

    std::string translate(const std::string &msgid) const;

    std::string getLanguage() const;

    Language getLang() const;

private:
    void loadLocalisation(std::string filename);
    Localisation();
    ~Localisation() = default;
    Localisation(const Localisation &) = delete;
    Localisation &operator=(const Localisation &) = delete;

    std::map<std::string, std::string> translations;
    std::string language;
    Language lang;
};

#endif // LOCALISATION_H
