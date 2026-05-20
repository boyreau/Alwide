;; From nvim-treesitter/nvim-treesitter
[
  (code_span)
  (link_title)
] @comment

(emphasis) @string.italic

(strong_emphasis) @string.strong

[
  (link_destination)
  (uri_autolink)
] @string

[
  (link_label)
  (link_text)
  (image_description)
] @string

[
  (backslash_escape)
  (hard_line_break)
] @string.strong

[
  (emphasis_delimiter)
  (code_span_delimiter)
] @keyword


(image ["!" "[" "]" "(" ")"] @number)
(inline_link ["[" "]" "(" ")"] @number)
(shortcut_link ["[" "]"] @number)

; NOTE: extension not enabled by default
; (wiki_link ["[" "|" "]"] @punctuation.delimiter)

((html_tag) @injection.content (#set! injection.language "html"))
((latex_block) @injection.content (#set! injection.language "latex"))
