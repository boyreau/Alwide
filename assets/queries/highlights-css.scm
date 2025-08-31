(comment) @comment

(tag_name) @function
(nesting_selector) @function
(universal_selector) @function

"~" @operator
">" @operator
"+" @operator
"-" @operator
"*" @operator
"/" @operator
"=" @operator
"^=" @operator
"|=" @operator
"~=" @operator
"$=" @operator
"*=" @operator

"and" @operator
"or" @operator
"not" @operator
"only" @operator

(attribute_selector (plain_value) @string)
(pseudo_element_selector (tag_name) @type)
(pseudo_class_selector (class_name) @type)

(class_name) @type
(id_name) @type
(namespace_name) @property
(property_name) @type
(feature_name) @property

(attribute_name) @attribute

(function_name) @function

((property_name) @variable
 (#match? @variable "^--"))
((plain_value) @variable
 (#match? @variable "^--"))

"@media" @keyword
"@import" @keyword
"@charset" @keyword
"@namespace" @keyword
"@supports" @keyword
"@keyframes" @keyword
(at_keyword) @keyword
(to) @keyword
(from) @keyword
(important) @keyword

(string_value) @string
(color_value) @string.special

(integer_value) @number
(float_value) @number
(unit) @type

"#" @punctuation.delimiter
"," @punctuation.delimiter
":" @punctuation.delimiter
