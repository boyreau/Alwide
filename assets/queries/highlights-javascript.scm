; Variables
;----------

(identifier) @variable

; Properties
;-----------

(property_identifier) @property

; Function and method definitions
;--------------------------------

(function_expression
  name: (identifier) @function)
(function_declaration
  name: (identifier) @function)
(method_definition
  name: (property_identifier) @function.method)

(pair
  key: (property_identifier) @function.method
  value: [(function_expression) (arrow_function)])

(assignment_expression
  left: (member_expression
          property: (property_identifier) @function.method)
  right: [(function_expression) (arrow_function)])

(variable_declarator
  name: (identifier) @function
  value: [(function_expression) (arrow_function)])

(assignment_expression
  left: (identifier) @function
  right: [(function_expression) (arrow_function)])

; Function and method calls
;--------------------------

(call_expression
  function: (identifier) @function)

(call_expression
  function: (member_expression
              property: (property_identifier) @method))

; Special identifiers
;--------------------

((identifier) @constructor
  (#match? @constructor "^[A-Z]"))

([
   (identifier)
   (shorthand_property_identifier)
   (shorthand_property_identifier_pattern)
   ] @constant
  (#match? @constant "^[A-Z_][A-Z\\d_]+$"))

((identifier) @variable
  (#match? @variable "^(arguments|module|console|window|document)$"))

((identifier) @function
  (#eq? @function "require"))

; Literals
;---------

(this) @variable
(super) @variable

[
  (true)
  (false)
  (null)
  (undefined)
  ] @constant

(comment) @comment

[
  (string)
  (template_string)
  ] @string

(regex) @string
(number) @number

; Tokens
;-------

[
  ";"
  (optional_chain)
  "."
  ","
  ] @punctuation.delimiter

[
  "-"
  "--"
  "-="
  "+"
  "++"
  "+="
  "*"
  "*="
  "**"
  "**="
  "/"
  "/="
  "%"
  "%="
  "<"
  "<="
  "<<"
  "<<="
  "="
  "=="
  "==="
  "!"
  "!="
  "!=="
  "=>"
  ">"
  ">="
  ">>"
  ">>="
  ">>>"
  ">>>="
  "~"
  "^"
  "&"
  "|"
  "^="
  "&="
  "|="
  "&&"
  "||"
  "??"
  "&&="
  "||="
  "??="
  ] @operator

[
  "("
  ")"
  "["
  "]"
  "{"
  "}"
  ] @punctuation.bracket

(template_substitution
  "${" @punctuation.special
  "}" @punctuation.special) @embedded

[
  "as"
  "async"
  "await"
  "break"
  "case"
  "catch"
  "class"
  "const"
  "continue"
  "debugger"
  "default"
  "delete"
  "do"
  "else"
  "export"
  "extends"
  "finally"
  "for"
  "from"
  "function"
  "get"
  "if"
  "import"
  "in"
  "instanceof"
  "let"
  "new"
  "of"
  "return"
  "set"
  "static"
  "switch"
  "target"
  "throw"
  "try"
  "typeof"
  "var"
  "void"
  "while"
  "with"
  "yield"
  ] @keyword


; Parse the contents of tagged template literals using
; a language inferred from the tag.

(call_expression
  function: [
              (identifier) @injection.language
              (member_expression
                property: (property_identifier) @injection.language)
              ]
  arguments: (template_string (string_fragment) @injection.content)
  (#set! injection.combined)
  (#set! injection.include-children))


; Parse regex syntax within regex literals

((regex_pattern) @injection.content
  (#set! injection.language "regex"))

; Parse JSDoc annotations in comments

((comment) @injection.content
  (#set! injection.language "jsdoc"))

; Parse Ember/Glimmer/Handlebars/HTMLBars/etc. template literals
; e.g.: await render(hbs`<SomeComponent />`)
(call_expression
  function: ((identifier) @_name
              (#eq? @_name "hbs"))
  arguments: ((template_string) @glimmer
               (#offset! @glimmer 0 1 0 -1)))
