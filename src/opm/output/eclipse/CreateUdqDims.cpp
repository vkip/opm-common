/*
  Copyright (c) 2018 Equinor ASA
  Copyright (c) 2018 Statoil ASA

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <opm/output/eclipse/AggregateUDQData.hpp>
#include <opm/output/eclipse/WriteRestartHelpers.hpp>

#include <opm/output/eclipse/InteHEAD.hpp>
#include <opm/output/eclipse/DoubHEAD.hpp>

#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQConfig.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQInput.hpp>

#include <opm/parser/eclipse/EclipseState/Schedule/UDQ/UDQActive.hpp>

#include <opm/parser/eclipse/EclipseState/EclipseState.hpp>
#include <opm/parser/eclipse/EclipseState/Schedule/Schedule.hpp>

#include <opm/parser/eclipse/Units/UnitSystem.hpp>
#include <opm/parser/eclipse/Units/Units.hpp>

#include <chrono>
#include <cstddef>
#include <vector>

namespace {


std::size_t entriesPerIUDQ()
{
    std::size_t no_entries = 3;
    return no_entries;
}

std::size_t entriesPerIUAD()
{
    std::size_t no_entries = 5;
    return no_entries;
}

std::size_t entriesPerZUDN()
{
    std::size_t no_entries = 2;
    return no_entries;
}

std::size_t entriesPerZUDL()
{
    std::size_t no_entries = 16;
    return no_entries;
}

std::size_t entriesPerIGph(const std::vector<int>& inteHead)
{
    std::size_t no_entries = inteHead[20];
    return no_entries;
}

// maximum number of wells
std::size_t nwmaxz(const std::vector<int>& inteHead)
{
        return inteHead[163];
}



int noWellUdqs(const Opm::Schedule& sched,
               const std::size_t    simStep)
{
    const auto& udqCfg = sched.getUDQConfig(simStep);
    std::size_t i_wudq = 0;
    for (const auto& udq_input : udqCfg.input()) {
        if (udq_input.var_type() ==  Opm::UDQVarType::WELL_VAR) {
            i_wudq++;
        }
    }   
    return i_wudq;
}

} // Anonymous

// #####################################################################
// Public Interface (createUdqDims()) Below Separator
// ---------------------------------------------------------------------

std::vector<int>
Opm::RestartIO::Helpers::
createUdqDims(const Schedule&     		sched,
              const std::size_t        	lookup_step,
              const std::vector<int>&   inteHead) 
{
    const auto& udqCfg = sched.getUDQConfig(lookup_step);
    const auto& udqActive = sched.udqActive(lookup_step);
    std::vector<int> udqDims; 
    // 0
    udqDims.emplace_back(udqCfg.size());
    // 1
    udqDims.emplace_back(entriesPerIUDQ());
    // 2
    udqDims.emplace_back(udqActive.IUAD_size());
    // 3
    udqDims.emplace_back(entriesPerIUAD());
    // 4
    udqDims.emplace_back(entriesPerZUDN());
    // 5
    udqDims.emplace_back(entriesPerZUDL());
    // 6
    udqDims.emplace_back(entriesPerIGph(inteHead));
    // 7
    udqDims.emplace_back(udqActive.IUAP_size());
    // 8
    udqDims.emplace_back(nwmaxz(inteHead));
    // 9
    udqDims.emplace_back(noWellUdqs(sched, lookup_step));

    return udqDims;
}
