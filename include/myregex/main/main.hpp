#ifndef MAINMREGEX
#define MAINMREGEX

#include <variant>

#include <myregex/automaton/nfa/nfa.hpp>
#include <myregex/automaton/dfa/dfa.hpp>
#include <myregex/automaton/table/table.hpp>
#include <utilities/range.hpp>
#include <utilities/selector.hpp>
#include <myregex/exceptions/error.hpp>

namespace myregex
{

    /// @brief Clase que almecena los resultados de match
    /// @tparam charT
    template <typename charT, typename idT>
    class caption
    {
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    private:
        typename std::__cxx11::basic_string<charT> _M_string;
        idT _M_id;

    public:
        caption() : _M_string({}), _M_id({}) {}
        caption(const typename std::__cxx11::basic_string<charT> &str, idT id) : _M_string(str), _M_id(id) {}
        caption(const charT *begin, const charT *end, idT id) : _M_string(begin, end), _M_id(id) {}
        inline const typename std::__cxx11::basic_string<charT> &str() const { return _M_string; }
        inline idT id() const { return _M_id; }
        ~caption() {}
    };

    /// @brief Almacena las constantes de myregex
    namespace constants
    {
        /// @brief Opciones de match
        enum class match_options : unsigned char
        {
            _S_first_sequence,
            _S_maximun_sequence,
            _S_default = _S_first_sequence
        };
        /// @brief Opciones de verificacion
        enum class verification_options : unsigned char
        {
        };
        /// @brief Opciones de busqueda
        enum class search_options : unsigned char
        {
            _S_first_coincidence,
            _S_multiple_coincidence,
            _S_default = _S_first_coincidence
        };
        /// @brief Opciones de remplazo
        enum class replace_options : unsigned char
        {
            _S_first_coincidence,
            _S_multiple_coincidence,
            _S_default = _S_first_coincidence
        };

        constexpr inline size_t caption_no_match = -1ULL;
        constexpr inline match_options match_first = match_options::_S_first_sequence;
        constexpr inline match_options match_maximun = match_options::_S_maximun_sequence;

        constexpr inline search_options search_first = search_options::_S_first_coincidence;
        constexpr inline search_options search_maximun = search_options::_S_multiple_coincidence;

        constexpr inline replace_options replace_first = replace_options::_S_first_coincidence;
        constexpr inline replace_options replace_maximun = replace_options::_S_multiple_coincidence;

    }

    template <typename charT, typename idT>
    class basic_regex
    {
    private:
        static_assert(std::is_same_v<charT, char> || std::is_same_v<charT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        std::variant<basic_nfa<charT, idT>, basic_dfa<charT, idT>, basic_table<charT, idT>> _M_expresions;

    public:
        /// @brief Constructor por defecto
        /// @param list
        basic_regex(std::variant<basic_nfa<charT, idT>, basic_dfa<charT, idT>, basic_table<charT, idT>> &&_vr) : _M_expresions(std::move(_vr)) {}
        static bool verification(const std::__cxx11::basic_string<charT> &str, const myregex::basic_nfa<charT, idT> &_nfa)
        {
            basic_string_range<charT> range = str;
            return myregex::basic_regex<charT, idT>::verification(range, _nfa);
        }
        static bool verification(const std::__cxx11::basic_string<charT> &str, const myregex::basic_dfa<charT, idT> &_dfa)
        {
            basic_string_range<charT> range = str;
            return myregex::basic_regex<charT, idT>::verification(range, _dfa);
        }
        static bool verification(const std::__cxx11::basic_string<charT> &str, const myregex::basic_table<charT, idT> &_table)
        {
            basic_string_range<charT> range = str;
            return myregex::basic_regex<charT, idT>::verification(range, _table);
        }
        static bool verification(basic_string_range<charT> &range, const myregex::basic_nfa<charT, idT> &_nfa);
        static bool verification(basic_string_range<charT> &range, const myregex::basic_dfa<charT, idT> &_dfa);
        static bool verification(basic_string_range<charT> &range, const myregex::basic_table<charT, idT> &_table);

        /// @brief Esta funcion verifica si la cadena representada por str es valida
        /// @param str Cadena a leer
        /// @return Verdarero si lo es falso si no
        inline bool verification(const std::__cxx11::basic_string<charT> &str) const; // { /*return myregex::basic_regex<charT, idT>::verification(str, );*/ }
        /// @brief Esta funcion verifica si la cadena representada por range es valida
        /// @param str rango de caracteres de una cadena (para lexers)
        /// @return Verdarero si lo es falso si no
        inline bool verification(basic_string_range<charT> &range) const;

        template <myregex::constants::match_options option>
        static myregex::caption<charT, idT> match(const std::__cxx11::basic_string<charT> &str, const myregex::basic_nfa<charT, idT> &_nfa)
        {
            basic_string_range<charT> range = str;
            return myregex::basic_regex<charT, idT>::template match<option>(range, _nfa);
        }
        template <myregex::constants::match_options option>
        static myregex::caption<charT, idT> match(const std::__cxx11::basic_string<charT> &str, const myregex::basic_dfa<charT, idT> &_dfa)
        {
            basic_string_range<charT> range = str;
            return myregex::basic_regex<charT, idT>::template match<option>(range, _dfa);
        }
        template <myregex::constants::match_options option>
        static myregex::caption<charT, idT> match(const std::__cxx11::basic_string<charT> &str, const myregex::basic_table<charT, idT> &_table)
        {
            basic_string_range<charT> range = str;
            return myregex::basic_regex<charT, idT>::template match<option>(range, _table);
        }

        template <myregex::constants::match_options option>
        static myregex::caption<charT, idT> match(basic_string_range<charT> &range, const myregex::basic_nfa<charT, idT> &_nfa);
        template <myregex::constants::match_options option>
        static myregex::caption<charT, idT> match(basic_string_range<charT> &range, const myregex::basic_dfa<charT, idT> &_dfa);
        template <myregex::constants::match_options option>
        static myregex::caption<charT, idT> match(basic_string_range<charT> &range, const myregex::basic_table<charT, idT> &_table);

        /// @brief Esta funcion capta o valida segun el parametro S las cadenas de caracteres dentro de str
        /// @tparam S Determina la operacion de la funcion
        /// @param str Cadena a leer
        /// @return Devuelve el iterador al final de la cadena captada o nullptr si hay error junto al identificador de estado de aceptacion
        template <myregex::constants::match_options option>
        inline myregex::caption<charT, idT> match(const std::__cxx11::basic_string<charT> &str) const;
        /// @brief Esta funcion capta o valida segun el parametro S las cadenas de caracteres dentro del range
        /// @tparam S Determina la operacion de la funcion
        /// @param str rango de caracteres de una cadena (para lexers)
        /// @return Devuelve el iterador al final de la cadena captada o nullptr si hay error junto al identificador de estado de aceptacion
        template <myregex::constants::match_options option>
        inline myregex::caption<charT, idT> match(basic_string_range<charT> &range) const;

        inline size_t size() const;
        inline void export_automaton(std::basic_ostream<charT> &out) const;
        inline void view() const { export_automaton(std::selector<charT>::stream()); }
    };

    template <typename idT>
    using CompatibleRegex = myregex::basic_regex<char, idT>;
    template <typename idT>
    using regex = myregex::basic_regex<char, idT>;
    /////////////////////////////////////////////////////////////////////
    template <typename idT>
    using UnicodeRegex = myregex::basic_regex<wchar_t, idT>;
    template <typename idT>
    using wregex = myregex::basic_regex<wchar_t, idT>;
} // namespace myregex

#include "main.tpp"

#endif