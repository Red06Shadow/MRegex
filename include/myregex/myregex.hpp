#ifndef MREGEX
#define MREGEX

#include "myregex_nfa.hpp"
#include "myregex_dfa.hpp"
#include "myregex_iterators.hpp"
#include "myregex_table.hpp"
#include "myregex_error.hpp"

#include <stack>
#include <queue>
#include <unordered_set>
#include <math.h>

#define TESTCODE false

namespace myregex
{
    /// @brief Clase que almecena los resultados de match
    /// @tparam CharT
    template <typename CharT>
    class caption
    {
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    private:
        typename std::__cxx11::basic_string<CharT>::const_iterator _M_iterator;
        const size_t _M_id;

    public:
        caption(const typename std::__cxx11::basic_string<CharT>::const_iterator &iterator, size_t id) : _M_iterator(iterator), _M_id(id) {}
        caption(const CharT *iterator, size_t id) : _M_iterator(iterator), _M_id(id) {}
        inline const typename std::__cxx11::basic_string<CharT>::const_iterator &iterator() const { return _M_iterator; }
        inline size_t id() const { return _M_id; }
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

    /// @brief Clase basica para la construcion y conversion de expresiones regulares
    /// @tparam CharT
    template <typename CharT>
    class basic_build
    {
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");

    private: // enums, structs
        using _M_begin_transitions = std::pair<size_t, std::vector<size_t>>;
        using _M_range_cualifiquer = std::pair<size_t, size_t>;

    private: // Funtions regex -> basic_nfa
        // Funciones para el parseo de expresiones regex a basic_nfa
        static CharT _S_parser_code_caracter_hexunicode(basic_string_range<CharT> &, const unsigned char);
        static CharT _S_parser_code_caracter_octal(basic_string_range<CharT> &);
        static CharT _S_parser_code_caracter(basic_string_range<CharT> &);
        static _M_range_cualifiquer _S_parser_cualifiquer_range(basic_string_range<CharT> &);
        static void _S_parser_cualifiquer(basic_string_range<CharT> &, basic_nfa<CharT> &, const _M_begin_transitions &, bool);
        static _M_begin_transitions _S_parser_nfa_parser_class_expresions(basic_string_range<CharT> &, basic_nfa<CharT> &);
        static size_t _S_build_nfa_parser_regular_expresions_basic(basic_string_range<CharT> &, basic_nfa<CharT> &, bool, size_t);
        static _M_begin_transitions _S_build_nfa_parser_or_expresions(basic_string_range<CharT> &, basic_nfa<CharT> &, bool, size_t);

    private: // Funtions basic_nfa -> basic_dfa
        // Funciones adicionales del convertidor de basic_nfa a basic_dfa
        static typename basic_dfa<CharT>::States _S_elipson_cloursers(typename basic_dfa<CharT>::States, const basic_nfa<CharT> &);
        static typename basic_dfa<CharT>::States _S_move(typename basic_dfa<CharT>::States, CharT, const basic_nfa<CharT> &);
        inline static typename basic_dfa<CharT>::States _S_move_elipson_cloursers(typename basic_dfa<CharT>::States states, CharT a, const basic_nfa<CharT> &nfa) { return _S_elipson_cloursers(_S_move(states, a, nfa), nfa); }

    public: // Funtions
        /// @brief Genera un basic_nfa basado en una cadena de caracteres que represente un regex
        /// @param str cadena de caracteres
        /// @return Devuelve el nfa generado
        static basic_nfa<CharT> build_nfa(const std::__cxx11::basic_string<CharT> &str);
#if TESTCODE
        /// @brief Genera un basic_nfa basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
        /// @param argv arreglo de cadenas de caracteres
        /// @param size cantidad de elementos dentro del arreglo
        /// @return Devuelve el nfa resultante
        static basic_nfa<CharT> build_nfa(const CharT *[], size_t);
#endif
        /// @brief Genera un basic_nfa basado en una losta de cadenas de caracteres que representen las expresiones regulares (Nota: Esta funcion es para usarla junto a los argumentos de la funcion main)
        /// @param list lista de cadenas de caracteres
        /// @return Devuelve el nfa resultante
        static basic_nfa<CharT> build_nfa(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<CharT>>> &list);
        /// @brief Convierte el basic_nfa a basic_dfa
        /// @param nfa
        /// @return Devuelve el dfa obtenido a partir del nfa
        static basic_dfa<CharT> convert_nfa_to_dfa(const basic_nfa<CharT> &nfa);
        /// @brief Convierte el basic_dfa a una tabla de transiciones de basic_dfa
        /// @param dfa
        /// @return Devuelve la tabla dfa
        static TableDFA convert_dfa_to_table(const basic_dfa<CharT> &dfa);

        // template <typename AllocatorRegexT = myregex::basic_dfa<CharT>>
        // void compile(const AllocatorRegexT& regular, const std::__cxx11::basic_string<CharT>& path);
    };

