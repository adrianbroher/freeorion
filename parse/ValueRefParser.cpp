#include "ValueRefParser.h"

#include "ConditionParserImpl.h"
#include "MovableEnvelope.h"

#include <boost/spirit/include/phoenix.hpp>


#define DEBUG_VALUEREF_PARSERS 0

// These are just here to satisfy the requirements of boost::spirit::qi::debug(<rule>).
#if DEBUG_VALUEREF_PARSERS
namespace std {
    inline ostream& operator<<(ostream& os, const std::vector<boost::variant<ValueRef::OpType, value_ref_payload<int>>>&) { return os; }
    inline ostream& operator<<(ostream& os, const std::vector<boost::variant<ValueRef::OpType, value_ref_payload<double>>>&) { return os; }
}
#endif

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

namespace parse { namespace detail {

    const reference_token_rule variable_scope(const parse::lexer& tok) {
        qi::_val_type _val;

        reference_token_rule variable_scope;
        variable_scope
            =   tok.Source_         [ _val = ValueRef::SOURCE_REFERENCE ]
            |   tok.Target_         [ _val = ValueRef::EFFECT_TARGET_REFERENCE ]
            |   tok.LocalCandidate_ [ _val = ValueRef::CONDITION_LOCAL_CANDIDATE_REFERENCE ]
            |   tok.RootCandidate_  [ _val = ValueRef::CONDITION_ROOT_CANDIDATE_REFERENCE ]
            ;

        variable_scope.name("Source, Target, LocalCandidate, or RootCandidate");

        return variable_scope;
    }

    statistic_enum_grammar::statistic_enum_grammar(const parse::lexer& tok) :
        statistic_enum_grammar::base_type(rule, "statistic_enum_grammar")
    {
        qi::_val_type _val;

        rule
            =   tok.CountUnique_    [ _val = ValueRef::UNIQUE_COUNT ]
            |   tok.Count_          [ _val = ValueRef::COUNT ]
            |   tok.If_             [ _val = ValueRef::IF ]
            |   tok.Sum_            [ _val = ValueRef::SUM ]
            |   tok.Mean_           [ _val = ValueRef::MEAN ]
            |   tok.RMS_            [ _val = ValueRef::RMS ]
            |   tok.Mode_           [ _val = ValueRef::MODE ]
            |   tok.Max_            [ _val = ValueRef::MAX ]
            |   tok.Min_            [ _val = ValueRef::MIN ]
            |   tok.Spread_         [ _val = ValueRef::SPREAD ]
            |   tok.StDev_          [ _val = ValueRef::STDEV ]
            |   tok.Product_        [ _val = ValueRef::PRODUCT ]
            ;
    }

    const name_token_rule container_type(const parse::lexer& tok) {
        name_token_rule container_type;
        container_type
            =   tok.Planet_
            |   tok.System_
            |   tok.Fleet_
            ;

        container_type.name("Planet, System, or Fleet");

        return container_type;
    }

    template <typename T>
    simple_variable_rules<T>::simple_variable_rules(
        const std::string& type_name, const parse::lexer& tok)
    {
        using phoenix::new_;

        qi::_1_type _1;
        qi::_val_type _val;
        qi::lit_type lit;
        const phoenix::function<construct_movable> construct_movable_;

        free_variable
            =  (tok.Value_ >> !lit('('))
                [ _val = construct_movable_(new_<ValueRef::Variable<T>>(
                    ValueRef::EFFECT_TARGET_VALUE_REFERENCE)) ]
            |   free_variable_name
                [ _val = construct_movable_(new_<ValueRef::Variable<T>>(
                    ValueRef::NON_OBJECT_REFERENCE, _1)) ]
            ;

        simple
            =   constant
            |   bound_variable
            |   free_variable
            ;

        variable_scope_rule = variable_scope(tok);
        container_type_rule = container_type(tok);
        initialize_bound_variable_parser<T>(
            bound_variable, unwrapped_bound_variable,
            value_wrapped_bound_variable, bound_variable_name,
            variable_scope_rule, container_type_rule, tok);

#if DEBUG_VALUEREF_PARSERS
        debug(bound_variable_name);
        debug(free_variable_name);
        debug(constant);
        debug(free_variable);
        debug(bound_variable);
        debug(simple);
#endif

        unwrapped_bound_variable.name(type_name + " unwrapped bound variable name");
        value_wrapped_bound_variable.name(type_name + " value-wrapped bound variable name");
        bound_variable_name.name(type_name + " bound variable name");
        free_variable_name.name(type_name + " free variable name");
        constant.name(type_name + " constant");
        free_variable.name(type_name + " free variable");
        bound_variable.name(type_name + " bound variable");
        simple.name(type_name + " simple variable expression");
    }

    // Explicit instantiation to prevent costly recompilation in multiple units
    template simple_variable_rules<int>::simple_variable_rules(
        const std::string& type_name, const parse::lexer& tok);
    template simple_variable_rules<double>::simple_variable_rules(
        const std::string& type_name, const parse::lexer& tok);

} }
