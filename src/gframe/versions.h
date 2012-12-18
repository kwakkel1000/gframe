//
//
// @ Project : gframe
// @ File Name : versions.h
// @ Date : 18-12-2012
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


#ifndef SRC_INCLUDE_VERSIONS_H
#define SRC_INCLUDE_VERSIONS_H

#include <vector>
#include <string>

class versions
{
    public:
        static versions& instance()
        {
            static versions obj;
            return obj;
        }

        void showVersion();
        void addVersion(std::string VersionItem);
        std::vector< std::string > getVersions();

    private:
        versions();
        ~versions();

        std::vector< std::string > m_VersionItems;

};

#endif // SRC_INCLUDE_VERSIONS_H_

