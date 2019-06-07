
#pragma once

#include <tuple>
#include <memory>

namespace TemplateHelpers
{
  // https://stackoverflow.com/questions/6953969/can-static-assert-check-if-a-type-is-a-vector
  // Allows to determine if typename is a specific template instantiation
  template<typename T, template<typename...> class Tmpl>
  struct is_instance_of_a_given_class_template : std::false_type {};

  template<template<typename...> class Tmpl, typename ...Args>
  struct is_instance_of_a_given_class_template< Tmpl<Args...>, Tmpl > : std::true_type {};
} // namespace TemplateHelpers
