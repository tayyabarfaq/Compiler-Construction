# 🧠 Compiler-Construction (C++)

This project is a **custom-built compiler frontend** written in C++, featuring a fully functional **Lexical Analyzer** and **Syntax Analyzer**. It demonstrates key concepts in compiler theory including **finite automata**, **tokenization**, **LL(1) parsing**, and **parse tree generation**.

---

## 📚 Project Documentation

These resources will help you understand both the **lexical** and **syntax** components of the compiler:

### 📘 Lexical Analyzer Docs
- `lexical_analyzer.pdf` – Language rules and token definitions
- `finite_automata_diagram.pdf` – DFA diagrams for the language
- `transitionTable__.xlsx` – DFA in Excel table format
- `transition_table.csv` – Converted table used by the lexer in code

### 📘 Syntax Analyzer Docs
- `Raw_grammar.pdf` – Raw grammar for the language
- `grammar_to_LL1_grammar.pdf` – Converted LL(1) grammar for parser
- `Parser_Example.pdf` – Simple example to understand parsing logic

### 🛠️ In Progress
- `Grammar_for_3AC.pdf` – Foundation for **Three Address Code (3AC)** generation *(Coming Soon)*

---

## 📦 Features

### 🔤 Lexical Analyzer
- Tokenizer built using a **Finite Automaton** loaded from a `.csv` table
- Supports detection of:
  - Identifiers
  - Keywords
  - Operators
  - Punctuation
  - Literals
- Error handling for unrecognized symbols (e.g. `@`)
- Input/output files:
  - 📝 `testcode.txt` – Write your source code here
  - 🧠 `tokens_table.txt` – Stores generated tokens
  - 🧾 `keyword_table.txt` – All recognized keywords
  - 📌 `literal_table.txt` – String and numeric literals
  - 📋 `symbol_table.txt` – All declared variables
  - ❗ `error_table.txt` – Invalid/unrecognized tokens
- following are links for understanding lexical part implementation
    - https://www.youtube.com/watch?v=aamWlHzqUKo&list=PLyI5MG8wpJ4cQpE29Ko9mupvinJW63IwN&index=7&pp=iAQB
    - https://www.youtube.com/watch?v=RY77FuGYDck&list=PLyI5MG8wpJ4cQpE29Ko9mupvinJW63IwN&index=8&pp=iAQB
    - https://www.youtube.com/watch?v=eSX1LwYbzYQ&list=PLyI5MG8wpJ4cQpE29Ko9mupvinJW63IwN&index=9&pp=iAQB
    - https://www.youtube.com/watch?v=X8HiyogW0lE&list=PLyI5MG8wpJ4cQpE29Ko9mupvinJW63IwN&index=10&pp=iAQB

### 📚 Syntax Analyzer
- Implements an **LL(1) parser**
- Uses preprocessed LL(1) grammar to:
  - Parse the token stream
  - Construct a **parse tree**
  - Detect **syntax errors**
- Output is structured for easy debugging and future semantic analysis

## 🧰 Built With

- Language: **C++17**
- IDE: **Visual Studio 2022**
- Core Concepts: DFA, LL(1) Parsing, Compiler Construction
- https://www.youtube.com/playlist?list=PLyI5MG8wpJ4cQpE29Ko9mupvinJW63IwN playlist i used to understand concepts of compiler 

---

## 🚀 How to Run

1. Clone the repository:

   ```bash
   git clone https://github.com/tayyabarfaq/Compiler-Construction
