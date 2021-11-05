% Classic format for competitive problems.
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
\RequirePackage{verbatim}
\RequirePackage{tikz}
\RequirePackage{enumerate}
\RequirePackage{amsmath}

\hyphenation{cons-tru-ção con-si-de-ra-da com-ple-xi-da-de in-di-vi-du-al-men-te}

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
\def\yes{yes}
\newcommand{\translate}[2]{\ifx\portuguese\yes{#1}\else{#2}\fi}

% Problem

\newcommand{\header}[2]{\lhead{#1}}
\newcommand{\trailer}[2]{}

\newenvironment{problem}[4]{

   	\begin{center}
		\Large{\textbf{\translate{Problema}{Problem} {#1} \\ {#2} }}{\\[.2cm]\footnotesize 
            \textbf{\translate{Limite de tempo}{Timelimit}: {#3}s}}{\\[.2cm]\footnotesize 
            \textbf{\translate{Limite de memória}{Memory limit}: {#4}MB\\}}
	\end{center}
}

\newenvironment{probleminput}[1]{
	\bigskip
	\begin{large}
		\textbf{\translate{Entrada}{Input}} \\
	\end{large}

    \input{#1}
}

\newenvironment{problemoutput}[1]{
	\bigskip
	\begin{large}
		\textbf{\translate{Saída}{Output}} \\
	\end{large}

    \input{#1}
}

\newenvironment{samples}[2]
{
	\bigskip
	\begin{large}
		\textbf{\translate{Exemplos}{Samples}} \\
	\end{large}

	\tabular{p{#1}p{#2}}
	\hline
	\translate{Entrada}{Input} & \translate{Saída}{Output} \\
}
{
	\hline
	\endtabular
}

\newcommand{\iosample}[4]
{
    \hline
    \begin{minipage}{#1}{ \vspace{2mm} \verbatiminput{#3} }
    \end{minipage} & 
    \begin{minipage}{#2}{ \vspace{2mm} \verbatiminput{#4} }
    \end{minipage} \vspace{0.5mm} \\
}

\newenvironment{problemnotes}[1]{
	\bigskip
	\begin{large}
		\textbf{\translate{Notas}{Notes}} \\
	\end{large}

    \input{#1}
}
{
}

\newenvironment{customnotes}[3]{
	\bigskip
	\begin{large}
		\textbf{\translate{#1}{#2}} \\
	\end{large}

    \input{#3}
}
{
}
