% Format used in OJ (onlinejudge.org) PDFs.
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{cp_oj}
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
\RequirePackage{lmodern}
\RequirePackage{booktabs}
\RequirePackage{lastpage}
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
%\pagenumbering{gobble}      % Remove page numbering

\oddsidemargin 0cm
\evensidemargin -2cm
\topmargin -1cm
\textwidth 17cm
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

\renewcommand{\rmdefault}{ptm}

\setlength{\parskip}{0.25\baselineskip}
\setlength{\columnseprule}{3pt}
\setlength{\columnsep}{30pt}
\linethickness{0.02in} 

% Metadata
\def\yes{yes}
\newcommand{\translate}[2]{\ifx\portuguese\yes{#1}\else{#2}\fi}

% Problem

\newcommand{\header}[2]{\rhead{\thepage/\pageref{LastPage}}}
\newcommand{\trailer}[2]{}

\newenvironment{problem}[4]{

	\begin{flushleft}
		{\Large \textbf{\textcolor{red}{#1}}\ \ \ \textcolor{blue}{\textbf{#2}}}
	\end{flushleft}
}

\newenvironment{probleminput}[1]{
	\begin{flushleft} 
		\textcolor{red}{\textbf{\translate{Entrada}{Input}}}
	\end{flushleft}

    \input{#1}
}

\newenvironment{problemoutput}[1]{
	\begin{flushleft} 
		\textcolor{red}{\textbf{\translate{Saída}{Output}}} \\
	\end{flushleft}

    \input{#1}
}

\newenvironment{samples}[2]
{
    \vspace{0.2in}
}
{
}

\newcommand{\iosample}[4]
{
    \noindent \textcolor{red}{\textbf{\translate{Exemplo de Entrada}{Samples Input}}}
    \verbatiminput{#3}
    \noindent \textcolor{red}{\textbf{\translate{Exemplo de Saída}{Samples Output}}}
    \verbatiminput{#4}
}

\newenvironment{problemnotes}[1]{

	\begin{flushleft} 
		\textcolor{red}{\textbf{\translate{Notas}{Notes}}} \\
	\end{flushleft}

    \input{#1}
}
{
}

\newcommand{\variosample}[4]
{
    \begin{center}
	\begin{tabular}{p{0.9\textwidth}}
	\rowcolor{red} \textbf{\textcolor{white}{\translate{Entrada}{Input}}} \\
    \begin{minipage}[t]{0.9\textwidth}{ \verbatiminput{#3} }
    \end{minipage} \\
    \bottomrule
    \end{tabular}	

    \vspace*{0.05in}

    \begin{tabular}{p{0.9\textwidth}}
	\hline
	\rowcolor{red} \textbf{\textcolor{white}{\translate{Saída}{Output}}} \\
    \begin{minipage}[t]{0.9\textwidth}{ \verbatiminput{#4} }
    \end{minipage} \\
    \bottomrule
    \end{tabular}	
    \end{center}
}

\newenvironment{customnotes}[3]{

	\begin{flushleft}
		\textcolor{red}{\textbf{\translate{#1}{#2}}} \\
	\end{flushleft}

    \input{#3}
}
{
}