    /// @brief Clase basica para el uso de expresiones regulares
    /// @tparam CharT
    /// @tparam AllocatorRegexT
    template <typename CharT, typename AllocatorRegexT = myregex::basic_dfa<CharT>>
    class basic_regex
    {
    public:
        static_assert(std::is_same_v<CharT, char> || std::is_same_v<CharT, wchar_t>, "Error: no se permiten tipos de datos que no sean de caracteres(solo char o wchar_t)");
        static_assert(std::is_same_v<AllocatorRegexT, myregex::basic_dfa<CharT>> || std::is_same_v<AllocatorRegexT, myregex::TableDFA>, "Error: no se permiten tipos de datos que no sean basic_dfa ni TableDFA");

    private:
        AllocatorRegexT allocator;

    public:
        /// @brief Constructor por defecto
        /// @param list
        basic_regex(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<CharT>>> &list)
        {
            basic_nfa<CharT> nfa = myregex::basic_build<CharT>::build_nfa(list);
            if constexpr (std::is_same_v<AllocatorRegexT, myregex::basic_dfa<CharT>>)
                allocator = myregex::basic_build<CharT>::convert_nfa_to_dfa(nfa);
            else if constexpr (std::is_same_v<AllocatorRegexT, TableDFA>)
                allocator = myregex::basic_build<CharT>::convert_dfa_to_table(myregex::basic_build<CharT>::convert_nfa_to_dfa(nfa));
        }
        basic_regex(const AllocatorRegexT &regularexpresion) : allocator(regularexpresion) {}
        basic_regex(AllocatorRegexT &&regularexpresion) : allocator(std::move(regularexpresion)) {}

        static bool verification(const std::__cxx11::basic_string<CharT> &str, const AllocatorRegexT &allocator_regex);
        static bool verification(basic_string_range<CharT> &range, const AllocatorRegexT &allocator_regex);

        /// @brief Esta funcion verifica si la cadena representada por str es valida
        /// @param str Cadena a leer
        /// @return Verdarero si lo es falso si no
        inline bool verification(const std::__cxx11::basic_string<CharT> &str) const { return myregex::basic_regex<CharT>::verification(str, allocator); }
        /// @brief Esta funcion verifica si la cadena representada por range es valida
        /// @param str rango de caracteres de una cadena (para lexers)
        /// @return Verdarero si lo es falso si no
        inline bool verification(basic_string_range<CharT> &range) const { return myregex::basic_regex<CharT>::verification(range, allocator); }

        template <myregex::constants::match_options option>
        static myregex::caption<CharT> _S_match(const std::__cxx11::basic_string<CharT> &str, const AllocatorRegexT &allocator_regex);
        // template <myregex::constants::match_options option>
        // inline static myregex::caption<CharT> &&_M_match(const std::__cxx11::basic_string<CharT> &str, const AllocatorRegexT &allocator_regex) { return std::move(myregex::basic_regex<CharT>::template _S_match<option>(str, allocator_regex)); }
        template <myregex::constants::match_options option>
        static myregex::caption<CharT> _S_match(basic_string_range<CharT> &range, const AllocatorRegexT &allocator_regex);
        // template <myregex::constants::match_options option>
        // inline static myregex::caption<CharT> &&_M_match(basic_string_range<CharT> &range, const AllocatorRegexT &allocator_regex) { return std::move(myregex::basic_regex<CharT>::template _S_match<option>(range, allocator_regex)); }

        /// @brief Esta funcion capta o valida segun el parametro S las cadenas de caracteres dentro de str
        /// @tparam S Determina la operacion de la funcion
        /// @param str Cadena a leer
        /// @return Devuelve el iterador al final de la cadena captada o nullptr si hay error junto al identificador de estado de aceptacion
        template <myregex::constants::match_options option>
        inline myregex::caption<CharT> match(const std::__cxx11::basic_string<CharT> &str) const { return myregex::basic_regex<CharT>::template _S_match<option>(str, allocator); }
        /// @brief Esta funcion capta o valida segun el parametro S las cadenas de caracteres dentro del range
        /// @tparam S Determina la operacion de la funcion
        /// @param str rango de caracteres de una cadena (para lexers)
        /// @return Devuelve el iterador al final de la cadena captada o nullptr si hay error junto al identificador de estado de aceptacion
        template <myregex::constants::match_options option>
        inline myregex::caption<CharT> match(basic_string_range<CharT> &range) const { return myregex::basic_regex<CharT>::template _S_match<option>(range, allocator); }

        inline size_t size() const { return allocator.size(); }
        inline void view() const { allocator.view(); }
        ~basic_regex() {}
    };

    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    template <typename AllocatorRegexT = myregex::basic_dfa<char>>
    using CompatibleRegex = myregex::basic_regex<char, AllocatorRegexT>;
    template <typename AllocatorRegexT = myregex::basic_dfa<char>>
    using regex = myregex::basic_regex<char, AllocatorRegexT>;
    /////////////////////////////////////////////////////////////////////
    template <typename AllocatorRegexT = myregex::basic_dfa<wchar_t>>
    using UnicodeRegex = myregex::basic_regex<wchar_t, AllocatorRegexT>;
    template <typename AllocatorRegexT = myregex::basic_dfa<wchar_t>>
    using wregex = myregex::basic_regex<wchar_t, AllocatorRegexT>;
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    using CompatibleBuild = myregex::basic_build<char>;
    using build = myregex::basic_build<char>;
    /////////////////////////////////////////////////////////////////////
    using UnicodeBuild = myregex::basic_build<wchar_t>;
    using wbuild = myregex::basic_build<wchar_t>;

