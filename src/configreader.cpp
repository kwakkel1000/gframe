//
//
// @ Project : gframe
// @ File Name : configreader.cpp
// @ Date : 31-10-2012
// @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2012 Gijs Kwakkel
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//

#include <gframe/configreader.h>
#include <gframe/output.h>
#include <gframe/glib.h>
#include <algorithm>
#include <cctype>
#include <string>
#include <fstream>

configreader::configreader()
{
    clearSettings();
}


configreader::~configreader()
{
}

void configreader::clearSettings()
{
    settings.clear();
}

void configreader::set_ConfigFile(std::string _ConfigFile)
{
    m_ConfigFile = _ConfigFile;
}
std::string configreader::get_ConfigFile()
{
    return m_ConfigFile;
}


bool configreader::readFile()
{
    return readFile(m_ConfigFile);
}
bool configreader::readFile(std::string _ConfigFile)
{
    output::instance().addOutput("readfile: " +  _ConfigFile);
    std::string line;
    std::string section("global"); // default value
    std::ifstream configfile;
    configfile.open(_ConfigFile.c_str());
    if (configfile.is_open())
    {
        int linenr = 0;
        while (configfile.good())
        {
            getline(configfile, line);
            linenr++;

            glib::trim(line);

            // Filter empty and comment lines
            if ((line.length()>0) && (line[0] != '#'))
            {
                // Debug
                // cout << "Line " << linenr << ": " << line << endl;

                // Detect section begin
                if (line[0] == '[')
                {
                    if (line[line.length()-1] == ']')
                    {
                        // Changing section...
                        section = line.substr(1, line.length()-2);
                        output::instance().addOutput("* Changed section to '" + section + "'", 6);
                    }
                    else
                    {
                        output::instance().addStatus(false, "Invalid section on line " + glib::stringFromInt(linenr) + ": " + line);
                    }
                }
                else
                {
                    std::transform(section.begin(), section.end(), section.begin(), (int(*)(int)) std::tolower);
                    if (section == "config")
                    {
                        std::string var = line;
                        glib::trim(var);
                        readFile(var);
                    }
                    else
                    {
                        uint pos = line.find("=");
                        if (pos != std::string::npos)
                        {
                            std::string var = line.substr(0, pos);
                            std::string value = line.substr(pos+1, line.length()-pos-1);
                            glib::trim(var);
                            glib::trim(value);

                            // cout << "* The variable '" << var << "' has value '" << value << "'" << endl;
                            // convert var to lower;
                            std::transform(var.begin(), var.end(), var.begin(), (int(*)(int)) std::tolower);
                            settings[var] = value;
                        }
                    }
                }
            }
        }
        configfile.close();
        output::instance().addStatus(true, "done reading: " + _ConfigFile);
        return true;
    }
    else
    {
        output::instance().addStatus(false, "Could not open file: " + _ConfigFile);
    }

    return false;
}


std::string configreader::getString(std::string varname)
{
    // values are saved lowercase, so convert varname to lower first.
    std::transform(varname.begin(), varname.end(), varname.begin(), (int(*)(int)) std::tolower);
    return settings[varname];
}

