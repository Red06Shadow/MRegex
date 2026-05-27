#include <nfa.hpp>
#include <iostream>

void NFA::view()
{
    std::cout << "Qnfa " << '[';
    for (size_t i = 0; i < _M_nfa_state.size(); i++)
    {
        std::cout << std::endl
                  << i << ": ";
        std::visit([](auto &&value) -> void
               {
                using _Type = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<_Type, NFA::_M_Char>)
                {
                    std::cout << value._M_char << '{'<< value._M_cualifiquer.min << ',';
                    if (value._M_cualifiquer.max != -1ULL)
                        std::cout << value._M_cualifiquer.max;
                    else
                        std::cout << 'I';
                    std::cout << '}';
                }
                else if constexpr (std::is_same_v<_Type, NFA::_M_Class>)
                {
                    std::cout << (value._M_inverter ? "![": "[") ;
                    for (auto &&i : value._M_refereces)
                    {
                        if (i.a == i.b)
                            std::cout << i.a << ',';
                        else
                            std::cout << '(' << i.a << ',' << i.b << "),";
                    }
                    std::cout << "]{" << value._M_cualifiquer.min << ',';
                    if (value._M_cualifiquer.max != -1ULL)
                        std::cout << value._M_cualifiquer.max;
                    else
                        std::cout << 'I';
                    std::cout << '}';
                }
                else if constexpr (std::is_same_v<_Type, NFA::_M_Group>)
                {
                    std::cout << "(Grupo){" << value._M_cualifiquer.min << ',';
                    if (value._M_cualifiquer.max != -1ULL)
                        std::cout << value._M_cualifiquer.max;
                    else
                        std::cout << 'I';
                    std::cout << '}';
                }
                else if constexpr (std::is_same_v<_Type, NFA::_M_Bifucacion>)
                {
                    std::cout << "{OR}";
                }
                return ; 
            }, _M_nfa_state[i].container());
    }
    std::cout << std::endl
              << ']' << std::endl;
    for (auto &&i : _M_nfa_begins)
    {
        std::cout << "Type: " << i.second << " Transition: " << std::endl;
        NFA::_S_view(i.first, _M_nfa_state, 0);
        std::cout << std::endl;
    }
    std::cout << "NFA groups references: " << std::endl;
    std::cout << '{';
    for (auto &&[key, value] : _M_nfa_groups)
    {
        std::cout << key << " : { ";
        for (auto &&offest : value)
            std::cout << offest << ' ';
        std::cout << " }";
    }
    std::cout << " }" << std::endl;
}

void NFA::_S_view(size_t position, const NFA::_M_States &vector, size_t anidation)
{
    
    if (position == -1ULL)
    {
        // std::cout << 'F';
        return;
    }
    std::visit([position, &vector, anidation](auto &&value) -> void
               {
                using _Type = std::decay_t<decltype(value)>;
                if constexpr (std::is_same_v<_Type, NFA::_M_Char> | std::is_same_v<_Type, NFA::_M_Class>)
                {
                    // std::cout << value._M_char << '{'<< value._M_cualifiquer.min << ',';
                    // if (value._M_cualifiquer.max != -1ULL)
                    //     std::cout << value._M_cualifiquer.max;
                    // else
                    //     std::cout << 'I';
                    // std::cout << '}' << "->";
                    std::cout << "{ " << position << " -> ";
                    if (value._M_next != -1ULL)
                        std::cout << value._M_next; 
                    else
                        std::cout << 'F';
                    std::cout << " }" << std::endl;
                    _S_view(value._M_next, vector, anidation);
                }
                // else if constexpr (std::is_same_v<_Type, NFA::_M_Class>)
                // {
                    // std::cout << (value._M_inverter ? "![": "[") ;
                    // for (auto &&i : value._M_refereces)
                    // {
                    //     if (i.a == i.b)
                    //         std::cout << i.a << ',';
                    //     else
                    //         std::cout << '(' << i.a << ',' << i.b << "),";
                    // }
                    // std::cout << "]{" << value._M_cualifiquer.min << ',';
                    // if (value._M_cualifiquer.max != -1ULL)
                    //     std::cout << value._M_cualifiquer.max;
                    // else
                    //     std::cout << 'I';
                    // std::cout << '}' << "->";
                    // _S_view(value._M_next, vector, anidation);
                // }
                else if constexpr (std::is_same_v<_Type, NFA::_M_Group>)
                {
                    // std::cout << '(';
                    // _S_view(value._M_reference, vector, anidation);
                    // std::cout << "){" << value._M_cualifiquer.min << ',';
                    // if (value._M_cualifiquer.max != -1ULL)
                    //     std::cout << value._M_cualifiquer.max;
                    // else
                    //     std::cout << 'I';
                    // std::cout << '}' << "->";
                    std::cout << "{ " << position << " -> ";
                    if (value._M_reference != -1ULL)
                        std::cout << value._M_reference; 
                    else
                        std::cout << 'F';
                    std::cout << " }" << std::endl;

                    _S_view(value._M_reference, vector, anidation);

                    std::cout << "{ " << position << " -> ";
                    if (value._M_next != -1ULL)
                        std::cout << value._M_next; 
                    else
                        std::cout << 'F';
                    std::cout << " }" << std::endl;
                    _S_view(value._M_next, vector, anidation);
                }
                else if constexpr (std::is_same_v<_Type, NFA::_M_Bifucacion>)
                {
                    // std::cout << "{";
                    // _S_view(value._M_lhs, vector, anidation + 4);
                    // std::cout << " | ";
                    // _S_view(value._M_rhs, vector, anidation + 4);
                    // std::cout << "}";
                    std::cout << "{ " << position << " -> " << value._M_lhs << " }" << std::endl;
                    _S_view(value._M_lhs, vector, anidation);
                    std::cout << "{ " << position << " -> " << value._M_rhs << " }" << std::endl;
                    _S_view(value._M_rhs, vector, anidation);
                }
                return ; }, vector[position].container());
}