    template <typename CharT>
    CharT basic_build<CharT>::_S_parser_code_caracter_hexunicode(basic_string_range<CharT> &range, const unsigned char _S)
    {
        CharT result;
        for (CharT i = 0; i < _S; i++)
        {
            if (range.peak() >= range.end())
                throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
            CharT caracter = *range.peak();
            if (caracter >= CharT('a') && caracter <= CharT('f'))
                result = result * 16 + (caracter - CharT('a') + 10);
            else if (caracter >= CharT('A') && caracter <= CharT('F'))
                result = result * 16 + (caracter - CharT('A') + 10);
            else if (caracter >= CharT('0') && caracter <= CharT('9'))
                result = result * 16 + (caracter - CharT('0'));
            else
                throw myregex::regex_error("caracter no permitido en el formato de caracter unicode", range.position(range.peak()), range, 1);
            range.next();
        }
        return result;
    }

    template <typename CharT>
    CharT basic_build<CharT>::_S_parser_code_caracter_octal(basic_string_range<CharT> &range)
    {
        char result;
        for (char i = 0; i < 3; i++)
        {
            if (range.peak() >= range.end() && i < 1)
                throw myregex::regex_error("formato de caracter octal incorrecto", range.position(range.end(), 1ULL), range, 2);
            char caracter = *range.peak();
            if (caracter >= '0' && caracter <= '7')
                result = result * 8 + (caracter - '0');
            else
                break;
            range.next();
        }
        return result;
    }

