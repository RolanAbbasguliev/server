#ifndef _BUILD_TESTS_
    #include "main.h"
#endif
#ifdef _BUILD_TESTS_
    #include "unittests_main.h"
#endif

/**
 * @brief Parsing request startline
 * 
 * @param i ID of the client
 * 
 * @return Nothing
 */
void parse_startline(int i)
{
    #ifndef _BUILD_TESTS_
        int id = conn_info[i].connection_id_of_req_struct;
    #endif
    #ifdef _BUILD_TESTS_
        int id = 1;
    #endif
    i = 0;
    std::vector<char> Meth, HTTP_v, adr, ftype;

    //printing buffer_ for debug
    for(int u = 0; u < sizeof(buffer_); ++u)
        std::cout << buffer_[u];

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

    if(WebKitFromBoundary_check(id))
        {/*std::cout << "*** " << std::to_string(WebKitFromBoundary_check(id)) << " ***" << std::endl;*/ Req[id].Method = "POST"; Req[id].bound_flag = 1;}
    else
    {   
        Req[id].bound_flag = 0;
        Req[id].File_Adr = convert(adr);
        Req[id].File_type = convert(ftype);
        Req[id].HTTP_version = convert(HTTP_v);
        std::cout << "*** " <<Req[id].Method << " ***" << std::endl;
    }
    //std::cout << "*** " << Req[id].Method << " : " << std::to_string(WebKitFromBoundary_check(id)) << " ***" << std::endl;
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

    if(Req[id].bound_flag == 1)
        std::getline(resp, Req[id].WebKitFormBoundary);
    else
    {
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
    }
    
    while(std::getline(resp, body_))
        b_r += body_;

    Req[id].Body = b_r;

    #ifndef _BUILD_TESTS_
        pick_method(id);
    #endif
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

bool WebKitFromBoundary_check(int id)
{
    std::string WebKitPattern = "WebKitFormBoundary";

    if(Req[id].Method != "POST" && Req[id].Method != "GET")
        if(int pos = Req[id].Method.find(WebKitPattern, 0) > 0)
            return true;
    else
        return false;
}