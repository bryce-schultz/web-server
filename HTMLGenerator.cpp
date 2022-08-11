#include "HTMLGenerator.h"

#include <sstream>

std::string HTMLGenerator::generateTag(const std::string& tag, const std::string& content, const std::string& cssId, const std::string& cssClass)
{
    std::ostringstream result;
    std::string props = "";

    if (cssId != "" && cssClass == "")
    {
        props = " id=\"" + cssId + "\"";
    }
    else if (cssId == "" && cssClass != "")
    {
        props = " class=\"" + cssClass + "\"";
    }
    else if (cssId != "" && cssClass != "")
    {
        props = " id=\"" + cssId + "\" class=\"" + cssClass + "\"";
    }

    result << "<" << tag << props << ">" << content << "</" << tag << ">";
    return result.str();
}