    template <typename CharT>
    CharT basic_build<CharT>::_S_parser_code_caracter(basic_string_range<CharT> &range)
    {
        CharT c;
        if (range.peak() >= range.end())
            throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
        switch (c = *range.peak())
        {
        case CharT('n'):
            range.next();
            return CharT('\n');
            break;
        case CharT('t'):
            range.next();
            return CharT('\t');
            break;
        case CharT('v'):
            range.next();
            return CharT('\v');
            break;
        case CharT('f'):
            range.next();
            return CharT('\f');
            break;
        case CharT('b'):
            range.next();
            return CharT('\b');
            break;
        case CharT('r'):
            range.next();
            return CharT('\r');
            break;
        case CharT('x'):
            range.next();
            return basic_build<CharT>::_S_parser_code_caracter_hexunicode(range, 2);
            break;
        case CharT('u'):
            range.next();
            return basic_build<CharT>::_S_parser_code_caracter_hexunicode(range, 4);
            break;
        case CharT('U'):
            range.next();
            return basic_build<CharT>::_S_parser_code_caracter_hexunicode(range, 8);
            break;
        default:
            if ('0' <= c && c <= '7')
                return basic_build<CharT>::_S_parser_code_caracter_octal(range);
            range.next();
            return c;
            break;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////Parser////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    template <typename CharT>
    typename basic_build<CharT>::_M_range_cualifiquer basic_build<CharT>::_S_parser_cualifiquer_range(basic_string_range<CharT> &range)
    {
        size_t n1 = 0;    // minimo
        size_t n2 = 0;    // maximo
        size_t *rn = &n1; // puntero al numero actual
        CharT caracter;
        bool invalid = true; // Determina si es valido el resultado
        size_t position_start = range.position(range.peak(), -1ULL);
        // Recorremos la cadena de caracteres
        while (range.peak() < range.end() and (caracter = *range.peak()) != '}')
        {
            // Si es un numero
            if (CharT('0') <= caracter && caracter <= CharT('9'))
            {
                // Realizamos los calculos para la conversion
                *rn = *rn * 10 + caracter - CharT('0');
                // Y desactivamos la invalidacion
                invalid = false;
            }
            else if (CharT(',') == caracter) // Si es una coma
            {
                // Y el rn apunta a el numero 2(maximo)
                if (invalid)
                    throw myregex::regex_error("min value no inserted", range.position(range.peak()), range, 16);
                if (rn == &n2)
                    throw myregex::regex_error("invalid subdivision", range.position(range.peak()), range, 4);
                // Cambiamos el puntero de n1(minimo) a n2(maximo)
                rn = &n2;
                // Y activamos la invalidacion
                invalid = true;
            }
            else // Si no solo se emite el error de caracter invalido
                throw myregex::regex_error("invalid caracter inserted", range.position(range.peak()), range, 5);
            range.next(); // Y por ultimo pasamos al siguiente
        }
        if (range.peak() == range.end()) // Si no se llego al final del cualificador, emitir error
            throw myregex::regex_error("unespect termination of range", range.position(range.end(), 1ULL), range, 6);
        if (invalid) // si es invalida, emitir tambien un error
            throw myregex::regex_error("invalid format inserted", position_start, range.position(range.peak()), range, 7);
        if (rn == &n1) // si el puntero permanece en n1(minimo), significa que el rango es {n1, n1} minimo igual a maximo
            n2 = n1;
        if (n2 < n1)
            throw myregex::regex_error("range whit min greather than max in this cualifiquer", position_start, range.position(range.peak()), range, 14);
        // Retornar el par de numeros
        return {n1, n2};
    }

    template <typename CharT>
    void basic_build<CharT>::_S_parser_cualifiquer(basic_string_range<CharT> &range, basic_nfa<CharT> &nfa, const basic_build<CharT>::_M_begin_transitions &_T_qAtr, bool isGroup)
    {
        CharT c = 0;
        // Si el rango llega al final salta a la sentemcia por defecto del switch
        if (range.peak() >= range.end())
            goto labelForEnd;
        // Si no obtenemos el caracter en esa posicion
        c = *range.peak();
        switch (c)
        {
        case CharT('?'):
        {
            // Si se cumple se agrega la opcionalidad al estado
            range.next();
            // Primera se agregan las transiciones generadas siempre y cuando no sean un grupo
            if (!isGroup)
                for (auto &&transition : _T_qAtr.second)
                    nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
            // luego se agrega la trasicion de la opcionalidad
            nfa.Q_transitions[{_T_qAtr.first, -1ULL}].push_back(nfa.Q_nfa.size());
            break;
        }
        case CharT('*'):
        {
            // Si se cumple se agrega la repeticion de cero a multiples veces
            range.next();
            // Si es un grupo las transiciones de repeticion se realizan desde el final hasta el principio
            if (isGroup)
                for (auto &&transition : _T_qAtr.second)
                    nfa.Q_transitions[{nfa.Q_nfa.size(), transition}].push_back(_T_qAtr.first);
            else // si no simplemente se agregan al mismo estado
                for (auto &&transition : _T_qAtr.second)
                    nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(_T_qAtr.first);
            nfa.Q_transitions[{_T_qAtr.first, -1ULL}].push_back(nfa.Q_nfa.size());
            break;
        }
        case CharT('+'):
        {
            // Si se cumple se agrega la repeticion de cero a multiples veces
            range.next();
            if (!isGroup)
                for (auto &&transition : _T_qAtr.second)
                    nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
            nfa.Q_transitions[{nfa.Q_nfa.size(), -1ULL}].push_back(_T_qAtr.first);
            break;
        }
        case CharT('{'):
        {
            range.next();
            _M_range_cualifiquer cualifiquer_range = basic_build<CharT>::_S_parser_cualifiquer_range(range);
            range.next();
            if (isGroup)
            {
                size_t n_state = nfa.Q_nfa.size() - _T_qAtr.first;
                std::vector<size_t> optionals_groups = {};
                if (cualifiquer_range.first == 0)
                    optionals_groups.push_back(_T_qAtr.first);
                for (size_t state = 1; state < cualifiquer_range.second; state++)
                {
                    size_t state_group_begin = nfa.Q_nfa.size();
                    if (cualifiquer_range.first <= state && state < cualifiquer_range.second)
                        // Agregar una trasicion de opcionalidad
                        optionals_groups.push_back(state_group_begin);
                    for (size_t state_group = 0; state_group < n_state; state_group++)
                    {
                        nfa.Q_nfa.push_back(nfa.Q_nfa.size());
                        for (auto &&[key, value] : nfa.Q_transitions)
                        {
                            if (key.first != _T_qAtr.first + state_group)
                                continue;
                            nfa.Q_transitions[{state_group_begin + state_group, key.second}].push_back(nfa.Q_nfa.size());
                        }
                    }
                }
                for (auto &&state : optionals_groups)
                    nfa.Q_transitions[{state, -1ULL}].push_back(nfa.Q_nfa.size());
            }
            else // si no simplemente se agregan al mismo estado
            {
                size_t state_iterator = _T_qAtr.first;
                for (size_t state = 0; state < cualifiquer_range.second; state++)
                {
                    // Si el estado actual esta entre el minimo y el maximo
                    if (cualifiquer_range.first <= state && state < cualifiquer_range.second)
                        // Agregar una trasicion de opcionalidad
                        nfa.Q_transitions[{state_iterator, -1ULL}].push_back(_T_qAtr.first + cualifiquer_range.second);
                    // Y luego se agregan todas las transiciones desde el estado actual hasta el proximo estado
                    for (auto &&transition : _T_qAtr.second)
                        nfa.Q_transitions[{state_iterator, transition}].push_back(nfa.Q_nfa.size());
                    // por ultimo se guarda el nuevo estado
                    state_iterator = nfa.Q_nfa.size();
                    // Y si el rango es menor que el maximo - 1 se agrega el nuevo estado generado
                    if (state < cualifiquer_range.second - 1ULL)
                        nfa.Q_nfa.push_back(state_iterator);
                }
            }
            break;
        }
        default:
        labelForEnd:
            // Por ultimo en caso de no cumplirse los casos anteriores o la condicion de final de cadena
            if (isGroup) // si es un grupo, no se agregan dichas transiciones
                break;
            // Pero si no, se agregan las transiciones del estado
            for (auto &&transition : _T_qAtr.second)
                nfa.Q_transitions[{_T_qAtr.first, transition}].push_back(nfa.Q_nfa.size());
            break;
        }
    }

    template <typename CharT>
    typename basic_build<CharT>::_M_begin_transitions basic_build<CharT>::_S_parser_nfa_parser_class_expresions(basic_string_range<CharT> &range, basic_nfa<CharT> &nfa)
    {
        size_t qA = nfa.Q_nfa.size();
        std::vector<size_t> transitions = {};
        std::unordered_set<CharT> alphabet = {};
        bool inverter = false;
        size_t position_start = range.position(range.peak(), -1ULL);

        nfa.Q_nfa.push_back(qA);
        // Chequeamos si es una clase invertida
        if (*range.peak() == CharT('^'))
        {
            // Si lo es hacer operaciones de inversion, por ahora no se ha implementado
            inverter = true;
            range.next();
        }
        CharT a;
        // Recorremos la cadena hasta llegar el final
        while (range.peak() < range.end() and (a = *range.peak()) != CharT(']'))
        {
            // Pasamos al siguiente valor
            size_t position_start_range = range.position(range.peak());
            range.next();

            if (a == CharT('-'))
                throw myregex::regex_error("invalid caracter in class with no initialice range whit caracter", range.position(range.peak(), -1ULL), range, 0);

            if (a == CharT('\\'))                                       // Si es un caracter de escape
                a = basic_build<CharT>::_S_parser_code_caracter(range); // Ejecutar caracteres de escape

            // Si el iterador llego a su fin
            if (range.peak() >= range.end())
                throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);

            // Si el caracter proximo no es un caracter '-'
            if (*(range.peak()) != CharT('-'))
                alphabet.insert(a); // Solamente insertar el caracter actual
            else
            {
                // En caso contrario obtenemos el rango de transiciones
                auto offset = range.offset(1);
                char b;
                if (offset >= range.end())
                    throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
                if ((b = *offset) == CharT(']'))
                    throw myregex::regex_error("termination max incurret", range.position(offset), range, 9);
                range.next();
                if (b == CharT('\\'))
                    b = basic_build<CharT>::_S_parser_code_caracter(range);
                else
                    range.next();
                if (b <= a)
                    throw myregex::regex_error("range whit min greather than max in this class", position_start_range, range.position(offset), range, 10);
                for (size_t i = a; i <= b; i++)
                    alphabet.insert(CharT(i));
            }
        }
        // Si la clase esta invertida se agregan todos los caracteres que no fueron detectados dentro del su definicion
        if (alphabet.empty())
            throw myregex::regex_error("this class is empty", position_start, range.position(range.peak()), range, 8);
        if (inverter)
        {
            // Se recorre todos los caracteres
            for (size_t i = 0; i < size_t(std::pow(256, sizeof(CharT))); i++)
            {
                // Si el caracter no se encuentra dentro del conjunto
                if (alphabet.count(CharT(i)) < 1)
                {
                    // Se agrega la transicion y luego el caracter al alfabeto
                    transitions.push_back(CharT(i));
                    nfa.Q_dictionary.insert(CharT(i));
                }
            }
        }
        else
        {
            // Si no solo se agregan las transiciones y caracteres al alfabeto definidos en la clase
            for (auto &&i : alphabet)
            {
                transitions.push_back(i);
                nfa.Q_dictionary.insert(i);
            }
        }
        // Retorna el estado inicial y las transiciones
        return {qA, transitions};
    }

    template <typename CharT>
    size_t basic_build<CharT>::_S_build_nfa_parser_regular_expresions_basic(basic_string_range<CharT> &range, basic_nfa<CharT> &nfa, bool isGroup, size_t id)
    {
        // Dado el rango de caracteres
        while (range.peak() < range.end())
        {
            // Obtenemos el caracter actual
            basic_build<CharT>::_M_begin_transitions _T_qAtr;
            CharT c = *range.peak();
            if (c == CharT('|') or c == CharT(')'))
                break;
            // Y pasamos al siguiente
            range.next();
            // Segun el caracter que sea
            switch (c)
            {
            case CharT('('):
            {
                size_t position_start = range.position(range.peak(), -1ULL);
                // Genera un grupo de reglas, se utiliza la recursion de la fincion recursiva
                _T_qAtr = basic_build<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, true, id);
                // Si no termina en la posicion correcta, error
                if (range.peak() >= range.end())
                    throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
                if (*range.peak() != CharT(')'))
                    throw myregex::regex_error("bad termination in group", range.position(range.peak()), range, 11);
                // Saltamos el final del grupo
                range.next();
                // Y agregamos su respectivo cualificador
                basic_build<CharT>::_S_parser_cualifiquer(range, nfa, _T_qAtr, true);
                break;
            }
            case CharT('['):
            {
                // Genera un estado para clases de caracteres
                _T_qAtr = basic_build<CharT>::_S_parser_nfa_parser_class_expresions(range, nfa);
                // Si no termina en la posicion correcta, error
                if (range.peak() >= range.end())
                    throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
                if (*range.peak() != CharT(']'))
                    throw myregex::regex_error("bad termination in class", range.position(range.peak()), range, 12);
                // Saltamos el final del grupo
                range.next();
                // Y agregamos su respectivo cualificador
                basic_build<CharT>::_S_parser_cualifiquer(range, nfa, _T_qAtr, false);
                break;
            }
            case CharT('*'):
            case CharT('+'):
            case CharT('?'):
            case CharT('{'):
                throw myregex::regex_error("cualifiquer unespect in this operation", range.position(range.peak(), -1ULL), range, 15);
            case CharT('\\'):
                c = basic_build<CharT>::_S_parser_code_caracter(range);
            default:
            {
                // Simplemente creamos el estado y agregamos la trsnicion
                size_t qA = nfa.Q_nfa.size();
                nfa.Q_dictionary.insert(c);
                nfa.Q_nfa.push_back(qA);
                // Y agregamos su respectivo cualificador
                basic_build<CharT>::_S_parser_cualifiquer(range, nfa, {qA, {size_t(c)}}, false);
                break;
            }
            }
        }
        size_t qEnd = nfa.Q_nfa.size();
        nfa.Q_nfa.push_back(qEnd);
        return qEnd;
    }

