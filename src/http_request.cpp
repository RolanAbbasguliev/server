#include "main.h"

/**
 * @brief Parsing request startline
 * 
 * @param i ID of the client
 * 
 * @return Nothing
 */
void parse_startline(int i)
{
    int id = conn_info[i].connection_id_of_req_struct;
    i = 0;
    std::vector<char> Meth, HTTP_v, adr, ftype;

    while(1)
    {
        if(buffer_[i] != ' ')
            Meth.push_back(buffer_[i]), i++;
        else
            {i++; break;}
    }

    while(1)
    {
        if(buffer_[i] != ' ')
            adr.push_back(buffer_[i]), i++;
        else
            {i++; break;}
    }

    while(1)
    {
        if(buffer_[i] != '\r')
            HTTP_v.push_back(buffer_[i]), i++;
        else
            break;
    }

    i = 0;
    while(1)
    {
        if(buffer_[i] != '.')
            i++;
        else
            {ftype = useless_func(i); break;}
        
    }


    Req[id].Method = convert(Meth);
    Req[id].File_Adr = convert(adr);
    Req[id].File_type = convert(ftype);
    Req[id].HTTP_version = convert(HTTP_v);

    parse_headers(id);
}

/**
 * @brief Parsing request headers & body
 * 
 * @param id ID of the client
 * 
 * @return Nothing
 */
void parse_headers(int id)
{
    std::istringstream resp(buffer_);
    std::string header;
    std::string body_, b_r;
    std::string::size_type index;
    while (std::getline(resp, header) && header != "\r") 
    {
        index = header.find(':', 0);
        if(index != std::string::npos) 
        {
            Req[id].Headers.insert(std::make_pair(
                boost::algorithm::trim_copy(header.substr(0, index)), 
                boost::algorithm::trim_copy(header.substr(index + 1))
            ));
        }
    }

    
    while(std::getline(resp, body_))
        b_r += body_;

    Req[id].Body = b_r;
    pick_method(id);
}

/**
 * @brief Searching and saving file type 
 * 
 * @param id ID of the client
 * 
 * @return std::vector<char> filetype without '.'
 */
std::vector<char> useless_func(int i)
{
    std::vector<char> s;
    i++;
    while(1)
    {
        if(buffer_[i] != ' ')
            s.push_back(buffer_[i]), i++;
        else
            break;
    }
    return s;
}

/**
 * @brief Converting std::vector to std::string
 * 
 * @param v std::vector<char> which will be converted 
 * 
 * @return std::string
 */
std::string convert(std::vector<char> v)
{
    std::string str(v.begin(), v.end());
    return str;
}