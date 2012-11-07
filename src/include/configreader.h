//
//
// @ Project : gframe
// @ File Name : configreader.h
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

#ifndef SRC_INCLUDE_CONFIGREADER_H
#define SRC_INCLUDE_CONFIGREADER_H

#include <string>
#include <map>

class configreader
{
    public:
        static configreader& instance()
        {
            static configreader obj;
            return obj;
        }

        void set_ConfigFile(std::string _ConfigFile);
        std::string get_ConfigFile();

        void clearSettings();
        bool readFile();
        bool readFile(std::string _ConfigFile);
        std::string getString(std::string varname);

    private:
        configreader();
        ~configreader();
        std::string m_ConfigFile;
        std::map< std::string, std::string > settings;
};

#endif // SRC_INCLUDE_CONFIGREADER_H