    template <typename CharT>
    typename basic_build<CharT>::_M_begin_transitions basic_build<CharT>::_S_build_nfa_parser_or_expresions(basic_string_range<CharT> &range, basic_nfa<CharT> &nfa, bool isGroup, size_t id)
    {
        // Preguntamos si el inicio es incorrect y lanzamos un error si es asi
        if (range.peak() == range.end())
            throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
        if (*(range.peak()) == CharT('|'))
            throw myregex::regex_error("bad start or operation", range.position(range.peak()), range, 14);
        std::vector<size_t> ends = {};
        // Obtenemos el estado final y lo agregamos
        size_t q0 = nfa.Q_nfa.size();
        nfa.Q_nfa.push_back(q0);
        // Obtenemos el primer estado
        // Agregamos una ∊-transition hacia ese estado
        nfa.Q_transitions[{q0, -1ULL}].push_back(nfa.Q_nfa.size());
        // Guardamos el estado final del resultado de la cadena
        if (range.peak() == range.end() || *range.peak() == CharT(')'))
            throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
        ends.push_back(basic_build<CharT>::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup, id));
        // Repite la condicion en caso de ser una operacion or
        while (range.peak() < range.end() && *(range.peak()) == CharT('|'))
        {
            range.next();
            if (range.peak() == range.end() || *range.peak() == CharT(')'))
                throw myregex::regex_error("end termination for regular expresion", range.position(range.end(), -1ULL), range, 0);
            // Agregamos una ∊-transition hacia ese estado
            nfa.Q_transitions[{q0, -1ULL}].push_back(nfa.Q_nfa.size());
            // Guardamos el estado final del resultado de la cadena
            ends.push_back(basic_build<CharT>::_S_build_nfa_parser_regular_expresions_basic(range, nfa, isGroup, id));
            // Si se detuvo en una tuberia volver a realizar el procedimiento
        }
        // Para cada estado final se le agregara una ∊-transition al estado de union de la tuberia
        for (auto &&i : ends)
            nfa.Q_transitions[{i, -1ULL}].push_back(nfa.Q_nfa.size());
        // Por ultimo generamos correctamente el estado
        if (!isGroup)
        {
            nfa.Q_nfa.push_back(nfa.Q_nfa.size());
            nfa.F_nfa.emplace(nfa.Q_nfa.size() - 1, id);
        }
        return {q0, {-1ULL}};
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////basic_dfa////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    template <typename CharT>
    typename myregex::basic_dfa<CharT>::States basic_build<CharT>::_S_elipson_cloursers(typename basic_dfa<CharT>::States states, const basic_nfa<CharT> &nfa)
    {
        std::stack<size_t> stack_status;
        typename basic_dfa<CharT>::States clourser = states;
        for (auto &&state : states)
            stack_status.push(state);
        while (!stack_status.empty())
        {
            size_t index = stack_status.top();
            stack_status.pop();
            typename basic_nfa<CharT>::Transitions::const_iterator iterator = nfa.Q_transitions.begin();
            if ((iterator = nfa.Q_transitions.find({index, -1ULL})) == nfa.Q_transitions.end())
                continue;
            for (auto &&state : iterator->second)
            {
                stack_status.push(state);
                clourser.insert(state);
            }
        }
        return clourser;
    }

