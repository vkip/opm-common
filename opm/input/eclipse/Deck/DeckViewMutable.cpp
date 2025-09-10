/*
  Copyright 2021 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <opm/input/eclipse/Deck/DeckViewMutable.hpp>

#include <functional>
#include <stdexcept>

void Opm::DeckViewMutable::add_keyword(Opm::DeckKeyword& kw) {
    this->keyword_index[kw.name()].push_back(this->keywords.size());
    this->keywords.push_back(std::ref(kw));
}

bool Opm::DeckViewMutable::has_keyword(const std::string& kw) const {
    return this->keyword_index.find(kw) != this->keyword_index.end();
}

bool Opm::DeckViewMutable::empty() const {
    return this->keywords.empty();
}

std::size_t Opm::DeckViewMutable::size() const {
    return this->keywords.size();
}

Opm::DeckKeyword& Opm::DeckViewMutable::operator[](std::size_t kw_index) {
    return this->keywords.at(kw_index).get();
}

Opm::DeckViewMutable Opm::DeckViewMutable::operator[](const std::string& kw_name) {
    DeckViewMutable dw;
    auto iter = this->keyword_index.find(kw_name);
    if (iter != this->keyword_index.end()) {
        for (const auto& kw_index : iter->second) {
            auto& kw = this->keywords[kw_index].get();
            dw.add_keyword(kw);
        }
    }
    return dw;
}

Opm::DeckKeyword& Opm::DeckViewMutable::front() {
    if (this->empty())
        throw std::logic_error("Tried to get front() from empty DeckViewMutable");

    return this->keywords.front().get();
}

Opm::DeckKeyword& Opm::DeckViewMutable::back() {
    if (this->empty())
        throw std::logic_error("Tried to get back() from empty DeckViewMutable");

    return this->keywords.back().get();
}

std::vector<std::size_t> Opm::DeckViewMutable::index(const std::string& keyword) const {
    auto iter = this->keyword_index.find(keyword);
    if (iter != this->keyword_index.end())
        return iter->second;

    return {};
}

std::size_t Opm::DeckViewMutable::count(const std::string& keyword) const {
    auto iter = this->keyword_index.find(keyword);
    if (iter == this->keyword_index.end())
        return 0;

    return iter->second.size();
}
