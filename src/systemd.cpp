/*  ========================================================================
    Copyright (C) 2020 Eaton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
    ========================================================================
*/
#include "systemd.h"
#include <systemd/sd-daemon.h>

#include <fstream>
#include <string>
#include <map>

//internal variables and functions
static std::string gPluginLastError = "";
/*static const std::map<fty::OperatingStatus, std::string> gOperatingStatusStr(
    {fty::OperatingStatus::Unkown, "Unknown"}
);*/

//public interfaces
const char * getPluginName() {
    return "Systemd plugin";
}

const char * getPluginLastError(){
    return gPluginLastError.c_str();
}

int createServiceStatusProvider(fty::ServiceStatusProvider** spp, const char * serviceName) {
    try {
        *spp = dynamic_cast<fty::ServiceStatusProvider*>(new fty::ServiceStatusSystemd (serviceName));
    }
    catch(const std::exception& e) {
        gPluginLastError = e.what();
        return -1;
    }

    gPluginLastError = "";
    return 0;
}

void deleteServiceStatusProvider(fty::ServiceStatusProvider* spp) {
    fty::ServiceStatusSystemd * ptr = dynamic_cast<fty::ServiceStatusSystemd *>(spp);
    delete ptr;
}


namespace fty
{
    ServiceStatusSystemd::ServiceStatusSystemd(const char * serviceName)
        : m_serviceName(serviceName)
        {}

    const char * ServiceStatusSystemd::getServiceName() const noexcept {
        return m_serviceName.c_str();
    }

    /// Set the Operating Status
    ///@param os [in] Operating Status to set
    ///@return 0 in success, -1  in case of error and message is stored in getPluginLastError
    int ServiceStatusSystemd::set(OperatingStatus os) noexcept {
        m_lastOperatingStatus = os;
        std::string statusToSet;

        //Specific systemd
        if(os == OperatingStatus::InService) {
            statusToSet+="READY=1\n";
        } else if(os == OperatingStatus::Stopping || os == OperatingStatus::ShuttingDown ) {
            statusToSet+="STOPPING=1\n";
        }

        statusToSet += "STATUS=Operating Status <" +
                        std::to_string(static_cast<uint8_t>(m_lastOperatingStatus)) +
                        ">, Health State <" + 
                        std::to_string(static_cast<uint8_t>(m_lastHealthState)) + ">\n";
        
        int returnVal = sd_notifyf(0, statusToSet.c_str());

        if(returnVal != 0) {
            gPluginLastError = "sd_notify error: " + std::to_string(returnVal);
        } else {
            gPluginLastError = "";
        }
        return returnVal;
    }

    /// Set the Health State
    ///@param hs [in] Health state to set
    ///@return 0 in success, -1  in case of error and message is stored in getPluginLastError
    int ServiceStatusSystemd::set(HealthState hs) noexcept {
        m_lastHealthState = hs;
        std::string statusToSet;

        statusToSet += "STATUS=Operating Status <" +
                        std::to_string(static_cast<uint8_t>(m_lastOperatingStatus)) +
                        ">, Health State <" + 
                        std::to_string(static_cast<uint8_t>(m_lastHealthState)) + ">\n";

        int returnVal = sd_notifyf(0,   "READY=1\n"
                                        "STATUS=Processing requests…\n");

        if(returnVal != 0) {
            gPluginLastError = "sd_notify error: " + std::to_string(returnVal);
        } else {
            gPluginLastError = "";
        }
        return returnVal;
    }

} //namespace example