    template <typename CharT>
    typename myregex::basic_dfa<CharT>::States basic_build<CharT>::_S_move(typename basic_dfa<CharT>::States states, CharT a, const basic_nfa<CharT> &nfa)
    {
        typename basic_dfa<CharT>::States result = {};
        for (auto &&state : states)
        {
            typename basic_nfa<CharT>::Transitions::const_iterator iterator = nfa.Q_transitions.begin();
            if ((iterator = nfa.Q_transitions.find({state, a})) != nfa.Q_transitions.end())
                result.insert(iterator->second[0]);
        }
        return result;
    }

    template <typename CharT>
    myregex::basic_dfa<CharT> basic_build<CharT>::convert_nfa_to_dfa(const basic_nfa<CharT> &nfa)
    {
        basic_dfa<CharT> dfa = basic_dfa<CharT>();
        typename basic_dfa<CharT>::States q0 = basic_build<CharT>::_S_elipson_cloursers({0}, nfa);
        std::map<typename basic_dfa<CharT>::States, size_t> status_map = {{q0, 0}};
        std::queue<typename basic_dfa<CharT>::States> queue_status;
        queue_status.push(q0);
        dfa.begin_Q_dfa.insert(0);
        dfa.Q_dictionary = nfa.Q_dictionary;
        dfa.Q_dfa.push_back(q0);

        while (!queue_status.empty())
        {
            typename basic_dfa<CharT>::States index = queue_status.front();
            queue_status.pop();
            size_t actual = status_map[index];

            for (auto &&state : index)
            {
                auto fiterator = nfa.F_nfa.find(state);
                if (fiterator != nfa.F_nfa.end())
                {
                    dfa.F_dfa.emplace(actual, fiterator->second);
                    break;
                }
            }

            for (auto &&caraceter : nfa.Q_dictionary)
            {
                typename basic_dfa<CharT>::States newindex = basic_build<CharT>::_S_move_elipson_cloursers(index, caraceter, nfa);
                if (newindex.empty())
                    continue;
                if (status_map.count(newindex) < 1)
                {
                    status_map[newindex] = dfa.Q_dfa.size();
                    dfa.Q_dfa.push_back(newindex);
                    queue_status.push(newindex);
                }
                dfa.Q_transitions[{actual, caraceter}] = status_map[newindex];
            }
        }
        return dfa;
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////TableDFA//////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    template <typename CharT>
    myregex::TableDFA basic_build<CharT>::convert_dfa_to_table(const basic_dfa<CharT> &dfa)
    {
        size_t sizeAlphabet = std::pow(256, sizeof(CharT));
        TableDFA table = TableDFA(dfa.Q_dfa.size(), sizeAlphabet, dfa.F_dfa);
        for (size_t state = 0; state < dfa.Q_dfa.size(); state++)
        {
            for (size_t letter = 0; letter < sizeAlphabet; letter++)
            {
                auto transition = dfa.Q_transitions.find({state, CharT(letter)});
                if (transition != dfa.Q_transitions.end())
                    table.Q_transitions[state][letter] = transition->second;
            }
        }
        return table;
    }
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////Regex////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    template <typename CharT>
    myregex::basic_nfa<CharT> basic_build<CharT>::build_nfa(const std::__cxx11::basic_string<CharT> &str)
    {
        basic_string_range<CharT> range = str;
        basic_nfa<CharT> nfa;
        nfa.begin_Q_nfa.push_back(basic_build<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, false, 0).first);
        return nfa;
    }

