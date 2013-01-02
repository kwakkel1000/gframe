//
//
// @ Project : gframe
// @ File Name : mainbase.h
// @ Date : 02-01-2013
// @ Author : Gijs Kwakkel
//
//
// Copyright (c) 2013 Gijs Kwakkel
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

#ifndef SRC_INCLUDE_MAINBASE_H
#define SRC_INCLUDE_MAINBASE_H

#include <string>
#include <vector>
#include <memory>

class mainbase
{
    public:
        mainbase(std::string name);
        ~mainbase();

        void parseArgs(std::vector<std::string> args);
        int run();

        void showHelp();
        void addHelpItem(std::string helpItem);

    private:
        class impl;
        std::unique_ptr<impl> pimpl;
};

#endif // SRC_INCLUDE_MAINBASE_H
