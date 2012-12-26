//
//
// @ Project : gframe
// @ File Name : glib.cpp
// @ Date : 29-10-2012
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



#include <gframe/glib.h>

#include <iterator>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

bool glib::iequals(std::string msInput1, std::string msInput2)
{
    return (toLower(msInput1) == toLower(msInput2));
}

// trim from start
std::string &glib::ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
std::string &glib::rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
std::string &glib::trim(std::string &s) {
        return ltrim(rtrim(s));
}

std::string glib::stringTimeFromIntSeconds(int _Seconds)
{
    return stringTimeFromIntSeconds(_Seconds, true);
}
std::string glib::stringTimeFromIntSeconds(int _Seconds, bool _WithWeeks=true)
{
    std::vector< int > _TimeVector = vectorTimeFromSecondsTime(_Seconds, _WithWeeks);
    if ((_WithWeeks && _TimeVector.size() == 6) || (!_WithWeeks && _TimeVector.size() == 5))
    {
        int iYear = _TimeVector[0];
        std::string sYear = stringFromInt(iYear);
        int offset = 0;
        int iWeek = 0;
        std::string sWeek;
        if (_WithWeeks)
        {
            offset = 1;
            iWeek = _TimeVector[1];
            sWeek = stringFromInt(iWeek);
        }
        else
        {
            offset = 0;
        }
        int iDay = _TimeVector[1+offset];
        std::string sDay = stringFromInt(iDay);
        int iHour = _TimeVector[2+offset];
        std::string sHour = stringFromInt(iHour);
        int iMinutes = _TimeVector[3+offset];
        std::string sMinutes = stringFromInt(iMinutes);
        int iSeconds = _TimeVector[4+offset];
        std::string sSeconds = stringFromInt(iSeconds);
        std::string sTimeString = "";
        if (iYear > 0)
        {
            sTimeString = sTimeString + sYear + " Year";
            if (iYear > 1)
            {
                sTimeString = sTimeString + "s";
            }
            sTimeString = sTimeString + " ";
        }
        if (iWeek > 0)
        {
            sTimeString = sTimeString + sWeek + " Week";
            if (iWeek > 1)
            {
                sTimeString = sTimeString + "s";
            }
            sTimeString = sTimeString + " ";
        }
        if (iDay > 0)
        {
            sTimeString = sTimeString + sDay + " Day";
            if (iDay > 1)
            {
                sTimeString = sTimeString + "s";
            }
            sTimeString = sTimeString + " ";
        }
        if (iHour > 0)
        {
            sTimeString = sTimeString + sHour + " Hour";
            if (iHour > 1)
            {
                sTimeString = sTimeString + "s";
            }
            sTimeString = sTimeString + " ";
        }
        if (iMinutes > 0)
        {
            sTimeString = sTimeString + sMinutes + " Minute";
            if (iMinutes > 1)
            {
                sTimeString = sTimeString + "s";
            }
            sTimeString = sTimeString + " ";
        }
        if (iSeconds > 0)
        {
            sTimeString = sTimeString + sSeconds + " Second";
            if (iSeconds > 1)
            {
                sTimeString = sTimeString + "s";
            }
            sTimeString = sTimeString + " ";
        }
        return sTimeString;
    }
    return "";
}

std::string glib::toLower(std::string msInput)
{
    std::transform(msInput.begin(), msInput.end(), msInput.begin(), (int(*)(int)) std::tolower);
    return msInput;
}

std::vector< int > glib::vectorTimeFromSecondsTime(int _Time)
{
    return vectorTimeFromSecondsTime(_Time, true);
}
std::vector< int > glib::vectorTimeFromSecondsTime(int miTime, bool _WithWeeks=true)
{
    // Year, Week, Day, Hour, Minutes, Seconds;
    // year (day * 365)
    // week (day * 7)
    // day (hour * 24)
    // hour (minutes * 60)
    // minutes (seconds * 60)
    // seconds (left)

    std::vector< int > vTime;
    int iYear = (miTime - (miTime % (365 * 24 * 60 * 60))) / (365 * 24 * 60 * 60);
    miTime = miTime - (iYear * 365 * 24 * 60 * 60);
    int iWeek = 0;
    if (_WithWeeks)
    {
        iWeek = (miTime - (miTime % (7 * 24 * 60 * 60))) / (7 * 24 * 60 * 60);
        miTime = miTime - (iWeek * 7 * 24 * 60 * 60);
    }
    int iDay = (miTime - (miTime % (24 * 60 * 60))) / (24 * 60 * 60);
    miTime = miTime - (iDay * 24 * 60 * 60);
    int iHour = (miTime - (miTime % (60 * 60))) / (60 * 60);
    miTime = miTime - (iHour * 60 * 60);
    int iMinutes = (miTime - (miTime % 60)) / 60;
    miTime = miTime - (iMinutes * 60);
    int iSeconds = miTime;
//    std::cout << "year " << iYear << " week " << iWeek << " day " << iDay << " hour " << iHour << " minutes " << iMinutes << " seconds " << iSeconds << std::endl;
    vTime.push_back(iYear);
    if (_WithWeeks)
    {
        vTime.push_back(iWeek);
    }
    vTime.push_back(iDay);
    vTime.push_back(iHour);
    vTime.push_back(iMinutes);
    vTime.push_back(iSeconds);
    return vTime;
}

std::vector< std::string > glib::split(std::string input)
{
    // construct a stream from the string
    std::stringstream strstr(input);

    // use stream iterators to copy the stream to the vector as whitespace separated strings
    std::istream_iterator<std::string> it(strstr);
    std::istream_iterator<std::string> end;
    std::vector<std::string> results(it, end);
    return results;
}

std::string glib::stringFromInt(int miInput)
{
/*    std::stringstream ssConversion;
    ssConversion << miInput;
    return ssConversion.str(); */
    return std::to_string(miInput);
}

int glib::intFromString(std::string msInput)
{
    int iOutput;
    std::stringstream ssConversion(msInput);
    ssConversion >> iOutput;
    return iOutput;
}