    template <typename CharT>
    myregex::basic_nfa<CharT> basic_build<CharT>::build_nfa(const std::initializer_list<std::pair<size_t, std::__cxx11::basic_string<CharT>>> &list)
    {
        basic_nfa<CharT> nfa;
        nfa.Q_nfa.push_back(0);
        nfa.begin_Q_nfa.push_back(0);
        for (auto &&expresion : list)
        {
            basic_string_range<CharT> range = expresion.second;
            nfa.Q_transitions[{0, -1ULL}].push_back(basic_build<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, false, expresion.first).first);
        }
        return nfa;
    }

#if TESTCODE
    template <typename CharT>
    basic_nfa<CharT> basic_build<CharT>::build_nfa(const CharT *argv[], size_t size)
    {
        basic_nfa<CharT> nfa;
        nfa.Q_nfa.push_back(0);
        nfa.begin_Q_nfa.push_back(0);
        for (size_t i = 1; i < size; i++)
        {
            basic_string_range<CharT> range = std::string(argv[i]);
            nfa.Q_transitions[{0, -1ULL}].push_back(basic_build<CharT>::_S_build_nfa_parser_or_expresions(range, nfa, false).first);
        }
        return nfa;
    }
#endif

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////caption///////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////

    template <typename CharT, typename AllocatorRegexT>
    bool basic_regex<CharT, AllocatorRegexT>::verification(const std::__cxx11::basic_string<CharT> &str, const AllocatorRegexT &allocator_regex)
    {
        size_t status = 0;
        size_t acceptance_status = -1ULL;
        size_t id = -1ULL;
        std::__cxx11::basic_string<char>::const_iterator iterator_caption = str.begin();
        while (iterator_caption < str.end())
        {
            char letter = *iterator_caption;
            if constexpr (std::is_same_v<AllocatorRegexT, basic_dfa<CharT>>)
            {
                auto transition = allocator_regex.transitions().find({status, letter});
                if (transition == allocator_regex.transitions().end())
                    return false;
                status = transition->second;
            }
            else if constexpr (std::is_same_v<AllocatorRegexT, TableDFA>)
                if ((status = allocator_regex.transitions()[status][letter]) == -1ULL)
                    return false; // No hay transición, rechazar
            iterator_caption++;
        }
        return iterator_caption == str.end() && allocator_regex.accepted_status().count(status) > 0;
    }
    template <typename CharT, typename AllocatorRegexT>
    bool basic_regex<CharT, AllocatorRegexT>::verification(basic_string_range<CharT> &range, const AllocatorRegexT &allocator_regex)
    {
        size_t status = 0;
        size_t acceptance_status = -1ULL;
        size_t id = -1ULL;
        while (range.peak() < range.end())
        {
            char letter = *range.peak();
            if constexpr (std::is_same_v<AllocatorRegexT, basic_dfa<CharT>>)
            {
                auto transition = allocator_regex.transitions.find({status, letter});
                if (transition == allocator_regex.transitions.end())
                    return false;
                status = transition->second;
            }
            else if constexpr (std::is_same_v<AllocatorRegexT, TableDFA>)
                if ((status = allocator_regex.transitions[status][letter]) == -1ULL)
                    return false; // No hay transición, rechazar
            range.peak()++;
        }
        return range.peak() == range.end() && allocator_regex.accepted_status().count(status) > 0;
    }
    template <typename CharT, typename AllocatorRegexT>
    template <myregex::constants::match_options option>
    myregex::caption<CharT> basic_regex<CharT, AllocatorRegexT>::_S_match(const std::__cxx11::basic_string<CharT> &str, const AllocatorRegexT &allocator_regex)
    {
        size_t status = 0;
        size_t acceptance_status = -1ULL;
        size_t id = -1ULL;
        std::__cxx11::basic_string<char>::const_iterator end_caption = std::__cxx11::basic_string<char>::const_iterator();
        for (std::__cxx11::basic_string<char>::const_iterator iterator_caption = str.begin(); iterator_caption < str.end(); iterator_caption++)
        {
            char letter = *iterator_caption;
            if constexpr (std::is_same_v<AllocatorRegexT, basic_dfa<CharT>>)
            {
                auto transition = allocator_regex.transitions().find({status, letter});
                if (transition == allocator_regex.transitions().end())
                    break; // No hay transición, rechazar
                status = transition->second;
            }
            else if constexpr (std::is_same_v<AllocatorRegexT, TableDFA>)
            {
                size_t next_state = allocator_regex.transitions()[status][letter];
                if (next_state == -1ULL)
                    break; // No hay transición, rechazar
                status = next_state;
            }
            if (status == acceptance_status)
                end_caption = iterator_caption + 1;
            else if (allocator_regex.accepted_status().count(status) > 0)
            {
                acceptance_status = status;
                if constexpr (option == myregex::constants::match_options::_S_first_sequence)
                    return {iterator_caption + 1, allocator_regex.F_dfa.at(acceptance_status)};
                else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
                {
                    end_caption = iterator_caption + 1;
                    id = allocator_regex.accepted_status().at(acceptance_status);
                }
            }
        }
        if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
            return myregex::caption<CharT>(end_caption, id);
        else if constexpr (option == myregex::constants::match_options::_S_first_sequence)
            return myregex::caption<CharT>(nullptr, -1ULL);
    }

