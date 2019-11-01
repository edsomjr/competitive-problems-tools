% Classic format of competitive problems.
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{cp_classic}
\LoadClass[12pt]{article}
 
\DeclareOption{portuguese}{\def\portuguese{yes}}
\ProcessOptions\relax

% Packages
\RequirePackage{fancyhdr}
\RequirePackage{tabularx,colortbl}
\RequirePackage{ifpdf}
\RequirePackage[T1]{fontenc}
\RequirePackage[utf8]{inputenc}
\RequirePackage[portuguese,english]{babel}
\RequirePackage{graphics}
\RequirePackage{graphicx}
\RequirePackage{amssymb, wrapfig}
\RequirePackage{xcolor,colortbl}
\RequirePackage{xcolor}

\ifpdf
\RequirePackage[pdftex]{hyperref}
\else
\RequirePackage[hypertex]{hyperref}
\fi

% Setup
\pagenumbering{gobble}      % Remove page numbering

\oddsidemargin 0cm
\evensidemargin -2cm
\topmargin -1cm
\textwidth 16cm
\textheight 23cm

\DeclareGraphicsExtensions{png}
\pagestyle{fancy}

\hypersetup{
	letterpaper,
	colorlinks=true,
	linkcolor=blue,
	urlcolor=blue,
	pdfpagemode=none,
}

% Metadata
\newcommand{\event}[1]{\def\printevent{#1}}
\newcommand{\translate}[2]{\ifx\portuguese\yes{#1}\else{#2}\fi}

% Problem
\lhead{\printevent}
\def\yes{yes}

\newenvironment{problem}[3]{

   	\begin{center}
		%\Large{\textbf{\iflanguage{portuguese}{Problema}{Problem} {#1} \\ {#2} }}{\\[.2cm]\footnotesize \textbf{Limite de tempo: {#3}s\\}}
		\Large{\textbf{\translate{Problema}{Problem} {#1} \\ {#2} }}{\\[.2cm]\footnotesize 
            \textbf{\translate{Limite de tempo}{Timelimit}: {#3}s\\}}
	\end{center}
}

%\newcounter{problemAutor}
%\newenvironment{ProblemaAutor}[4]{
%	\stepcounter{problemAutor}
%	\newpage
%	\begin{center}
%		\Large{\textbf{Problema {#1} \\ {#2} }}{\\[.2cm]\footnotesize \textbf{Limite de tempo: {#3}s\\}}{
%			\footnotesize Autor: {#4}
%		}
%	\end{center}
%}


% Código-fonte
\newcommand{\codigofonte}[1]{Nome do arquivo fonte: {#1}}

% Entrada
\newcommand{\Entrada}{
	\bigskip
	\begin{large}
		\textbf{Entrada} \\
	\end{large}
}

% Saida
\newcommand{\Saida}{
	\bigskip
	\begin{large}
		\textbf{Saída} \\
	\end{large}
}

\newcommand{\Interacao}{
	\bigskip
	\begin{large}
		\textbf{Interação} \\
	\end{large}
}

\newcommand{\Notas}{
	\bigskip
	\begin{large}
		\textbf{Notas} \\
	\end{large}
}

% Exemplo
\newenvironment{Exemplo}
{
	
	\tabularx{\textwidth}{XX}
	%    {@{\extracolsep{\fill}}|l|l|}
	%    {|l|l@{\extracolsep{\fill}|}}
	\hline
	Entrada & Saída \\\hline 
}
{
	\hline
	\endtabularx
}

% Exemplo de Entrada
\newenvironment{ExemploEntrada}
{
	\bigskip
	\begin{large}
		\textbf{Exemplo} \\
	\end{large}
}
{
}

% Sample Output


