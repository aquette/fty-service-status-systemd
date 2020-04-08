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
#include <fty_service_status.h>

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

int main (int argc, char *argv[]) {

    //first argument is the path for plugins
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <pathToPlugin>" << std::endl;
        return 1;
    }

    try {

        int returnCodeOp = 0;
        int returnCodeHs = 0;

        std::cout << "Loading the plugin" << argv[1] << std::endl;

        //load the plugin
        fty::ServiceStatusPluginWrapper myPlugin(argv[1]);

        //create a object to provide the status (The object is stored under a shared_ptr with a special deleter)
        fty::ServiceStatusProviderPtr statusProvider = myPlugin.newServiceStatusProviderPtr(argv[0]);

        std::cout << "Set the status \"Starting\"" << std::endl;

        returnCodeOp = statusProvider->set(fty::OperatingStatus::Starting);
        returnCodeHs = statusProvider->set(fty::HealthState::Ok);
        std::cout << "Return code for Operation Status "<< std::to_string(returnCodeOp) << " and for health states " << std::to_string(returnCodeHs) << std::endl;

        std::cout << "Waiting for 10 seconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));

        std::cout << "Set the status \"In Service\"" << std::endl;
        returnCodeOp = statusProvider->set(fty::OperatingStatus::InService);
        returnCodeHs = statusProvider->set(fty::HealthState::Ok);
        std::cout << "Return code for Operation Status "<< std::to_string(returnCodeOp) << " and for health states " << std::to_string(returnCodeHs) << std::endl;

        std::cout << "Waiting for 10 seconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));

        std::cout << "Set the status \"Stopping\"" << std::endl;
        returnCodeOp = statusProvider->set(fty::OperatingStatus::Stopping);
        returnCodeHs = statusProvider->set(fty::HealthState::Ok);
        std::cout << "Return code for Operation Status "<< std::to_string(returnCodeOp) << " and for health states " << std::to_string(returnCodeHs) << std::endl;

        std::cout << "Waiting for 10 seconds" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(10));

        std::cout << "Set the status \"Stopped\"" << std::endl;
        returnCodeOp = statusProvider->set(fty::OperatingStatus::Stopped);
        returnCodeHs = statusProvider->set(fty::HealthState::Ok);
        std::cout << "Return code for Operation Status "<< std::to_string(returnCodeOp) << " and for health states " << std::to_string(returnCodeHs) << std::endl;

        return 0;

    } catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}