//
//
// @ Project : gframe
// @ File Name : versions.cpp
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

#include <gframe/versions.h>
#include <gframe/output.h>
#include <gframe/glib.h>

void versions::showVersion()
{
    size_t longeststring = 0;
    for (size_t Version_iterator = 0; Version_iterator < m_VersionItems.size(); Version_iterator++)
    {
        if (m_VersionItems[Version_iterator].size() > longeststring)
        {
            longeststring = m_VersionItems[Version_iterator].size();
        }
    }
    std::string Block = " ++++++";
    for (size_t longeststring_Iterator = 0; longeststring_Iterator < longeststring; longeststring_Iterator++)
    {
        Block = Block + "+";
    }
    output::instance().addStatus(true, Block);
    for (size_t Version_iterator = 0; Version_iterator < m_VersionItems.size(); Version_iterator++)
    {
        std::string tmplength = " +  " + m_VersionItems[Version_iterator];
        for (size_t tmplength_Iterator = 0; tmplength_Iterator < (longeststring - m_VersionItems[Version_iterator].size()); tmplength_Iterator++)
        {
            tmplength = tmplength + " ";
        }
        tmplength = tmplength + "  +";
        output::instance().addStatus(true, tmplength);
    }
    output::instance().addStatus(true, Block);
}

void versions::addVersion(std::string VersionItem)
{
    m_VersionItems.push_back(VersionItem);
}

std::vector< std::string > versions::getVersions()
{
    return m_VersionItems;
}

versions::versions()
{
}

versions::~versions()
{
}
