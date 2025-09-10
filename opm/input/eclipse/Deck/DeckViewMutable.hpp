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

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DECKVIEWMUTABLE_HPP
#define DECKVIEWMUTABLE_HPP

#include <opm/input/eclipse/Deck/DeckKeyword.hpp>

#include <iterator>
#include <unordered_map>

namespace Opm {


class DeckViewMutable {
public:
    typedef std::vector<std::reference_wrapper<DeckKeyword>> storage_type;


    struct Iterator {
        explicit Iterator(storage_type::iterator inner_iter) :
            inner(inner_iter)
        {}

        using difference_type = storage_type::iterator::difference_type;
        using iterator_category = storage_type::iterator::iterator_category;
        using pointer = DeckKeyword*;
        using reference = DeckKeyword&;
        using value_type = DeckKeyword;

        DeckKeyword& operator*() { return this->inner->get(); }
        DeckKeyword* operator->() { return &this->inner->get(); }

        Iterator& operator++()    { ++this->inner; return *this; }
        Iterator  operator++(int) { auto tmp = *this; ++this->inner; return tmp; }

        Iterator& operator--()    { --this->inner; return *this; }
        Iterator  operator--(int) { auto tmp = *this; --this->inner; return tmp; }

        Iterator::difference_type operator-(const Iterator &other) const { return this->inner - other.inner; }
        Iterator operator+(Iterator::difference_type shift) const { Iterator tmp = *this; tmp.inner += shift; return tmp;}

        friend bool operator== (const Iterator& a, const Iterator& b) { return a.inner == b.inner; };
        friend bool operator<= (const Iterator& a, const Iterator& b) { return a.inner <= b.inner; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.inner != b.inner; };

    private:
        storage_type::iterator inner;
    };

    Iterator begin() { return Iterator(this->keywords.begin()); }
    Iterator end() { return Iterator(this->keywords.end()); }

    DeckKeyword& operator[](std::size_t index);
    DeckViewMutable operator[](const std::string& keyword);
    std::vector<std::size_t> index(const std::string& keyword) const;
    std::size_t count(const std::string& keyword) const;
    DeckKeyword& front();
    DeckKeyword& back();

    DeckViewMutable() = default;
    void add_keyword(DeckKeyword& kw);
    bool has_keyword(const std::string& kw) const;
    bool empty() const;
    std::size_t size() const;

    template<class Keyword>
    bool has_keyword() const {
        return this->has_keyword( Keyword::keywordName );
    }

    template<class Keyword>
    DeckViewMutable get() const {
        return this->operator[](Keyword::keywordName);
    }

private:
    storage_type keywords;
    std::unordered_map<std::string, std::vector<std::size_t>> keyword_index;
};

}

template<>
struct std::iterator_traits<Opm::DeckViewMutable::Iterator>
{
    using difference_type = Opm::DeckViewMutable::Iterator::difference_type;
    using iterator_category = Opm::DeckViewMutable::Iterator::iterator_category;
    using pointer = Opm::DeckViewMutable::Iterator::pointer;
    using reference = Opm::DeckViewMutable::Iterator::reference;
    using value_type = Opm::DeckViewMutable::Iterator::value_type;
};

#endif