    template <typename CharT, typename AllocatorRegexT>
    template <myregex::constants::match_options option>
    myregex::caption<CharT> basic_regex<CharT, AllocatorRegexT>::_S_match(basic_string_range<CharT> &range, const AllocatorRegexT &allocator_regex)
    {
        size_t status = 0;
        size_t acceptance_status = -1ULL;
        size_t id = -1ULL;
        // typename basic_string_range<CharT>::iterator iterator_caption;
        typename basic_string_range<CharT>::iterator end_caption = std::__cxx11::basic_string<char>::const_iterator();
        while (range.peak() < range.end())
        {
            char letter = *range.peak();
            if constexpr (std::is_same_v<AllocatorRegexT, basic_dfa<CharT>>)
            {
                auto transition = allocator_regex.transitions().find({status, letter});
                if (transition == allocator_regex.transitions().end())
                    break; // No hay transición, rechazar
                status = transition->second;
            }
            else if constexpr (std::is_same_v<AllocatorRegexT, TableDFA>)
            {
                size_t next_state = allocator_regex.transitions()[status][letter];
                if (next_state == -1ULL)
                    break; // No hay transición, rechazar
                status = next_state;
            }
            if (status == acceptance_status)
                end_caption = range.peak() + 1;
            else if (allocator_regex.accepted_status().count(status) > 0)
            {
                acceptance_status = status;
                if constexpr (option == myregex::constants::match_options::_S_first_sequence)
                    return {range.peak() + 1, allocator_regex.accepted_status().at(acceptance_status)};
                else if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
                {
                    end_caption = range.peak() + 1;
                    id = allocator_regex.accepted_status().at(acceptance_status);
                }
            }
            range.next();
        }
        if constexpr (option == myregex::constants::match_options::_S_maximun_sequence)
            return myregex::caption<CharT>(end_caption, id);
        else if constexpr (option == myregex::constants::match_options::_S_first_sequence)
            return myregex::caption<CharT>(nullptr, -1ULL);
    }
} // namespace myregex

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#endif