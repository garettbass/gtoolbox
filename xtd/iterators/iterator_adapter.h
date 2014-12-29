#pragma once
#include <type_traits>

namespace xtd {

    //==========================================================================

    template <typename It, typename Adaptor>
    class iterator_adapter : Adaptor
    {
        It _it;
        
        typedef iterator_adapter<It, Adaptor> This;
        
        typedef It                                      _iterator;
        typedef typename std::add_const<It>::type _const_iterator;
        
    public:

        typedef typename It::difference_type difference_type;
        
        typedef decltype(Adaptor()(      *_iterator()))       reference;
        typedef decltype(Adaptor()(*_const_iterator())) const_reference;
        
        typedef typename std::remove_reference<      reference>       value_type;
        typedef typename std::remove_reference<const_reference> const_value_type;
        
        typedef typename std::add_pointer<      value_type>       pointer;
        typedef typename std::add_pointer<const_value_type> const_pointer;
        
    public:

        iterator_adapter() = default;

        iterator_adapter(It it) : _it(it) {}
        
        iterator_adapter(const This&) = default;
        
    public:

        bool operator == (const This& it) const { return _it == it._it; }
        bool operator != (const This& it) const { return _it != it._it; }
        
        const_reference operator * () const { return Adaptor::operator()(*_it); }
              reference operator * ()       { return Adaptor::operator()(*_it); }
        
        const_pointer operator -> () const { return &Adaptor::operator()(*_it); }
              pointer operator -> ()       { return &Adaptor::operator()(*_it); }
        
        iterator_adapter& operator ++ () { ++_it; return *this; }
        
        iterator_adapter operator ++ (int) { This it(*this); ++_it; return it; }
        
        void swap(iterator_adapter& it) { swap(_it, it._it); }

    };

    //==========================================================================

} // namespace xtd