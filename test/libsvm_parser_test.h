/**
 * @file libsvm_parser_test.h
 * @author Sean Massung
 */

#ifndef _META_LIBSVM_PARSER_TEST_H_
#define _META_LIBSVM_PARSER_TEST_H_

#include "unit_test.h"
#include "io/libsvm_parser.h"

namespace meta {
namespace testing {

namespace {
void label() {
    auto same = {"a 12:2e-3 15:4.01 99:22 122:1",
                 "a  12:2e-3 15:4.01   99:22 122:1  "};
    for (auto& text : same) {
        ASSERT(io::libsvm_parser::label(text) == class_label{"a"});
        auto counts = io::libsvm_parser::counts(text);
        ASSERT(counts.size() == 4);
        ASSERT(counts[0].first == 12);
        ASSERT(counts[0].second == 2e-3);
        ASSERT(counts[1].first == 15);
        ASSERT(counts[1].second == 4.01);
        ASSERT(counts[2].first == 99);
        ASSERT(counts[2].second == 22.0);
        ASSERT(counts[3].first == 122);
        ASSERT(counts[3].second == 1.0);
    }
}

void no_label() {
    auto same = {"1:2e-3 2:4.01 3:22 13:1", "1:2e-3 2:4.01   3:22 13:1  "};
    for (auto& text : same) {
        auto counts = io::libsvm_parser::counts(text, false);
        ASSERT(counts.size() == 4);
        ASSERT(counts[0].first == 1);
        ASSERT(counts[0].second == 2e-3);
        ASSERT(counts[1].first == 2);
        ASSERT(counts[1].second == 4.01);
        ASSERT(counts[2].first == 3);
        ASSERT(counts[2].second == 22.0);
        ASSERT(counts[3].first == 13);
        ASSERT(counts[3].second == 1.0);
    }
}

void bad_label() {
    auto bad = {"thisdatahasnospaces", "thishasspacelast ", " missing"};
    for (auto& text : bad) {
        try {
            class_label lbl = io::libsvm_parser::label(text);
            FAIL("An exception was not thrown on invalid input");
        }
        catch (io::libsvm_parser::libsvm_parser_exception ex) {
            // nothing, we want an exception!
        }
    }
}

void bad_counts() {
    auto bad = {"",       "lis:uvfs agi uy:", "label :9 5:5",   "label 9: 5:5",
                "label ", "label : :::",      "label 9:9 9::9", "label 5:"};
    for (auto& text : bad) {
        try {
            auto counts = io::libsvm_parser::counts(text);
            std::cout << text << std::endl;
            FAIL("An exception was not thrown on invalid input");
        }
        catch (io::libsvm_parser::libsvm_parser_exception ex) {
            // nothing, we want an exception!
        }
    }
}
}

void libsvm_parser_tests() {
    testing::run_test("libsvm-parser-label", [&]() { label(); });
    testing::run_test("libsvm-parser-no-label", [&]() { no_label(); });
    testing::run_test("libsvm-parser-bad-label", [&]() { bad_label(); });
    testing::run_test("libsvm-parser-bad-counts", [&]() { bad_counts(); });
}
}
}

#endif