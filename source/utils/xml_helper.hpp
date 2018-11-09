#ifndef XML_HELPER_HPP
#define XML_HELPER_HPP

#include <string>

class xml_iterator
{
private:
    xml_iterator();
    virtual ~xml_iterator();

    /**
     * @brief get value from iterator
     * @return value
     */
    virtual std::string operator* () = 0;

    /**
     * @brief judge it's end or not
     * @return has result or not
     */
    virtual bool operator! () = 0;

    /**
     * @brief move iterator to next
     */
    virtual void moveNext() = 0;
};


class xml_helper {
private:
    std::string m_xml;

public:
    xml_helper();
    ~xml_helper();


    /**
     * @brief GetXmlString
     * @return
     */
    std::string GetXmlString();

    /**
     * @brief set xml node, if value is not exist, it will add it and set it's value
     * @param xpath : where node set to
     * @param value : node's value
     * @return if xpath is not exist, return false. if xpath is right return true
     */
    bool SetXmlNode(const std::string& xpath, const std::string& value);

    /**
     * @brief AddXmlNode
     * @param xpath : where node insert to
     * @param node  : node's name
     * @param value : node's value
     * @return
     */
    bool AddXmlNode(const std::string& xpath, const std::string& node, const std::string& value);

    /**
     * @brief GetXmlNode
     * @param xpath : where node get from
     * @param value : node's value
     * @return if xpath is not exist, return false, if xpath is right, reture true
     */
    bool GetXmlNode(const std::string& xpath, std::string& value);

    /**
     * @brief GetXmlNodes
     * @param xpath
     * @return xml_iterator ptr
     */
    xml_iterator* GetXmlNodes(const std::string& xpath);

};

#endif // end of XML_HELPER_HPP
/* EOF */
