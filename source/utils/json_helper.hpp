#ifndef JSON_HELPER_HPP
#define JSON_HELPER_HPP

#include <string>

class json_iterator {
public:
    json_iterator();
    virtual ~json_iterator();

    /**
     * @brief get value from iterator
     * @return get iterator value
     */
    virtual std::string operator* () = 0;

    /**
     * @brief judeg it's end or not
     * @return has result or not
     */
    virtual bool operator! () = 0;

    /**
     * @brief move iterator to next pos
     */
    virtual void moveNext() = 0;
};

class json {
private:
    std::string _m_body;

public:
    json(std::string body);
    ~json();

    /**
     * @brief getJsonBody
     * @return
     */
    std::string getJsonString();

    /**
     * @brief SetJsonNode
     * @param path
     * @param value
     * @return
     */
    bool SetJsonNode(const std::string& path, const std::string& value);

    /**
     * @brief SetJsonArrayNode
     * @param path
     * @param value
     * @return
     */
    bool SetJsonArrayNode(const std::string& path, const std::string value);

    /**
     * @brief GetJsonNode
     * @param path
     * @param value
     * @return
     */
    bool GetJsonNode(const std::string& path, const std::string& value);

    /**
     * @brief GetJsonArrayNode
     * @param path
     * @return
     */
    json_iterator* GetJsonArrayNode(const std::string& path);
};

#endif // end of JSON_HELPER_HPP
/* EOF */
