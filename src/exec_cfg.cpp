#include "main.h"

char config_buff[3000]; ///< Buffer for configuration file
std::map<std::string, std::string> config; ///< Map for pairs param-value from configuration file

/**
 * @brief Loading cfg file config.cfg for reading
 * 
 * @return Nothing
 */
void load_cfg_file()
{
    FILE *F;
    int i = 0, cfg_size = 0;

    if((F = fopen("config.cfg", "rb")) == NULL)
        actions_log("CONFIG ERROR");

    memset(&config_buff, 0, sizeof(config_buff));
    
    while(1)
    {
        int ch = fgetc(F);
        if(ch == EOF)
            {cfg_size = i + 1; break;}

        config_buff[i] = ch;
        i++;
    }
    fclose(F);
    parse_cfg_file(cfg_size);
}

/**
 * @brief Parsing config.cfg file
 * 
 * @param cfg_size Size of config.cfg
 * 
 * @return Nothing
 */
void parse_cfg_file(int cfg_size)
{
    std::vector<char> param_name, param_value;
    int flag = 0;
    
    for(int i = 0; i < cfg_size; ++i)
    {
        if(config_buff[i] == ':')
            flag = 1;
        else if(config_buff[i] == '\n')
            {flag = 0; config.insert(std::pair<std::string, std::string>(convert(param_name) ,convert(param_value))); param_name.clear(); param_value.clear();}
        else if(flag == 0)
            param_name.push_back(config_buff[i]);
        else if(flag == 1)
            param_value.push_back(config_buff[i]);
    }

    exec_cfg();
}

/**
 * @brief Applying params from cfg file
 * 
 * @return Nothing
 */
void exec_cfg()
{
    for(auto it = config.cbegin(); it != config.cend(); ++it)
    {
        std::string par = it->first;

        if(par == "PORT")
            _PORT_ = str_to_int(it->second);
        if(par == "MAXIMUM_CONNECTIONS")
            MAXIMUM_CONNECTIONS = str_to_int(it->second);
        if(par == "FILES_PATH")
            FILES_PATH = it->second;
    }
}


/**
 * @brief Converting std::string value to int
 * 
 * @param string std::string for converting
 * 
 * @return int value
 */
int str_to_int(std::string string)
{
    std::stringstream str(string);

    int x = 0;
    str >> x;

    return x;
}