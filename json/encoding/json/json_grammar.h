#pragma once
#include "./json_number_grammar.h"
#include "./json_string_grammar.h"

namespace json {

    //--------------------------------------------------------------------------

    peg_grammar(json_grammar)
    {
        peg_rules(
            colon,
            comma,
            ws,
            null_keyword,
            false_keyword,
            true_keyword,
            number,
            string,
            value,
            element,
            elements,
            array_enter,
            array_leave,
            array,
            property_name,
            property_value,
            property,
            properties,
            object_enter,
            object_leave,
            table,
            document
        );

        // utility \\...........................................................

        peg_rule(
            colon,
            peg::ascii_grammar::colon
        );

        peg_rule(
            comma,
            peg::ascii_grammar::comma
        );

        peg_rule(ws, star<peg::ascii_grammar::whitespace>);

        peg_warning(
            warning_trailing_comma,
            "trailing comma",
            peg::ascii_grammar::comma
        );
        
        // values \\............................................................
        
        peg_rule(null_keyword,  peg_literal("null"));
        peg_rule(false_keyword, peg_literal("false"));
        peg_rule(true_keyword,  peg_literal("true"));

        peg_rule(number, json_number_grammar::number);
        peg_rule(string, json_string_grammar::string);

        peg_rule(
            value,
            choice<
                null_keyword,
                false_keyword,
                true_keyword,
                number,
                string,
                array,
                table
            >
        );
        
        // arrays \\............................................................

        peg_rule(
            element,
            value
        );

        peg_rule(
            elements,
            sequence<
                ws,
                element,
                star<sequence<ws, comma, ws, element>>,
                ws,
                opt<warning_trailing_comma>
            >
        );
        
        peg_rule(
            array_enter,
            peg::ascii_grammar::bracket_left
        );

        peg_error(
            error_incomplete_array,
            "incomplete array",
            not_predicate<peg::ascii_grammar::bracket_right>
        );

        peg_rule(
            array_leave,
            choice<
                peg::ascii_grammar::bracket_right,
                error_incomplete_array
            >
        );
        
        peg_rule(
            array,
            sequence<
                array_enter,
                opt<elements>,
                array_leave
            >
        );
        
        // objects \\...........................................................

        peg_rule(
            property_name,
            json_string_grammar::string
        );

        peg_rule(
            property_value,
            value
        );

        peg_rule(
            property,
            sequence<
                property_name,
                ws,
                colon,
                ws,
                property_value,
                ws>
        );

        peg_rule(
            properties,
            sequence<
                ws,
                property,
                star<sequence<ws, comma, ws, property>>,
                ws,
                opt<warning_trailing_comma>
            >
        );
        
        peg_rule(
            object_enter,
            peg::ascii_grammar::brace_left
        );
        
        peg_error(
            error_incomplete_object,
            "incomplete object",
            not_predicate<peg::ascii_grammar::brace_right>
        );
        
        peg_rule(
            object_leave,
            choice<
                peg::ascii_grammar::brace_right,
                error_incomplete_object
            >
        );

        peg_rule(
            table,
            sequence<
                object_enter,
                opt<properties>,
                object_leave
            >
        );
        
        // document \\..........................................................
        
        peg_rule(
            document,
            sequence<
                ws,
                choice<array, table>,
                ws
            >
        );

    };

    //--------------------------------------------------------------------------

} // namespace json
