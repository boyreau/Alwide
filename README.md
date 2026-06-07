# Alwide — A LightWeight IDE

> **"Sublime Text" in the terminal.** Alwide is a strong and powerful TUI IDE which is user friendly. It aims to give the same user experience of graphical IDE but in terminal. Though a simple SSH connection but want an easy tool ? Or you need something lighter than VSC or Jetbrain suite ? Vim may be a bit rought sometimes to do simple actions ?


[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
[![C](https://img.shields.io/badge/language-C-orange.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Tree-Sitter](https://img.shields.io/badge/highlighting-Tree--Sitter-green.svg)](https://tree-sitter.github.io/tree-sitter/)
[![LSP](https://img.shields.io/badge/intelligence-LSP-yellow.svg)](https://microsoft.github.io/language-server-protocol/)

<p align="center">
  <img width="700" alt="Alwide Screenshot" src="https://github.com/user-attachments/assets/f73b961f-0fc2-4c0a-81b6-3e391a96031b" />
</p>

https://github.com/user-attachments/assets/c6f40db1-bc5e-4c90-88a5-c7e5a5c72059



---

## The Modern Terminal Experience

Alwide is designed for users who want more than `nano` but find `vim` or `emacs` too rusty. It’s the perfect companion for everything from quick config file, bash/python scripts to bigger projects.

- **Zero Learning Curve:** Full mouse support means you can click, drag-select, and scroll just like a desktop app. It’s the friendliest way to work in a terminal.
- **Sublime-Inspired:** We aim to bring the speed and "vibe" of Sublime Text into the terminal, extended with powerful modern features like LSP.
- **Fast & Lightweight:** Written in pure C. It starts in milliseconds, a single binary around 10Mb.
- **Advanced Features:** Built-in **Treesitter** for hight quality highlight and **LSP** support gives you VS Code-like intelligence (completions, hover docs, goto definition) in your terminal.
- **Open & Close:** Alwide provide persistent experience for everyfiles. Quit and Reopen as nothing happens (reopen previous files, cursor position, workspace setup). Undo/Redo available when reopening the file. Copy in Alwide, paste in your terminal.
- **Easy and clear code base:** Want to understand how it works ? Add a feature ? Clone, Read, Write, Compile. Can be used for education/curiosity.


### Supported Languages

Large built-int supported language. Your preferred language is missing ? You can add it in few minutes, clone and ask your best llm friend to add it and it will work !

**C/C++, Python, Java, Go, Rust, JavaScript/TypeScript, Dart, Lua, Bash, HTML, CSS, JSON, Markdown, VHDL, Assembly, and more.**

---

## Getting Started

For now to use al you have to compile it !

### 1. Clone the Source
```bash
git clone --recursive https://github.com/arnauda-gh/Alwide.git
cd Alwide
```

### 2. Install Dependencies
**Ubuntu / Debian:**
```bash
# Core build tools
sudo apt install make clang libncursesw5-dev rustup

# Ensure Clang 18+ and Rust are ready
rustup update stable
# (If clang < 18) Visit https://apt.llvm.org/ for the latest version
```

**Install Tree-Sitter C API:**
```bash
sudo make -C lib/tree-sitter/ install
```

### 3. Build & Install
```bash
make
# manage the config setup
make install  # Note: No sudo needed! It handles ~/.config/al/ for you.
```

Now launch it:
```bash
al  # Open current directory
```

---

## Essential Shortcuts

| Shortcut | Action |
| :--- | :--- |
| `Ctrl + S` | **Save** & Auto-format |
| `Ctrl + Q` | **Quit** Alwide |
| `Ctrl + E` | **File Explorer** toggle |
| `Ctrl + O` | **Open File** toggle |
| `Ctrl + Space` | **Completion Popup** toggle |
| `Ctrl + W` | **Close** Current Tab |
| `Ctrl + /` | **Toggle Comment** |
| `Ctrl + R` | **Format** Code (LSP) |
| `Ctrl + Z/Y` | **Undo / Redo** |
| `Shift + Arrows` | **Select Text** |

---

## Configuration

Your preferences live in `~/.config/al/`:
- `config.json`: Themes, tab defaults, and global settings.
- `languages-features.json`: Custom LSP commands and per-language tweaks.

---

## Contributing

For now Alwide need to be tested on differents mouse drivers, and distributions. Any help will be usefull !

Check out our [**Technical Documentation**](doc/architecture.md) to dive into the internals.

---

## License

Distributed under the **MIT License**. See `LICENSE` for more information.


