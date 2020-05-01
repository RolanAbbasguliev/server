#include "main.h"

std::string convert(std::vector<char> v)
{
    std::string str(v.begin(), v.end());
    return str;
}

void parse_startline(int i)
{   

    int id = conn_info[i].connection_id_of_req_struct, flag = 0, sl_flag = 0;

    std::vector<char> Meth, HTTP_v, adr, ftype;

    for(int u = 0; u < sizeof(buffer_); ++u)
        std::cout << buffer_[u];

    for(int i = 0; i != -1; ++i)
    {
        if(buffer_[i] == ' ')
        {
            buffer_[i] = 0;
            break;
        }
        else
        {
            Meth.push_back(buffer_[i]);
            buffer_[i] = 0;
        }
    }

    for(int i = 0; i != -1; ++i)
    {
        if((buffer_[i] == 0 || buffer_[i] == '/'))
        {
            if(sl_flag == 1)
                adr.push_back(buffer_[i]);
            else if(buffer_[i] == 0)
                ;
            else
                sl_flag = 1;
        }
        else
        {
            if(buffer_[i] == ' ')
            {
                buffer_[i] = 0, flag = 0;
                break;
            }
            else
            {
                if(buffer_[i] == '.')
                {
                    if(buffer_[i + 1] == '.' && buffer_[i + 2] == '/')
                        adr.push_back(buffer_[i]), adr.push_back(buffer_[i]), adr.push_back(buffer_[i + 2]);
                    else
                        flag = 1;
                }
                if(flag == 1)
                {
                    if(buffer_[i] != '.')
                        ftype.push_back(buffer_[i]);
                }

                adr.push_back(buffer_[i]);
                buffer_[i] = 0;
            }
        }
    }

    for(int i = 0; i != -1; ++i)
    {
        if(buffer_[i] == 0)
            ;
        
        else
        {
            if(buffer_[i] == '\n')
                break;
            else
            {
                HTTP_v.push_back(buffer_[i]);
                buffer_[i] = 0;
            }
        }
    }
  
    sl_flag = 0;

    Req[id].Method = convert(Meth);
    Req[id].File_Adr = convert(adr);
    Req[id].File_type = convert(ftype);

    std::cout << "\n\n----------------------\n" << Req[id].File_Adr << " || " << Req[id].File_type << "\n---------------------\n" << std::endl;

    Req[id].HTTP_version = convert(HTTP_v);
    
    parse_headers(id);
}

void parse_headers(int id)
{
    std::vector<char> last_h, last_h_body, buf_local;
    int flag = 0;
    
    for(int i = 0; i < 2049; ++i)
    {
        if(buffer_[i] == 0)
            ;

        else
        {
            if(flag != 1)
            {
                buf_local.push_back(buffer_[i]);
                if(buffer_[i] == ':')
                {
                    buf_local.pop_back();
                    last_h = buf_local;
                    buf_local.clear();
                    flag = 1;
                }
            }
            else
            {
                if(buffer_[i] == '\n')
                {
                    flag = 0;
                    buf_local.erase(buf_local.begin());
                    last_h_body = buf_local;
                    buf_local.clear();

                    Req[id].Headers.insert(std::pair<std::string, std::string>(convert(last_h) ,convert(last_h_body)));
                }
                else
                    buf_local.push_back(buffer_[i]);
            } 
        }
    }
    pick_method(id);    
}

