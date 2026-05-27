#ifndef _NFA
#define _NFA

#include <variant>
#include <vector>
#include <stdexcept>
#include <map>
#include <set>

class _regex;

class NFA
{
public:
    struct _M_Cualifiquer
    {
        size_t min;
        size_t max;
        _M_Cualifiquer(size_t _a, size_t _b) : min(_a), max(_b) {}
    };
    struct _M_Char
    {
        char _M_char;
        size_t _M_next;
        _M_Cualifiquer _M_cualifiquer;
        _M_Char(char c) : _M_char(c), _M_next(-1ULL), _M_cualifiquer(1, 1) {}
        inline bool operator==(const char c) const { return _M_char == c; }
    };
    struct _M_Group
    {
        size_t _M_reference;
        size_t _M_next;
        _M_Cualifiquer _M_cualifiquer;
        _M_Group(size_t reference) : _M_reference(reference), _M_next(-1ULL), _M_cualifiquer(1, 1) {}
    };
    struct _M_Range
    {
        char a;
        char b;
        _M_Range(char c) : a(c), b(c) {}
        _M_Range(char _a, char _b) : a(_a), b(_b) {}
    };
    struct _M_Class
    {
        bool _M_inverter;
        std::vector<_M_Range> _M_refereces;
        _M_Cualifiquer _M_cualifiquer;
        size_t _M_next;
        _M_Class() : _M_inverter(false), _M_refereces({}), _M_next(-1ULL), _M_cualifiquer(1, 1) {}
        inline void _M_push(_M_Range range) { _M_refereces.push_back(range); }
        inline bool operator==(const char c) const
        {
            for (auto &&i : _M_refereces)
            {
                if (i.a <= c && c <= i.b)
                    return true;
            }
            return false;
        }
    };
    struct _M_Bifucacion
    {
        size_t _M_lhs;
        size_t _M_rhs;
        _M_Bifucacion(size_t lhs, size_t rhs) : _M_lhs(lhs), _M_rhs(rhs) {}
    };

    class _M_State
    {
    private:
        std::variant<NFA::_M_Char, NFA::_M_Group, NFA::_M_Bifucacion, NFA::_M_Class> _M_refrence;

    public:
        _M_State(const NFA::_M_Char &_fect) : _M_refrence(_fect) {}
        _M_State(const NFA::_M_Group &_fect) : _M_refrence(_fect) {}
        _M_State(const NFA::_M_Bifucacion &_fect) : _M_refrence(_fect) {}
        _M_State(const NFA::_M_Class &_fect) : _M_refrence(_fect) {}
        inline const std::variant<NFA::_M_Char, NFA::_M_Group, NFA::_M_Bifucacion, NFA::_M_Class> &container() const { return _M_refrence; }
        void _M_next(size_t position)
        {
            std::visit([position](auto &&value) -> void
                       {
                        using _Type = std::decay_t<decltype(value)>;
                        if constexpr (std::is_same_v<_Type, NFA::_M_Char> | std::is_same_v<_Type, NFA::_M_Group> | std::is_same_v<_Type, NFA::_M_Class>)
                            value._M_next = position;
                        else
                            throw std::runtime_error("Error: imposible operation in this section code"); }, _M_refrence);
        }
        void _M_set_cualifiquer(NFA::_M_Cualifiquer cualifiquer)
        {
            std::visit([cualifiquer](auto &&value) -> void
                       {
                        using _Type = std::decay_t<decltype(value)>;
                        if constexpr (std::is_same_v<_Type, NFA::_M_Char> | std::is_same_v<_Type, NFA::_M_Group> | std::is_same_v<_Type, NFA::_M_Class>)
                            value._M_cualifiquer = cualifiquer;
                        else
                            throw std::runtime_error("Error: imposible cualifiquer in this section code"); }, _M_refrence);
        }
        friend _regex;
        ~_M_State() {}
    };

    using _M_States = std::vector<NFA::_M_State>;
    using _M_States_Begins = std::vector<std::pair<size_t, int>>;
    using _M_Group_Map = std::map<size_t, std::set<size_t>>;

private:
    _M_States _M_nfa_state;
    _M_States_Begins _M_nfa_begins;
    _M_Group_Map _M_nfa_groups;

private:
    static void _S_view(size_t position, const _M_States &vector, size_t anidation);

public:
    NFA() : _M_nfa_state(), _M_nfa_begins(), _M_nfa_groups() {}
    void view();
    ~NFA() {}
    friend _regex;
};

#endif